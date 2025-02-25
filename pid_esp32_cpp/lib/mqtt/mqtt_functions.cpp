#include "mqtt_functions.h"

MqttManager mqttManager;

MqttManager::MqttManager() : client(espClient)
{
}

// Initialize MQTT connection
void MqttManager::begin()
{
  client.setServer(mqtt_server.c_str(), mqtt_port);
  client.setCallback([this](char *topic, byte *payload, unsigned int length)
                     { this->handleCallback(topic, payload, length); });
}

// Reconnect to MQTT broker
void MqttManager::reconnect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32", "jeilson", "1234"))
    {
      Serial.println("Connected to MQTT broker!");
      subscribeTopics();
    }
    else
    {
      Serial.print("Failed to connect, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// Subscribe to MQTT topics
void MqttManager::subscribeTopics()
{
  for (const auto &pair : mqttTopics)
  {
    client.subscribe(pair.second.c_str());
  }
}

// Handle MQTT messages
void MqttManager::handleCallback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.printf("Received message: %s (Topic: %s)\n", message.c_str(), topic);

  processMessage(String(topic), message);
}

// Process MQTT messages
void MqttManager::processMessage(const String &topic, const String &message)
{
  try
  {
    auto it = mqttTopics.begin();
    for (; it != mqttTopics.end(); ++it)
    {
      if (it->second == topic) // Verifica se o tópico recebido está no map
      {
        String key = it->first;

        if (key == "setpointReceive")
        {
          float value = message.toFloat();
          if (value > 30 && value < 90)
          {
            SP = value;
            Serial.printf("Setpoint updated: %.2f\n", SP);
            eepromManager.saveFloatEEPROM(SP_ADDRESS, SP);
          }
        }
        else if (key == "thresholdVariationReceive")
        {
          float value = message.toFloat();
          if (value > 0 && value < 100)
          {
            thresholdVariation = value;
            eepromManager.saveFloatEEPROM(THRESHOLD_VARIATION_ADDRESS, thresholdVariation);
            Serial.printf("Threshold Variation updated: %.2f\n", thresholdVariation);
          }
        }
        else if (key == "thresholdUpperReceive")
        {
          float value = message.toFloat();
          if (value > 0 && value < 100)
          {
            thresholdUpper = value;
            eepromManager.saveFloatEEPROM(THRESHOLD_UPPER_ADDRESS, thresholdUpper);
            Serial.printf("Threshold Upper updated: %.2f\n", thresholdUpper);
          }
        }
        else if (key == "thresholdLowerReceive")
        {
          float value = message.toFloat();
          if (value > 0 && value < 100)
          {
            thresholdLower = value;
            eepromManager.saveFloatEEPROM(THRESHOLD_LOWER_ADDRESS, thresholdLower);
            Serial.printf("Threshold Lower updated: %.2f\n", thresholdLower);
          }
        }
        else if (key == "phoneReceive")
        {
          toNumber = message;
          eepromManager.saveStringEEPROM(TO_NUMBER_ADDRESS, toNumber, TO_NUMBER_MAX_LENGTH);
          Serial.println("Phone number updated");
        }
        else if (key == "alertThresholdOptionReceive")
        {
          alertThresholdOption = message;
          eepromManager.saveStringEEPROM(ALERT_THRESHOLD_OPTION_ADDRESS, alertThresholdOption, ALERT_THRESHOLD_OPTION_MAX_LENGTH);
        }
        else if (key == "thresholdOptionReceive")
        {
          thresholdOption = message;
          eepromManager.saveStringEEPROM(THRESHOLD_OPTION_ADDRESS, thresholdOption, THRESHOLD_OPTION_MAX_LENGTH);
          Serial.println("Threshold option updated");
        }
        else if (key == "start" && message == "startreceive")
        {
          publishInitialData();
          xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
        }

        return; // Sai da função após processar a mensagem
      }
    }

    Serial.printf("Unrecognized topic: %s\n", topic.c_str());
  }
  catch (const std::exception &e)
  {
    Serial.printf("Error processing topic %s: %s\n", topic.c_str(), e.what());
  }
}

// Publish initial data to MQTT broker
void MqttManager::publishInitialData()
{
  Serial.println("Publishing initial data...");

  client.publish(mqttTopics["phone"].c_str(), String(toNumber).substring(3).c_str());
  client.publish(mqttTopics["thresholdVariation"].c_str(), String(thresholdVariation).c_str());
  client.publish(mqttTopics["thresholdUpper"].c_str(), String(thresholdUpper).c_str());
  client.publish(mqttTopics["thresholdLower"].c_str(), String(thresholdLower).c_str());
  client.publish(mqttTopics["alertThresholdOption"].c_str(), alertThresholdOption.c_str());
  client.publish(mqttTopics["thresholdOption"].c_str(), thresholdOption.c_str());
  client.publish(mqttTopics["setpoint"].c_str(), String(SP).c_str());
  client.publish(mqttTopics["kp"].c_str(), String(Kp).c_str());
  client.publish(mqttTopics["ki"].c_str(), String(Ki).c_str());
  client.publish(mqttTopics["kd"].c_str(), String(Kd).c_str());
  client.publish(mqttTopics["settlingTime"].c_str(), String(settlingTime).c_str());
  client.publish(mqttTopics["maxPeak"].c_str(), String(maxPico).c_str());

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
}

// Publish message to MQTT broker
void MqttManager::publish(const char *topic, const char *message)
{
  client.publish(topic, message);
}

// Handle MQTT messages
void MqttManager::loop()
{
  client.loop();
}

// Check if connected to MQTT broker
bool MqttManager::connected()
{
  return client.connected();
}