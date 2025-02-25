#include <myEEPROM.h>

EEPROMManager eepromManager; // global instance of EEPROMManager

EEPROMManager::EEPROMManager() {} // Constructor

// Setup EEPROM
void EEPROMManager::setupEEPROM() {
    if (!EEPROM.begin(LENGHT_EEPROM))
        ESP.restart();
}

// Save string in EEPROM
void EEPROMManager::saveStringEEPROM(int enderecoInicio, String str, int maxLength) {
    int tamanho = str.length();

    if (enderecoInicio + maxLength > LENGHT_EEPROM)
        return;

    for (int i = 0; i < maxLength; i++) {
        if (i < tamanho) {
            EEPROM.write(enderecoInicio + i, str[i]);
        } else {
            EEPROM.write(enderecoInicio + i, '\0');
        }
    }
    EEPROM.commit();
}

// Read string from EEPROM
String EEPROMManager::readStringEEPROM(int enderecoInicio, int maxLength) {
    String str = "";

    for (int i = 0; i < maxLength; i++) {
        char c = EEPROM.read(enderecoInicio + i);
        if (c == '\0') {
            break;
        }
        str += c;
    }
    return str;
}

// Save float in EEPROM
void EEPROMManager::saveFloatEEPROM(int enderecoInicio, float valor) {
    EEPROM.put(enderecoInicio, valor);
    EEPROM.commit();
}

// Read float from EEPROM
float EEPROMManager::readFloatEEPROM(int enderecoInicio) {
    float valor;
    EEPROM.get(enderecoInicio, valor);
    return valor;
}

// Save parameters in EEPROM
void EEPROMManager::saveParamsEEPROM() {
    saveStringEEPROM(SSID_ADDRESS, ssid, SSID_MAX_LENGTH);
    saveStringEEPROM(PASSWORD_ADDRESS, password, PASSWORD_MAX_LENGTH);
    saveStringEEPROM(SSID_MAX_LENGTH + PASSWORD_MAX_LENGTH, mqttManager.mqtt_server, BROKER_IP_MAX_LENGTH);
    saveStringEEPROM(BROKER_USER_ADDRESS, mqttManager.mqtt_user, BROKER_USER_MAX_LENGTH);
    saveStringEEPROM(BROKER_PASSWORD_ADDRESS, mqttManager.mqtt_password, BROKER_PASSWORD_MAX_LENGTH);
    saveFloatEEPROM(THRESHOLD_VARIATION_ADDRESS, thresholdVariation);
    saveStringEEPROM(ALERT_THRESHOLD_OPTION_ADDRESS, alertThresholdOption, ALERT_THRESHOLD_OPTION_MAX_LENGTH);
    saveStringEEPROM(THRESHOLD_OPTION_ADDRESS, thresholdOption, THRESHOLD_OPTION_MAX_LENGTH);
    saveFloatEEPROM(THRESHOLD_UPPER_ADDRESS, thresholdUpper);
    saveFloatEEPROM(THRESHOLD_LOWER_ADDRESS, thresholdLower);
    saveStringEEPROM(TO_NUMBER_ADDRESS, toNumber, TO_NUMBER_MAX_LENGTH);
    saveFloatEEPROM(SP_ADDRESS, SP);
}

// Read parameters from EEPROM
void EEPROMManager::readParamsEEPROM() {
    ssid = readStringEEPROM(SSID_ADDRESS, SSID_MAX_LENGTH);
    password = readStringEEPROM(PASSWORD_ADDRESS, PASSWORD_MAX_LENGTH);
    mqttManager.mqtt_server = readStringEEPROM(BROKER_IP_ADDRESS, BROKER_IP_MAX_LENGTH);
    mqttManager.mqtt_user = readStringEEPROM(BROKER_USER_ADDRESS, BROKER_USER_MAX_LENGTH);
    mqttManager.mqtt_password = readStringEEPROM(BROKER_PASSWORD_ADDRESS, BROKER_PASSWORD_MAX_LENGTH);
    thresholdVariation = readFloatEEPROM(THRESHOLD_VARIATION_ADDRESS);
    alertThresholdOption = readStringEEPROM(ALERT_THRESHOLD_OPTION_ADDRESS, ALERT_THRESHOLD_OPTION_MAX_LENGTH).c_str();
    thresholdOption = readStringEEPROM(THRESHOLD_OPTION_ADDRESS, THRESHOLD_OPTION_MAX_LENGTH);
    thresholdUpper = readFloatEEPROM(THRESHOLD_UPPER_ADDRESS);
    thresholdLower = readFloatEEPROM(THRESHOLD_LOWER_ADDRESS);
    toNumber = readStringEEPROM(TO_NUMBER_ADDRESS, TO_NUMBER_MAX_LENGTH);
    SP = readFloatEEPROM(SP_ADDRESS);
}
