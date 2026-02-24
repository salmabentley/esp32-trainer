#include <Arduino.h>
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

// Pins (Matches your previous code)
#define TRIG1 4
#define ECHO1 16
#define TRIG2 18
#define ECHO2 5
#define LED_PIN 13
#define BUZZER 14
#define PUNCH_PIN 32

Adafruit_NeoPixel strip(5, LED_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar1(TRIG1, ECHO1, 200);
NewPing sonar2(TRIG2, ECHO2, 200);

void setup() {
    Serial.begin(115200);
    
    // 1. Test LCD
    lcd.init();
    lcd.backlight();
    lcd.print("SYSTEM CHECK");
    delay(1000);

    // 2. Test LEDs
    strip.begin();
    strip.setBrightness(50);
    Serial.println("Testing LEDs...");
    lcd.setCursor(0,1); lcd.print("LEDs: RGB Mix   ");
    strip.fill(strip.Color(255, 0, 0)); strip.show(); delay(500);
    strip.fill(strip.Color(0, 255, 0)); strip.show(); delay(500);
    strip.fill(strip.Color(0, 0, 255)); strip.show(); delay(500);
    strip.fill(strip.Color(0, 0, 0));   strip.show();

    // 3. Test Buzzer
    Serial.println("Testing Buzzer...");
    lcd.clear(); lcd.print("Buzzer: BEEP");
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, HIGH); delay(200); digitalWrite(BUZZER, LOW);

    lcd.clear();
    lcd.print("Sensors Live:");
}

void loop() {
    // 4. Test Ultrasonics
    int d1 = sonar1.ping_cm();
    int d2 = sonar2.ping_cm();

    // 5. Test Touch
    int t = touchRead(PUNCH_PIN);

    // Display to LCD
    lcd.setCursor(0, 0);
    lcd.print("S1:"); lcd.print(d1); lcd.print(" S2:"); lcd.print(d2); lcd.print("  ");
    lcd.setCursor(0, 1);
    lcd.print("Touch: "); lcd.print(t); lcd.print("    ");

    // Print to Serial for debugging
    Serial.printf("Sonar1: %d cm | Sonar2: %d cm | Touch: %d\n", d1, d2, t);

    delay(200); // Fast enough to be responsive
}