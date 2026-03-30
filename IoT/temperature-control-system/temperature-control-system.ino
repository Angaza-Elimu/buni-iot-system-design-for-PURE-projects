// ======================= PROJECT: Temperature Control System =======================
// Components: ESP32, DHT22, OLED, Push Button, RGB LED, Fan Motor, Servo Motor
// Functionality: Controls a fan motor based on temperature readings & a servo motor based on humidity
// FUNCTIONALITIES
// - DHT22 Sensor measures temperature and humidity.
// - OLED Display shows real-time temperature and humidity values.
// - Fan is activated when the temperature exceeds 29°C.
// - Push Button toggles the system ON/OFF(when long-pressed) and stops the servo & fan when double-pressed.
// - RGB LED visually indicates system states:
//   - BLUE → Blinking every 2s (System ON).
//   - GREEN → Temperature ≤ 24°C (Cool).
//   - YELLOW → Temperature 25°C - 27.9°C (Moderate).
//   - RED → Temperature ≥ 29°C (High, Servo ON).
// - Serial Monitor logs all actions and system statuses for debugging.

// Author: Angaza Elimu - Buni Team | May, 2025

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <ESP32Servo.h>

// ==================== PIN DEFINITIONS ====================
#define DHTPIN 7           // DHT22 data pin
#define DHTTYPE DHT22      // DHT sensor type
#define OLED_RESET -1      // OLED reset (not used)
#define I2C_SCL 2  //SCL
#define I2C_SDA 42  //SDA
#define BUTTON_PIN 4       // Push button pin
#define RED_PIN 19         // RGB LED - Red
#define GREEN_PIN 33       // RGB LED - Green
#define BLUE_PIN 16        // RGB LED - Blue
#define INA_PIN 38         // FAN PINS
#define INB_PIN 35         // FAN PINS
#define SERVO_PIN 20        // Servo motor pin

// ==================== OBJECT INITIALIZATION ====================
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
Servo servoMotor;

// ==================== SYSTEM VARIABLES ====================
const float MAX_TEMP_THRESHOLD = 29.0;
const float MIN_TEMP_THRESHOLD = 27.0;
const float MAX_HUMID_THRESHOLD = 85.0;

bool systemActive = false;
bool fanRunning = false;
unsigned long lastButtonPress = 0;
unsigned long debounceDelay = 50;
int buttonPressCount = 0;

