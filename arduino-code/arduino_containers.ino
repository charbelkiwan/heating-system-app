#include <Servo.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN  2
#define ECHO_PIN     3
#define RED_LED1_PIN  4
#define GREEN_LED1_PIN  5
#define RED_LED2_PIN  6
#define GREEN_LED2_PIN  7
#define SERVO1_PIN   8
#define SERVO2_PIN   9

SoftwareSerial espSerial(10, 11); // RX, TX

Servo servo1;
Servo servo2;

static bool checked = false;

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

  pinMode(RED_LED1_PIN, OUTPUT);
  pinMode(GREEN_LED1_PIN, OUTPUT);
  pinMode(RED_LED2_PIN, OUTPUT);
  pinMode(GREEN_LED2_PIN, OUTPUT);

  servo1.write(0); // Initialize servos to 0 position
  servo2.write(0); // Initialize servos to 0 position
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
if (distance < 25 && !checked) {
    Serial.println("Object detected!");
    espSerial.println("Car arrived to containers");
    checked = true;
    stopthecar();
  } else {
    // Continue with normal operation
    if (espSerial.available()) {
      String input = espSerial.readStringUntil('\n');
      Serial.println(input);
      if (input.equals("1\r")) {
        digitalWrite(GREEN_LED1_PIN, HIGH); // Turn on green LED 1
        digitalWrite(RED_LED1_PIN, LOW); // Turn off red LED 1
        servo1.write(180);
        delay(5000); // Adjust the delay as needed
        servo1.write(0); // Reset servo to initial position
        delay(100);
        digitalWrite(GREEN_LED1_PIN, LOW); // Turn off green LED 1
        digitalWrite(RED_LED1_PIN, HIGH); // Turn on red LED 1
        startthecar();
        delay(5000);
        checked = false;
      } else if (input.equals("2\r")) {
        digitalWrite(GREEN_LED2_PIN, HIGH); // Turn on green LED 2
        digitalWrite(RED_LED2_PIN, LOW); // Turn off red LED 2
        servo2.write(180);
        delay(5000); // Adjust the delay as needed
        servo2.write(0); // Reset servo to initial position
        delay(100);
        digitalWrite(GREEN_LED2_PIN, LOW); // Turn off green LED 2
        digitalWrite(RED_LED2_PIN, HIGH); // Turn on red LED 2
        startthecar();
        delay(5000);
        checked = false;
      }
    }
  }

  delay(1000); // Delay for stability
}
