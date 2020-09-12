/*
    ec_5.c - EC-5 sensor library for adafruit feather M0

    Licence MIT
*/
#include "ec_5.h"
#include <Arduino.h>

/**
 * @brief Function initializes the input pins of the EC-5 sensor
 * @param inputPin: analog pin connected to the ec-5's output.
 * @param pwrPin: digital pin connected to ec-5 pwr pin. If not connected, set to 0
*/
void ec5init(ec5Pins input, ec5Pins pwr)
{
    inputPin = input;
    pwrPin = pwr;
    pinMode(inputPin, INPUT);
    if(pwrPin != 0)pinMode(pwrPin, OUTPUT);
}

/**
 * @brief  Function reads raw voltage reading.
 * @retval average value of 10 raw voltage readings from ec5 sensor.
 */
float ec5VoltageReading()
{
    if(pwrPin != 0)digitalWrite(pwrPin, HIGH);
    int array[10], sum=0;
    float avg;
    for(int i=0; i<10;i++)
    {
        delayMicroseconds(15000);
        array[i] = analogRead(inputPin);    
    }
    digitalWrite(pwrPin, LOW);
    for(int x:array)
    {
        Serial.print("x: "); Serial.println(x);
        sum+=x;   
    }
    Serial.print("sum: "); Serial.println(sum);
    avg = (float)sum / 10;
    Serial.print("avg: "); Serial.println(avg); 
    return avg;   
}
