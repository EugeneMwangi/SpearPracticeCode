/*
    ec_5.h - ec_5 soil moisture sensor library for Adafruit Feather M0

    License MIT
*/

#ifndef EC_5_H_
#define EC_5_H_

#include <Arduino.h>

typedef int ec5Pins;
ec5Pins inputPin;
ec5Pins pwrPin;
void ec5init(ec5Pins input,  ec5Pins pwr=0);
float ec5VoltageReading();
float ec5VWCReading();
#endif // EC_5_H_