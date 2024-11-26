#include <Arduino.h>

// Variables for the traffic light
const int trafficRedPin = 9;
const int trafficYellowPin = 10;
const int trafficGreenPin = 11;

// Variables for the pedestrian light
const int pedRedPin = 12;
const int pedGreenPin = 13;

// Button pin for pedestrian crossing
const int buttonPin = 2;

// Timing durations
const unsigned long greenDuration = 5000;
const unsigned long yellowDuration = 5000;
const unsigned long redDuration = 20000;

// Debounce timing
const unsigned long debounceDelay = 50;

// Declaration of PedestrianLight class
class PedestrianLight;

// TrafficLight class definition
class TrafficLight {
private:
    int redPin, yellowPin, greenPin; // Traffic light pins
    unsigned long previousMillis; // State and timing variables
    bool pedestrianWaiting;
    bool wasGreenBefore; // Flag to track if the previous state was GREEN
    enum TrafficState { RED, YELLOW, GREEN };
    TrafficState currentState;

public:
    // Constructor
    TrafficLight(int rPin, int yPin, int gPin)
        : redPin(rPin), yellowPin(yPin), greenPin(gPin),
          previousMillis(0), pedestrianWaiting(false),
          wasGreenBefore(true), currentState(GREEN) {
        // Initialize pin modes
        pinMode(redPin, OUTPUT);
        pinMode(yellowPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        digitalWrite(greenPin, HIGH); // Start with the green light on
    }

    // Method to set pedestrian waiting flag
    void setPedestrianWaiting(bool waiting) {
        pedestrianWaiting = waiting;
    }

    // Method to update traffic light states
    void update(unsigned long currentMillis, PedestrianLight &pedLight);

private:
    // Method to handle the GREEN state
    void handleGreenState(unsigned long currentMillis) {
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        if (currentMillis - previousMillis >= greenDuration) {
            previousMillis = currentMillis;
            digitalWrite(greenPin, LOW);
            if (pedestrianWaiting) {
                currentState = YELLOW;
                wasGreenBefore = true;
            } else {
                previousMillis = millis();
                digitalWrite(greenPin, HIGH);
            }
        }
    }

    // Method to handle the YELLOW state
    void handleYellowState(unsigned long currentMillis) {
        digitalWrite(yellowPin, HIGH);
        if (currentMillis - previousMillis >= yellowDuration) {
            previousMillis = currentMillis;
            digitalWrite(yellowPin, LOW);
            currentState = wasGreenBefore ? RED : GREEN;
        }
    }

    // Method to handle the RED state
    void handleRedState(unsigned long currentMillis, PedestrianLight &pedLight);
};

// PedestrianLight class definition
class PedestrianLight {
private:
    int redPin, greenPin;

public:
    // Constructor
    PedestrianLight(int rPin, int gPin)
        : redPin(rPin), greenPin(gPin) {
        // Initialize pin modes
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        digitalWrite(redPin, HIGH); // Start with the red light on
    }

    // Method to activate the green pedestrian light
    void activate() {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
    }

    // Method to deactivate the green pedestrian light
    void deactivate() {
        digitalWrite(greenPin, LOW);
        digitalWrite(redPin, HIGH);
    }
};

// Method implementation for TrafficLight class that requires PedestrianLight
void TrafficLight::update(unsigned long currentMillis, PedestrianLight &pedLight) {
    switch (currentState) {
        case GREEN:
            handleGreenState(currentMillis);
            break;
        case YELLOW:
            handleYellowState(currentMillis);
            break;
        case RED:
            handleRedState(currentMillis, pedLight);
            break;
    }
}

void TrafficLight::handleRedState(unsigned long currentMillis, PedestrianLight &pedLight) {
    digitalWrite(redPin, HIGH);
    pedLight.activate(); // Activate the pedestrian light
    if (currentMillis - previousMillis >= redDuration) {
        previousMillis = currentMillis;
        digitalWrite(redPin, LOW);
        pedLight.deactivate(); // Deactivate the pedestrian light
        pedestrianWaiting = false;
        currentState = YELLOW;
        wasGreenBefore = false;
    }
}

// Main function and global variables
TrafficLight trafficTrafficLight(trafficRedPin, trafficYellowPin, trafficGreenPin);
PedestrianLight pedestrianLight(pedRedPin, pedGreenPin);
unsigned long lastDebounceTime = 0;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    unsigned long currentMillis = millis();
    trafficTrafficLight.update(currentMillis, pedestrianLight);

    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && (currentMillis - lastDebounceTime) > debounceDelay) {
        trafficTrafficLight.setPedestrianWaiting(true);
        lastDebounceTime = currentMillis;
    }
}