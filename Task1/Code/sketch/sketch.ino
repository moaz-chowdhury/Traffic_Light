const int redPin = 9;
const int yellowPin = 10;
const int greenPin = 11;

enum TrafficState { RED, YELLOW, GREEN };

class TrafficLight {
private:
    int redPin;
    int yellowPin;
    int greenPin;
    TrafficState currentState;
    bool wasGreenBefore;

public:
    TrafficLight(int rPin, int yPin, int gPin)
        : redPin(rPin), 
        yellowPin(yPin), 
        greenPin(gPin), 
        currentState(RED), 
        wasGreenBefore(false) {}

    void setup() {
        pinMode(redPin, OUTPUT);
        pinMode(yellowPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        setState(RED);  // Start with the Red light on
    }

    void update() {
        switch (currentState) {
            case RED:
                setState(RED);
                delay(20000); // Stay Red for 20 seconds
                setState(YELLOW);
               wasGreenBefore = false; // Reset flag
                break;

            case YELLOW:
                setState(YELLOW);
                delay(5000); // Stay Yellow for 5 seconds
                if (wasGreenBefore) {
                    setState(RED);  // Go back to Red after Green
                } else {
                    setState(GREEN); // Go to Green after Red
                   wasGreenBefore = true; // Set flag for next Yellow transition
                }
                break;

            case GREEN:
                setState(GREEN);
                delay(20000); // Stay Green for 20 seconds
                setState(YELLOW); // Transition to Yellow
                break;
        }
    }

private:
    void setState(TrafficState state) {
        // Turn off all lights initially
        digitalWrite(redPin, LOW);
        digitalWrite(yellowPin, LOW);
        digitalWrite(greenPin, LOW);

        // Turn on the appropriate light
        currentState = state;
        switch (state) {
            case RED:
                digitalWrite(redPin, HIGH);
                break;
            case YELLOW:
                digitalWrite(yellowPin, HIGH);
                break;
            case GREEN:
                digitalWrite(greenPin, HIGH);
                break;
        }
    }
};

// Instantiate the TrafficLight object
TrafficLight trafficLight(redPin, yellowPin, greenPin);

void setup() {
    trafficLight.setup();
}

void loop() {
    trafficLight.update();
}
