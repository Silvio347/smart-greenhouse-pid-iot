#include "globals.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define WDT_TIMER 10

void controlTask(void *pvParameters);
void sensorTask(void *pvParameters);

String ssid = "Your_SSID";
String password = "Your_Password";

// MQTT broker settings
String mqtt_server = "Your_Broker_IP";
int mqtt_port = 1883;
String mqtt_user = "Your_User";
String mqtt_password = "Your_Password";
const char *humidity_topic = "esp32/humidity";
const char *pwm_topic = "esp32/pwm";
const char *error_topic = "esp32/error";
const char *setpoint_topic = "esp32/setpoint";
const char *phone_topic = "esp32/phone";
const char *start_topic = "esp32/start";
const char *threshold_topic = "esp32/threshold";
const char *maxPeak_topic = "esp32/maxPeak";
const char *settlingTime_topic = "esp32/settlingTime";
const char *overshoot_topic = "esp32/overshoot";
const char *thresholdUpper_topic = "esp32/thresholdUpper";
const char *thresholdLower_topic = "esp32/thresholdLower";
const char *thresholdOption_topic = "esp32/thresholdOption";
const char *alertThresholdOption_topic = "esp32/alertThresholdOption";
const char *alertThreshold_topic = "esp32/alertThreshold";
const char *thresholdVariation_topic = "esp32/thresholdVariation";
const char *thresholdLowerReceive_topic = "esp32/thresholdLowerReceive";
const char *thresholdUpperReceive_topic = "esp32/thresholdUpperReceive";
const char *alertThresholdOptionReceive_topic = "esp32/alertThresholdOptionReceive";
const char *setpointReceive_topic = "esp32/setpointReceive";
const char *phoneReceive_topic = "esp32/phoneReceive";
const char *thresholdOptionReceive_topic = "esp32/thresholdOptionReceive";
const char *thresholdVariationReceive_topic = "esp32/thresholdVariationReceive";
const char *kp_topic = "esp32/kp";
const char *ki_topic = "esp32/ki";
const char *kd_topic = "esp32/kd";

// Global variables
float Kp = 1.0, Ki = 0.1, Kd = 0.01, SP = 50.0;
float erro = 0, erro1 = 0, erro2 = 0, cv = 0, cv1 = 0, PV = 0;
float Tm = 1.0;                      // Time interval in seconds
int ledChannel = 0;                  // PWM channel
int thresholdVariation = 15;         // Threshold for variation
int thresholdUpper = 90;             // Threshold for alert
int thresholdLower = 40;             // Threshold for alert
String alertThresholdOption = "sms"; // sms or call
String thresholdOption = "Lim";      // Lim or Hys

float maxPico = 0.0;            // Maximum Peak
unsigned long startTime;        // Start time for TA calculation
unsigned long settlingTime = 0; // Accommodation Time
bool isSettled = false;         // Flag to indicate when the system is stabilized

unsigned long serverStartTime = 0; // Start time for the server
bool serverRunning = true;         // Flag to indicate if the server is running
uint8_t reconnectWiFi = 0;         // Counter to reconnect WiFi
uint8_t thresholdCounter = 0;      // Counter to send alert
bool lastHeatState = false;        // Flag to indicate the last state of the cooler