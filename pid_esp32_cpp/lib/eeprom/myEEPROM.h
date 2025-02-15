#ifndef MYEEPROM_H
#define MYEEPROM_H

#include <EEPROM.h>
#include <Arduino.h>
#include <globals.h>
#include <notification.h>
#include <PubSubClient.h>

//***************************************************************************************************
// Function  : Begin na EEPROM
// Arguments : none
// Return    : none
//***************************************************************************************************
void setupEEPROM();

//***************************************************************************************************
// Function  : Salva os parâmetros de WI-FI e IP da API
// Arguments : none
// Return    : none
//***************************************************************************************************
void saveParamsEEPROM();

//***************************************************************************************************
// Function  : Obtém o SSID, PASSWORD e IP da API salvos na EEPROM
// Arguments : none
// Return    : none
//***************************************************************************************************
void readParamsEEPROM();

void saveFloatEEPROM(int enderecoInicio, float valor);
float readFloatEEPROM(int enderecoInicio);
void saveStringEEPROM(int enderecoInicio, String str, int maxLength);

#endif