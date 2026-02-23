#include <Arduino.h>
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <LiquidCrystal_I2C.h>
#include "secrets.h"

//ultrasonic sensors
const int TRIG_PIN_1 = 4;
const int ECHO_PIN_1 = 16;
const int TRIG_PIN_2 = 18;
const int ECHO_PIN_2 = 5;
const int MAX_DISTANCE = 200;

//leds
const int LED_PIN = 13; 

//lcd
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int lcdColumns = 16;
const int lcdRows = 2;

//buzzer
const int BUZZER_PIN = 14;

//capacitative sensors
const int PUNCH_PIN = 32;
const int THRESHOLD = 40;

//game variables
int rounds = 0;
unsigned long startTime;
unsigned long reactionTime;
bool actionActive = false;
const String combinations[] = {"punch", "kick", "punch+kick", "kick+punch"};

//server setup
// AsyncWebServer server(80);

// // This function replaces %PLACEHOLDERS% in HTML with real data
// String processor(const String& var){
//   if(var == "DISTANCE") return String(random(10, 50)); // Replace with your sonar.ping_cm()
//   if(var == "STATUS") return "Fighter Ready";
//   return String();
// }

Adafruit_NeoPixel strip(5, LED_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

//functions for actuators
void setLED(String colour) {
    if (colour == "red") {
        strip.fill(strip.Color(255,0,0));
    } else if (colour == "green"){
        strip.fill(strip.Color(0,255,0));
    } else if (colour == "orange"){
        strip.fill(strip.Color(255,165,0));
    } else {
        strip.fill(strip.Color(0,0,0)); // default to off
    }
    strip.show();
}

void buzz(int duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

void displayMessage(String message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

void startRound() {
    rounds++;
    displayMessage("Round " + String(rounds));
    delay(2000);

    displayMessage("3...");
    buzz(500);
    setLED("red");
    delay(1000);

    displayMessage("2...");
    buzz(500);
    setLED("orange");
    delay(1000);

    displayMessage("1...");
    buzz(500);
    setLED("green");
    delay(1000);

    displayMessage("Get Ready...");
    delay(random(0, 5000)); 
    startTime = millis();
    actionActive = true;
}

void runTrainer() {
    int choice = random(0, sizeof(combinations)/sizeof(combinations[0])); 

    Serial.print("Target Action: ");
    Serial.println(combinations[choice]);

    switch (choice) {
        case 0: // "punch"
        executePunch();
        break;

        case 1: // "kick"
        executeKick();
        break;

        case 2: // "punch+kick"
        executePunch();
        executeKick();
        break;

        case 3: // "kick+punch"
        executeKick();
        executePunch();
        break;
    }
    }

void executePunch() {
    setLED("green"); 
    buzz(100);
}

void executeKick() {
    setLED("red");
    buzz(100);
}

void setup() {
    Serial.begin(115200);


    //led strips
    strip.begin();
    strip.setBrightness(50); 
    strip.show();

    //ultrasonic sensors
    // pinMode(TRIG_PIN_1, OUTPUT);
    // pinMode(ECHO_PIN_1, INPUT);
    // pinMode(ECHO_PIN_2, INPUT);
    // pinMode(TRIG_PIN_2, OUTPUT);

    //buzzer
    pinMode(BUZZER_PIN, OUTPUT);

    //lcd display
    lcd.init();              
    lcd.backlight();
}

void loop() {

}
