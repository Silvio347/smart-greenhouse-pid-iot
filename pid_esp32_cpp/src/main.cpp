#include <globals.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "esp_task_wdt.h"
#include <my_web_server.h>
#include <mqtt_functions.h>
#include <notification.h>

DHT dht(14, DHT11);

void setup()
{
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);

  WiFi.mode(WIFI_MODE_APSTA);

  Serial.begin(115200);
  dht.begin();

  setupEEPROM();

  esp_task_wdt_init(WDT_TIMER, true);
  esp_task_wdt_add(NULL);

  readParamsEEPROM();

  connectWiFi();
  setupmqtt();

  ledcSetup(ledChannel, 5000, 10);
  ledcAttachPin(PWMPIN, ledChannel);
  pinMode(coolerPin, OUTPUT);

  xTaskCreate(controlTask, "Control Task", 4096, NULL, 1, NULL);
  xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
  xTaskCreate(wifiandwdt, "Sensor Task", 4096, NULL, 1, NULL);

  maxPico = PV;
}

void loop()
{
  client.loop();

  // 10 min to turn off web server
  if (serverRunning && (millis() - serverStartTime >= 600000))
  {
    server.close();
    serverRunning = false;
    Serial.println("Server shut down after 10 minutes.");
  }
  if (!client.connected())
  {
    reconnectMQTT();
  }

  server.handleClient();
}

// Task for sensor reading and publishing to MQTT broker
void sensorTask(void *pvParameters)
{
  while (true)
  {
    PV = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!(isnan(PV) || isnan(PV)))
    {
      client.publish(humidity_topic, String(int(PV)).c_str());
      client.publish(error_topic, String(erro).c_str());
      cv = constrain(cv, 0, 1023);

      int pwmValue = map(cv, 0, 1023, 0, 100);
      pwmValue = constrain(pwmValue, 0, 100); 
      client.publish(pwm_topic, String(pwmValue).c_str());
    }

    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Task for PID control
void controlTask(void *pvParameters)
{
  while (true)
  {
    erro = SP - PV;

    cv = cv1 + (Kp + (Ki * Tm) + (Kd / Tm)) * erro + (-Kp - (2 * Kd / Tm)) * erro1 + ((Kd / Tm) * erro2);

    // Updating Maximum Peak (MP)
    if (PV > maxPico)
    {
      maxPico = PV;
      client.publish(maxPeak_topic, String(maxPico).c_str());
    }
    if (!isSettled && abs(PV - SP) > 2.0)
    {
      startTime = millis();
      isSettled = false;
    }

    bool coolerOn = (PV > SP);

    if (coolerOn != lastHeatState)
    {
      lastHeatState = coolerOn;
      if (coolerOn)
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

    // Checking the settling time (TA) (if the system has stabilized within 2% of the setpoint)
    if (!isSettled && abs(PV - SP) <= 2.0)
    { // 2% of setpoint
      settlingTime = (millis() - startTime) / 1000.0;
      client.publish(settlingTime_topic, String(settlingTime).c_str());
      isSettled = true; // The system is stabilized
    }

    float overshoot = (maxPico - SP) / SP * 100.0;
    if (overshoot < 0)
      overshoot = 0.0;

    client.publish(overshoot_topic, String(overshoot).c_str());

    // PID tuning using Ziegler-Nichols
    if (isSettled)
    {
      Kp = 0.6 * maxPico;
      Ki = (2 * Kp) / max(settlingTime / 1000.0, 1.0);
      Kd = (Kp * max(settlingTime / 1000.0, 1.0)) / 8.0;
      client.publish(kp_topic, String(Kp).c_str());
      client.publish(ki_topic, String(Ki).c_str());
      client.publish(kd_topic, String(Kd).c_str());
    }

    if (!coolerOn)
    {
      ledcWrite(ledChannel, (int)map(constrain(cv, 0.0, 1023.0), 0, 1023, 0, 1023));
    }
    else
    {
      ledcWrite(ledChannel, 0);
    }

    cv1 = cv;
    erro2 = erro1;
    erro1 = erro;

    checkThresholdViolation(); // alert user

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void wifiandwdt(void *pvParameters)
{
  while (true)
  {
    esp_task_wdt_reset();

    reconnectWiFi++;
    if (reconnectWiFi >= 30)
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        connectWiFi();
      }
      reconnectWiFi = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}