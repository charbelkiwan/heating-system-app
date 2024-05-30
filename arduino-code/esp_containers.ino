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

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);


  connectToWiFi();
  startServer();

  // Start mDNS with hostname "esp_containers"
  if (!MDNS.begin("esp_containers")) {
    Serial.println("Error setting up MDNS responder!");
  }
}

void loop() {
  MDNS.update();
  server.handleClient();  // Handle incoming HTTP requests

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)


  if (arduinoSerial.available()) {
    String command = arduinoSerial.readStringUntil('\n');
    // String command = arduinoSerial.readString();
    Serial.println(command);
    if (command.equals("Start the car\r")) {
      Serial.println("Sending Start the car to server");
      delay(1000);
      sendToServer("/start_car_forward");
    } else if (command.equals("Stop the car\r")) {
      Serial.println("Sending Stop the car to server");
      delay(1000);
      sendToServer("/stop_car");
    }
      else if (command.equals("1\r")) {
        Serial.println("Floor number received from server: 1");
        sendToArduino(1);
    }
      else if (command.equals("2\r")) {
        Serial.println("Floor number received from server: 1");
        sendToArduino(2);
    }
      else if (command.equals("Car arrived to containers\r")) {
        Serial.println("Sending Car arrived to containers to server");
        delay(1000);
        sendToServer("/car_arrived_to_containers");
    }
  }
}
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

  server.begin();  // Start the server
}

void sendToServer(String message) {
  HTTPClient http;

  // Construct the complete URL
  String url = "http://" + String(serverIP) + ":" + String(serverPort) + message;

  // Use the correct begin method
  http.begin(client, url);  // Pass the WiFiClient object and the URL

  http.addHeader("Content-Type", "text/plain");

  int httpResponseCode = http.POST(message);

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
