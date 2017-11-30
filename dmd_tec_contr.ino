/*****************************************************************
 *  DMD TEC Controller
 *   
 *  Author: Enrique Mendez, 10/19/2017
 *     
 *     PI Controller for driving the TEC
 *     Designed to work with the Adafruit Feather M0 Bluefruit LE
 *     
 *     MIN BLE Firmware (Codebase Rev.) = 0.7.0
 *     
 *     For feathers, to use the DAC, it is necessary to set 
 *     analogResolution to 10, and to NOT assign the pinMode of 
 *     the DAC.
******************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"

#include "BluefruitConfig.h"
#include "bluefruit_initializer.h"
#include "bluetooth_interface.h"
#include "controller.h"               

#define OFFSET 338                    //Offset output voltage in quanta. Compensates
                                        //for diode drop.1.1

// Create the bluefruit object
    Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

//Define the system parameters
  float measuredResistance,
    setpoint,
    errorSignal, 
    driveSignal;
    

  const float VCC = 3.3;                //Supply rail voltage (Volts)
  const float DACResolution = 1024.0;   //Number of DAC Divisions.
  const float RL  = 0.1;                //Load resistance (Ohms)
  
//Algorithm parameters in SI units unless noted.
   float dt;              //Timestep for numerical integration.
   float K;                 //Proportional Gain                     (amps per kohm)
                                    //desired driving force at zero integration.
   float Ti;                //Integration Time Constant             (seconds)
                                    // time scale at which integration becomes of same magnitude as P control
                                    // Ideally, ~ the timescale of the system.
   float I;                 //Integrated Value                      ()
   bool rst = 0;  
void setup() {
  K = 0.1;
  Ti = 60;
  I = 0;
  setpoint = 14e3;
  //INITIALIZE PINS
      pinMode(A1, INPUT);
      
  //SETUP COMMUNICATIONS 
  //*** Bluetooth and PC.
  //while(!Serial);
    Serial.begin(115200);
    Serial.print(F("Initialising the Bluefruit LE module: "));
    if( !ble.begin(VERBOSE_MODE) ){
      error( F("Couldn't find Bluefruit, make sure it's in command mode & check wiring.") );
    }
    Serial.println( F("OK!") );

    initialize(ble);

    write(ble, tecKCharId, (int)(100*K));
    write(ble, tecTCharId, (int)(100*Ti));
    write(ble, tecSetpointCharId, (int)(setpoint));
  //Setup the Feather M0 DAC.
    analogWriteResolution(10);
  
  //Set t=0;
    differentialTime();
}

void loop() { 
  delay(200);
  //Take measurements and convert to SI units
    measuredResistance = readResistance();
    dt = differentialTime()*1e-6;

  //Perform PID Calculations.
        errorSignal = (setpoint - measuredResistance)/1000;     //Convert to kiloohms.
        I += dt*errorSignal;                                                                                                    
        driveSignal = K*(errorSignal + I/Ti)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ;                   //(Amps/Kiloohm * Kiloohms);
          //Convert from amps to 0 . 1023 (ADC divisions)
        driveSignal = (DACResolution / VCC)*RL*driveSignal;
    
      //Allow for driveSignal saturation.
        driveSignal = max(min( OFFSET + (int) round(driveSignal), 1023), 0);

  //Transmit information over bluetooth
    // >> Output
      write(ble, tecDriveCharId, (int16_t) driveSignal);
      write(ble, tecThermistorCharId, (int16_t) measuredResistance);
    // << Input 
      K = read(ble, tecKCharId)/100.;
      delay(10);
      Ti = read(ble, tecTCharId)/100.;
      delay(10);
      setpoint = read(ble, tecSetpointCharId);
      delay(10);
      rst = read(ble, tecResetCharId);
      if(rst != 0){
        write(ble, tecResetCharId, 0);
        I = 0;
      }
   Serial.print("Integration is  : "); Serial.println(I);   
   Serial.print("Drive is  : "); Serial.println(driveSignal);
   Serial.print("K received: "); Serial.println(K);
   Serial.print("T received: "); Serial.println(Ti);
   Serial.print("Setpoint received: "); Serial.println(setpoint);
   
  //Drive current
      analogWrite(A0, driveSignal);
}
