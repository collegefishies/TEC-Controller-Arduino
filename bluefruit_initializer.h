#ifndef BLUEFRUIT_INIT_H
#define BLUEFRUIT_INIT_H


#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"


/* The service information */
extern int32_t tecServiceId;
extern int32_t tecKCharId;
extern int32_t tecTCharId;
extern int32_t tecSetpointCharId;
extern int32_t tecThermistorCharId;
extern int32_t tecDriveCharId;
extern int32_t tecResetCharId;

void error(const __FlashStringHelper* err);
void initialize(Adafruit_BluefruitLE_SPI &ble);
#endif //BLUEFRUIT_INIT_H
