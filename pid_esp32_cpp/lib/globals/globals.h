#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#define WDT_TIMER 10
#define SSID_ESP "ESP32"
#define PASSWORD_ESP "1234ssss"
#define TIMEOUT_WIFI 8
#define MAX_SECONDS_TO_ALERT 30
#define coolerPin 12
#define PWMPIN 13

// Addresses and their sizes to save in EEPROM
#define LENGHT_EEPROM 1024
#define SSID_ADDRESS 0
#define SSID_MAX_LENGTH 32
#define PASSWORD_ADDRESS (SSID_ADDRESS + SSID_MAX_LENGTH)
#define PASSWORD_MAX_LENGTH 32
#define BROKER_IP_ADDRESS (PASSWORD_ADDRESS + PASSWORD_MAX_LENGTH)
#define BROKER_IP_MAX_LENGTH 16
#define CLIENT_ADDRESS_1 81
#define CLIENT_LENGHT 68
#define CLIENT_ADDRESS_2 (CLIENT_ADDRESS_1 + CLIENT_LENGHT)
#define DATE_TIME_ADDRESS (CLIENT_ADDRESS_2 + CLIENT_LENGHT)
#define DATE_TIME_LENGHT 20
#define THRESHOLD_VARIATION_ADDRESS (BROKER_IP_ADDRESS + BROKER_IP_MAX_LENGTH)
#define THRESHOLD_VARIATION_MAX_LENGTH 4
#define BROKER_USER_ADDRESS (THRESHOLD_VARIATION_ADDRESS + THRESHOLD_VARIATION_MAX_LENGTH)
#define BROKER_USER_MAX_LENGTH 32
#define BROKER_PASSWORD_ADDRESS (BROKER_USER_ADDRESS + BROKER_USER_MAX_LENGTH)
#define BROKER_PASSWORD_MAX_LENGTH 32

#define ALERT_THRESHOLD_OPTION_ADDRESS (THRESHOLD_VARIATION_ADDRESS + THRESHOLD_VARIATION_MAX_LENGTH)
#define ALERT_THRESHOLD_OPTION_MAX_LENGTH 4

#define THRESHOLD_OPTION_ADDRESS (ALERT_THRESHOLD_OPTION_ADDRESS + ALERT_THRESHOLD_OPTION_MAX_LENGTH)
#define THRESHOLD_OPTION_MAX_LENGTH 4

#define THRESHOLD_UPPER_ADDRESS (THRESHOLD_OPTION_ADDRESS + THRESHOLD_OPTION_MAX_LENGTH)
#define THRESHOLD_UPPER_MAX_LENGTH 4

#define THRESHOLD_LOWER_ADDRESS (THRESHOLD_UPPER_ADDRESS + THRESHOLD_UPPER_MAX_LENGTH)
#define THRESHOLD_LOWER_MAX_LENGTH 4

#define TO_NUMBER_ADDRESS (THRESHOLD_LOWER_ADDRESS + THRESHOLD_LOWER_MAX_LENGTH)
#define TO_NUMBER_MAX_LENGTH 16

#define SP_ADDRESS (TO_NUMBER_ADDRESS + TO_NUMBER_MAX_LENGTH)
#define SP_MAX_LENGTH 4

void controlTask(void *pvParameters);
void sensorTask(void *pvParameters);
void wifiandwdtTask(void *pvParameters);
void createAP();
void myDelayMillis(uint16_t tempo);

// Wi-Fi settings
extern String ssid;
extern String password;

// Global variables
extern float Kp, Ki, Kd, SP;                  // PID constants
extern float erro, erro1, erro2, cv, cv1, PV; // PID variables
extern float Tm;                              // Time of the last measurement
extern int ledChannel;                        // PWM channel
extern int thresholdVariation;                // Thresholds for alert
extern String alertThresholdOption;           // Thresholds for alert
extern String thresholdOption;                // Thresholds for alert
extern int thresholdUpper;                    // Thresholds for alert
extern int thresholdLower;                    // Thresholds for alert
extern float maxPico;                         // Maximum Peak
extern unsigned long startTime;               // Start time for TA calculation
extern unsigned long settlingTime;            // Accommodation Time
extern bool isSettled;                        // Flag to indicate when the system is stabilized

extern unsigned long serverStartTime; // Start time for the server
extern bool serverRunning;            // Flag to indicate if the server is running
extern uint8_t reconnectWiFi;         // Counter to reconnect WiFi
extern uint8_t thresholdCounter;      // Counter to send threshold
extern bool lastHeatState;            // Last state of the cooler

#endif