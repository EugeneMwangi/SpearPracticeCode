# MHZ19 CO2 Sensor Library

Please read below for procedures to follow when calling specific functions for the mh-z19 co2 sensor

## Automatic Baseline Correction

After the module works for some time, it can judge the zero point intelligently and do the zero calibration
automatically. The calibration cycle is every 24 hours since the module is power on. The zero point is 400ppm.
This method is suitable for office and home environment, not suitable for agriculture greenhouse, farm,
refrigerator, etc.. If the module is used in latter environment, please turn off this function.

## Zero Calibration

The module should be placed in 400pm standard CO2 gas or clean outdoor environment for at least 20 min. Then send the command to perform zero calibration. 
Caution: Forbid sending this command in other environment except above.

## Span Point Calibration

Put the module in 2000ppm co2 gas, stability for at least 20 min. If the span value is 2000ppm, then HIGH byte(4th byte in the array) = 2000/256, LOW byte(5th byte in the array) = 2000 % 256. Then send the command.
Caution: Zero calibration should be done before span calibration. It is recommended to use 2000ppm as the SPAN calibration value. If lower value as the span value is needed, choose value above 1000ppm

## Calculating the CO2 Value

The getSerialData is used to obtain the co2 value. This value is the third(HIGH level output) and fourth(LOW level value) byte of the response array from the sensor.
The following formula is used:
> HIGH * 256 + LOW


## Calculating for Checksum Value

* Add Byte 1 to Byte 7 of the command or response array.
* Subtract the resulting sum from 0xff i.e. 
    > 0xff - sum
* Add1 to the new value i.e. 
    > (0xff-sum) + 0x01