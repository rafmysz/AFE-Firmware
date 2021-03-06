/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-API-MQTT.h"

AFEMQTT::AFEMQTT() {}

void AFEMQTT::begin() {
  NetworkConfiguration = Data.getNetworkConfiguration();
  sprintf(deviceName, "%s", Device.configuration.name);
  Broker.setClient(esp);
  if (strlen(MQTTConfiguration.host) > 0) {
    Broker.setServer(MQTTConfiguration.host, MQTTConfiguration.port);
  } else if (MQTTConfiguration.ip[0] > 0) {
    Broker.setServer(MQTTConfiguration.ip, MQTTConfiguration.port);
  } else {
    isConfigured = false;
  }

  Broker.setCallback(MQTTMessagesListener);
  sprintf(mqttTopicForSubscription, "%s#", MQTTConfiguration.topic);
  Data = {};
}

void AFEMQTT::disconnect() {
  if (Broker.connected()) {
    Broker.disconnect();
  }
}

void AFEMQTT::listener() {
  if (Broker.connected()) {
    Broker.loop();
  } else {
    connect();
  }
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          NetworkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {
#ifndef T0_SHELLY_1_CONFIG
      if (ledStartTime == 0) {
        ledStartTime = millis();
      }
#endif
      if (delayStartTime == 0) {
        delayStartTime = millis();

        /* LWT Topic */
        char mqttLWTMessage[38];
        sprintf(mqttLWTMessage, "%sstate", MQTTConfiguration.topic);

        if (Broker.connect(deviceName, MQTTConfiguration.user,
                           MQTTConfiguration.password, mqttLWTMessage, 2, false,
                           "disconnected")) {

#ifdef DEBUG
          Serial << endl
                 << "INFO: Connected" << endl
                 << "INFO: Subscribing to : " << mqttTopicForSubscription;
#endif

          Broker.subscribe((char *)mqttTopicForSubscription);

#ifdef DEBUG
          Serial << endl << "INFO: Subsribed";
#endif

          eventConnectionEstablished = true;
          delayStartTime = 0;
#ifndef T0_SHELLY_1_CONFIG
          ledStartTime = 0;
          Led.off();
#endif
          connections = 0;

          return;
        }
      }
#ifndef T0_SHELLY_1_CONFIG
      if (millis() > ledStartTime + 500) {
        Led.toggle();
        ledStartTime = 0;
      }
#endif
      if (millis() >

          delayStartTime + (NetworkConfiguration.waitTimeConnections * 1000)) {
        connections++;
#ifdef DEBUG
        Serial << endl
               << "INFO: MQTT Connection attempt: " << connections + 1
               << " from " << NetworkConfiguration.noConnectionAttempts
               << ", connection status: " << Broker.state()
               << ", connection time: " << millis() - delayStartTime << "ms";

#endif
        delayStartTime = 0;
      }

      if (connections >= NetworkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();

        delayStartTime = 0;
#ifndef T0_SHELLY_1_CONFIG
        ledStartTime = 0;
        Led.off();
#endif
        connections = 0;

#ifdef DEBUG
        Serial << endl
               << "WARN: Not able to connect to MQTT.Going to sleep mode for "
               << NetworkConfiguration.waitTimeSeries << "sec.";
#endif
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

void AFEMQTT::publish(const char *type, const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", MQTTConfiguration.topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publish(const char *type, float value, uint8_t width,
                      uint8_t precision) {
  char message[10];
  dtostrf(value, width, precision, message);
  publish(type, message);
}

void AFEMQTT::publish(const char *topic, const char *type,
                      const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publishToMQTTBroker(const char *topic, const char *message) {
  if (Broker.state() == MQTT_CONNECTED) {
#ifdef DEBUG
    Serial << endl << "INFO: MQTT publising:  " << topic << "  \\ " << message;
#endif
    Broker.publish(topic, message);
  }
}

boolean AFEMQTT::eventConnected() {
  boolean returnValue = eventConnectionEstablished;
  eventConnectionEstablished = false;
  return returnValue;
}
