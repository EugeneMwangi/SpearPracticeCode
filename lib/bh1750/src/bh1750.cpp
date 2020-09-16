/**
 * BH1750 Library
 * 
 * Copyright 2020 Eugene Mwangi <eugene.mwangi@illuminumgreenhouses.com>
 * Based on previous work by:
 *      Christopher Laws: <http://www.metaclaws.com/>
 * 
 * Manages communication with BH1750 Light Sensor using the I2C protocol
*/

#include <Arduino.h>
#include "bh1750.h"

/* ========================== Public Functions =================== */ 

/**
 * @brief Initializes the mode and address of the bh1750
 * @param mode: mode of the sensor
 * @param addr: address of the sensor
*/
BH1750::BH1750(Mode mode, byte addr)
{
    _mode = mode;
    _addr = addr;
}
/**
 * @brief Reads the lux value
 * @retval Returns float value
*/
float BH1750::readLuxValue()
{
    byte data[2] = {0};
    float luxValue = 0.0f;
    writeCommand();
    delay(_DELAY_MS_);
    readData(data);
    int highByte = static_cast<int>(data[0]);
    int lowByte = static_cast<int>(data[1]);
    luxValue = (highByte*256 + lowByte) / 1.2;

    return luxValue;
}

/* ============================ Private Functions =============================== */

/**
 * @brief Writes commands to the bh1750
*/
void BH1750::writeCommand()
{
    byte addr;
    addr = _addr<<1;
    //TO DO: Power Up the bh1750

    //TO DO: Send the address

    //TO DO: Check for HIGH ACK value from bh1750

    //Send OpeCode
    switch(_mode)
    {
        case CONTINUOUSLY_H_RESOLUTION_MODE :
            {
                //TO DO: Send command
                break;
            }
        case CONTINUOUSLY_H_RESOLUTION_MODE2 :
            {
                //TO DO: Send command
                break;
            }
        case CONTINUOUSLY_L_RESOLUTION_MODE :
            {
                //TO DO: Send command
                break;
            }
        case ONE_TIME_H_RESOLUTION_MODE : 
            {
                //TO DO: Send command
                break;
            }
        case ONE_TIME_H_RESOLUTION_MODE2 :
            {
                //TO DO: Send command
                break;
            }
        case ONE_TIME_L_RESOLUTION_MODE :
            {
                //TO DO: Send command
                break;
            }
        default:
            {
                Serial.println("INVALID MODE SELECTED");
            }
    }

    //TO DO: Check for HIGH ACK value from bh1750

    //TO DO: Power Down the bh1750
}

/**
 * @brief Reads data from the bh1750
 * @param data: pointer to an array buffer to store the data
*/
void BH1750::readData(byte *data)
{
    byte addr = (_addr<<1) | (1<<0);

    //TO DO: Power up the sensor

    //TO DO: Send the address.

    //TO DO: Read the HIGH ACK value from the sensor

    //TO DO: Read the HIGH Byte from the sensor

    //TO DO: Send HIGH ACK value to the sensor

    //TO DO: Read the LOW Byte from the sensor

    //TO DO: Send LOW ACK value to the sensor

    //TO DO: Power down the sensor
}