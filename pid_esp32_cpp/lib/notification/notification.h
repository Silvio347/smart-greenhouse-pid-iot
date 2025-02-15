#ifndef notification_H
#define notification_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <mqtt_client.h>
#include <my_web_server.h>
#include <PubSubClient.h>

void makeCall();
void sendSMS();
void pushNotification();
void handleAlert();
void checkThresholdViolation();

// Credenciais do Twilio
extern const char *accountSid;
extern const char *authToken;
extern const char *fromNumber;
extern String toNumber;

#endif