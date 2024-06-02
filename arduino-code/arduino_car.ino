#include <Servo.h>
#include <SoftwareSerial.h>

#define MOTOR1_PIN1 2
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 4
#define MOTOR2_PIN2 5
#define SERVO_PIN 6 // Add the pin for the servo motor

#define RED_LED1_PIN 9

SoftwareSerial espSerial(11, 12);
Servo myServo; // Create a servo object

void setup()
{
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  espSerial.begin(9600);
  Serial.begin(9600);

  pinMode(RED_LED1_PIN, OUTPUT);

  myServo.attach(SERVO_PIN); // Attach the servo to the specified pin
  myServo.write(0);          // Set the servo to 0 degrees by default

  digitalWrite(RED_LED1_PIN, HIGH); // Turn on red LEDs
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
  analogWrite(MOTOR1_PIN1, 130); // Set low PWM value
  analogWrite(MOTOR1_PIN2, 0);
  analogWrite(MOTOR2_PIN1, 130); // Set low PWM value
  analogWrite(MOTOR2_PIN2, 0);

  digitalWrite(RED_LED1_PIN, LOW); // Turn off red LEDs
}

void startCarBackward()
{
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 130); // Set low PWM value
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, 130); // Set low PWM value

  digitalWrite(RED_LED1_PIN, LOW); // Turn off red LEDs
}

void stopTheCar()
{
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 0);
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, 0);

  digitalWrite(RED_LED1_PIN, HIGH); // Turn on red LEDs
}

void openServo()
{
  delay(3000);
  for (int i = 1; i < 51; i++)
  {
    myServo.write(i); // Rotate the servo to 180 degrees
    delay(50);
  }
  myServo.write(70); // Rotate the servo to 180 degrees
  delay(3000);       // Example delay, adjust as needed
  myServo.write(0);  // Rotate the servo back to 0 degrees
}
