#include <mqtt_functions.h>

WiFiClient espClient;
PubSubClient client(espClient);

void setupmqtt()
{
  client.setServer(mqtt_server.c_str(), mqtt_port);
  client.setCallback(mqttCallback);
}

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.println("Connecting broker MQTT...");
    if (client.connect("ESP32", mqtt_user.c_str(), mqtt_password.c_str()))
    {
      Serial.println("Connected!");
      client.subscribe(start_topic);
      client.subscribe(thresholdLowerReceive_topic);
      client.subscribe(thresholdUpperReceive_topic);
      client.subscribe(alertThresholdOptionReceive_topic);
      client.subscribe(setpointReceive_topic);
      client.subscribe(phoneReceive_topic);
      client.subscribe(thresholdOptionReceive_topic);
      client.subscribe(thresholdVariationReceive_topic);
    }
    else
    {
      Serial.print("Fail to connect, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  String message = "";
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println("msg: " + message + " + topic: " + String(topic));

  if (String(topic) == setpointReceive_topic)
  {
    try
    {
      float data = message.toFloat();
      if ((data > 30) && (data < 90))
      {
        SP = data;
        Serial.printf("Setpoint updated: %.2f\n", SP);
        saveFloatEEPROM(SP_ADDRESS, SP);
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Fail to convert to float in setpoint");
    }
  }
  else if (String(topic) == thresholdVariationReceive_topic)
  {
    try
    {
      float data = message.toFloat();
      if ((data > 0) && (data < 100))
      {
        thresholdVariation = data;
        saveFloatEEPROM(THRESHOLD_VARIATION_ADDRESS, thresholdVariation);
        Serial.printf("Threshold Variation updated: %.2f\n", thresholdVariation);
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Fail to convert to float in threshold variation");
    }
  }
  else if (String(topic) == thresholdUpperReceive_topic)
  {
    try
    {
      float data = message.toFloat();
      if ((data > 0) && (data < 100))
      {
        thresholdUpper = data;
        saveFloatEEPROM(THRESHOLD_UPPER_ADDRESS, thresholdUpper);
        Serial.printf("Threshold Upper updated: %.2f\n", thresholdUpper);
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Fail to convert to float in threshold upper");
    }
  }
  else if (String(topic) == thresholdLowerReceive_topic)
  {
    try
    {
      float data = message.toFloat();
      if ((data > 0) && (data < 100))
      {
        thresholdLower = data;
        saveFloatEEPROM(THRESHOLD_LOWER_ADDRESS, thresholdLower);
        Serial.printf("Threshold Lower updated: %.2f\n", thresholdLower);
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Fail to convert to float in threshold lower");
    }
  }
  else if (String(topic) == phoneReceive_topic)
  {
    toNumber = message.c_str();
    saveStringEEPROM(TO_NUMBER_ADDRESS, toNumber, TO_NUMBER_MAX_LENGTH);
    Serial.println("Phone number updated");
  }
  else if (String(topic) == alertThresholdOptionReceive_topic)
  {
    alertThresholdOption = message.c_str();
    saveStringEEPROM(ALERT_THRESHOLD_OPTION_ADDRESS, alertThresholdOption, ALERT_THRESHOLD_OPTION_MAX_LENGTH);
  }
  else if (String(topic) == thresholdOptionReceive_topic)
  {
    thresholdOption = message.c_str();
    saveStringEEPROM(THRESHOLD_OPTION_ADDRESS, thresholdOption, THRESHOLD_OPTION_MAX_LENGTH);
    Serial.println("Threshold option updated");
  }
  else if (String(topic) == start_topic)
  {
    if (message != "startreceive")
      return;

    Serial.println("Connected!");

    // substring(3) for remove '+55'
    client.publish(phone_topic, String(toNumber).substring(3).c_str());
    client.publish(thresholdVariation_topic, String(thresholdVariation).c_str());
    client.publish(thresholdUpper_topic, String(thresholdUpper).c_str());
    client.publish(thresholdLower_topic, String(thresholdLower).c_str());
    client.publish(alertThresholdOption_topic, alertThresholdOption.c_str());
    client.publish(thresholdOption_topic, thresholdOption.c_str());
    client.publish(setpoint_topic, String(SP).c_str());
    client.publish(kp_topic, String(Kp).c_str());
    client.publish(ki_topic, String(Ki).c_str());
    client.publish(kd_topic, String(Kd).c_str());
    client.publish(settlingTime_topic, String(settlingTime).c_str());
    client.publish(maxPeak_topic, String(maxPico).c_str());

    if (PV < SP)
    {
      client.publish("esp32/cooler", "on");
      digitalWrite(coolerPin, HIGH);
    }
    else
    {
      client.publish("esp32/cooler", "off");
      digitalWrite(coolerPin, LOW);
    }
    // xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
  }
}
