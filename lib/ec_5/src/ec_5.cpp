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

  /**
   * @brief  Function converts rar voltage reading to volumetric water content(VWC) reading.
   * @retval Calculated volumetric water content value.
   */
  float ec5VWCReading()
  {
    float avg = ec5VoltageReading();  
    /*      
     * Using a 10 bit ADC, the formula used is:
     *      VWC = 0.0014*(ADC output) - 0.4697
     *      link: https://www.researchgate.net/publication/320668407_An_Arduino-Based_Wireless_Sensor_Network_for_Soil_Moisture_Monitoring_Using_Decagon_EC-5_Sensors
     */
    float vwcValue = (0.0041 * avg) - 0.4895;
    Serial.print("VWC Value: "); Serial.println(vwcValue);
    return vwcValue;
  }