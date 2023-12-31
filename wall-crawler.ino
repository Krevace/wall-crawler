#include <Servo.h> //Arduino-Libraries
#include <HCSR04.h> //HCSR04 by Martin Sosic

#define TRIG_PIN1 3
#define ECHO_PIN1 4
#define TRIG_PIN2 5
#define ECHO_PIN2 6

UltraSonicDistanceSensor leftSensor(ECHO_PIN1, TRIG_PIN1); 
UltraSonicDistanceSensor frontSensor(ECHO_PIN2, TRIG_PIN2); 

Servo rightMotor;
Servo leftMotor; 

double distanceLeft;
double distanceFront;
int overCount = 0; 
long startMillis = 0;

void setup() {
  rightMotor.attach(11);
  leftMotor.attach(10);
  Serial.begin(9600);
}

void loop() {
  distanceLeft = leftSensor.measureDistanceCm();
  distanceFront = frontSensor.measureDistanceCm();
  if (distanceLeft > 30) {
    overCount++;
    startMillis = millis();
  }
  if (overCount && millis() - startMillis > 1000) overCount = 0;
  if (overCount >= 15) {
    overCount = 0;
    stopMotor();
    turnLeft();
    driveForward(true);
    delay(1000);
  } else if (distanceFront < 8) {
    stopMotor();
    turnRight();
    stopMotor();
    delay(500);
    distanceLeft = leftSensor.measureDistanceCm();
    if (distanceLeft > 30) turnLeft();
  } else driveForward(false);
}

void driveForward(bool leftTurn) {
  int change = (!overCount && !leftTurn) ? (8 - distanceLeft) : 0;
  if (change > 3) change = 3;
  if (change < -3) change = -3;
  Serial.println(change);
  rightMotor.writeMicroseconds(1444 + change);
  leftMotor.writeMicroseconds(1550 + change);
}

void stopMotor() {
  rightMotor.writeMicroseconds(1500); 
  leftMotor.writeMicroseconds(1500);
}

void turnRight() {
  rightMotor.writeMicroseconds(1544); 
  leftMotor.writeMicroseconds(1550);
  delay(925);
  stopMotor();
}

void turnLeft() {
  rightMotor.writeMicroseconds(1450); 
  leftMotor.writeMicroseconds(1450);
  delay(925);
  stopMotor();
}
