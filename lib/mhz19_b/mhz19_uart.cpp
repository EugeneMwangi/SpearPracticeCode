/*
    mhz19_uart.cpp - MH-Z19 co2 sensor library for adafruit feather M0

    Licence MIT
*/

#include "mhz19_uart.h"
#include "Arduino.h"

#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

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
void MHZ19_uart::writeCommand(uint8_t *cmd, uint8_t *response = NULL)
{
    //Send the first 8 bits of the command with the size of the array
    Serial1.write(cmd, REQUEST_CNT);

    //Send the checksum value
    Serial1.write(getChecksum(cmd));

    // wait until TX buffer is empty then flush the Tx buffer
    Serial1.flush();

    if(response !=NULL)
    {
        int i = 0;
        while(Serial1.available()<=0)
        {
            if(++i> WAIT_READ_TIMES)
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
        Serial1.readBytes(response, RESPONSE_CNT);
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
    for(int i = 0; i < REQUEST_CNT; i++)
    {
        sum+=cmd[i];
    }
    sum = 0xff - sum + 0x01;
    return sum;
}