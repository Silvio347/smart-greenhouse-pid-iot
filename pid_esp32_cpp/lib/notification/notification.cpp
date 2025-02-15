#include <notification.h>
#include <EEPROM.h>
#include <myEEPROM.h>

// Credenciais do Twilio
const char *accountSid = "Your_SSID";
const char *authToken = "Your_Auth_Token";
const char *fromNumber = "+Your_Twilio_Number";
String toNumber = "+Your_Number";

// Call to phone when the umidity is higher than the threshold
void makeCall()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    String url = "https://api.twilio.com/2010-04-01/Accounts/" +
                 String(accountSid) + "/Calls.json";

    String dados = "To=" + String(toNumber) +
                   "&From=" + String(fromNumber) +
                   "&Url=http://demo.twilio.com/docs/voice.xml";

    http.begin(url);
    http.setAuthorization(accountSid, authToken); // Autenticação HTTP
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int codigoResposta = http.POST(dados);

    if (codigoResposta == 201)
    {
      Serial.println("Call made successfully!");
    }
    else
    {
      Serial.print("Fail to make call: ");
      Serial.println(http.getString());
    }

    http.end();
  }
  else
  {
    Serial.println("Error: No WiFi.");
  }
}

// Send SMS when the humidity is higher than the threshold
void sendSMS()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    String url = "https://api.twilio.com/2010-04-01/Accounts/" +
                 String(accountSid) + "/Messages.json";

    String dados = "To=" + String(toNumber) +
                   "&From=" + String(fromNumber) +
                   "&Body=Alerta! Umidade acima do limite!";

    http.begin(url);
    http.setAuthorization(accountSid, authToken);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int codigoResposta = http.POST(dados);

    if (codigoResposta == 201)
    {
      Serial.println("SMS sent successfully!");
    }
    else
    {
      Serial.print("Fail to send SMS: ");
      Serial.println(http.getString());
    }

    http.end();
  }
  else
  {
    Serial.println("Error: No WiFi.");
  }
}

void pushNotification()
{
  client.publish("esp32/alert", "Alert! Turning off the load because the system not settling.");
}

void handleAlert()
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

void checkThresholdViolation()
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