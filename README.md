# CI4 Internet Of Things With MySQL Arduino Code

For Server Or Web Code visit this [repository.](https://github.com/saronggos/ci4-iot-mysql)

### Library 
- Wifi.h if you using ESP32 or esp8266wifi.h for ESP8266
- DHT (For Example in send data temperature and humidity to database)

### Explanation
1. Include Wi-Fi Library. If your board is ESP32, use 
	```cpp
    #include <WIFI.h>
	```
	and if you using ESP8266, use

	```c++
	#include <ESP8266WiFi.h>
	```
	
2. Include DHT library (if you using dht for this project)  
	```cpp
    #include "DHT.h"
	```
	
3.  Configure Connection

	  ```cpp
	  const char* ssid     = "[change your ssid]";
	  const char* password = "[change your password]";
	  const char* host = "[change to you hostname]";
	  const int port = 80;  //port default is 80. if you have different port, you can change it
	  ```

4. Configure DHT pin and type ```DHT11 or DHT22 or DHT21```

   ```cpp 
   #define DHTPIN 4
   #define DHTTYPE DHT11
   DHT dht(DHTPIN, DHTTYPE);
   ```

5. Set timeout time 

   ```cpp	
   // Current time
   unsigned long currentTime = millis();
   // Previous time
   unsigned long previousTime = 0; 
   // Define timeout time in milliseconds (example: 5000ms = 5s)
   const long timeoutTime = 5000;
   ```

6. Beginning DHT and connect to access point

   ```cpp
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
   ```

7. Connect to hostname

   ```cpp
   WiFiClient client;
     
     if (!client.connect(host, port)) {
       Serial.println("Connection failed");
       return;
     }
   ```

8. Get temperature and humidity from DHT sensor

   ```cpp
   float temp = dht.readTemperature(false);
   float humid = dht.readHumidity();
   ```

9. Make get request and configure api-key

   ```cpp
   String apiUrl = "/hardware?";
     apiUrl += "api=5299d80c61ee33e5fb5b8a193fae1798d7ffb248";
     apiUrl += "&key=9b37226a1880b64a98df56c0d30056f0";
     apiUrl += "&temp="+String(temp);
     apiUrl += "&hum="+String(humid);
     
   
     // Set header Request
     client.print(String("GET ") + apiUrl + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n");
   ```

10. Stop system when disconnect from server

    ```cpp
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 3000) {
          Serial.println(">>> Client Timeout !");
          Serial.println(">>> Operation failed !");
          client.stop();
          return;
        }
      }
    ```

11. Read response from server

    ```cpp
    while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println(line);
      }
    ```

12. Delay for next submit

    ```cpp
    delay(60000);
    ```

### Full Source Code

```cpp
#include <WiFi.h>
#include "DHT.h"

const char* ssid     = "SSID";
const char* password = "password";
const char* host = "hostname";
const int port = 80;

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 5000;

void setup() {
  Serial.begin(115200);
  dht.begin();


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

  float temp = dht.readTemperature(false);
  float humid = dht.readHumidity();

  String apiUrl = "/hardware?";
  apiUrl += "api=5299d80c61ee33e5fb5b8a193fae1798d7ffb248";
  apiUrl += "&key=9b37226a1880b64a98df56c0d30056f0";
  apiUrl += "&temp="+String(temp);
  apiUrl += "&hum="+String(humid);

  client.print(String("GET ") + apiUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 3000) {
      Serial.println(">>> Client Timeout !");
      Serial.println(">>> Operation failed !");
      client.stop();
      return;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
   delay(60000);
}
```

