#include <Wire.h>

const int pedRedPin = 12;
const int pedGreenPin = 13;
const int pedBlinkingOrangePin = 4;  // New pin for blinking orange

unsigned long previousBlinkMillis = 0;
const unsigned long blinkDuration = 500;  // 0.5 seconds for blinking
bool blinkState = false;
bool isGreen = false;

void setup() {
    Wire.begin(9);  // Address 9 for pedestrian light
    Wire.onReceive(receiveEvent);
    pinMode(pedRedPin, OUTPUT);
    pinMode(pedGreenPin, OUTPUT);
    pinMode(pedBlinkingOrangePin, OUTPUT);
    
    // Initial state: Red
    digitalWrite(pedRedPin, HIGH);
    digitalWrite(pedGreenPin, LOW);
    digitalWrite(pedBlinkingOrangePin, LOW);
    Serial.begin(9600);
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Handle blinking orange light
    if (isGreen) {
        if (currentMillis - previousBlinkMillis >= blinkDuration) {
            blinkState = !blinkState;
            digitalWrite(pedBlinkingOrangePin, blinkState);
            previousBlinkMillis = currentMillis;
        }
    } else {
        digitalWrite(pedBlinkingOrangePin, LOW);
    }
}

void receiveEvent(int howMany) {
    char command = Wire.read();
    
    switch(command) {
        case 'G':  // Green
            digitalWrite(pedRedPin, LOW);
            digitalWrite(pedGreenPin, HIGH);
            isGreen = true;
            Serial.println("Pedestrian: Green");
            break;
            
        case 'R':  // Red
            digitalWrite(pedRedPin, HIGH);
            digitalWrite(pedGreenPin, LOW);
            isGreen = false;
            Serial.println("Pedestrian: Red");
            break;
    }
}