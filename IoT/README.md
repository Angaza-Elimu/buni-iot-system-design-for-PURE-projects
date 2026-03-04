# IoT Projects – PURE Curriculum

## Overview
This repository contains foundational IoT practical projects designed to introduce learners to hardware components, microcontrollers, connectivity, and cloud dashboards using ESP32.

Projects Covered:
1. Basic Smart Room Indicator System
2. GPIO & Sensor Interfacing
3. IoT Connectivity & HTTP Communication
4. Complete IoT Data Visualization System

---

# 1️⃣ Introduction to IoT Hardware Components

## Project Title
Basic Smart Room Indicator System

## Objective
- Identify IoT hardware components
- Differentiate sensors, actuators & controllers
- Build a basic input-output embedded system

## Components
- ESP32
- Breadboard
- LED + 220Ω resistor
- Push button
- Buzzer
- Jumper wires

## System Flow
Button (Sensor) → ESP32 (Controller) → LED/Buzzer (Actuators)

## Implementation Steps
1. Connect LED to GPIO 2 (via resistor)
2. Connect Push Button to GPIO 4
3. Connect Buzzer to GPIO 5
4. Write logic:
   - If button pressed → LED ON + Buzzer ON
   - Else → OFF

## Learning Outcome
Learners understand:
- Input vs Output
- Voltage & current basics
- GPIO control

---

# 2️⃣ Understanding Microcontrollers, GPIO & Sensors

## Focus
- Analog vs Digital signals
- ADC usage
- Sensor interfacing

## Example Sensors
- DHT11/DHT22
- Soil moisture sensor
- LDR

## Key Skills
- Reading ADC values
- Threshold comparison logic
- Sensor calibration

---

# 3️⃣ IoT Connectivity Basics

## Project Title
Remote Temperature Monitoring System

## Objective
- Connect ESP32 to WiFi
- Send HTTP requests
- Display cloud data

## Steps
1. Configure WiFi credentials
2. Read DHT temperature
3. Send HTTP request every 20 seconds
4. Verify cloud dashboard

## Deliverables
- Screenshot of cloud data
- Code snippet
- Data flow explanation

---

# 4️⃣ Cloud Communication & IoT Dashboards

## Project Title
Complete IoT Data Visualization System

## Objective
- Send multiple sensor readings
- Build dashboard widgets
- Visualize real-time data

## Sensors Used
- DHT
- Soil moisture
- LDR
- RGB LED (remote control)

## Dashboard Should Display
- Temperature graph
- Humidity graph
- Soil moisture %
- Light intensity
- RGB LED control

## Learning Outcomes
- Multi-sensor integration
- Cloud visualization
- Remote device control
- Data interpretation

---

# Capstone Presentation Guide
Each group presents:
- Problem statement
- System architecture
- Components used
- Data flow
- Real-world application
- Challenges & solutions
