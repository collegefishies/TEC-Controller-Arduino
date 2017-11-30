#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"

bool write( Adafruit_BluefruitLE_SPI &ble, int32_t Id, int16_t value);
int32_t read( Adafruit_BluefruitLE_SPI &ble, int32_t Id);
#endif //BLUETOOTH_INTERFACE_H
