/*************************************************************************
Title:    Iowa Scaled Engineering I2C-MCP3427 Example Sketch
Authors:  Nathan D. Holmes <maverick@drgw.net>
File:     $Id: $
License:  GNU General Public License v3

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

    For more information about the Iowa Scaled Engineering I2C-MCP3427, see:
    http://www.iascaled.com/store/I2C-MCP3427

*************************************************************************/

#include <Wire.h>
#include <MCP3427.h>

MCP3427 adcBoard;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();

  // This initialization assumes that the I2C-MCP3427 is in its default state - both address
  // lines are floating.  If it's not, you may have to put in a different begin() statement
  adcBoard.begin();
}

void loop() {
  // Simplest usage - just prints the voltage on channel 0
  Serial.print("Channel 0: ");
  Serial.print(adcBoard.analogReadVoltage(0));
  Serial.print("V\n");

  // More complex use - set the channel gain on 1 to 8x for small signals
  // and specify that the resolution is only 12 bits (240Hz reads)
  // Also, read the raw ADC value and convert it to a voltage later
  MCP3427::ADCGain gain = MCP3427::GAIN_8X;
  MCP3427::ADCBitDepth bitDepth = MCP3427::ADC_12_BITS;
  int ch1Value = adcBoard.analogRead(1, gain, bitDepth);

  Serial.print("Channel 1: ");
  Serial.print(ch1Value);
  Serial.print(" = ");
  Serial.print(adcBoard.convertToVoltage(ch1Value, gain, bitDepth));
  Serial.print("V\n");
 
  delay(250);
  Serial.print("\n");
}
