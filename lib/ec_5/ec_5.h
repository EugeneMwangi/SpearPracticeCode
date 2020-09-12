/*
    ec_5.h - ec_5 soil moisture sensor library for Adafruit Feather M0

    License MIT
*/

#ifndef EC_5_H_
#define EC_5_H_

/**
 * @brief A structure to store the connection pins of the ec_5
 * @param pwrpin: The pwr line of the ec_5 is connected to this pin
 * @pa
*/
typedef int ec5Input;

void init( ec5Input inputPin,  ec5Input pwrPin = 0);
float ec5VoltageReading();
float ec5VWCReading();
#endif // EC_5_H_