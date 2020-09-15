/**
 * SHT1x Library
 * 
 * Copyright 2020 Eugene Mwangi <eugene.mwangi@illuminumgreenhouses.com>
 * Based on previous work by:
 *      Jonathan Oxer: <jon@oxer.com.au> / <www.practicalarduino.com>
 * 
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature / humidity sensors from Sensirion (www.sensirion.com)
*/

#ifndef SHT1X_H_
#define SHT1X_H_

#include <Arduino.h>

#define TEMP_MEASUREMENT_CMD                0b00000011Ul
#define RH_MEASUREMENT_CMD                  0b00000101Ul
#define STATUS_REG_READ                     0b00000111Ul
#define STATUS_REG_WRITE                    0b00000110Ul
#define SOFT_RST_CMD                        0b00011110Ul

#define RH_MEASUREMENT_RESOLUTION           12  // Default measurement resolution for humidity
#define TEMP_MEASUREMENT_RESOLUTION         14  // Default measurement resolution for temperature

#define SHT1X_DEBUG                         1
#define SHT1X_RUN                           0
class Sht1x
{
    public:
        float readHumidity();
        float readTemperatureC();
        void setStatus();
        Sht1x(int dataPin, int clkPin);

    private:
        int _dataPin;
        int _clockPin;
        boolean _ackFlag1 = false, _ackFlag2 = false;
        void sendCommand(uint8_t cmd);
        void waitForMeasurement();
        int getData();
        void transmissionStartSequence();
        #if SHT1X_RUN
            int shiftIn(int _numBits);
            void skipCRCCheck();
        #endif // SHT1X_RUN
};
#endif // SHT1X_H_