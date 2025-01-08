#include <Wire.h>

// Traffic light pins for D1
const int D1RedPin = 9;
const int D1YellowPin = 10;
const int D1GreenPin = 11;
const int D1BlinkingOrangePin = 5;  // New pin for blinking orange
const int buttonPin = 2;

// Timing durations
const unsigned long greenDuration = 5000;  // 5 seconds
const unsigned long yellowDuration = 2000; // 2 seconds
const unsigned long redDuration = 5000;    // 5 seconds
const unsigned long blinkDuration = 500;   // 0.5 seconds for blinking

unsigned long previousMillis = 0;
unsigned long previousBlinkMillis = 0;
bool buttonPressed = false;
bool blinkState = false;

enum State {
    D1_GREEN,   // State 1: D1 Green, D3 Red
    D1_YELLOW,  // State 2: D1 Yellow, D3 Yellow
    D1_RED,     // State 3: D1 Red, D3 Green
    D3_YELLOW,  // State 4: D1 Yellow, D3 Yellow
    D3_RED      // State 5: D1 Green, D3 Red
};

State currentState = D1_GREEN;

void setup() {
    Wire.begin();
    pinMode(D1RedPin, OUTPUT);
    pinMode(D1YellowPin, OUTPUT);
    pinMode(D1GreenPin, OUTPUT);
    pinMode(D1BlinkingOrangePin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.begin(9600);
    
    // Initial state: D1 Green, D3 Red
    setD1Lights(HIGH, LOW, LOW); // Green
    notifyD3('R'); // Red
    notifyPed('R'); // Red
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Handle blinking orange light
    if (currentState == D1_GREEN) {
        if (currentMillis - previousBlinkMillis >= blinkDuration) {
            blinkState = !blinkState;
            digitalWrite(D1BlinkingOrangePin, blinkState);
            previousBlinkMillis = currentMillis;
        }
    } else {
        digitalWrite(D1BlinkingOrangePin, LOW);
    }
    
    // Check button press
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
        buttonPressed = true;
    }

    // State machine
    switch (currentState) {
        case D1_GREEN:  // State 1
            if (currentMillis - previousMillis >= greenDuration) {
                setD1Lights(LOW, HIGH, LOW); // Yellow
                notifyD3('Y'); // Yellow
                notifyPed('R'); // Red
                previousMillis = currentMillis;
                currentState = D1_YELLOW;
            }
            break;

        case D1_YELLOW:  // State 2
            if (currentMillis - previousMillis >= yellowDuration) {
                setD1Lights(LOW, LOW, HIGH); // Red
                notifyD3('G'); // Green
                if (buttonPressed) {
                    notifyPed('G'); // Green if button was pressed
                }
                previousMillis = currentMillis;
                currentState = D1_RED;
            }
            break;

        case D1_RED:  // State 3
            if (currentMillis - previousMillis >= redDuration) {
                setD1Lights(LOW, HIGH, LOW); // Yellow
                notifyD3('Y'); // Yellow
                notifyPed('R'); // Red
                previousMillis = currentMillis;
                currentState = D3_YELLOW;
            }
            break;

        case D3_YELLOW:  // State 4
            if (currentMillis - previousMillis >= yellowDuration) {
                setD1Lights(HIGH, LOW, LOW); // Green
                notifyD3('R'); // Red
                notifyPed('R'); // Red
                buttonPressed = false; // Reset button press
                previousMillis = currentMillis;
                currentState = D1_GREEN;
            }
            break;
    }
}

void setD1Lights(int green, int yellow, int red) {
    digitalWrite(D1GreenPin, green);
    digitalWrite(D1YellowPin, yellow);
    digitalWrite(D1RedPin, red);
}

void notifyD3(char state) {
    Wire.beginTransmission(10);
    Wire.write(state);
    Wire.endTransmission();
}

void notifyPed(char state) {
    Wire.beginTransmission(9);
    Wire.write(state);
    Wire.endTransmission();
}