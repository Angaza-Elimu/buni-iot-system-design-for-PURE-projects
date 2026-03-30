# 🌱 Smart Farm Kit - ESP-NOW Wireless Irrigation System

## Project Overview

This system uses two ESP32 microcontrollers communicating wirelessly via the **ESP-NOW protocol** to automate plant irrigation. The first ESP32 (Transmitter/Master) reads environmental sensor data and broadcasts it over the air. The second ESP32 (Receiver/Slave) receives that data and actuates a relay to control a water pump based on the received conditions.

**Key Advantage:** Separating sensing from actuation allows the sensor unit to be placed in the soil near the plant while the pump controller can sit at the water source — connected only by the wireless link.

---

## System Architecture

```
TRANSMITTER ESP32 (Master)              RECEIVER ESP32 (Slave)
──────────────────────────              ──────────────────────────
Soil Moisture Sensor ──┐                ┌── Relay → Water Pump
Light Sensor          ──┤                │
DHT22 (Temperature)   ──┼─ ESP-NOW ────►├── Serial Monitor
HC-SR04 (Tank Level)  ──┘  WiFi Ch6     │
RGB LEDs (tank status)                  └── Controls based on
                                            soilMoisture threshold
```

---

## Hardware Components

### Transmitter Hardware (Master)
| Component | Pin | Purpose |
|-----------|-----|---------|
| **ESP32** | - | Master controller - reads all sensors |
| **DHT22** | Pin 7 | Temperature sensor (air/soil proxy) |
| **Soil Moisture Sensor** | Pin 5 | Analog resistive sensor - inserted into soil |
| **LDR (Light Sensor)** | Pin 4 | Reads ambient light intensity |
| **HC-SR04 Ultrasonic** | Trig: Pin 35<br>Echo: Pin 38 | Measures water tank level from above |
| **RGB LEDs** | Red: Pin 16<br>Green: Pin 33<br>Blue: Pin 19 | Tank level indicator:<br>🔴 Red = Low<br>🔵 Blue = Medium<br>🟢 Green = Full |

### Receiver Hardware (Slave)
| Component | Pin | Purpose |
|-----------|-----|---------|
| **ESP32** | - | Slave controller - controls pump |
| **Relay Module** | Pin 20 | Switches water pump circuit ON/OFF |
| **Water Pump** | Via Relay | Controlled by receiver relay |

---

## ESP-NOW Communication Protocol

### Why ESP-NOW for this project?

| Feature | Benefit |
|---------|---------|
| **No router needed** | Works in remote farm/field environments |
| **Low latency (~1ms)** | Near-instant sensor-to-actuator response |
| **Low power** | Suitable for battery-powered sensor nodes |
| **Up to 250 bytes/message** | Enough for all four sensor readings |
| **Broadcast mode** | Transmitter doesn't need to know receiver MAC address |

### Communication Flow

1. **Transmitter** reads sensors every ~2.5 seconds
2. Packs readings into a formatted string:
   ```
   TankLevel:45.23,SoilMoisture:67.12,LightIntensity:23.45,SoilTemperature:28.30
   ```
3. Broadcasts string on **WiFi Channel 6**
4. **Receiver** hears broadcast, auto-registers transmitter as master
5. `onReceive()` fires — `sscanf()` parses string into float variables
6. `loop()` checks variables against thresholds → relay ON or OFF

---

## Sensor Readings & Calculations

### 1. 💧 Soil Moisture (0-100%)
```cpp
soilvalue = analogRead(soilpin);           // Raw: 0-1023
moisturepercentage = map(soilvalue, 1023, 0, 0, 100);
```
- **Dry soil** = High ADC value (→1023) = 0%
- **Wet soil** = Low ADC value (→0) = 100%
- Reading is **inverted** because resistive sensor reads HIGH when dry

### 2. ☀️ Light Intensity (0-100%)
```cpp
light_Intensity_Value = analogRead(lightsensor);
lightintesitypercentage = map(light_Intensity_Value, 0, 1023, 0, 100);
```
- **Dark** = Low ADC value (→0) = 0%
- **Bright** = High ADC value (→1023) = 100%

### 3. 🌡️ Soil Temperature (°C)
```cpp
float t = dht.readTemperature();  // Celsius
```
- Reads ambient air temperature using DHT22
- Used as a **proxy** for soil temperature in this demo
- **Note:** A DS18B20 waterproof probe would be more accurate for production

