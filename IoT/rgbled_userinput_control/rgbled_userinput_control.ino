/*
  Blink RGB LED Based on User Input
  Enter:
  - 'r' or 'R' for Red
  - 'g' or 'G' for Green
  - 'b' or 'B' for Blue
  The program waits for user input via the grb
  Serial Monitor.
*/

// Define RGB LED pins (adjust these based on your wiring)
const int redPin   = 16;
const int greenPin = 33;
const int bluePin  = 19;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud

  // Set RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("Enter r, g, or b to blink the corresponding LED:");
}

void loop() {
  // Wait for user input from Serial Monitor
  if (Serial.available() > 0) {
    char userInput = Serial.read();
    userInput = tolower(userInput);  // Convert to lowercase

    if (userInput == 'r') {
      Serial.println("Blinking Red LED...");
      blinkColor(255, 0, 0);
    } else if (userInput == 'b') {
      Serial.println("Blinking Green LED...");
      blinkColor(0, 0, 255);
    } else if (userInput == 'g') {
      Serial.println("Blinking Blue LED...");
      blinkColor(0, 255, 0);
    } else {
      Serial.println("Invalid input. Please enter r, g, or b.");
    }

    // Prompt for next input
    Serial.println("Enter r, g, or b to blink the corresponding LED:");
  }
}

// Function to blink the RGB LED with the specified color
void blinkColor(int redValue, int greenValue, int blueValue) {
  // Blink 3 times
  for (int i = 0; i < 3; i++) {
    // Turn LED on
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
    delay(1000);  // On for 1000 ms

    // Turn LED off
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(1000);  // Off for 1000 ms
  }
}
