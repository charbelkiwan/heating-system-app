#include <SoftwareSerial.h>

#define MOTOR1_PIN1 2
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 4
#define MOTOR2_PIN2 5

SoftwareSerial espSerial(10, 11);

void setup()
{
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  espSerial.begin(9600);
  Serial.begin(9600);
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
  }
}

void startCarForward()
{
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
}

void startCarBackward()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void stopTheCar()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}
