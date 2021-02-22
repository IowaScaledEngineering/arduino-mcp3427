/*************************************************************************
Title:    Arduino I2C-MCP3427 Library
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

#include <MCP3427.h>

MCP3427::MCP3427()
{
	this->address = 0x68;
	this->i2cInterface = NULL;
}

bool MCP3427::begin(TwoWire& i2cInterface, uint8_t address)
{
	this->i2cInterface = &i2cInterface;
	this->address = address;
	return true;
}

bool MCP3427::begin(uint8_t address)
{
	return this->begin(Wire, address);
}

uint8_t MCP3427::getLowAddrBits(bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered)
{
	if (jp0l_soldered)
	{
		// ADR0 Low
		if (jp1l_soldered)
			return 0b000;  // A0 = Low, A1 = Low
		else if (jp1h_soldered)
			return 0b010;  // A0 = Low, A1 = High
		else
			return 0b001;  // A0 = Low, A1 = Float
	} else if (jp0h_soldered) {
		// ADR0 High
		if (jp1l_soldered)
			return 0b100;  // A0 = High, A1 = Low
		else if (jp1h_soldered)
			return 0b110;  // A0 = High, A1 = High
		else
			return 0b101;  // A0 = High, A1 = Float
	} else {
		// ADR0 Float
		if (jp1l_soldered)
			return 0b011;  // A0 = Float, A1 = Low
		else if (jp1h_soldered)
			return 0b111;  // A0 = Float, A1 = High
		else
			return 0b000;  // A0 = Float, A1 = Float
	}
	return 0b000;  // Eh, how'd we get here?
}

bool MCP3427::begin(bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered)
{
	return this->begin(Wire, jp0h_soldered, jp0l_soldered, jp1h_soldered, jp1l_soldered);
}

bool MCP3427::begin(TwoWire& i2cInterface, bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered)
{
	uint8_t baseAddr = 0x68 + getLowAddrBits(jp0h_soldered, jp0l_soldered, jp1h_soldered, jp1l_soldered);
	return this->begin(Wire, baseAddr);
}

bool MCP3427::writeConfiguration(uint8_t configByte)
{
	this->configByte = configByte;
	this->i2cInterface->beginTransmission(this->address);
	this->i2cInterface->write(this->configByte);
	if(0 != this->i2cInterface->endTransmission(true))
		return false;

	return true;
}

uint8_t MCP3427::computeConfigByte(uint8_t channel, bool continuousConversion, ADCBitDepth bitDepth, ADCGain gain)
{
	uint8_t configByte = 0;
	
	if (channel < 4)
		configByte |= (channel & 0x03) << 5;

	if (continuousConversion)
		configByte |= 0x10;
		
	switch(bitDepth)
	{
		case ADC_12_BITS:
			break; // Do nothing, 00
		case ADC_14_BITS:
			configByte |= 0x04;
			break;
		default:
		case ADC_16_BITS:
			configByte |= 0x08;
			break;
	}

	switch(gain)
	{
		default:
		case GAIN_1X:
			break;

		case GAIN_2X:
			configByte |= 0x01;
			break;

		case GAIN_4X:
			configByte |= 0x02;
			break;

		case GAIN_8X:
			configByte |= 0x03;
			break;
	}
	
	return configByte;
}

void MCP3427::delayByBitDepth(ADCBitDepth bitDepth)
{
	switch(bitDepth)
	{
		case ADC_16_BITS:
		default:
			delay(68);
			break;
		case ADC_14_BITS:
			delay(35);
			break;
		case ADC_12_BITS:
			delay(4);
			break;
	}
}

float MCP3427::convertToVoltage(int16_t adcVal, ADCGain gain, ADCBitDepth bitDepth)
{
	float divisor = 32767.0;
	float retval;
	
	// Mask off unused bits
	if(ADC_12_BITS == bitDepth)
		divisor = 2047.0;
	else if (ADC_14_BITS == bitDepth)
		divisor = 8191.0;
	
	switch(gain)
	{
		case GAIN_1X:
			retval = (2.048/divisor) * adcVal;
			break;

		case GAIN_2X:
			retval = (1.024/divisor) * adcVal;
			break;

		case GAIN_4X:
			retval = (0.512/divisor) * adcVal;
			break;

		case GAIN_8X:
			retval = (0.256/divisor) * adcVal;
			break;
			
		default:
			retval = 0.0;
			break;
	}

	return(retval);
}

float MCP3427::analogReadVoltage(uint8_t channel, ADCGain gain, ADCBitDepth bitDepth)
{
	return convertToVoltage(this->analogRead(channel, gain, bitDepth), gain, bitDepth);
}

int16_t MCP3427::analogRead(uint8_t channel, ADCGain gain, ADCBitDepth bitDepth)
{
	// This is the "simple mode" function
	uint8_t configByte = this->computeConfigByte(channel, false, bitDepth, gain);
	this->writeConfiguration(configByte | 0x80); // 0x80 starts the next conversion

	// Roughly wait the amount of time we need
	this->delayByBitDepth(bitDepth);

	bool conversionDone = false;
	uint8_t reattempts = 0;

	this->adcVal = 0;

	while(!conversionDone && reattempts++ < 8)
	{
		if (3 != this->i2cInterface->requestFrom(this->address, (uint8_t)3))
		{
			// Read error, got less than the expected byte
			continue;
		}

		int16_t adcVal = ((uint16_t)this->i2cInterface->read())<<8;
		adcVal |= (uint16_t)this->i2cInterface->read();

		uint8_t confByte = this->i2cInterface->read();
		if (confByte & 0x80)
		{
			// /RDY is still high, conversion not done
			delayMicroseconds(200);
			continue;
		} else {
			conversionDone = true;
			this->adcVal = adcVal;
		}
	}
	
	if (conversionDone)
	{
		return (this->adcVal);
	}
	
	return 0;
}

