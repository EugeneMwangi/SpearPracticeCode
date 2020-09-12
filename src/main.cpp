#include <Arduino.h>
#include "mhz19_uart.h"
#include "config.h"


#if CO2_SENSOR_ENABLED
//Declare an object for the mh-z19 co2 sensor
MHZ19_uart mhz19;
#endif // CO2_SENSOR_ENABLED
MHZ19_uart mhz19;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  #if CO2_SENSOR_ENABLED
  //set auto calibration off
  mhz19.setAutoCalibration(false);
  #if ZERO_CALIBRATION_ENABLED
  mhz19.calibrateZero();
  #endif // ZERO_CALIBRATION_ENABLED
  #endif // CO2_SENSOR_ENABLED
}

void loop() {
  // put your main code here, to run repeatedly:
  #if CO2_SENSOR_ENABLED
  int co2Val = mhz19.getPPM();
  Serial.print("CO2 reading = "); Serial.println(co2Val);
  delay(2000);
  #endif // CO2_SENSOR_ENABLED
}