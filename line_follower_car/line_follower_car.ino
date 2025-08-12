// === Motor Driver Pins ===
const int IN1 = 9;   // Left motor IN1
const int IN2 = 8;   // Left motor IN2
const int IN3 = 7;   // Right motor IN3
const int IN4 = 6;   // Right motor IN4
const int ENA = 10;  // Left motor speed (PWM)
const int ENB = 5;  // Right motor speed (PWM)

// === IR Sensor Pins ===
const int LEFT_SENSOR = A0;   // Left IR sensor output
const int RIGHT_SENSOR = A1;  // Right IR sensor output

// === Speed Control (0 to 255) ===
int motorSpeed = 90;

void setup() {
  // Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // IR Sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  // Serial Monitor (optional)
  Serial.begin(9600);
}

void loop() {
  // Read IR sensors
  int leftValue = digitalRead(LEFT_SENSOR);
  int rightValue = digitalRead(RIGHT_SENSOR);

  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print(" | Right: ");
  Serial.println(rightValue);

  // Logic: LOW = Black line, HIGH = White surface
  if (leftValue == LOW && rightValue == LOW) {
    moveForward();
  } 
  else if (leftValue == LOW && rightValue == HIGH) {
    turnLeft();
  } 
  else if (leftValue == HIGH && rightValue == LOW) {
    turnRight();
  } 
  else {
    stopMotors(); // Both off line
  }
}

// === Motor Movement Functions ===

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Left motor backward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Right motor forward
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Left motor forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Right motor backward
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
