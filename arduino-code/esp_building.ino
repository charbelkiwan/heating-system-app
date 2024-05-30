#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

#define WIFI_SSID "ulfg2"
#define WIFI_PASSWORD "Lello123"

const char* serverIP = "192.168.216.104";
const uint16_t serverPort = 5000;

SoftwareSerial arduinoSerial(D2, D3);  // RX, TX
WiFiClient client;
ESP8266WebServer server(80);  // Initialize ESP8266WebServer on port 80

void startServer();
void sendToServer(String message, bool openServo = false);

void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wi-Fi");
}

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  connectToWiFi();
  startServer();

  // Start mDNS with hostname "esp_containers"
  if (!MDNS.begin("esp_building")) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("MDNS responder started");
  }
}

void loop() {
  MDNS.update();
  server.handleClient();  // Handle incoming HTTP requests

  if (arduinoSerial.available()) {
    String command = arduinoSerial.readStringUntil('\n');
    command.trim();  // Remove newline characters
    Serial.println(command);

    if (command.equals("Start car\r")) {
      Serial.println("Sending Start the car to server");
      delay(1000);
      sendToServer("/start_car_backward");
    } else if (command.equals("Stop the car\r")) {
      Serial.println("Sending Stop the car to server");
      delay(1000);
      sendToServer("/stop_car");
    } else if (command.equals("Stop the car and open servo\r")) {
      Serial.println("Sending Stop the car and open servo to server");
      delay(1000);
      sendToServer("/stop_car", true);
    } else if (command.equals("1\r")) {
      Serial.println("Floor number received from server: 1");
      sendToArduino(1);
    } else if (command.equals("2\r")) {
      Serial.println("Floor number received from server: 2");
      sendToArduino(2);
    }
  }
}

void startServer() {
  Serial.println("Starting server");

  server.on("/floor", HTTP_POST, []() {  // Handler for the "/floor" route
    Serial.println("Getting from server /floor");
    if (server.hasArg("floor")) {
      String floorNumber = server.arg("floor");
      Serial.println("Floor number received from client: " + floorNumber);
      arduinoSerial.println(floorNumber);  // Forward floor number to Arduino
      server.send(200, "text/plain", "Floor number received");
    } else {
      server.send(400, "text/plain", "Bad Request: Missing floor number");
    }
  });

  server.on("/car_arrived_to_containers", HTTP_POST, []() {
    Serial.println("Getting from server /car_arrived_to_containers");
    arduinoSerial.println("Car arrived to containers");
    server.send(200, "text/plain", "Car arrived to containers send to esp building");
  });

  server.begin();  // Start the server
}

void sendToServer(String message, bool openServo) {
  HTTPClient http;

  // Construct the complete URL
  String url = "http://" + String(serverIP) + ":" + String(serverPort) + message;

  // Use the correct begin method
  http.begin(client, url);  // Pass the WiFiClient object and the URL

  // Add necessary headers
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Prepare POST data
  String postData = openServo ? "open_servo=true" : "";

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.printf("[HTTP] POST request sent, response code: %d\n", httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = http.getString();
      Serial.println("Response payload: " + payload);
    }
  } else {
    Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

void sendToArduino(int floorNumber) {
  String message = String(floorNumber);
  Serial.println("Sending to Arduino: " + message);  // Debug print
  arduinoSerial.println(message);
}
