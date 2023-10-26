#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

ESP8266WebServer server(80);

const int ledPin = D6;
bool ledState = LOW; // Declare and initialize ledState
DHT dht14(D4, DHT11);

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void) {
  Serial.begin(115200);
  init_wifi("_4most", "55556666");

  pinMode(ledPin, OUTPUT);

  dht14.begin();

  server.on("/", HTTP_GET, []() {
    float t = dht14.readTemperature();
    float h = dht14.readHumidity();

    String html = "<html><head><body>";
      html += "<center>64113889 Supanut Koomruck</center><br>";
      html += "<p style='text-align: center;'>LED Status: <span style='color: ";
      if (ledState == HIGH) {
      html += "green;'>On</span>";
      } else {
      html += "red;'>Off</span>";
      }
      html += "</p>";
      html += "<center>Humidity = " + String(h) + " %</center>";
      html += "<center>Temperature = " + String(t) + " C</center>";
      html += "</p>";
      html += "<center><form action='/TurnOn' method='get'><input type='submit' value='Turn On'  text-align: center; text-decoration:></form>";
      html += "<form action='/TurnOff' method='get'><input type='submit' value='Turn Off'  text-align: center; text-decoration:></form></center>";
      html += "</div></body></html>";


      html += "<script>";
      html += "function updateData() {";
      html += "  var xhr = new XMLHttpRequest();";
      html += "  xhr.open('GET', '/data', true);";
      html += "  xhr.onreadystatechange = function() {";
      html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
      html += "      var data = JSON.parse(xhr.responseText);";
      html += "      document.getElementById('temperature').textContent = data.temperature + ' C';";
      html += "      document.getElementById('humidity').textContent = data.humidity + ' %';";
      html += "    }";
      html += "  };";
      html += "  xhr.send();";
      html += "}";
      html += "setInterval(updateData, 500);";
      html += "</script>";

    server.send(200, "text/html", html);
  });

  server.on("/TurnOn", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH);
    ledState = HIGH; // Update ledState
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/TurnOff", HTTP_GET, []() {
    digitalWrite(ledPin, LOW);
    ledState = LOW; // Update ledState
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/data", HTTP_GET, []() {
    float temperature = dht14.readTemperature();
    float humidity = dht14.readHumidity();

    String data = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    server.send(200, "application/json", data);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
