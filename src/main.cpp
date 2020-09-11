#include <Arduino.h>
#include "mhz19_uart.h"

#if CO2_SENSOR_ENABLED
#endif // CO2_SENSOR_ENABLED

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MHZ19_uart.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}