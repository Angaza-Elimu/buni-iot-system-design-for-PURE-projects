/*
  Simple RGB LED Button Control
  - 1 Button connected to digital pin 2 (using internal pull-up)
  - 1 RGB LED connected to PWM pins:
      Red   -> pin 9
      Green -> pin 10
      Blue  -> pin 11
  - Each button press cycles through the states:
      0: LED OFF
      1: Red
      2: Green
      3: Blue
*/

const int buttonPin = 4;   // Button connected to pin 4
const int redPin    = 16;  // Red LED pin
const int greenPin  = 33;  // Green LED pin
const int bluePin   = 19;  // Blue LED pin

int currentState = 0;      // Current LED state (0 to 3)
int lastButtonState = HIGH; // To store the previous button state for debouncing

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Set up button pin with internal pull-up resistor
  pinMode(buttonPin, INPUT);

  // Set the RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("Press the button to change the RGB LED color.");
}

void loop() {
  // Read the current button state
  int reading = digitalRead(buttonPin);

  // Check if the button was pressed (active LOW) and debounced
  if (reading == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce delay
    if (digitalRead(buttonPin) == LOW) {  // Confirm button is still pressed
      // Cycle to the next state
      currentState = (currentState + 1) % 4; // 0 to 3
      Serial.print("Current State: ");
      Serial.println(currentState);

      // Update the LED based on the current state
      updateLED(currentState);

      // Wait until the button is released
      while (digitalRead(buttonPin) == LOW);
      delay(50); // Additional debounce delay after release
    }
  }

  // Save the current button state for next loop iteration
  lastButtonState = reading;
}

// Function to update the RGB LED based on state
void updateLED(int state) {
  // Turn off all colors first
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  // Set the LED color based on the current state
  if (state == 1) {
    // Red
    analogWrite(redPin, 255);
  } else if (state == 2) {
    // Green
    analogWrite(greenPin, 255);
  } else if (state == 3) {
    // Blue
    analogWrite(bluePin, 255);
  }
  // state 0 remains OFF
}
