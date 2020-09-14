# SHT1x Library
SHT1x is a humidity and temerature Sensor
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
* Issue measurement command[ref](#sending-a-command)