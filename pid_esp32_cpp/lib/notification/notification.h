#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <mqtt_client.h>
#include <my_web_server.h>
#include <PubSubClient.h>

class Notification
{
public:
    Notification(); // Constructor

    void makeCall();                // Make a call to alert
    void sendSMS();                 // Send SMS to alert
    void pushNotification();        // Push notification to alert
    void handleAlert();             // Handle alert
    void checkThresholdViolation(); // Check threshold violation

private:
    void sendTwilioRequest(const String &url, const String &data); // Send request to Twilio
};

extern Notification notification; // global instance of Notification

// Twilio credentials API
extern const char *accountSid; // Twilio account SID
extern const char *authToken;  // Twilio auth token
extern const char *fromNumber; // Twilio number
extern String toNumber;        // User number

#endif
