/*

   AFE Firmware for smart home devices build on ESP8266
   More info: https://github.com/tschaban/AFE-Firmware
   LICENCE: http://opensource.org/licenses/MIT
 */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <AFE-EEPROM.h>
#include <AFE-Data-Structures.h>


class AFEDataAccess
{
private:
  AFEEEPROM Eeprom;

public:
AFEDataAccess();
NETWORK 	getNetworkConfiguration();
MQTT			getMQTTConfiguration();
RELAY			getRelayConfiguration(uint8_t id);
SWITCH		getSwitchConfiguration(uint8_t id);
DS18B20   getDS18B20Configuration();
FIRMWARE  getFirmwareConfiguration();

void saveNetworkConfiguration(NETWORK configuration);
void saveMQTTConfiguration(MQTT configuration);
void saveRelayConfiguration(uint8_t id, RELAY configuration);
void saveSwitchConfiguration(uint8_t id, SWITCH configuration);
void saveDS18B20Configuration(DS18B20 configuration);
void saveFirmwareConfiguration(FIRMWARE configuration);

const char getVersion();

boolean getRelayState(uint8_t id);
void saveRelayState(uint8_t id, boolean state);

uint8_t getDeviceMode();
void saveDeviceMode(uint8_t mode);

uint8_t getLanguage();
void saveLanguage(uint8_t language);

boolean isDebuggable();
void setDebuggable(boolean state);

};
#endif