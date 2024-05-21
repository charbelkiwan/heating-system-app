#include <Stepper.h>
#include <SoftwareSerial.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define STEP_PIN1 2
#define STEP_PIN2 3
#define STEP_PIN3 4
#define STEP_PIN4 5

#define STEPS_PER_REV 2048

Stepper stepper(STEPS_PER_REV, STEP_PIN1, STEP_PIN3, STEP_PIN2, STEP_PIN4);
SoftwareSerial espSerial(6, 7); // RX, TX

static bool checked = false;
static String targetFloor;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  espSerial.begin(9600);

  stepper.setSpeed(15); // Set the speed of the stepper motor
}

void loop() {
  if (espSerial.available()) {
    String command = espSerial.readStringUntil('\n');
    Serial.println(command);
    if (command.equals("1\r")) {
      targetFloor = "1";
    }
    else if (command.equals("2\r")) {
      targetFloor = "2";
    }
    else if (command.equals("Car arrived to containers\r")) {
      checked = false;
    }
  }  

  long duration, distance;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  // If an object is detected within 5 cm
  if (distance < 25 && !checked) {
    Serial.println("Object detected within 5 cm");
    checked = true;
    espSerial.println("Stop the car");
    delay(2000);
  }

  if (checked && targetFloor.equals("1")) {
    // Turn stepper motor - Option 1
    Serial.println("Going to the first floor");
    delay(2000);
    stepper.step(4 * STEPS_PER_REV); // rotate 4 times
    delay(10000);
    stepper.step(-4 * STEPS_PER_REV);
    targetFloor = ""; // Reset targetFloor after operation
    espSerial.println("Start car");
  } else if (checked && targetFloor.equals("2")) {
    // Turn stepper motor - Option 2
    Serial.println("Going to the second floor");
    delay(2000);
    stepper.step(8 * STEPS_PER_REV); // rotate 8 times
    delay(10000);
    stepper.step(-8 * STEPS_PER_REV);
    targetFloor = ""; // Reset targetFloor after operation
    espSerial.println("Start car");
  }

  delay(100); // Small delay to avoid excessive processing
}
