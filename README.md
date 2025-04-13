Smart Irrigation System using ESP8266

Overview
This project implements a Smart Watering System using an ESP8266 microcontroller, DHT11 sensor, soil moisture sensor, and rain sensor. It provides an interactive web interface to monitor environmental conditions and control watering schedules remotely.

Features
Real-time Monitoring:

Temperature and Humidity (DHT11)

Soil Moisture Level

Rain Detection

Automated Watering:

Manually toggle watering on/off via the web interface.

Schedule watering based on user-defined time intervals.

Data Storage:

Stores watering schedules in EEPROM to retain settings across power cycles.

Web-based Control Panel:

View sensor readings and control the system remotely using a web browser.
view web dashboard https://marif-githubs.github.io/Smart-Watering-System/

Components Used
ESP8266 WiFi module

DHT11 Temperature & Humidity Sensor

Soil Moisture Sensor

Rain Sensor

Relay Module (for water pump control)

5V Power Supply


Pin Configuration
Component             Pin

DHT11 Sensor          D2

Soil Moisture Sensor  A0

Rain Sensor           01

Watering Relay        D3


Software Requirements
Arduino IDE (with ESP8266 board support installed)

Libraries Required:

ESP8266WiFi

ESP8266WebServer

ESP8266mDNS

ArduinoJson

DHT

EEPROM


Web Interface
The system hosts a web page where users can:

View live sensor data.

Manually turn the watering system on/off.

Set and save watering schedules.


Code Explanation
WiFi Connection: Connects ESP8266 to a predefined WiFi network.

Web Server: Serves an HTML page for user interaction.

Sensor Readings: Reads values from the temperature, humidity, and moisture sensors.

Watering Control: Allows manual and scheduled watering operations.

EEPROM Storage: Stores user-defined schedules to retain settings after a reset.


How to Use
Upload the code to ESP8266 using Arduino IDE.

Ensure sensors are properly connected to their respective pins.

Power the system and access the web interface via the ESP8266's local IP address.

Monitor sensor values and configure watering schedules as needed.


Future Improvements
Implement cloud-based data logging.

Add mobile app integration.

Use AI to optimize watering based on weather forecasts.


Troubleshooting
WiFi Not Connecting? Double-check the SSID and password in the code.

Web Page Not Loading? Ensure ESP8266 is powered and connected to WiFi.

Incorrect Sensor Readings? Verify sensor wiring and power supply.


This project is a step towards automating irrigation systems efficiently, ensuring optimal plant hydration while conserving water.
