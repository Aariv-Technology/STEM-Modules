#include <Servo.h>

// Motor Pins
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 6;
const int IN4 = 5;
const int ENA = 9;  // PWM
const int ENB = 3;  // PWM

// Ultrasonic Pins
const int trigPin = 12;
const int echoPin = 13;

// Servo
const int servoPin = 11;
Servo myServo;

// Speed (0-255)
int speedLeft = 255;
int speedRight = 255;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(90);
  delay(1000);
}

void loop() {
  int distance = getDistance();
  Serial.println(distance);

  if (distance > 20) {
    moveForward();
  } else {
    stopMotors();
    delay(300);

    myServo.write(150);  // Look Left
    delay(500);
    int leftDistance = getDistance();

    myServo.write(30);   // Look Right
    delay(500);
    int rightDistance = getDistance();

    myServo.write(90);   // Center
    delay(300);

    if (leftDistance > rightDistance) {
      turnLeft();
      delay(400);
    } else {
      turnRight();
      delay(400);
    }
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
