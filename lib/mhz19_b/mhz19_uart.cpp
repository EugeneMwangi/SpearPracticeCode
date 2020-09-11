/*
    mhz19_uart.cpp - MH-Z19 co2 sensor library for adafruit feather M0

    Licence MIT
*/

#include "mhz19_uart.h"
#include "Arduino.h"
/**
 * @brief The constructor initializes the uart serial communication
 */
MHZ19_uart::MHZ19_uart()
{
    Serial1.begin(9600);
}

/**
 * @brief The destructor can be used to delete any memory in heap once 
 *        an object is out of scope. It is a virtual destructor, which 
 *        means that both base and derived class objects with memory in 
 *        the heap will be deleted, therefore avoiding memory leak.
*/
MHZ19_uart::~MHZ19_uart()
{}

/**
 * @brief If true, this function sets the sensor to auto calibrate itself 
 *        in every 24 hrs after being powered on. 
 * @param autocalib: a boolean that determines if auto calibration will be s
 *                   set or not
*/
void MHZ19_uart::setAutoCalibration(boolean autocalib)
{
    writeCommand(autocalib? autocalib_on : autocalib_off);
}

/**
 * @brief Sends a command to the sensor to erform zero calibration.
*/
void MHZ19_uart::calibrateZero()
{
    writeCommand(zerocalib);
}
/**
 * @brief Sends command to set the span of the sensor. Put the module
*/