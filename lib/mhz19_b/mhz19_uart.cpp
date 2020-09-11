/*
    mhz19_uart.cpp - MH-Z19 co2 sensor library for adafruit feather M0

    Licence MIT
*/

#include "mhz19_uart.h"
#include "Arduino.h"
/**
 * @ 
 */
MHZ19_uart::MHZ19_uart()
{
    Serial1.begin();
}
