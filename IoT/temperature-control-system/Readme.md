# Temperature-Controlled System using DHT22, OLED, Servo Motor, Push Button, and RGB LED

## Project Overview
This project implements a **temperature-controlled system** that dynamically responds to temperature changes and user input. The system operates as follows:

- DHT22 Sensor measures temperature and humidity.
- OLED Display shows real-time temperature and humidity values.
- Fan is activated when the temperature exceeds 29°C.
- Push Button toggles the system ON/OFF(when long-pressed) and stops the servo & fan when double-pressed.
- RGB LED visually indicates system states:
  - BLUE → Blinking every 2s (System ON).
  - GREEN → Temperature ≤ 24°C (Cool).
  - YELLOW → Temperature 25°C - 27.9°C (Moderate).
  - RED → Temperature ≥ 29°C (High, Servo ON).
- Serial Monitor logs all actions and system statuses for debugging.

## Summary of Functionality
### **1 DHT22 Sensor**
- Reads **temperature** and **humidity** in real time.
- Provides input for system decision-making.

### **2 OLED Display**
- Shows **temperature** and **humidity** readings dynamically.
- Displays system status updates.

### **3 Push Button Controls**
- **Single Press**: Toggles system **ON/OFF**.
- **Double Press**: Stops the **servo motor** if running.

### **4 Servo Motor Operations**
- Moves continuously **if temp ≥ 26°C** (Cooling Mode).
- Stops **if temp < 26°C** or button is **double-pressed**.

### **5 RGB LED Indicators**
- **BLUE** = System ON (blinks every **2 seconds**).
- **RED** = Temperature **≥ 26°C** (Cooling Mode, Servo ON).
- **YELLOW** = Temperature **25-26°C** (Moderate Temperature).
- **GREEN** = Temperature **≤ 24°C** (Cool State).

## Components Required
- **ESP32 / Arduino Board**
- **DHT22 Temperature & Humidity Sensor**
- **0.96" OLED Display (SSD1306)**
- **Servo Motor (SG90 or MG995)**
- **Push Button**
- **RGB LED (Common Anode or Cathode)**
- **Resistors (for LED and button connections)**
- **Jumper Wires**
- **Breadboard / PCB**

## Setup and Wiring
| Component      | ESP32 Pin |
|--------------|----------|
| DHT22 Signal | GPIO 7   |
| OLED SDA     | GPIO 21  |
| OLED SCL     | GPIO 22  |
| Servo Motor  | GPIO 13  |
| Push Button  | GPIO 4   |
| RGB LED Red  | GPIO 16  |
| RGB LED Green| GPIO 33  |
| RGB LED Blue | GPIO 19  |

## Installation & Dependencies
1. Install **Arduino IDE** (or use **PlatformIO** for VS Code).
2. Install the following libraries:
   ```sh
   Arduino Library Manager → Install:
   - Adafruit SSD1306
   - Adafruit GFX
   - DHT sensor library
   - ESP32Servo library
   ```
3. Upload the Arduino Sketch (provided in `main.ino`).
4. Open Serial Monitor (`115200 baud rate`) to debug.

## Future Improvements
- Implement **Wi-Fi connectivity** to send temperature data to a cloud dashboard.
- Add a **buzzer alert system** for high-temperature warnings.
- Enhance the **button debounce logic** for smoother toggling.
- Enhance the **max-threshold value** to be set dynamically.

## 📌 License
This project is open-source and available under the **MIT License**. Feel free to use, modify, and share!
