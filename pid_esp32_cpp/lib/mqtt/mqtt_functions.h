#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>
#include <myEEPROM.h>
#include <notification.h>
#include <map>

class MqttManager
{
public:
    MqttManager();

    void begin();                                                         // Initialize MQTT connection
    void reconnect();                                                     // Reconnect to MQTT broker
    void handleCallback(char *topic, byte *payload, unsigned int length); // Handle MQTT messages
    void publishInitialData();                                            // Publish initial data to MQTT broker
    void publish(const char *topic, const char *message);                 // Publish message to MQTT broker
    void loop();                                                          // Handle MQTT messages
    bool connected();                                                     // Check if connected to MQTT broker

    String mqtt_server = "Your_Broker_IP";  // MQTT broker IP
    int mqtt_port = 1883;                   // MQTT broker port
    String mqtt_user = "Your_User";         // MQTT user
    String mqtt_password = "Your_Password"; // MQTT password

    std::map<String, String> mqttTopics = { // MQTT topics
        {"temperature", "esp32/temperature"},
        {"humidity", "esp32/humidity"},
        {"pwm", "esp32/pwm"},
        {"error", "esp32/error"},
        {"setpoint", "esp32/setpoint"},
        {"phone", "esp32/phone"},
        {"start", "esp32/start"},
        {"threshold", "esp32/threshold"},
        {"maxPeak", "esp32/maxPeak"},
        {"settlingTime", "esp32/settlingTime"},
        {"overshoot", "esp32/overshoot"},
        {"thresholdUpper", "esp32/thresholdUpper"},
        {"thresholdLower", "esp32/thresholdLower"},
        {"thresholdOption", "esp32/thresholdOption"},
        {"alertThresholdOption", "esp32/alertThresholdOption"},
        {"alertThreshold", "esp32/alertThreshold"},
        {"thresholdVariation", "esp32/thresholdVariation"},
        {"thresholdLowerReceive", "esp32/thresholdLowerReceive"},
        {"thresholdUpperReceive", "esp32/thresholdUpperReceive"},
        {"alertThresholdOptionReceive", "esp32/alertThresholdOptionReceive"},
        {"setpointReceive", "esp32/setpointReceive"},
        {"phoneReceive", "esp32/phoneReceive"},
        {"thresholdOptionReceive", "esp32/thresholdOptionReceive"},
        {"thresholdVariationReceive", "esp32/thresholdVariationReceive"},
        {"kp", "esp32/kp"},
        {"ki", "esp32/ki"},
        {"kd", "esp32/kd"}};

private:
    WiFiClient espClient; // WiFi client
    PubSubClient client;  // MQTT client

    void processMessage(const String &topic, const String &message); // Process MQTT messages
    void subscribeTopics();                                          // Subscribe to MQTT topics
};

extern MqttManager mqttManager; // global instance of MqttManager

#endif
