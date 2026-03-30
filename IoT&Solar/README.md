# IoT + Renewable Energy Projects – PURE Curriculum

## Overview
These projects combine solar energy systems with IoT automation and cloud monitoring for sustainable smart systems.

Projects Covered:
1. Smart Solar-Powered Light Monitoring System
2. Solar-Powered Smart Farm System
3. Smart Irrigation System Using Solar Power

---

# 1️⃣ Smart Solar-Powered Light Monitoring System

## Objective
- Automate lighting based on ambient light
- Monitor battery voltage
- Send real-time data to cloud

## Architecture
LDR → ESP32 → Relay → Light  
Battery → Voltage Sensor → ESP32  
ESP32 → WiFi → Cloud Dashboard

## Automation Logic
- If dark → Light ON
- If bright → Light OFF
- Monitor battery voltage
- Send status to dashboard

## Dashboard Displays
- Light status
- Light intensity
- Battery voltage

---

# 2️⃣ Solar-Powered Smart Farm System

## Objective
- Monitor soil parameters
- Automate irrigation
- Prevent battery over-discharge

## Sensors
- Soil moisture
- NPK
- pH
- Temperature & humidity
- Battery voltage

## Automation Rule
IF soil dry AND battery sufficient → Pump ON  
IF battery low → Pump OFF

## Dashboard Displays
- Soil data
- Climate data
- Battery level
- Pump status

## Learning Focus
- Energy-aware automation
- Sustainable farming
- IoT decision systems

---

# 3️⃣ Smart Irrigation Using Solar Power

## Objective
- Efficient water usage
- Solar-powered irrigation
- Cloud-based monitoring

## Decision Rules
- Irrigate only when soil dry
- Avoid irrigation when battery low
- Run pump for fixed time

## Cloud Dashboard
- Soil moisture %
- Irrigation history
- Pump ON/OFF
- Battery level

---

# Sustainability Impact

Learners analyze:
- Water conservation
- Energy efficiency
- Climate-smart agriculture
- Off-grid smart farming

---

# Capstone Presentation Requirements

Each group must present:
1. Problem definition
2. System diagram
3. Automation logic
4. Solar integration explanation
5. Dashboard demonstration
6. Sustainability benefits
