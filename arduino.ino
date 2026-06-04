#include <Servo.h>

Servo myServo;

int angle = 90;

void setup() {

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  Serial.begin(9600);

  myServo.attach(9);
  myServo.write(angle);
}

void loop() {

  int left = digitalRead(2);
  int right = digitalRead(3);

  int target = 90;

  String direction = "CENTER";

  if (left == 1 && right == 0) {
    target = 10;          // Left limit
    direction = "LEFT";
  }

  else if (left == 0 && right == 1) {
    target = 170;         // Right limit
    direction = "RIGHT";
  }

  else {
    target = 90;
    direction = "CENTER";
  }

  // Smooth movement
  if (angle < target)
    angle++;

  else if (angle > target)
    angle--;

  myServo.write(angle);

  // Send direction and ACTUAL angle to ESP32
  Serial.print(direction);
  Serial.print(",");
  Serial.println(angle);

  delay(15);
}