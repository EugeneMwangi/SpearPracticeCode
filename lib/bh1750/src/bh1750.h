/**
 * BH1750 Library
 * 
 * Copyright 2020 Eugene Mwangi <eugene.mwangi@illuminumgreenhouses.com>
 * Based on previous work by:
 *      Christopher Laws: <http://www.metaclaws.com/>
 * 
 * Manages communication with BH1750 Light Sensor using the I2C protocol
*/

#ifndef BH1750_H_
#define BH1750_H_

#include <Arduino.h>
#include "Wire.h"

//Define the commands
#define POWER_DOWN_CMD                              0x00
#define POWER_ON_CMD                                0x01
#define RESET_CMD                                   0x07

#define DEFAULT_MTREG                               69

/**
 * @brief Defines the different applicable modes
*/
typedef enum mode 
{
    UNCONFIGURED,
    CONTINUOUSLY_H_RESOLUTION_MODE = 0x10,
    CONTINUOUSLY_H_RESOLUTION_MODE2 = 0x11,
    CONTINUOUSLY_L_RESOLUTION_MODE = 0x13,
    ONE_TIME_H_RESOLUTION_MODE = 0x20,
    ONE_TIME_H_RESOLUTION_MODE2 = 0x21,
    ONE_TIME_L_RESOLUTION_MODE = 0x23
}Mode;

class BH1750
{
    public:
        BH1750(TwoWire *i2c,byte addr = 0x23);
        bool begin(Mode mode);
        float readLuxValue(bool maxWait = false);
        bool setMTReg(byte mTReg);
        ~BH1750();

    private:
        Mode _mode = UNCONFIGURED;
        byte _addr;
        byte _mTReg = (byte)DEFAULT_MTREG;
        const float _cvtVal = 1.2f;
        TwoWire* _i2c = nullptr;
        byte writeCommand(byte cmd);
        void readData(unsigned int *data);
        void delayPeriod(bool maxWait);
};
#endif // BH1750_H_