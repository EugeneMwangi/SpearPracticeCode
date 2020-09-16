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
 * @param i2c: ptr to i2c object
 * @param addr: address of the sensor
*/
BH1750::BH1750(TwoWire* i2c, byte addr)
{
    _addr = addr;
    _i2c  = i2c;
}
/**
 * @brief Deletes any memory in the heap
*/
BH1750::~BH1750()
{
    _i2c  = nullptr;
}
/**
 * @brief Sets the mode of the sensor
*/
bool BH1750::begin(Mode mode)
{
    //Set default transmission result to a value out of normal range
    byte ack = 5;

    //Check if mode is valid
    switch(mode)
    {
        case CONTINUOUSLY_H_RESOLUTION_MODE:
        case CONTINUOUSLY_H_RESOLUTION_MODE2:
        case CONTINUOUSLY_L_RESOLUTION_MODE:
        case ONE_TIME_H_RESOLUTION_MODE:
        case ONE_TIME_H_RESOLUTION_MODE2:
        case ONE_TIME_L_RESOLUTION_MODE:

            ack = writeCommand(mode);
            //Wait a few moments to wake up
            delay(10);
            break;
        default:
            // String is stored in flash
            Serial.println(F("[BH1750] ERROR: Invalid Mode"));
            break;
    }
    switch (ack)
    {
        case 0:
            _mode = mode;
            return true;
        case 1: // too long for transmit buffer
            Serial.println(F("[BH1750] ERROR: too long for transmit buffer"));
            break;
        case 2: // received NACK on transmit of address
            Serial.println(F("[BH1750] ERROR: received NACK on transmit of address"));
            break;
        case 3: // received NACK on transmit of data
            Serial.println(F("[BH1750] ERROR: received NACK on transmit of data"));
            break;
        case 4: // other error
            Serial.println(F("[BH1750] ERROR: other error"));
            break;
        default:
            Serial.println(F("[BH1750] ERROR: undefined error"));
            break;
    }
    return false;
}
/**
 * @brief Configures te bh1750 MTReg(measurement time register) value
 * @param MTreg: new value to be configured
 * @return bool true is MTreg is successfully set
 *              false if MTreg is not changed or parameter is out of range
*/
bool BH1750::setMTReg(byte mTReg)
{
    //Bug: lowest value seems to be 32!
    if (mTReg <= 31 || mTReg > 254) 
    {
        Serial.println(F("[BH1750] ERROR: MTreg out of range"));
        return false;
    }
    byte ack = 5;
    // Send MTreg and the current mode to the sensor
    //   High bit: 01000_MT[7,6,5]
    ack = writeCommand((0b01000 << 3) | (mTReg >> 5));
    //    Low bit: 011_MT[4,3,2,1,0]
    ack |= writeCommand((0b011 << 5 )  | (mTReg & 0b11111));
    // Input Measurement Command
    ack |= writeCommand(_mode);

    // Wait a few moments to wake up
    delay(10);

    // Check result code
    switch (ack) 
    {
        case 0:
            _mTReg = mTReg;
            // Delay for specific continuous mode to get valid values
            switch (_mode) 
            {
                case CONTINUOUSLY_L_RESOLUTION_MODE:
                    delay(24 * _mTReg/(byte)DEFAULT_MTREG);
                    break;
                case CONTINUOUSLY_H_RESOLUTION_MODE:
                case CONTINUOUSLY_H_RESOLUTION_MODE2:
                    delay(180 * _mTReg/(byte)DEFAULT_MTREG);
                    break;
                default:
                    break;
            }
            return true;
        case 1: // too long for transmit buffer
            Serial.println(F("[BH1750] ERROR: too long for transmit buffer"));
            break;
        case 2: // received NACK on transmit of address
            Serial.println(F("[BH1750] ERROR: received NACK on transmit of address"));
            break;
        case 3: // received NACK on transmit of data
            Serial.println(F("[BH1750] ERROR: received NACK on transmit of data"));
            break;
        case 4: // other error
            Serial.println(F("[BH1750] ERROR: other error"));
            break;
        default:
            Serial.println(F("[BH1750] ERROR: undefined error"));
            break;
    }

    return false;    
}
/**
 * @brief Reads the lux value
 * @retval Returns float value
*/
float BH1750::readLuxValue(bool maxWait)
{
    unsigned int data = 0;
    float luxValue = -1.0f;
    if (_mode == UNCONFIGURED)
    {
        Serial.println(F("[BH1750] Device is not configured!"));
        return -2.0;
    }
    writeCommand(_mode);
    delayPeriod(maxWait);
    readData(&data);
    luxValue = (float) data;
    if(luxValue != -1.0f)
    {
        #if BH1750_DEBUG
        Serial.print(F("[BH1750] Raw value: "));
        Serial.println(luxValue);
        #endif // BH1750_DEBUG

        if (_mTReg != DEFAULT_MTREG) 
        {
            luxValue *= (float)((byte)DEFAULT_MTREG/(float)_mTReg);
            // Print MTreg factor if debug enabled
            #if BH1750_DEBUG
            Serial.print(F("[BH1750] MTreg factor: "));
            Serial.println( String((float)((byte)DEFAULT_MTREG/(float)_mTReg)) );
            #endif // BH1750_DEBUG
        }
        if (_mode == ONE_TIME_H_RESOLUTION_MODE2 || _mode == CONTINUOUSLY_H_RESOLUTION_MODE2) 
        {
            luxValue /= 2;
        }
        // Convert raw value to lux
        luxValue /= _cvtVal;

        // Print converted value if debug enabled
        #if BH1750_DEBUG
        Serial.print(F("[BH1750] Converted float value: "));
        Serial.println(luxValue);
        #endif // BH1750_DEBUG        
    }

    return luxValue;
}

/* ============================ Private Functions =============================== */

/**
 * @brief Writes commands to the bh1750
 * @param cmd: command to be sent
 * @return byte ACK value
*/
byte BH1750::writeCommand(byte cmd)
{
    _i2c->beginTransmission(_addr);
    _i2c->write(cmd);
    return  _i2c->endTransmission();
}

/**
 * @brief Reads data from the bh1750
 * @param data: pointer to an array buffer to store the data
*/
void BH1750::readData(unsigned int *data)
{
    if(2==_i2c->requestFrom((int)_addr, (int)2))
    {
        *data = _i2c->read();
        *data <<= 8;
        *data |= _i2c->read();
    }
}
/**
 * @brief determines the delay time period for a measurement to be taken
 * @param maxWait: Determines if measurements have a maximum measurement time 
 *                 or a typical measurement time
*/
void BH1750::delayPeriod(bool maxWait)
{
    // Wait for measurement to be taken.
    // . The maxWait argument determines which measurement wait time is
    // used when a one-time mode is being used. The typical (shorter)
    // measurement time is used by default and if maxWait is set to True then
    // the maximum measurement time will be used. See data sheet pages 2, 5
    // and 7 for more details.
    // A continuous mode measurement can be read immediately after re-sending
    // the mode command.
    switch(_mode)
    {
        #define XCALC  (_mTReg / (byte) DEFAULT_MTREG)

        case ONE_TIME_L_RESOLUTION_MODE:
            maxWait ? delay(24 * XCALC) : delay(16 * XCALC);
            break;
        case ONE_TIME_H_RESOLUTION_MODE:
        case ONE_TIME_H_RESOLUTION_MODE2:
            maxWait ? delay(180 * XCALC) : delay(120 * XCALC);
            break;
        default:
            break;
    }
}







