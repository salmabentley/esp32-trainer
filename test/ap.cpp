#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#define LED_RED 25

const char *ssid = "esp32";
const char *password = "password";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.begin();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.print("<title>ESP32 Web Server</title></head><body><h1>ESP32 Web Server</h1>");
            client.print("<p><a href=\"/LED=ON\"><button style=\"height:100px;width:100px;font-size:24px;\">LED ON</button></a></p>");
            client.print("<p><a href=\"/LED=OFF\"><button style=\"height:100px;width:100px;font-size:24px;\">LED OFF</button></a></p>");
            client.print("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c!='\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /LED=ON")) {
          digitalWrite(LED_RED, HIGH);
        } else if (currentLine.endsWith("GET /LED=OFF")) {
          digitalWrite(LED_RED, LOW);
        }
      }
    }

  }
}

