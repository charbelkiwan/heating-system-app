#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* serverAddress = "192.168.0.8";
const int serverPort = 5000;

SoftwareSerial arduinoSerial(D5, D6); // RX, TX

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  arduinoSerial.begin(9600); // Set baud rate for communication with Arduino

  server.on("/open_door", HTTP_GET, [](){
    if (server.hasArg("floor")) {
      int floor = server.arg("floor").toInt();
      arduinoSerial.print("open:");
      arduinoSerial.println(floor);
      server.send(200, "text/plain", "Door opened for floor " + String(floor));
    } else {
      server.send(400, "text/plain", "Bad Request");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // Forward data from Arduino to server
  while (arduinoSerial.available()) {
    Serial.write(arduinoSerial.read());
  }

  // Forward data from server to Arduino
  if (Serial.available()) {
    arduinoSerial.write(Serial.read());
  }
}
