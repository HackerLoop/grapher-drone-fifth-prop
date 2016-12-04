#include <Arduino.h>
#include <Servo.h>

#define PULSE_TIMEOUT 20000

#define TRIGGER_1 2
#define ECHO_1 3
#define TRIGGER_2 4
#define ECHO_2 5

#define DISTANCE_OFFSET_1 0
#define DISTANCE_OFFSET_2 0.27
#define DISTANCE_TRESHOLD 7

#define ESC_OUT 9

Servo esc;

void setupSensor(int trigger, int echo) {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trigger, 0);
}

void setup() {
  SerialUSB.begin(115200);

  setupSensor(TRIGGER_1, ECHO_1);
  setupSensor(TRIGGER_2, ECHO_2);

  esc.attach(ESC_OUT);

  delay(100);
  SerialUSB.println("start");
}

double distanceForSensor(int trigger, int echo) {
  digitalWrite(trigger, 1);
  delayMicroseconds(10);
  digitalWrite(trigger, 0);

  unsigned long duration = pulseIn(echo, 1, PULSE_TIMEOUT);
  double distance = (duration/2) / 29.1;
  return distance;
}

bool triggeredDistance(double distance) {
  return distance > 0 && distance <= DISTANCE_TRESHOLD;
}

void loop() {
  double distance1 = distanceForSensor(TRIGGER_1, ECHO_1);
  delay(5);
  double distance2 = distanceForSensor(TRIGGER_2, ECHO_2);

  SerialUSB.print(distance1);
  SerialUSB.print(" ");
  SerialUSB.println(distance2);

  if (triggeredDistance(distance1 - DISTANCE_OFFSET_1 ) && triggeredDistance(distance2 - DISTANCE_OFFSET_2)) {
    SerialUSB.println("triggered");
    esc.write(60);
  } else {
    esc.write(45);
  }

  delay(15);
}
