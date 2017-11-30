#include "bluetooth_interface.h"

bool write( Adafruit_BluefruitLE_SPI &ble, int32_t Id, int16_t value) {
  bool success;
  char sendBuffer[32] = {0,};
  sprintf(sendBuffer, "AT+GATTCHAR=%d, %d", Id, value);
  success = ble.sendCommandCheckOK(sendBuffer);
  return success;
}

int32_t read( Adafruit_BluefruitLE_SPI &ble, int32_t Id) {
  int32_t reply;
  char sendBuffer[32] = {0,};
  sprintf(sendBuffer, "AT+GATTCHAR=%d", Id);
  ble.sendCommandWithIntReply(sendBuffer, &reply);
  return reply;
}

