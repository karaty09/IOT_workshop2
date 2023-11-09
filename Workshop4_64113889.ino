#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

const char* ssid = "IR_Lab";
const char* password = "ccsadmin";

const String writeKey = "74OQKGD78GQTZIGX";  // Replace with your ThingSpeak API Key
const String serverName = "http://api.thingspeak.com/update?api_key=" + writeKey;

DHT dht(D4, DHT11);  // Initialize the DHT sensor on pin D4

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("74OQKGD78GQTZIGX");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();  // Initialize the DHT sensor
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      float temp = dht.readTemperature();  // Read temperature from the DHT sensor
      float humid = dht.readHumidity();    // Read humidity from the DHT sensor

      if (!isnan(temp) && !isnan(humid)) {  // Check if the readings are valid
        String serverPath = serverName + "&field1=" + String(humid, 2) + "&field2=" + String(temp, 2);
        http.begin(client, serverPath.c_str());
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
      } else {
        Serial.println("DHT sensor reading error.");
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
