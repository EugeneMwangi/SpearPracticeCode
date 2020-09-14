# SHT1x Library
SHT1x is a humidity and temperature Sensor
## Communication with the sensor
For sending a command to the sensor, DATA is valid on the rising edge of the serial clock(SCK) and must remain stable while SCK is high.
For reading data from the sensor, DATA is valid Tv after SCK has gone low and remains valid until the next falling edge of SCK
### Starting up the sensor
After power up the sensor needs 11ms to get to Sleep State. No commands must be sent before that time
### Sending a Command
* Issue a Transmission Start sequence.
* Send the command.
* Acknowledgement of reception of command by the sensor.
### Measuring RH and T
* Issue measurement command [(ref)](#sending-a-command)
* Wait 20/80/320 ms for a 8/12/14 bit measurement.
* Sht1x pulls data line low and enters idle mode to signal measurement comletion.
* Microcontroller reads the data.
* MCU sends acknowledgement bit.
### CRC Check
The CRC check is neglected in this library and the mcu sets the DATA line after reading of the LSB byte.
