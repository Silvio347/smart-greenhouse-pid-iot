# Smart Greenhouse PID IoT

In the project, one LED simulates a humidifier and another simulates a cooler/exhaust fan, regulating the environment intelligently.

PID (Proportional-Integral-Derivative) control dynamically adjusts the humidifier to avoid sudden oscillations and ensure more precise control, unlike a simple on/off switch.

The automation is integrated into a Flutter application, which communicates via MQTT with an ESP32. The ESP32 reads the sensor data and sends it to the app, where the user can configure the setpoint and activate alerts (SMS, connection and notification) if the humidity exceeds the defined limits or there is a sudden variation. All settings are saved in the ESP32's EEPROM to ensure persistence.

Demonstration: https://youtu.be/MMjNIxWRMoc
