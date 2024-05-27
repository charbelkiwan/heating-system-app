#include <Servo.h>
#include <SoftwareSerial.h>

#define MOTOR1_PIN1 2 
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 4
#define MOTOR2_PIN2 5
#define SERVO_PIN 6 // Add the pin for the servo motor
#define WHITE_LED1_PIN  7
#define WHITE_LED2_PIN  8
#define RED_LED1_PIN    9
#define RED_LED2_PIN    10


SoftwareSerial espSerial(11, 12);
Servo myServo;  // Create a servo object

void setup()
{
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  espSerial.begin(9600);
  Serial.begin(9600);

  pinMode(WHITE_LED1_PIN, OUTPUT);
  pinMode(WHITE_LED2_PIN, OUTPUT);
  pinMode(RED_LED1_PIN, OUTPUT);
  pinMode(RED_LED2_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);  // Attach the servo to the specified pin
  myServo.write(0);           // Set the servo to 0 degrees by default
}

void loop()
{
  if (espSerial.available())
  {
    String command = espSerial.readStringUntil('\n');
    Serial.println(command);
    if (command.equals("Start car forward\r"))
    {
      startCarForward();
    }
    else if (command.equals("Start car backward\r"))
    {
      startCarBackward();
    }
    else if (command.equals("Stop car\r"))
    {
      stopTheCar();
    }
    else if (command.equals("Open servo\r"))
    {
      openServo();
    }
  }
}

void startCarForward()
{
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);

  digitalWrite(WHITE_LED1_PIN, HIGH); // Turn on white LEDs
  digitalWrite(WHITE_LED2_PIN, HIGH);
  digitalWrite(RED_LED1_PIN, LOW);    // Turn off red LEDs
  digitalWrite(RED_LED2_PIN, LOW);
}

void startCarBackward()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);

  digitalWrite(WHITE_LED1_PIN, HIGH); // Turn on white LEDs
  digitalWrite(WHITE_LED2_PIN, HIGH);
  digitalWrite(RED_LED1_PIN, LOW);    // Turn off red LEDs
  digitalWrite(RED_LED2_PIN, LOW);
}

void stopTheCar()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);

  digitalWrite(WHITE_LED1_PIN, LOW); // Turn off white LEDs
  digitalWrite(WHITE_LED2_PIN, LOW);
  digitalWrite(RED_LED1_PIN, HIGH);  // Turn on red LEDs
  digitalWrite(RED_LED2_PIN, HIGH);
}

void openServo()
{
  delay(3000);
  myServo.write(180);  // Rotate the servo to 180 degrees
  delay(2000);         // Example delay, adjust as needed
  myServo.write(0);    // Rotate the servo back to 0 degrees
}
