#include "AFE-Configuration-Panel.h"

AFEConfigurationPanel::AFEConfigurationPanel() {
  language = Data.getLanguage();
}

String AFEConfigurationPanel::getSite(const String option, uint8_t command,
                                      boolean redirect) {

  String page;
  redirect ? page = Site.generateHeader(10) : page = Site.generateHeader(0);

  if (option == "upgrade") {
    page += Site.addUpgradeSection();
  } else if (option == "reset") {
    page += Site.addResetSection(command);
  } else if (option == "exit") {
    page += Site.addExitSection();
  } else if (option == "help") {
    if (command == 0) {
      page += Site.addHelpSection();
    } else if (command == 1 || command == 2) {
      page += Site.addExitSection();
    }
  } else {
    page += "<h1>Page Not Found</h1>";
  }

  page += Site.generateFooter();
  delay(10);
  return page;
}

String AFEConfigurationPanel::getLanguageConfigurationSite(uint8_t command,
                                                           uint8_t lang) {
  String page;
  if (command == SERVER_CMD_SAVE) {
    Data.saveLanguage(lang);
    page += Site.generateHeader(10);
    page += Site.addExitSection();
  } else {
    page += Site.generateHeader();
    page += "<form action=\"/?option=language&cmd=1\"  method=\"post\">";
    page += Site.addLanguageConfiguration();
    page += "<input type=\"submit\" class=\"b bs\" value=\"";
    page += language == 0 ? "Zapisz" : "Save";
    page += "\"></form>";
  }
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getDeviceConfigurationSite(uint8_t command,
                                                         DEVICE data) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
    Device.begin();
  }
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=device&cmd=1\"  method=\"post\">";
  page += Site.addDeviceConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getNetworkConfigurationSite(uint8_t command,
                                                          NETWORK data) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=network&cmd=1\"  method=\"post\">";
  page += Site.addNetworkConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getMQTTConfigurationSite(uint8_t command,
                                                       MQTT data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=mqtt&cmd=1\"  method=\"post\">";
  page += Site.addMQTTBrokerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String
AFEConfigurationPanel::getDomoticzServerConfigurationSite(uint8_t command,
                                                          DOMOTICZ data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=domoticz&cmd=1\" method=\"post\">";
  page += Site.addDomoticzServerConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

#ifndef T0_SHELLY_1_CONFIG
String AFEConfigurationPanel::getLEDConfigurationSite(
    uint8_t command, LED data[sizeof(Device.configuration.isLED)],
    uint8_t dataLedID) {

  if (command == SERVER_CMD_SAVE) {
    for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
      if (Device.configuration.isLED[i]) {
        Data.saveConfiguration(i, data[i]);
      } else {
        break;
      }
    }
    Data.saveSystemLedID(dataLedID);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=led&cmd=1\"  method=\"post\">";
  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i]) {
      page += Site.addLEDConfiguration(i);
    } else {
      break;
    }
  }
  page += Site.addSystemLEDConfiguration();

  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
#endif

String AFEConfigurationPanel::getRelayConfigurationSite(uint8_t command,
                                                        RELAY data,
                                                        uint8_t relayIndex) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(relayIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=relay";
  page += relayIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addRelayConfiguration(relayIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

#if defined(T1_CONFIG) || defined(T2_CONFIG)
String AFEConfigurationPanel::getRelayStatConfigurationSite(
    uint8_t command, REGULATOR data, uint8_t regulatorType) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data, regulatorType);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=";
  page += regulatorType == THERMOSTAT_REGULATOR ? "thermostat" : "humidistat";
  page += "&cmd=1\"  method=\"post\">";

  regulatorType == THERMOSTAT_REGULATOR
      ? page += Site.addRegulatorConfiguration(THERMOSTAT_REGULATOR)
      : page += Site.addRegulatorConfiguration(HUMIDISTAT_REGULATOR);

  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
#endif

String AFEConfigurationPanel::getSwitchConfigurationSite(uint8_t command,
                                                         SWITCH data,
                                                         uint8_t switchIndex) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(switchIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=switch";
  page += switchIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addSwitchConfiguration(switchIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

/* DS18B20 and DHxx Sensors */
#ifdef T1_CONFIG
String AFEConfigurationPanel::getDS18B20ConfigurationSite(uint8_t command,
                                                          DS18B20 data)

#endif

#if defined(T2_CONFIG) || defined(T5_CONFIG)
    String AFEConfigurationPanel::getDHTConfigurationSite(uint8_t command,
                                                          DH data)
#endif

#if defined(T1_CONFIG) || defined(T2_CONFIG) || defined(T5_CONFIG)
{
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();

#ifdef T1_CONFIG
  page += "<form action=\"/?option=ds18b20&cmd=1\"  method=\"post\">";
  page += Site.addDS18B20Configuration();
#else
  page += "<form action=\"/?option=DHT&cmd=1\"  method=\"post\">";
  page += Site.addDHTConfiguration();
#endif
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
#endif

#if defined(T3_CONFIG)
String AFEConfigurationPanel::getPIRConfigurationSite(const String option,
                                                      uint8_t command, PIR data,
                                                      uint8_t PIRIndex) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(PIRIndex, data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=pir";
  page += PIRIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addPIRConfiguration(PIRIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

#endif

#if defined(T5_CONFIG)
String AFEConfigurationPanel::getContactronConfigurationSite(
    const String option, uint8_t command, CONTACTRON data,
    uint8_t contactronIndex) {

  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(contactronIndex, data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=contactron";
  page += contactronIndex;
  page += "&cmd=1\"  method=\"post\">";
  page += Site.addContactronConfiguration(contactronIndex);
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::getGateConfigurationSite(const String option,
                                                       uint8_t command,
                                                       GATE data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form action=\"/?option=gate&cmd=1\"  method=\"post\">";
  page += Site.addGateConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
#endif

#if defined(T6_CONFIG)
String AFEConfigurationPanel::getSerialPortConfigurationSite(uint8_t command,
                                                             SERIALPORT data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=UART&cmd=1\"  method=\"post\">";
  page += Site.addSerialPortConfiguration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
String
AFEConfigurationPanel::getHPMA115S0SesnorConfigurationSite(uint8_t command,
                                                           HPMA115S0 data) {
  if (command == SERVER_CMD_SAVE) {
    Data.saveConfiguration(data);
  }

  String page = Site.generateHeader();
  page += "<form action=\"/?option=HPMA115S0&cmd=1\"  method=\"post\">";
  page += Site.addHPMA115S0Configuration();
  page += "<input type=\"submit\" class=\"b bs\" value=\"";
  page += language == 0 ? "Zapisz" : "Save";
  page += "\"></form>";
  page += Site.generateFooter();
  return page;
}
#endif

String AFEConfigurationPanel::firmwareUpgradeSite() {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader();
  page += "<form method=\"post\" action=\"\" "
          "enctype=\"multipart/form-data\">";
  page += Site.addUpgradeSection();
  page += "</form>";
  page += Site.generateFooter();
  return page;
}

String AFEConfigurationPanel::postFirmwareUpgradeSite(boolean status) {
  String page;
  page.reserve(siteBufferSize);
  page = Site.generateHeader(15);
  page += Site.addPostUpgradeSection(status);
  page += Site.generateFooter();
  return page;
}
