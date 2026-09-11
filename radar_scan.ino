#include <Servo.h>
Servo servo;
const int servoPin = 3;
int curDegree = 0;
int largestDegree;

long dur;
float curDist;
int largestDist;

const int trigPin = 10;
const int echoPin = 11;

void setup() {
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

// Scans current distance at current degree and notes the largest distance at
// its corresponding degree
void scan() {
  servo.write(curDegree);

  delay(30); // Accounting for physical constraints


  // Measuring distance between object and sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculating current distance in cm/us
  dur = pulseIn(echoPin, HIGH, 30000); // 30000 -> Accounting for 1s delay when object is out of range
  curDist = dur * 0.034 / 2; // 0.034, speed of sound in cm/us

  Serial.print("Current Distance: ");
  Serial.println(curDist);

  // Noting the largest distance and its corresponding degree
  if(curDist > largestDist) {
    largestDist = curDist;
    largestDegree = curDegree;
  }
}

void loop() {
  
  // Constant sweep of surroundings & moving towards clearest (largest dist) path
  if(curDegree <= 180) { // 180 degree max for this servo
    scan();
    curDegree+=10;
  } else {
    servo.write(largestDegree);
    Serial.println(largestDist);

    delay(1500); // Shows current best path for 1.5s

    // Resets to check best path again
    curDegree = 0;
    largestDegree = 0;
    largestDist = 0;
  }
}
