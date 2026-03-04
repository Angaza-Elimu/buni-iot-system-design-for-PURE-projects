/*
   Project: Servo Motor Control Using a Push Button
   Description: This project controls a servo motor using a push button. When the button
                is pressed, the servo moves full swing clockwise. When released, it moves
                back full swing counter-clockwise.
*/

#include <ESP32Servo.h>  // ESP32-specific Servo library

const int buttonPin = 5;  // Push button pin
const int servoPin = 20;   // Servo motor pin

Servo myServo;  // Create a Servo object

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);  // Use internal pull-up resistor
    myServo.attach(servoPin);  // Attach servo to pin
}

void loop() {
    if (digitalRead(buttonPin) == LOW) {
        myServo.write(180);  // Move full-swing clockwise when pressed
    } else {
        myServo.write(0);    // Move back full-swing counterclockwise when released
    }
}
