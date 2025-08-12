#include <Wire.h>
#include <MPU6050.h>
#include <avr/wdt.h>  // Watchdog

MPU6050 mpu;

// Motor Pins
#define M1_IN1 8
#define M1_IN2 9
#define M2_IN1 10
#define M2_IN2 11

float Kp = 40.0, Ki = 0.5, Kd = 1.5;
float error, previous_error = 0, integral = 0, output = 0;
float setPoint = 0.0, filteredAngle = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setWireTimeout(3000, true);  // prevent I2C freeze

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 failed");
    while (1);
  }

  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);

  lastTime = millis();
  delay(2000);
  Serial.println("Balancing...");

  wdt_enable(WDTO_250MS);  // Enable watchdog reset (250 ms)
}

void loop() {
  wdt_reset();  // Keep watchdog alive

  unsigned long now = millis();

  // Read MPU6050 safely
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accAngle = atan2(ay, az) * 180 / PI;
  float gyroRate = gx / 131.0;

  float dt = (now - lastTime) / 1000.0;
  if (dt < 0.005 || dt > 0.5) dt = 0.01;
  lastTime = now;

  filteredAngle = 0.98 * (filteredAngle + gyroRate * dt) + 0.02 * accAngle;

  error = filteredAngle - setPoint;
  integral += error * dt;
  integral = constrain(integral, -100, 100);

  float derivative = (error - previous_error) / dt;
  output = Kp * error + Ki * integral + Kd * derivative;
  output = constrain(output, -255, 255);
  previous_error = error;

  if (abs(output) < 30) {
    stopMotors();
  } else if (output > 0) {
    moveForward(output);
  } else {
    moveBackward(-output);
  }

  Serial.print("Angle: ");
  Serial.print(filteredAngle);
  Serial.print(" | Output: ");
  Serial.println(output);
}

void moveForward(int pwm) {
  pwm = constrain(pwm, 0, 255);
  analogWrite(M1_IN1, pwm);
  digitalWrite(M1_IN2, LOW);
  analogWrite(M2_IN1, pwm);
  digitalWrite(M2_IN2, LOW);
}

void moveBackward(int pwm) {
  pwm = constrain(pwm, 0, 255);
  digitalWrite(M1_IN1, LOW);
  analogWrite(M1_IN2, pwm);
  digitalWrite(M2_IN1, LOW);
  analogWrite(M2_IN2, pwm);
}

void stopMotors() {
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);
}
