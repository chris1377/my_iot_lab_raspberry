# My IoT Lab - Raspberry Pi 5

Complete IoT prototyping suite migrated from Arduino to Raspberry Pi 5 (RP1 architecture), featuring physical hardware control, local HTTP/REST web servers, and MQTT messaging protocols implemented in C++.

---

## 🛠️ Hardware & Components
- Hardware: Raspberry Pi 5
- GPIO Library: lgpio (required for the new RP1 architecture on Pi 5)
- HTTP Library: cpp-httplib
- MQTT Library: libmosquitto
- Components used:
  * LED (with current-limiting resistor)
  * Pushbutton
  * 10kΩ resistors for Pull-down configuration

---

## 🔌 Wiring Guide

### 1. Breadboard Power Rails
- 3.3V: Connected from physical pin 1 of the Raspberry Pi to the vertical Red (+) rail of the breadboard.
- GND (Ground): Connected from a GND pin (e.g., physical pin 6) of the Raspberry Pi to the vertical Blue (-) rail of the breadboard.

### 2. LED Circuit (GPIO 17)
- Anode (long leg): Connected via jumper wire to GPIO 17 (Physical pin 11).
- Cathode (short leg): Connected to a resistor, which then completes the circuit to the Ground (Blue / -) rail.

### 3. Pushbutton Circuit with Pull-down (GPIO 27)
- Power: One pin of the push button is connected to the Red (+) 3.3V rail.
- Signal: A wire runs from the opposite pin directly to GPIO 27 (Physical pin 13).
- Pull-down: From the same signal pin, a 10kΩ resistor goes down to the Ground (Blue / -) rail to prevent floating states (antenna effect) when the button is unpressed.

---

## 📂 Project Suite & Architecture Overview

### 1. Pin Diagnostics (read_pin.cpp)
- Description: A diagnostic script designed to read the real-time logical state of the button pin.
- Purpose: Essential for troubleshooting physical circuits, verifying that the external 10kΩ pull-down resistor works properly, and ensuring there are no floating states or wiring faults.

### 2. Direct Hardware Management (manage_led.cpp)
- Description: A standalone direct control script mimicking standard microcontroller logic (similar to basic Arduino sketches).
- Purpose: Reads the button state locally within a continuous loop and directly commands the physical LED to turn on or off without network intervention.

### 3. Local HTTP REST Web Server (led_http.cpp)
- Description: A native C++ Web Server running via cpp-httplib on port 8080.
- Purpose: Hosts a lightweight HTML control page with clickable buttons. It exposes standard REST endpoints (/ON and /OFF) to manipulate the Raspberry Pi's hardware pins directly from any browser on the local network.

### 4. MQTT Network Integration (led_mqtt.cpp)
- Description: An asynchronous MQTT subscriber node that connects to a local broker (such as Mosquitto).
- Purpose: Listens to specific command topics (laboratorio/attuatori/led) to trigger physical outputs based on messages published remotely, laying the groundwork for professional IoT architectures and Node-RED dashboards.

---

## 🚀 Compilation and Execution

Make sure you have the required system libraries installed (liblgpio-dev, libmosquitto-dev). Compiled binaries are stored inside the bin/ directory.

### 1. Pin Diagnostics
g++ -o bin/read_pin read_pin.cpp -llgpio
./bin/read_pin

### 2. Direct Hardware Management
g++ -o bin/manage_led manage_led.cpp -llgpio
./bin/manage_led

### 3. HTTP Web Server for LED Control
Compiled with multi-threading support (-lpthread) and the C++11 standard:
g++ -std=c++11 -o bin/led_http led_http.cpp -llgpio -lpthread
./bin/led_http
Access via browser: http://YOUR-RASPBERRY-IP:8080

### 4. MQTT Client for Node-RED Integration
g++ -o bin/led_mqtt led_mqtt.cpp -llgpio -lmosquitto
./bin/led_mqtt
