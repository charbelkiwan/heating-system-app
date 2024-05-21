#include <Servo.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN  2
#define ECHO_PIN     3
#define SERVO1_PIN   8
#define SERVO2_PIN   9

SoftwareSerial espSerial(10, 11); // RX, TX

Servo servo1;
Servo servo2;

void startthecar() {
  Serial.println("Sending to esp: Start The car");
  espSerial.println("Start the car");
  delay(500);
}

void stopthecar(){
  Serial.println("Sending to esp: Stop The car");
  espSerial.println("Stop the car");
  delay(500);
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
}

void loop() {
  long duration, distance;

  // Send a pulse to the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Read the pulse from the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  distance = duration * 0.034 / 2;

  // Check the distance and stop the car if an object is detected
  if (distance < 15) {
    Serial.println("Object detected!");
    stopthecar();
  } else {
    // Continue with normal operation
    if (espSerial.available()) {
      String input = espSerial.readStringUntil('\n');
      Serial.println(input);
      if (input.equals("1\r")) {
        servo1.write(180);
        delay(5000); // Adjust the delay as needed
        servo1.write(0); // Reset servo to initial position
        delay(100);
        startthecar();
      } else if (input.equals("2\r")) {
        servo2.write(180);
        delay(5000); // Adjust the delay as needed
        servo2.write(0); // Reset servo to initial position
        delay(100);
        startthecar();
      }
    }
  }

  delay(1000); // Delay for stability
}
