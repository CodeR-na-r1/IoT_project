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

  String host = "";

  int port = 0;

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
    
    int hostSize = EEPROM.read(addr++);

    host = "";
    host.reserve(hostSize);

    for (int i(0); i < hostSize; i++) {
      host += char(EEPROM.read(addr++));
    }
    
    port = (EEPROM.read(addr++) & 0xFF) << 24;
    port = port | ((EEPROM.read(addr++) & 0xFF) << 16);
    port = port | ((EEPROM.read(addr++) & 0xFF) << 8);
    port = port | (EEPROM.read(addr++) & 0xFF);

    return;
  }

  String getPassword() {
    return this->password;
  }

  String getWiFiSSID() {
    return this->wifiSSID;
  }

  String getHost() {
    return this->host;
  }

  int getPort() {
    return this->port;
  }

  void saveConfig(String _WiFiSSID, String _password, String _host, int _port) {
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
    
    EEPROM.write(addr++, _host.length() & 0xFF);

    for (int i(0); i < _host.length(); i++) {
      EEPROM.write(addr++, _host[i]);
    }
    
    EEPROM.write(addr++, (_port & 0xFF000000) >> 24);
    EEPROM.write(addr++, (_port & 0x00FF0000) >> 16);
    EEPROM.write(addr++, (_port & 0x0000FF00) >> 8);
    EEPROM.write(addr++, _port & 0x000000FF);

    EEPROM.commit();

    return;
  }
};

#endif