/*************************************************************************
Title:    Arduino I2C-IRSENSE Library Header
Authors:  Nathan D. Holmes <maverick@drgw.net>
File:     $Id: $
License:  GNU General Public License v3

This is an Arduino-compatible MCP3427 ADC library for the 
Iowa Scaled Engineering I2C-MCP3427 Qwiic board.   For more information about the
I2C-MCP3427, see here:  https://www.iascaled.com/store/I2C-MCP3427

LICENSE:
    Copyright (C) 2021 Nathan D. Holmes & Michael D. Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

#ifndef _MCP3427_H_
#define _MCP3427_H_

#include <Wire.h>
#include <Arduino.h>

class MCP3427
{
	public:
		MCP3427();
		bool begin(uint8_t address = 0x68);
		bool begin(TwoWire& i2cInterface, uint8_t address = 0x68);
		bool begin(bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered );
		bool begin(TwoWire& i2cInterface, bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered);

		enum ADCGain { GAIN_1X = 1, GAIN_2X = 2, GAIN_4X = 4, GAIN_8X = 8 };
		enum ADCBitDepth { ADC_16_BITS = 16, ADC_14_BITS = 14, ADC_12_BITS = 12 };


		int16_t analogRead(uint8_t channel, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS);
		float analogReadVoltage(uint8_t channel, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS);
		float convertToVoltage(int16_t adcVal, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS);

	private:
		TwoWire* i2cInterface;
		uint8_t address;
		int16_t adcVal;
		void delayByBitDepth(ADCBitDepth bitDepth);
		uint8_t getLowAddrBits(bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered);
		bool writeConfiguration(uint8_t configByte);
		uint8_t computeConfigByte(uint8_t channel, bool continuousConversion = false, ADCBitDepth bitDepth = 16, ADCGain gain = 1);
		uint8_t configByte;

};

#endif