// ==================== FUNCTION TO CONTROL RGB LED ====================
void setRGB(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

// ==================== FUNCTION TO SWEEP FAN ====================
void spinFan() {
  if (fanRunning) {
    // Spin the fan
    digitalWrite(INA_PIN,LOW);
    Serial.println("Fan spinning");
    digitalWrite(INB_PIN,HIGH);
  }
  else{
    // Stop the fan
    digitalWrite(INA_PIN,LOW);
    Serial.println("Fan stopped");
    digitalWrite(INB_PIN,LOW);
  }
}

// ==================== FUNCTION TO CONTROL SERVO ====================
void controlServo(float humidity) {
  if (humidity >= MAX_HUMID_THRESHOLD) {
    Serial.println("High Humidity! Sweeping Servo to 180 degrees");
    servoMotor.write(180);
    delay(1000);
  } else if (humidity < MAX_HUMID_THRESHOLD - 1) {
    Serial.println("Humidity Normal. Returning Servo to 0 degrees");
    servoMotor.write(0);
    delay(1000);
  }
}

// ==================== FUNCTION TO READ TEMPERATURE ====================
float readTemperature() {
  float temp = dht.readTemperature();
  return (isnan(temp)) ? -1 : temp;  // Handle sensor read error
}

// ==================== FUNCTION TO READ HUMIDITY ====================
float readHumidity() {
  float hum = dht.readHumidity();
  return (isnan(hum)) ? -1 : hum;
}

// ==================== FUNCTION TO UPDATE OLED ====================
void updateOLED(float temperature, float humidity) {
  display.clearDisplay();

  // Title
  display.setTextSize(1); // Smaller title to save space
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temp Control System");

  // Divider line
  display.setCursor(0, 10);
  display.println("--------------------");

  // Temperature
  display.setTextSize(2); // Larger text for key values
  display.setCursor(0, 20);
  display.print("T: ");
  display.print(temperature, 1); // 1 decimal place
  display.println("C");

  // Humidity
  display.setCursor(0, 45);
  display.print("H: ");
  display.print(humidity, 1);
  display.println("%");

  display.display();
}

// ==================== FUNCTION TO HANDLE BUTTON PRESS ====================
void handleButtonPress() {
  static unsigned long firstPressTime = 0; // Store time of first press

  if (millis() - lastButtonPress > debounceDelay) {
    lastButtonPress = millis();
    buttonPressCount++;

    Serial.print("Button Press Count: ");
    Serial.println(buttonPressCount); // Print the number of detected button presses

    if (buttonPressCount == 1) {
      firstPressTime = millis(); // Record time of first press
    }
    else if (buttonPressCount == 2) {
      if (millis() - firstPressTime <= 300) { // Double press detected within 300ms
        fanRunning = false;  // Stop the stop
        spinFan();
      }
      buttonPressCount = 0; // Reset press count after action
    }
  }
}

// ==================== SETUP FUNCTION ====================
void setup() {
  Serial.begin(115200);
  dht.begin();
  servoMotor.attach(SERVO_PIN);

  // Initialize I2C with custom SDA and SCL
  Wire.begin(I2C_SDA, I2C_SCL);  // Important fix for custom I2C pins

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed to initialize.");
    while (1);
  }
  display.display();
  delay(2000);

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // pins functionality definition
  pinMode(INA_PIN, OUTPUT);
  pinMode(INB_PIN, OUTPUT);

  // Initial state
  setRGB(0, 0, 0); // RGB off
}

// ==================== LOOP FUNCTION ====================
void loop() {
  // read DHT22 sensor value and display on OLED screen
  float temperature = readTemperature();
  float humidity = dht.readHumidity();
  updateOLED(temperature, humidity);

  if (temperature == -1) {
    Serial.println("DHT22 Sensor Error!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Button handling
  if (digitalRead(BUTTON_PIN) == LOW) {
    handleButtonPress();
  }

  // Handle system activation/deactivation
  if (buttonPressCount == 1) {
    systemActive = !systemActive;
    Serial.print("System ");
    Serial.println(systemActive ? "Activated" : "Deactivated");
    buttonPressCount = 0;
  }

  // Control Fan Motor and RGB LED
  if (systemActive) {
    setRGB(0, 0, 255); // Blue (System ON)

    if (temperature >= MAX_TEMP_THRESHOLD) {
      Serial.println("High Temp! Activating Cooling...");
      fanRunning = true; // start fan spinning
      setRGB(255, 0, 0); // Red (High Temp)
      spinFan();
    }
    else if (temperature >= MAX_TEMP_THRESHOLD - 1) {
      Serial.println("Moderate Temperature.");
      setRGB(255, 255, 0); // Yellow (Moderate Temp)
    }
    else if (temperature < MAX_TEMP_THRESHOLD) {
      Serial.println("Cool Temperature.");
      fanRunning = false; // stop fan
      setRGB(0, 255, 0); // Green (Cool Temp)
      spinFan();
    }
    else if (temperature <= MIN_TEMP_THRESHOLD) {
      Serial.println("Cool Temperature.");
      setRGB(0, 255, 0); // Green (Cool Temp)
    }

    controlServo(humidity);

  } else {
    setRGB(0, 0, 0); // Turn off LED (System OFF)

    // stop the fan
    fanRunning = false; // change fan state
    spinFan();
  }

  delay(2000);  // System update delay
}
