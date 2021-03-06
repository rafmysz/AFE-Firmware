/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-UART.h"

AFEUART::AFEUART() {}

void AFEUART::begin() {
  AFEDataAccess Data;
  SERIALPORT configuration = Data.getSerialPortConfiguration();
  SerialBus.init(configuration.RXD, configuration.TXD, false, 64);
  SerialBus.begin(9600);
  SerialBus.println();
}

void AFEUART::send(const uint8_t *cmd) {
#ifdef DEBUG
  Serial << endl << "UART: Sending commnad:  ";
#endif
  for (uint8_t i = 0; i < sizeof(cmd); i++) {
    SerialBus.write(cmd[i]);
#ifdef DEBUG
    Serial << " 0x";
    Serial.print(cmd[i], HEX);
#endif
  }
  delay(10);
}
