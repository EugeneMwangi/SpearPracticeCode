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
// #include "Wire.h"

//Define the commands
#define POWER_DOWN_CMD                              0x00U
#define POWER_ON_CMD                                0x01U
#define RESET_CMD                                   0x07U
#define CONTINUOUSLY_H_RESOLUTION_MODE_CMD          0x10U
#define CONTINUOUSLY_H_RESOLUTION_MODE2_CMD         0x11U
#define CONTINUOUSLY_L_RESOLUTION_MODE_CMD          0x13U
#define ONE_TIME_H_RESOLUTION_MODE_CMD              0x20U
#define ONE_TIME_H_RESOLUTION_MODE2_CMD             0x21U
#define ONE_TIME_L_RESOLUTION_MODE_CMD              0x23U

#define _DELAY_MS_                                   120

/**
 * @brief Defines the different applicable modes
*/
typedef enum mode 
{
    CONTINUOUSLY_H_RESOLUTION_MODE,
    CONTINUOUSLY_H_RESOLUTION_MODE2,
    CONTINUOUSLY_L_RESOLUTION_MODE,
    ONE_TIME_H_RESOLUTION_MODE,
    ONE_TIME_H_RESOLUTION_MODE2,
    ONE_TIME_L_RESOLUTION_MODE
}Mode;

class BH1750
{
    public:
        BH1750(Mode mode, byte addr);
        float readLuxValue();

    private:
        Mode _mode;
        byte _addr;
        void writeCommand();
        void readData(byte *data);
};
#endif // BH1750_H_