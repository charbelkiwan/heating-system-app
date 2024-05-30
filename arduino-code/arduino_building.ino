#include <Stepper.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define STEP_PIN1 2
#define STEP_PIN2 3
#define STEP_PIN3 4
#define STEP_PIN4 5
#define TRIG_PIN 8
#define ECHO_PIN 9
#define SERVO_PIN 10
#define GREEN_LED1_PIN 11
#define GREEN_LED2_PIN 12

#define STEPS_PER_REV 2048

Stepper stepper(STEPS_PER_REV, STEP_PIN1, STEP_PIN3, STEP_PIN2, STEP_PIN4);
SoftwareSerial espSerial(6, 7); // RX, TX
Servo myServo;

static bool checked = false;
static String targetFloor;

void setup()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  espSerial.begin(9600);

  myServo.attach(SERVO_PIN);
  myServo.write(0); // Initialize servo position to 0 degrees

  pinMode(GREEN_LED1_PIN, OUTPUT);
  pinMode(GREEN_LED2_PIN, OUTPUT);

  stepper.setSpeed(15); // Set the speed of the stepper motor
}

void loop()
{
  if (espSerial.available())
  {
    String command = espSerial.readStringUntil('\n');
    Serial.println(command);
    if (command.equals("1\r"))
    {
      targetFloor = "1";
    }
    else if (command.equals("2\r"))
    {
      targetFloor = "2";
    }
    else if (command.equals("Car arrived to containers\r"))
    {
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
  if (distance < 10 && !checked)
  {
    Serial.println("Object detected within 10 cm");
    checked = true;
    espSerial.println("Stop the car and open servo");
    delay(5000);
  }

  if (checked && targetFloor.equals("1"))
  {
    // Turn stepper motor - Option 1
    Serial.println("Going to the first floor");
    digitalWrite(GREEN_LED1_PIN, HIGH); // Turn on green LED 1
    delay(2000);
    stepper.step(3 * STEPS_PER_REV); // Rotate 3 times

    delay(1000); // Wait for the servo to reach the position
    for(int i = 1; i < 51; i++){
      myServo.write(i);
      delay(50);
    }
    myServo.write(70);

    delay(3000); // Wait for the servo to reach the position

    myServo.write(0);
    delay(1000); // Wait for the servo to reach the position

    delay(5000);
    stepper.step(-3 * STEPS_PER_REV);
    digitalWrite(GREEN_LED1_PIN, LOW); // Turn off green LED 1
    targetFloor = "";                  // Reset targetFloor after operation
    espSerial.println("Start car");
  }
  else if (checked && targetFloor.equals("2"))
  {
    // Turn stepper motor - Option 2
    Serial.println("Going to the second floor");
    digitalWrite(GREEN_LED2_PIN, HIGH); // Turn on green LED 2
    delay(2000);
    stepper.step(5 * STEPS_PER_REV); // Rotate 6 times

    delay(1000); // Wait for the servo to reach the position
    for(int i = 1; i < 51; i++){
      myServo.write(i);
      delay(50);
    }
    myServo.write(70);

    delay(3000); // Wait for the servo to reach the position

    myServo.write(0);
    delay(1000); // Wait for the servo to reach the position

    delay(5000);
    stepper.step(-5 * STEPS_PER_REV);
    digitalWrite(GREEN_LED2_PIN, LOW); // Turn off green LED 2
    targetFloor = "";                  // Reset targetFloor after operation
    espSerial.println("Start car");
  }

  delay(100); // Small delay to avoid excessive processing
}