### 4. 🚰 Water Tank Level (mm)
```cpp
pingTravelTime = pulseIn(echoPin, HIGH);
distanceToTarget = (pingTravelTime / 2) * 0.343;  // mm
```
- HC-SR04 measures distance from sensor to water surface
- **Smaller distance** = More water (sensor at top looking down)

| Distance Range | LED Color | Tank Status |
|----------------|-----------|-------------|
| ≤100mm and >50mm | 🟢 GREEN | Tank is full |
| ≤50mm and >10mm | 🔵 BLUE | Tank is medium |
| <10mm | 🔴 RED | Tank is critically low |

---

## Automated Irrigation Control

### Receiver Logic (loop())

```cpp
while (soilMoisture < soilthreshold) {
    Serial.println("WARNING: Soil moisture critically low!");
    digitalWrite(relaypin, HIGH);  // Pump ON
}
digitalWrite(relaypin, LOW);  // Pump OFF
```

| Condition | Relay Behavior |
|-----------|----------------|
| `soilMoisture < 35%` | **Relay ON** — Pump runs until moisture rises above 35% |
| `soilMoisture ≥ 35%` | **Relay OFF** — Soil is adequately moist |

### Configurable Thresholds (Transmitter)
```cpp
int soilthreshold = 35;              // % moisture threshold
int lightthreshold = 35;             // % light threshold
int soiltemperaturethreshold = 35;   // °C threshold

float maximumwaterlevel = 100;       // mm — tank full
float mediumwaterlevel = 50;         // mm — tank medium
float minimumwaterlevel = 10;        // mm — tank low
```

---

## Setup Instructions

### 1. Transmitter Setup
1. Connect all sensors according to pin configuration above
2. Upload `angaza_elimu_demo_transmitter.ino` to master ESP32
3. Open Serial Monitor at **9600 baud**
4. Note the MAC address displayed on startup

### 2. Receiver Setup
1. Connect relay module to **Pin 20**
2. Upload `angaza_elimu_demo_receiver.ino` to slave ESP32
3. Open Serial Monitor at **9600 baud**
4. Device will **auto-register** transmitter on first broadcast

### 3. Communication Setup
- Both devices must use **WiFi Channel 6**
- Both must be in **WIFI_STA** (Station) mode
- Receiver automatically detects and registers new transmitters

---

## Serial Monitor Output

### Transmitter Example:
```
ESP-NOW Example - Broadcast Master
Wi-Fi parameters:
  Mode: STA
  MAC Address: XX:XX:XX:XX:XX:XX
  Channel: 6

soil tempt = 28.30
soil level = 512
light intensity = 345
water level = 67.50
Broadcasting: TankLevel:67.50,SoilMoisture:50.05,LightIntensity:33.70,SoilTemperature:28.30
```

### Receiver Example:
```
ESP-NOW Slave — Waiting for master...
  Mode: STA
  MAC Address: YY:YY:YY:YY:YY:YY
  Channel: 6

Received from master XX:XX:XX:XX:XX:XX (broadcast)
Raw message: TankLevel:67.50,SoilMoisture:50.05,LightIntensity:33.70,SoilTemperature:28.30

Parsed values:
  Tank Level      : 67.500 mm
  Soil Moisture   : 50.05 %
  Light Intensity : 33.70 %
  Soil Temperature: 28.30 C

--- Current Readings ---
Tank: 67.50mm | Moisture: 50.050% | Light: 33.700% | Temp: 28.300C
```

---

## Full System Decision Flow

### TRANSMITTER (every ~4 seconds):
1. ✅ Read HC-SR04 → `distanceToTarget` (mm) → light correct LED
2. ✅ Read soilpin → `moisturepercentage` (%)
3. ✅ Read LDR → `lightintesitypercentage` (%)
4. ✅ Read DHT22 → `temperature` (°C)
5. ✅ Pack all into string → broadcast via ESP-NOW

### RECEIVER (continuous):
1. ⏳ Waiting for broadcast...
2. 📡 `onReceive()` fires → `sscanf()` parses string
3. 📊 `tankLevel`, `soilMoisture`, `lightIntensity`, `soilTemperature` updated
4. 🔍 `loop()` checks:
   - `soilMoisture < 35%`?
     - **YES** → Relay HIGH (pump ON) → re-check until moisture rises
     - **NO** → Relay OFF (pump OFF)
5. 🖥️ Print debug readings every 1 second

