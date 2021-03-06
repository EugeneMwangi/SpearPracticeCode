/*
    mhz19_uart.h - mh-z19 co2 sensor library for Adafruit Feather M0

    License MIT
*/

#ifndef MHZ19_UART_H_
#define MHZ19_UART_H_

#include <Arduino.h>

#define SERIAL_TIMEOUT		 500

#define REQUEST_CNT          8 //Number of bytes sent from the controller to the mhz19 for any command
#define RESPONSE_CNT	     9 //Number of bytes received from sensor.
class MHZ19_uart
{
	public:
		MHZ19_uart(Stream *stream);
		int getPPM();


		void begin();
		void setAutoCalibration(boolean autocalib);
		void calibrateZero();
		void calibrateSpan(int ppm);
		~MHZ19_uart();

	protected:
		void writeCommand(uint8_t *cmd, uint8_t *response = NULL);

    private:
		//Pointer to the Serial1
		Stream *_serial = nullptr;
        //Define data variables to hold the 8 byte commands
		// serial command
		uint8_t getppm[REQUEST_CNT]			= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
		uint8_t zerocalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
		uint8_t spancalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
		uint8_t autocalib_on[REQUEST_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
		uint8_t autocalib_off[REQUEST_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};
		uint8_t getChecksum(uint8_t *packet);
		int getSerialData();
};
#endif // MHZ19_UART_H_