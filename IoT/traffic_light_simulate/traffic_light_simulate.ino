/*
  Traffic Light Simulation with Buttons and RGB LEDs
  Components:
    - 1 Breadboard
    - 2 RGB LEDs (each with Red, Green, Blue pins)
    - 2 Push buttons
  Operation:
    - Button 1 (pin 2): Controls RGB LED 1 (pins 5, 6, 7) cycling through:
         Red  -> Green -> Blue
    - Button 2 (pin 3): Controls RGB LED 2 (pins 9, 10, 11) cycling through:
         Blue -> Green -> Red (reverse order)
    - On each button press, the selected RGB LED blinks 3 times (1 sec on, 1 sec off)
      before moving to the next color.
*/

const int button1Pin = 4;  // Button 1 for first RGB LED
const int button2Pin = 5;  // Button 2 for second RGB LED

// RGB LED 1 pins (Traffic Light 1)
const int led1RedPin   = 16;
const int led1GreenPin = 33;
const int led1BluePin  = 19;

// RGB LED 2 pins (Traffic Light 2)
const int led2RedPin   = 10;
const int led2GreenPin = 9;
const int led2BluePin  = 11;

// State variables to track current color for each RGB LED (0, 1, 2)
int state1 = 0; // For LED 1: 0=Red, 1=Green, 2=Blue
int state2 = 0; // For LED 2: 0=Blue, 1=Green, 2=Red

void setup() {
  // Start Serial communication for debugging
  Serial.begin(9600);

  // Set push button pins as input with internal pull-up resistors
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  // Set all RGB LED pins as outputs
                              // RGBLED 1
  pinMode(led1RedPin, OUTPUT);
  pinMode(led1GreenPin, OUTPUT);
  pinMode(led1BluePin, OUTPUT);
                              // RGBLED 2
  pinMode(led2RedPin, OUTPUT);
  pinMode(led2GreenPin, OUTPUT);
  pinMode(led2BluePin, OUTPUT);

  Serial.println("Traffic Light Simulation Started");
  Serial.println("Press Button 1 for RGBLED 1 control (Red, Green, Blue)");
  Serial.println("Press Button 2 for RGBLED 2 control (Blue, Green, Red)");
}

void loop() {
  // Check if Button 1 is pressed (active LOW)
  if (digitalRead(button1Pin) == LOW) {
    delay(50); // Debounce
    if (digitalRead(button1Pin) == LOW) {
      cycleLED1(); // Change LED1 state and blink
      // Wait until button is released
      while (digitalRead(button1Pin) == LOW);
      delay(50);
    }
  }

  // Check if Button 2 is pressed (active LOW)
  if (digitalRead(button2Pin) == LOW) {
    delay(50); // Debounce
    if (digitalRead(button2Pin) == LOW) {
      cycleLED2(); // Change LED2 state and blink
      // Wait until button is released
      while (digitalRead(button2Pin) == LOW);
      delay(50);
    }
  }
}

// Function to cycle LED1 through Red, Green, Blue
void cycleLED1() {
  int r = 0, g = 0, b = 0;
  // Set color based on state1
  switch (state1) {
    case 0: // Red
      r = 255;
      break;
    case 1: // Green
      g = 255;
      break;
    case 2: // Blue
      b = 255;
      break;
  }
  Serial.print("RGBLED1 Set Color: ");
  if (state1 == 0) Serial.println("Red");
  else if (state1 == 1) Serial.println("Green");
  else if (state1 == 2) Serial.println("Blue");

  // Blink LED1 3 times
  for (int i = 0; i < 3; i++) {
    setLEDColor(led1RedPin, led1GreenPin, led1BluePin, r, g, b);
    delay(1000);  // LED on for 1 second
    setLEDColor(led1RedPin, led1GreenPin, led1BluePin, 0, 0, 0);
    delay(1000);  // LED off for 1 second
  }
  // Move to the next color (cycle through 0,1,2)
  state1 = (state1 + 1) % 3;
}

// Function to cycle LED2 through Blue, Green, Red (reverse order)
void cycleLED2() {
  int r = 0, g = 0, b = 0;
  // Set color based on state2
  switch (state2) {
    case 0: // Blue
      b = 255;
      break;
    case 1: // Green
      g = 255;
      break;
    case 2: // Red
      r = 255;
      break;
  }
  Serial.print("RGBLED2 Set Color: ");
  if (state2 == 0) Serial.println("Blue");
  else if (state2 == 1) Serial.println("Green");
  else if (state2 == 2) Serial.println("Red");

  // Blink LED2 3 times
  for (int i = 0; i < 3; i++) {
    setLEDColor(led2RedPin, led2GreenPin, led2BluePin, r, g, b);
    delay(1000);  // LED on for 1 second
    setLEDColor(led2RedPin, led2GreenPin, led2BluePin, 0, 0, 0);
    delay(1000);  // LED off for 1 second
  }
  // Move to the next color (cycle through 0,1,2)
  state2 = (state2 + 1) % 3;
}

// Function to set an RGB LED's color using PWM
void setLEDColor(int redPin, int greenPin, int bluePin, int redVal, int greenVal, int blueVal) {
  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);
}
