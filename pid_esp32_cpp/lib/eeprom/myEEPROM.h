#ifndef MYEEPROM_H
#define MYEEPROM_H

#include <EEPROM.h>
#include <Arduino.h>
#include <globals.h>
#include <notification.h>
#include <PubSubClient.h>

class EEPROMManager
{
public:
    EEPROMManager(); // Constructor

    void setupEEPROM();                                                   // Setup EEPROM
    void saveParamsEEPROM();                                              // Save parameters in EEPROM
    void readParamsEEPROM();                                              // Read parameters from EEPROM
    void saveFloatEEPROM(int enderecoInicio, float valor);                // Save float in EEPROM
    float readFloatEEPROM(int enderecoInicio);                            // Read float from EEPROM
    void saveStringEEPROM(int enderecoInicio, String str, int maxLength); // Save string in EEPROM
    String readStringEEPROM(int enderecoInicio, int maxLength);           // Read string from EEPROM
};

extern EEPROMManager eepromManager; // global instance of EEPROMManager

#endif
