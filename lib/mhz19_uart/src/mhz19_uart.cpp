/*
    mhz19_uart.cpp - MH-Z19 co2 sensor library for adafruit feather M0

    Licence MIT
*/

#include "mhz19_uart.h"
#include <Arduino.h>

#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

/**
 * @brief Initializes the Serial1 communication
 * @param * stream: pointer to Serial1 
*/
MHZ19_uart::MHZ19_uart(Stream * stream)
{
    _serial = stream;
}
/**
 * @brief Initializes _serial to nullptr
*/
MHZ19_uart::~MHZ19_uart()
{
    _serial = nullptr;
}

/**
 * @brief If true, this function sets the sensor to auto calibrate itself 
 *        in every 24 hrs after being powered on. 
 * @param autocalib: a boolean that determines if auto calibration will be 
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
 * @brief Sends command to set the span of the sensor. 
 * @param ppm. The span threshold value
*/
void MHZ19_uart::calibrateSpan(int ppm)
{
    if (ppm < 1000) return;
    spancalib[3] =  uint8_t(ppm/256);
    spancalib[4] = uint8_t(ppm%256);
    writeCommand(spancalib);
}

/**
 * @brief Obtains the co2 ppm reading
 * @retval returns the Co2 ppm value.
*/
int MHZ19_uart::getPPM()
{
    return getSerialData();
}

/**
 * @brief Sends commands to the sensor.
 * @param cmd- the command to be sent.
 * @param response holds the response of the sensor, if required, otherwise set 
 *        to NULL.
*/
void MHZ19_uart::writeCommand(uint8_t *cmd, uint8_t *response)
{
    //Initialize the hardware serial. Set baud to 9600(arbitrary?)
    // _serial->begin(9600);
    while(_serial->available()>0){_serial->read();}
    //Send the first 8 bits of the command with the size of the array
    _serial->write(cmd, REQUEST_CNT);

    //Send the checksum value
    _serial->write(getChecksum(cmd));

    // wait until TX buffer is empty then flush the Tx buffer
    _serial->flush();

    if(response !=NULL)
    {
        unsigned long time = millis();
        while(_serial->available()<=0)
        {
            if(millis() - time >= SERIAL_TIMEOUT)
            {
               Serial.println("error: can't get MH-Z19 response");
                return;
            }
            /*
                Passes control to other tasks when called. 
                Ideally yield() should be used in functions that will 
                take awhile to complete.
            */
            yield();
        }
        //Read the response from the sensor
        _serial->readBytes(response, RESPONSE_CNT);
    }
}

/**
 * @brief Reads the data from the sensor
 * @retval The data from the sensor
*/
int MHZ19_uart::getSerialData()
{
    uint8_t buf[RESPONSE_CNT] = {0x00};
    
    writeCommand(getppm, buf);
    int co2=0;
    //Parse the data
    if(buf[0] == 0xff && buf[1] == 0x86 && buf[RESPONSE_CNT-1] == getChecksum(buf))
    {
        //Calculate for the calue of co2 in ppm
        co2 = (int)(buf[2] * 256 + buf[3]);
    }
    else
    {
        co2 = -1;
    }
    return co2;    
}

/**
 * @brief Calculates the checksum value
 * @param cmd: This is the command to be sent or the response received.
 * @retval Returns the checksum value
*/

uint8_t MHZ19_uart::getChecksum(uint8_t cmd[])
{
    uint8_t sum = 0x00;
    for(int i = 1; i < REQUEST_CNT; i++)
    {
        sum+=cmd[i];
    }
    sum = 0xff - sum + 0x01;
    return sum;
}