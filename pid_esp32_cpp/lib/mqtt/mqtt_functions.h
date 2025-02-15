#ifndef mqttfunctions_H
#define mqttfunctions_H

#include <PubSubClient.h>
#include <globals.h>
#include <Arduino.h>
#include <WiFi.h>
#include <myEEPROM.h>
#include <notification.h>

extern WiFiClient espClient;
extern PubSubClient client;

void setupmqtt();
void reconnectMQTT();
void mqttCallback(char *topic, byte *payload, unsigned int length);


#endif