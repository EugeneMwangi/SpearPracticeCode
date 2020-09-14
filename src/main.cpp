#include <Arduino.h>
#include "mhz19_uart.h"
#include "config.h"
#include "ec_5.h"

#if CO2_ENABLED
  //Declare an object for the mh-z19 co2 sensor
  MHZ19_uart mhz19;
#endif // CO2_ENABLED
#if EC5_ENABLED
  #define EC5_INPUT  A0
#endif // EC5_ENABLED
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MHZ19_uart mhz19;
  #if CO2_ENABLED
    //set auto calibration off
    mhz19.setAutoCalibration(false);
    #if ZERO_CALIBRATION_ENABLED
      mhz19.calibrateZero();
    #endif // ZERO_CALIBRATION_ENABLED
  #endif // CO2_ENABLED
  #if EC5_ENABLED
    ec5Init(EC5_INPUT);//Pass the analog input pin. Pwr pin is optional
  #endif // EC5_ENABLED
}

void loop() {
  // put your main code here, to run repeatedly:
  #if CO2_ENABLED
    int co2Val = mhz19.getPPM();
    Serial.print("CO2 reading = "); Serial.println(co2Val);
    delay(2000);
  #endif // CO2_ENABLED
  #if EC5_ENABLED
    float VWCReading = ec5VWCReading();
  #endif // EC5_ENABLED
}