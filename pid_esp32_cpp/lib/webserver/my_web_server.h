#ifndef mywebserver_H
#define mywebserver_H

#include <EEPROM.h>
#include <myEEPROM.h>
#include <HTTPClient.h>
#include <notification.h>
#include <WebServer.h>
#include <string.h>
#include <globals.h>
#include <mqtt_functions.h>
#include <PubSubClient.h>
#include <FS.h>
#include <Update.h>

extern WebServer server;

void handleWiFiForm();
void handleRoot();
void handleMQTTForm();
void setup_web_server();
void connectWiFi();
void handleFirmwareUpload();
void handleUploadFirmware();

#endif
