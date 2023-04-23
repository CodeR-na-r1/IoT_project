#ifndef EEPROM_MODULE
#define EEPROM_MODULE

#ifndef EEPROM_LIB
#define EEPROM_LIB
#include <EEPROM.h>
#endif

#define START_ADDRESS 0

class ConfigManager {

  int addr = START_ADDRESS;

  String wifiSSID = "";

  String password = "";

public:
  ConfigManager() {
    EEPROM.begin(4096);
  }

  bool hasConfig() {
    return bool(EEPROM.read(START_ADDRESS));
  }

  void loadConfig() {
    addr = START_ADDRESS + 1;

    int ssidSize = EEPROM.read(addr++);

    wifiSSID = "";
    wifiSSID.reserve(ssidSize);

    for (int i(0); i < ssidSize; i++) {
      wifiSSID += char(EEPROM.read(addr++));
    }

    int passwordSize = EEPROM.read(addr++);

    password = "";
    password.reserve(passwordSize);

    for (int i(0); i < passwordSize; i++) {
      password += char(EEPROM.read(addr++));
    }

    return;
  }

  String getPassword() {
    return password;
  }

  String getWiFiSSID() {
    return wifiSSID;
  }

  void saveConfig(String _WiFiSSID, String _password) {
    addr = START_ADDRESS + 1;
    
    if (!this->hasConfig()) {
      EEPROM.write(START_ADDRESS, 1);
    }

    EEPROM.write(addr++, _WiFiSSID.length() & 0xFF);

    for (int i(0); i < _WiFiSSID.length(); i++) {
      EEPROM.write(addr++, _WiFiSSID[i]);
    }

    EEPROM.write(addr++, _password.length() & 0xFF);

    for (int i(0); i < _password.length(); i++) {
      EEPROM.write(addr++, _password[i]);
    }

    EEPROM.commit();

    return;
  }
};

#endif