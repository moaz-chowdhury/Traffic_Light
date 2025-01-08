#include <Wire.h>

const int D3RedPin = 9;
const int D3YellowPin = 10;
const int D3GreenPin = 11;

void setup() {
    Wire.begin(10);  // Address 10 for D3 traffic light
    Wire.onReceive(receiveEvent);
    pinMode(D3RedPin, OUTPUT);
    pinMode(D3YellowPin, OUTPUT);
    pinMode(D3GreenPin, OUTPUT);
    
    // Initial state: Red
    digitalWrite(D3RedPin, HIGH);
    digitalWrite(D3YellowPin, LOW);
    digitalWrite(D3GreenPin, LOW);
    Serial.begin(9600);
}

void loop() {
    // State controlled by I2C commands
}

void receiveEvent(int howMany) {
    char command = Wire.read();
    
    switch(command) {
        case 'R':  // Red
            digitalWrite(D3RedPin, HIGH);
            digitalWrite(D3YellowPin, LOW);
            digitalWrite(D3GreenPin, LOW);
            Serial.println("D3: Red");
            break;
            
        case 'Y':  // Yellow
            digitalWrite(D3RedPin, LOW);
            digitalWrite(D3YellowPin, HIGH);
            digitalWrite(D3GreenPin, LOW);
            Serial.println("D3: Yellow");
            break;
            
        case 'G':  // Green
            digitalWrite(D3RedPin, LOW);
            digitalWrite(D3YellowPin, LOW);
            digitalWrite(D3GreenPin, HIGH);
            Serial.println("D3: Green");
            break;
    }
}