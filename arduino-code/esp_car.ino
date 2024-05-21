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

  // Start mDNS with hostname "esp_car"
  if (!MDNS.begin("esp_car")) {
    Serial.println("Error setting up MDNS responder!");
  }
}

void loop() {
  MDNS.update();
  server.handleClient();  // Handle incoming HTTP requests
}

void startServer() {
  Serial.println("Starting server");

  // Handle requests to start car forward
  server.on("/start_car_forward", HTTP_POST, []() {
    Serial.println("Getting from server /start_car_forward");
    arduinoSerial.println("Start car forward");
    server.send(200, "text/plain", "Car should be moving forward");
  });

  // Handle requests to start car backward
  server.on("/start_car_backward", HTTP_POST, []() {
    Serial.println("Getting from server /start_car_backward");
    arduinoSerial.println("Start car backward");
    server.send(200, "text/plain", "Car should be moving backward");
  });

  // Handle requests to stop car
  server.on("/stop_car", HTTP_POST, []() {
    Serial.println("Getting from server /stop_car");
    arduinoSerial.println("Stop car");
    server.send(200, "text/plain", "Car should be stopping");
  });

  server.begin();  // Start the server
}
