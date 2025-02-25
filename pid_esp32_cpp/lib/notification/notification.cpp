#include "notification.h"
#include <EEPROM.h>
#include <myEEPROM.h>

Notification notification; // Global instance of Notification

// Twilio credentials API
const char *accountSid = "Your_SSID";           // Twilio account SID
const char *authToken = "Your_Auth_Token";      // Twilio auth token
const char *fromNumber = "+Your_Twilio_Number"; // Twilio number
String toNumber = "+Your_Number";               // User number

Notification::Notification() {} // Constructor

// Send request to Twilio API
void Notification::sendTwilioRequest(const String &url, const String &data)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(url);
    http.setAuthorization(accountSid, authToken);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int responseCode = http.POST(data);
    if (responseCode == 201)
    {
      Serial.println("Request sent successfully!");
    }
    else
    {
      Serial.print("Failed to send request: ");
      Serial.println(http.getString());
    }

    http.end();
  }
  else
  {
    Serial.println("Error: No WiFi.");
  }
}

// Make a call to alert when the humidity is above the limit
void Notification::makeCall()
{
  String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(accountSid) + "/Calls.json";
  String data = "To=" + String(toNumber) +
                "&From=" + String(fromNumber) +
                "&Url=http://demo.twilio.com/docs/voice.xml";

  sendTwilioRequest(url, data);
}

// Send SMS when the humidity is above the limit
void Notification::sendSMS()
{
  String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(accountSid) + "/Messages.json";
  String data = "To=" + String(toNumber) +
                "&From=" + String(fromNumber) +
                "&Body=Alerta! Umidade acima do limite!";

  sendTwilioRequest(url, data);
}

// Send push notification when the system is not settling
void Notification::pushNotification()
{
  mqttManager.publish("esp32/alert", "Alert! Turning off the load because the system is not settling.");
}

// Manage alert based on the option chosen
void Notification::handleAlert()
{
  thresholdCounter++;
  if (thresholdCounter < MAX_SECONDS_TO_ALERT)
    return;

  if (alertThresholdOption == "sms")
  {
    sendSMS();
  }
  else if (alertThresholdOption == "call")
  {
    makeCall();
  }
  else
  {
    pushNotification();
  }

  thresholdCounter = 0;
}

// Check threshold violation based on the option chosen
void Notification::checkThresholdViolation()
{
  if (thresholdOption == "Var")
  {
    if (PV < thresholdLower || PV > thresholdUpper)
    {
      handleAlert();
    }
  }
  else
  {
    if (abs(PV - SP) > thresholdVariation)
    {
      handleAlert();
    }
  }
}
