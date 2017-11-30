#include "bluefruit_initializer.h"

/*
 *  Initializes the bluetooth, and defines the transmitting variables, or characteristics.
 *  Each characteristic has its own UUID. I usually just make them up. I don't think their 
 *  actual value matters.
 * 
 *  Each characteristic has an int32_t ID number. You need to remember this when you're 
 *  sending messages over bluetooth.
 * 
 */

int32_t tecServiceId;
int32_t tecKCharId;
int32_t tecTCharId;
int32_t tecSetpointCharId;
int32_t tecDriveCharId;
int32_t tecResetCharId;
int32_t tecThermistorCharId;

void error(const __FlashStringHelper* err) {
  Serial.println(err);
  while (1);
}

void initialize(Adafruit_BluefruitLE_SPI &ble)
{

    /**************************************************************************/
    /*!
        @brief  Sets up the HW an the BLE module (this function is called
                automatically on startup)
    */
    /**************************************************************************/

   boolean success;
 
  // Perform a factory reset to make sure everything is in a known state.
      Serial.println( F("Performing a factory reset: ") );
      if( !ble.factoryReset() ){
        error(F("Couldn't factory reset"));
      }

  // Disable command echo from Bluefruit
      ble.echo(false);

      Serial.println("Requesting Bluefruit info:");
      ble.info();

  //Set the device name
      Serial.println("Setting the device name to 'Fermi.2: DMD TEC': ");
    
      if( !ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Fermi.2: DMD TEC")) ){
        error(F("Could not set device name"));
      }
      
  //Set the Automation IO service definition (UUID=0x1815)
      Serial.println(F("Adding PID Service Definition (UUID=0x1815)"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=0x1815") , &tecServiceId);
      if(! success) {
        error(F("Could not add HRM service"));
      }

  //Add the proportional gain characterstic
      Serial.println(F("Adding the K proportional gain characteristic (UUID = 0x2A7E)"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2A7E,PROPERTIES=0x1A,MIN_LEN=1,MAX_LEN=4,VALUE=5,DATATYPE=3,DESCRIPTION=Proportionality Constant,PRESENTATION=17-00-00-27-01-00-00"), &tecKCharId);
      if(! success) {
        error(F("Could not add K constant"));
      }
  // Add the setpoint characteristic
      Serial.println(F("Adding the setpoint"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x1B7F,PROPERTIES=0X1A,MIN_LEN=1,MAX_LEN=4,VALUE=0, DATATYPE=3,DESCRIPTION=Setpoint,PRESENTATION=17-00-00-27-01-00-00"), &tecSetpointCharId);
      if(! success) {
        error(F("Could not add setpoint characteristic"));
      }
 // Add the measured resistance characteristic
      Serial.println(F("Adding the thermistor"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2B7F,PROPERTIES=0X1A,MIN_LEN=1,MAX_LEN=4,VALUE=0, DATATYPE=3,DESCRIPTION=Thermistor,PRESENTATION=17-00-00-27-01-00-00"), &tecThermistorCharId);
      if(! success) {
        error(F("Could not add thermistor characteristic"));
      }
  //Add the T characteristic
      Serial.println(F("Adding the T time constant characteristic (UUID = 0x2A7F)"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2A7F,PROPERTIES=0x1A, MIN_LEN=1,MAX_LEN=4,VALUE=10,DATATYPE=3,DESCRIPTION=Integration Time Constant,PRESENTATION=17-00-00-27-01-00-00"), &tecTCharId);
      if(! success) {
        error(F("Could not add T constant"));
      }
  //Add the Reset characteristic
      Serial.println(F("Adding the RESET characteristic (UUID = 0x2B80)"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2B80,PROPERTIES=0x1A, MIN_LEN=1,MAX_LEN=4,VALUE=10,DATATYPE=3,DESCRIPTION=Reset,PRESENTATION=17-00-00-27-01-00-00"), &tecResetCharId);
      if(! success) {
        error(F("Could not add reset"));
      }

  // Add the drive force characteristic
      Serial.println(F("Adding the current drive force"));
      success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2A80,PROPERTIES=0X12,MIN_LEN=1,MAX_LEN=4,VALUE=0, DATATYPE=3"), &tecDriveCharId);
      if(! success) {
        error(F("Could not add current drive force characteristic"));
      }
      

  //Let attributes take effect.
      Serial.println(F("Resetting BLE"));
      if( !ble.sendCommandCheckOK(F("ATZ"))){
        error(F("Could not reset BLE"));
      }
}
