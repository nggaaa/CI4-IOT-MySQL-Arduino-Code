//Include All Libraries
#include <WiFi.h>
#include "DHT.h"

//Configure Connection
const char* ssid     = "SSID";
const char* password = "password";
const char* host = "hostname";
const int port = 80;

// DHT Config
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 5000ms = 5s)
const long timeoutTime = 5000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Networking
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  WiFiClient client;
  
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }

  // DHT get temp dan hum
  float temp = dht.readTemperature(false);
  float humid = dht.readHumidity();

  // f
  String apiUrl = "/hardware?";
  apiUrl += "api=5299d80c61ee33e5fb5b8a193fae1798d7ffb248";
  apiUrl += "&key=9b37226a1880b64a98df56c0d30056f0";
  apiUrl += "&temp="+String(temp);
  apiUrl += "&hum="+String(humid);
  

  // Set header Request
  client.print(String("GET ") + apiUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  //If disconnected during the specified time, the system will stop
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 3000) {
      Serial.println(">>> Client Timeout !");
      Serial.println(">>> Operation failed !");
      client.stop();
      return;
    }
  }

  // Read response from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  // 
   delay(60000);
}
