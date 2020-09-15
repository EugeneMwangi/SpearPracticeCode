#include <Arduino.h>
#include "mhz19_uart.h"
#include "config.h"
#include "ec_5.h"
#include "sht1x.h"

#if CO2_ENABLED
  //Declare an object for the mh-z19 co2 sensor
  MHZ19_uart mhz19(&Serial1);
#endif // CO2_ENABLED
#if EC5_ENABLED
  #define EC5_INPUT  A0
#endif // EC5_ENABLED
#if SHT1X_ENABLED
  #define SHT_SCK     12
  #define SHT_SDA     10
  Sht1x sht(SHT_SDA, SHT_SCK);
#endif // SHT1X_ENABLED
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
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
    Serial.print("VWC Value: "); Serial.println(VWCReading);
  #endif // EC5_ENABLED
  #if SHT1X_ENABLED
    float shtTemperature = sht.readTemperatureC();
    Serial.print("SHT1x temperature reading: "); Serial.println(shtTemperature);
  #endif // SHT1X_ENABLED
  delay(5000);
}