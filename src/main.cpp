// #include <Arduino.h>
// #include <NewPing.h>
// #include <Adafruit_NeoPixel.h>
// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <LittleFS.h>
// #include "secrets.h"

// //ultrasonic sensors
// const int TRIG_PIN_1 = 4;
// const int ECHO_PIN_1 = 16;
// const int TRIG_PIN_2 = 18;
// const int ECHO_PIN_2 = 5;

// //leds
// const int LED_PIN = 13; 

// //lcd
// const int SDA_PIN = 21;
// const int SCL_PIN = 22;

// //buzzer
// const int BUZZER_PIN = 14;

// //capacitative sensors
// const int PUNCH_PIN = 32;

// //server setup
// // AsyncWebServer server(80);

// // // This function replaces %PLACEHOLDERS% in HTML with real data
// // String processor(const String& var){
// //   if(var == "DISTANCE") return String(random(10, 50)); // Replace with your sonar.ping_cm()
// //   if(var == "STATUS") return "Fighter Ready";
// //   return String();
// // }

// Adafruit_NeoPixel strip(5, LED_PIN, NEO_GRB + NEO_KHZ800);

// void setup() {
//     Serial.begin(115200);


//     //led strips
//     strip.begin();
//     strip.setBrightness(50); 
//     strip.show();

//     //ultrasonic sensors
//     pinMode(TRIG_PIN_1, OUTPUT);
//     pinMode(ECHO_PIN_1, INPUT);
//     pinMode(ECHO_PIN_2, INPUT);
//     pinMode(TRIG_PIN_2, OUTPUT);

//     //buzzer
//     pinMode(BUZZER_PIN, OUTPUT);

//     //lcd display
    
// }

// void loop() {

// }

#include <Wire.h>
#include <Arduino.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}