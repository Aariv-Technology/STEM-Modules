#include <Servo.h>   // Library to control servo motor

// Ultrasonic sensor pins
#define TRIG_PIN 4
#define ECHO_PIN 5

// Servo motor pin
#define SERVO_PIN 9

// Servo angles for lid control
#define OPEN_ANGLE 0
#define CLOSE_ANGLE 75

// Distance (in cm) to detect hand/object
#define DETECT_DISTANCE 60

// How long lid stays open (milliseconds)
#define OPEN_TIME 5000

// Number of readings to average for stable distance
#define NUM_SAMPLES 50

Servo dustbinServo;     // Create servo object
bool lidOpen = false;   // Track lid state

/* -------- Distance Reading -------- */

// Take one distance reading from ultrasonic sensor
long getDistanceOnce() {
  // Send trigger pulse (10 microseconds)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms

  // If no signal received, return invalid value
  if (duration == 0) return -1;

  // Convert time to distance in cm
  return duration * 0.034 / 2;
}

// Take multiple readings and calculate average distance
// This reduces noise and gives stable measurement
long getAverageDistance() {
  long sum = 0;
  int count = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    long d = getDistanceOnce();
    
    // Only consider valid readings
    if (d > 0) {
      sum += d;
      count++;
    }
    delay(5);  // small delay between readings
  }

  // If no valid readings
  if (count == 0) return -1;

  // Return average distance
  return sum / count;
}

// Move servo to given angle
// Attach → move → detach to reduce vibration and power usage
void moveServo(int angle) {
  dustbinServo.attach(SERVO_PIN);
  dustbinServo.write(angle);
  delay(500);          // allow servo to reach position
  dustbinServo.detach();
}

void setup() {
  Serial.begin(9600);   // Start serial monitor

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Start with lid closed
  moveServo(CLOSE_ANGLE);

  Serial.println("Smart Dustbin Ready");
}

void loop() {

  // Get stable average distance
  long distance = getAverageDistance();

  // Print distance for debugging
  Serial.print("Avg Distance: ");
  Serial.println(distance);

  // If lid is closed AND object detected within range
  if (!lidOpen && distance > 0 && distance <= DETECT_DISTANCE) {
    
    Serial.println("Opening lid");
    moveServo(OPEN_ANGLE);   // Open lid
    lidOpen = true;

    delay(OPEN_TIME);        // Keep lid open for some time

    Serial.println("Closing lid");
    moveServo(CLOSE_ANGLE);  // Close lid
    lidOpen = false;

    delay(1000);             // Small delay before next detection
  }

  delay(200);   // Loop delay to reduce rapid triggering
}
