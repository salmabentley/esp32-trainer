#include <Arduino.h>
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <LiquidCrystal_I2C.h>
#include "secrets.h"
#include <queue>

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
const int THRESHOLD = 20;

//game variables
int rounds = 0;
unsigned long startTime;
unsigned long reactionTime;
bool actionActive = false;
const String combinations[] = {"punch", "kick", "punch+kick", "kick+punch"};
std::queue<String> actionQueue;

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
// NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

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
    tone(BUZZER_PIN, 500, duration);
}

void displayMessage(String message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
    for (int i=0; i < lcdColumns; i++) {
        message = " " + message;  
    } 
    message = message + " "; 
    for (int pos = 0; pos < message.length(); pos++) {
        lcd.setCursor(0, row);
        lcd.print(message.substring(pos, pos + lcdColumns));
        delay(delayTime);
    }
}

     // When the combo started
unsigned long lastUpdate;    // Last time we refreshed the LCD
const int refreshRate = 50;  // Refresh LCD every 50ms (prevents flickering)

void runTimer() {
    unsigned long currentMillis = millis();
    unsigned long elapsed = currentMillis - startTime;

    // Only update the display every 50ms
    if (currentMillis - lastUpdate >= refreshRate) {
      lastUpdate = currentMillis;

      // Format: "Time: 1234ms"
        lcd.setCursor(0, 1); 
        lcd.print("Time: ");
        lcd.print(elapsed);
        lcd.print("ms    "); // Spaces clear old digits
    }
}

//technique functions

void executePunch() {
    actionQueue.push("punch");
    setLED("green"); 
    buzz(100);
}

void executeKick() {
    actionQueue.push("kick");
    setLED("red");
    buzz(100);
}

void checkAction(String actionType) {
    // 1. Check if the action matches the required step in the combo
    if (!actionQueue.empty() && actionType == actionQueue.front()) {
        displayMessage("Correct!");
        setLED("green");
        buzz(500);
        
    } 
    else {
        displayMessage("Wrong move!");
        setLED("red");
        buzz(500);
    }
    actionQueue.pop();
}

void initiateSensors() {
    int distance = sonar1.ping_cm();

    if (distance > 0 && distance < 7) {
        checkAction("kick");
        Serial.println(distance);
        delay(250); 
    }
// (sonar2.ping_cm() > 0 && sonar2.ping_cm() < 7))
    int punchValue = touchRead(PUNCH_PIN);
    if (punchValue < THRESHOLD) { 
        checkAction("punch");
        Serial.println("punch detected");
        delay(250); 
    }
}

//game functions
void endTrainer() {
    displayMessage("Training Complete!");
    setLED("off");
    scrollText(1, "Check the web interface for your stats!", 300, 16);
    while (true); //loop forever to end trainer
}


void runTrainer() {
    int choice = random(0, sizeof(combinations)/sizeof(combinations[0])); 

    Serial.print("Target Action: ");
    Serial.println(combinations[choice]);

    switch (choice) {
        case 0: // "punch"
        executePunch();
        displayMessage("Kizami!");
        break;

        case 1: // "kick"
        executeKick();
        displayMessage("Mawashi!");
        break;

        case 2: // "punch+kick"
        executePunch();
        executeKick();
        displayMessage("Punch + Kick!");
        break;

        case 3: // "kick+punch"
        executeKick();
        executePunch();
        displayMessage("Kick + Punch!");
        break;
    }
}

void startRound() {
    while(!actionQueue.empty()) actionQueue.pop();
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
    runTrainer();
}



void setup() {
    Serial.begin(115200);   
    randomSeed(analogRead(0));

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
    if (!actionActive && rounds < 5) {
        startRound(); // This sets actionActive = true and loads the queue
    } else if (actionActive) {
        initiateSensors();
        runTimer();

        if (actionQueue.empty()) {
            reactionTime = millis() - startTime;
            actionActive = false; 
            displayMessage("Reaction Time:");
            lcd.setCursor(0,1);
            lcd.print(String(reactionTime) + " ms");
            delay(3000); 
        }
    } else if (rounds >=5) {
        endTrainer();
    }
}