---

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| **Baud Rate** | 9600 |
| **Broadcast Interval** | ~2.5 seconds |
| **Debug Print Interval** | 1 second (receiver) |
| **WiFi Channel** | 6 |
| **WiFi Mode** | Station (STA) |
| **Protocol** | ESP-NOW (peer-to-peer) |
| **Message Buffer Size** | 128 bytes (transmitter), 64 bytes (receiver) |
| **Communication Range** | ~200m outdoors (line of sight) |

---

## Libraries Required

```cpp
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include <DHT.h>           // For DHT22 (transmitter only)
#include <vector>          // For master list (receiver only)
```

**Installation:** Install DHT sensor library via Arduino Library Manager

---

## Known Issues & Suggested Improvements

| Issue | Suggested Fix |
|-------|---------------|
| `constrain()` result discarded in transmitter | `moisturepercentage = constrain(moisturepercentage, 0, 100)` |
| Typo: `lightintesitycondition` vs `lightintensitycondition` | Standardise spelling across function signature |
| Receiver buffer (64 bytes) < transmitter buffer (128 bytes) | Increase receiver `char received[]` to 128 bytes |
| **No pump timeout if transmitter goes offline** | Add `millis()`-based timeout — if no message in 30s, relay goes LOW |
| Only soil moisture controls relay on receiver | Add light and temperature threshold checks in receiver `loop()` |
| DHT22 reads air, not soil temperature | Replace with DS18B20 waterproof probe for true soil temperature |
| No acknowledgement from receiver to transmitter | Implement unicast reply from receiver to confirm relay state |

---

## Key Features

✅ **Wireless Communication** — ESP-NOW protocol for fast, low-latency data transmission  
✅ **Multi-Sensor Monitoring** — Soil moisture, temperature, light, and water level  
✅ **Automated Irrigation** — Self-regulating water pump control based on soil moisture  
✅ **Visual Indicators** — RGB LED for tank level status  
✅ **Scalable** — Multiple receivers can listen to one transmitter  
✅ **Auto-Registration** — Receivers automatically detect and register transmitters  
✅ **Real-time Updates** — Continuous sensor monitoring and broadcasting  
✅ **No Internet Required** — Direct ESP32-to-ESP32 communication

---

## Future Enhancements

- 📊 Add data logging to SD card or cloud service (ThingSpeak, Firebase)
- 🔄 Implement bidirectional communication for remote control
- 🧪 Add more sensor types (pH, conductivity, NPK sensors)
- 📱 Create mobile app for remote monitoring and control
- 🔋 Implement power-saving sleep modes for battery operation
- 💧 Add water flow meter to track irrigation volume
- ⏱️ Add RTC module for scheduled irrigation times
- 🌐 Add WiFi gateway to bridge ESP-NOW data to internet/MQTT

---

## Troubleshooting

### ❌ No communication between devices
- ✅ Verify both devices are on **Channel 6**
- ✅ Check MAC addresses display correctly in Serial Monitor
- ✅ Ensure devices are within range (~200m outdoors, ~50m indoors)
- ✅ Confirm both use **WIFI_STA** mode

### ❌ Incorrect sensor readings
- ✅ Verify all pin connections match code
- ✅ Check sensor power supply (3.3V or 5V as required)
- ✅ Calibrate sensors in known conditions
- ✅ Check for loose wires or poor connections

### ❌ Relay not activating
- ✅ Verify relay pin connection (Pin 20)
- ✅ Check `soilmoisturethreshold` setting (default: 35%)
- ✅ Ensure relay module has adequate power (may need external 5V)
- ✅ Test relay manually: `digitalWrite(20, HIGH)` in setup

### ❌ Pump runs continuously
- ✅ Check if transmitter is still broadcasting (check Serial Monitor)
- ✅ Verify moisture sensor is reading correctly
- ✅ Implement timeout safety feature (see Known Issues)

---

## Safety Considerations

⚠️ **Important:**
- Always use a relay rated for your pump's voltage and current
- Keep electrical connections away from water
- Add fuses to protect against short circuits
- Implement timeout safety to prevent pump from running indefinitely
- Test thoroughly before deploying in production environment

---

## License

This project is part of the **Angaza Elimu** educational initiative.

---

## Credits

**Developed by:** Angaza Elimu IoT Team  
**Documentation Version:** 1.0  
**Last Updated:** March 2026

---

## Support

For questions, improvements, or bug reports related to this project, please contact the Angaza Elimu team.

---

**🌱 Happy Smart Farming! 🌱**
