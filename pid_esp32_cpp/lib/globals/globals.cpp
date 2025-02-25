#include "globals.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define WDT_TIMER 10

void controlTask(void *pvParameters);
void sensorTask(void *pvParameters);

// Wi-Fi settings
String ssid = "Your_SSID";
String password = "Your_Password";

// Global variables
float Kp = 1.0, Ki = 0.1, Kd = 0.01, SP = 50.0;                // PID parameters
float erro = 0, erro1 = 0, erro2 = 0, cv = 0, cv1 = 0, PV = 0; // Control variables
float Tm = 1.0;                                                // Time interval in seconds
int ledChannel = 0;                                            // PWM channel
int thresholdVariation = 15;                                   // Threshold for variation
int thresholdUpper = 90;                                       // Threshold for alert
int thresholdLower = 40;                                       // Threshold for alert
String alertThresholdOption = "sms";                           // sms or call
String thresholdOption = "Lim";                                // Lim or Hys

float maxPico = 0.0;            // Maximum Peak
unsigned long startTime;        // Start time for TA calculation
unsigned long settlingTime = 0; // Accommodation Time
bool isSettled = false;         // Flag to indicate when the system is stabilized

unsigned long serverStartTime = 0; // Start time for the server
bool serverRunning = true;         // Flag to indicate if the server is running
uint8_t reconnectWiFi = 0;         // Counter to reconnect WiFi
uint8_t thresholdCounter = 0;      // Counter to send alert
bool lastHeatState = false;        // Flag to indicate the last state of the cooler