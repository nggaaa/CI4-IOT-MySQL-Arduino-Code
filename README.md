# CI4 Internet Of Things With MySQL Arduino Code
For Server Or Web Code visit this [repository.](https://github.com/saronggos/ci4-iot-mysql)

Im using HTTPS for communication device with server

### Library 
- DHT (For Example in send data temperature and humidity to database)

### Full Source ESP8266

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "DHT.h"



// Configuration
const char* ssid = "Dlovecollection blok 9";
const char* pass = "Islamhargamati";
const char* host = "https://saronggos-iot.000webhostapp.com";

String api = "7e9c973ae0bb37f3c3d7d91845b0b63fed9c9598";
String key = "34126de04c4d107221df8a23e2be2467";

#define DHTPIN 0
#define DHTTYPE DHT11

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const uint8_t fingerprint[20] = {0x5b, 0xfb, 0xd1, 0xd4, 0x49, 0xd3, 0x0f, 0xa9, 0xc6, 0x40, 0x03, 0x34, 0xba, 0xe0, 0x24, 0x05, 0xaa, 0xd2, 0xe2, 0x01};


ESP8266WiFiMulti WiFiMulti;
DHT dht(DHTPIN, DHTTYPE);
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Dlovecollection blok 9", "Islamhargamati");
}

void loop() {
    // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
    // client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");

    // DHT Read
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    
    String apiUrl = host;
    apiUrl += "/hardware?";
    apiUrl += "api=" + api;
    apiUrl += "&key=" + key;
    apiUrl += "&temp=" + String(temp);
    apiUrl += "&hum=" + String(hum);

    if (https.begin(*client, apiUrl)) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 1h before next round...");
  delay(3600000);
}
```

