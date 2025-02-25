#ifndef MY_WEB_SERVER_H
#define MY_WEB_SERVER_H

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

class MyWebServer
{
public:
    MyWebServer();               // Constructor
    void begin();                // Start the server
    void connectWiFi();          // Connect to WiFi
    void handleRoot();           // Root page
    void handleWiFiForm();       // WiFi configuration page
    void handleMQTTForm();       // MQTT configuration page
    void handleFirmwareUpload(); // Firmware upload page
    void handleUploadFirmware(); // Firmware upload handler
    void handleClient();         // Handle client requests
    void close();                // Close the server
private:
    void createAP();                    // Create an access point
    void setupRoutes();                 // Setup routes
    void myDelayMillis(uint16_t tempo); // Delay in milliseconds
    WebServer server;                   // Web server
};

extern MyWebServer webServer; // Web server global object 

#endif
