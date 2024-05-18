#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const int ultrasonicPin = 2;
const int doorMotor1 = 4;
const int doorMotor2 = 5;
const int ultrasonicThreshold = 20;
const int esp8266Pin = 6;

Servo servo1;
Servo servo2;

const char* serverAddress = "heating-system.versel.app"; // Update with your server address
const int serverPort = 5000; // Update with your server port

void setup() {
  pinMode(ultrasonicPin, INPUT);
  pinMode(doorMotor1, OUTPUT);
  pinMode(doorMotor2, OUTPUT);
  pinMode(esp8266Pin, OUTPUT);
  servo1.attach(doorMotor1);
  servo2.attach(doorMotor2);
}

void loop() {
  if (detectBox()) {
    sendSignalToESP();
    delay(5000);
    int requestedFloor = getRequestedFloor();
    if (requestedFloor != -1) {
      openDoor(requestedFloor);
      delay(5000); // Adjust time for box retrieval
      closeDoor(requestedFloor);
    }
  }
}

bool detectBox() {
  int distance = getDistance();
  return distance < ultrasonicThreshold;
}

int getDistance() {
  long duration;
  digitalWrite(ultrasonicPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicPin, LOW);
  duration = pulseIn(ultrasonicPin, HIGH);
  return duration * 0.034 / 2;
}

int getRequestedFloor() {
  WiFiClient client;
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed");
    return -1;
  }

  client.print("GET / HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n\r\n");

  while (!client.available()) {
    delay(100);
  }
  String response = client.readStringUntil('\r');
  client.stop();

  int pos = response.indexOf("floor:");
  if (pos != -1) {
    String floorStr = response.substring(pos + 6);
    return floorStr.toInt();
  } else {
    Serial.println("Requested floor not found");
    return -1;
  }
}

void openDoor(int requestedFloor) {
  if (requestedFloor == 1) {
    servo1.write(90); // Adjust angle as per your setup for container 1
    delay(1000); // Adjust delay for servo movement
  } else if (requestedFloor == 2) {
    servo2.write(90); // Adjust angle as per your setup for container 2
    delay(1000); // Adjust delay for servo movement
  }
}

void closeDoor(int requestedFloor) {
  if (requestedFloor == 1) {
    servo1.write(0); // Adjust angle as per your setup for container 1
    delay(1000); // Adjust delay for servo movement
  } else if (requestedFloor == 2) {
    servo2.write(0); // Adjust angle as per your setup for container 2
    delay(1000); // Adjust delay for servo movement
  }
}

void sendSignalToESP() {
  digitalWrite(esp8266Pin, HIGH);
  delay(1000);
  digitalWrite(esp8266Pin, LOW);
}
