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

#include <Arduino.h>
#include "sht1x.h"

/**
 * @brief initialize the data and clock ins of the sht1x
 * @param dataPin: pin connected to sht1x data pin
 * @param clkPin: pin connected to sht1x clock pin
*/
Sht1x::Sht1x(int dataPin, int clkPin)
{
    _dataPin = dataPin;
    _clockPin = clkPin;
}

/* ======================== Public Methods ====================*/
/**
 * @brief Reads the current temperature in degrees Celsius
 * @retval Returns value as a float
*/
float Sht1x::readTemperatureC()
{
    int _val = 0; //Raw value returned from sensor
    float _temp = 0.0f; //Temperature value derived from raw value

    //Conversion coefficients from SHT1x datasheet
    const float d1 = -39.6; // For @3V VDD
    const float d2 = 0.01; // For 14bit  
    sendCommand(TEMP_MEASUREMENT_CMD);
    if((_ackFlag1 != true) && (_ackFlag2 != true))
    {
        Serial.println("Error sending temperature command");
        return -1.0f;
    }
    waitForMeasurement();
    _val = getData();
    // skipCRCCheck();

    _temp = d1 + (d2 * _val);

    return _temp;
}
/**
 * @brief Reads current temperature-corrected relative humidity
 * @retval Returns value as a float
*/
float Sht1x::readHumidity()
{
    int _val = 0;                       // Raw humidity value returned from sensor
    int _linearHumidity = 0;            // Linearly-corrected humidity
    float _correctedHumidity = 0.0f;    // Temperature-corrected humidity
    float _temp = 0.0f;                 // Current Temperature in Celsius 

    //Conversion coefficients from SHT!x datasheet
    const float c1 = -4.00f;          // For 12 bit resolution
    const float c2 = 0.0405f;         // For 12 bit resolution
    const float c3 = -0.0000028f;     // For 12 bit resolution
    const float t1 = 0.01f;           // For 12 bit resolution
    const float t2 = 0.00008f;        // For 12 bit resolution

    //Obtain Value from sensor
    sendCommand(RH_MEASUREMENT_CMD);
    if((_ackFlag1 != true) && (_ackFlag2 != true))
    {
        Serial.println("Error sending humidity command");
        return -1.0f;
    }
    waitForMeasurement();
    _val = getData();         
    // skipCRCCheck();

    //Perform linear correction on the data
    _linearHumidity = c1 + (c2 * _val) + (c3 * _val * _val);

    //Perform temperature correction on the data
    _temp = readTemperatureC();
    _correctedHumidity = ((_temp - 25) * (t1 + (t2 * _val))) + _linearHumidity;

    return _correctedHumidity;
}

/* ================================ Private Methods ============================== */

/**
 * @brief Lowers DATA line while SCK is high, followed by a low pulse 
 *        on SCK and raising DATA again while SCK is still high.
*/
void Sht1x::transmissionStartSequence()
{
    pinMode(_dataPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    // delayMicroseconds(11000);               // In case called immediately after power up
    digitalWrite(_clockPin,HIGH);
    digitalWrite(_dataPin, HIGH);
    // delayMicroseconds(5);
    digitalWrite(_dataPin, LOW);
    // delayMicroseconds(5);
    digitalWrite(_clockPin, LOW);
    // delayMicroseconds(5);
    digitalWrite(_clockPin, HIGH);
    // delayMicroseconds(5);
    digitalWrite(_dataPin, HIGH);
    digitalWrite(_clockPin, LOW);
}

/**
 * @brief Sends commands to the sensor.
*/
void Sht1x::sendCommand(uint8_t cmd)
{
    int ack;
    transmissionStartSequence();
    //Send command to the sensor.
    shiftOut(_dataPin, _clockPin, MSBFIRST,cmd);
    //Verify we get the correct ack
    digitalWrite(_clockPin, HIGH);
    pinMode(_dataPin, INPUT);
    ack = digitalRead(_dataPin);
    //if ack->HIGH, error.
    ack?(_ackFlag1 = false):(_ackFlag1 = true);
    digitalWrite(_clockPin, LOW);
    ack = digitalRead(_dataPin);
    //if ack -> LOW, error
    ack?(_ackFlag2 = true):(_ackFlag2 = false);

}
/**
 * @brief Waits for SHT1x to pull DATA line LOW when request for temperature or humidity is made
*/
void Sht1x::waitForMeasurement()
{
    int ack;
    pinMode(_dataPin, INPUT);
    while(1)
    {
        ack = digitalRead(_dataPin);
        if(ack == LOW)break;
        /*
            Passes control to other tasks when called. 
            Ideally yield() should be used in functions that will 
            take awhile to complete.
        */
        yield();
    }
}
/**
 * @brief Obtains the data from the sensor
 * @retval Returns an integer value
*/
int Sht1x::getData()
{
    uint8_t data[2] = {0};
    int value = 0;
    pinMode(_dataPin, INPUT);
    pinMode(_clockPin, OUTPUT);

    for(int i=0; i<2; ++i)
    {
        for(int j=0; j<8;++j)
        {
            digitalWrite(_clockPin, HIGH);
            data[i] |= (digitalRead(_dataPin)<<(7-j));
            digitalWrite(_clockPin,LOW);            
        }
        pinMode(_dataPin, OUTPUT);
        digitalWrite(_dataPin, HIGH);
        if(i==1)break;
        digitalWrite(_dataPin, LOW);
        digitalWrite(_clockPin, HIGH);
        digitalWrite(_clockPin, LOW);
        pinMode(_dataPin, INPUT);
    }
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
    for(int i = 0; i< 2; i++)
    {
        for(int j = 0; j<8;j++)
        {
            value = value | (data[i]<<(7-j));
        }
    }

    return value;
}