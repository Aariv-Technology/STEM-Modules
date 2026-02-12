#include <Servo.h>

#define TRIG_PIN 4
#define ECHO_PIN 5
#define SERVO_PIN 9

#define OPEN_ANGLE 0
#define CLOSE_ANGLE 75
#define DETECT_DISTANCE 60
#define OPEN_TIME 5000

#define NUM_SAMPLES 50

Servo dustbinServo;
bool lidOpen = false;

/* -------- Distance Reading -------- */
long getDistanceOnce() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

long getAverageDistance() {
  long sum = 0;
  int count = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    long d = getDistanceOnce();
    if (d > 0) {
      sum += d;
      count++;
    }
    delay(5);
  }

  if (count == 0) return -1;
  return sum / count;
}

void moveServo(int angle) {
  dustbinServo.attach(SERVO_PIN);
  dustbinServo.write(angle);
  delay(500);          // allow servo to reach position
  dustbinServo.detach();
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  moveServo(CLOSE_ANGLE);

  Serial.println("Smart Dustbin Ready");
}

void loop() {
  long distance = getAverageDistance();

  Serial.print("Avg Distance: ");
  Serial.println(distance);

  if (!lidOpen && distance > 0 && distance <= DETECT_DISTANCE) {
    Serial.println("Opening lid");
    moveServo(OPEN_ANGLE);
    lidOpen = true;

    delay(OPEN_TIME);

    Serial.println("Closing lid");
    moveServo(CLOSE_ANGLE);
    lidOpen = false;

    delay(1000);
  }

  delay(200);
}
