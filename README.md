# Arduino Library for the Iowa Scaled Engineering I2C-MCP3427

### Summary

The [Iowa Scaled Engineering I2C-MCP3427](https://www.iascaled.com/store/I2C-MCP3427) is a [Qwiic-compatible I2C](https://www.sparkfun.com/qwiic) 16-bit delta-sigma analog to digital converter (ADC) based on
on the [Microchip MCP3427](https://www.microchip.com/wwwproducts/en/MCP3427).

The MCP3427 offers:
* Delta-sigma architecture for reliable, low-noise conversions
* Dual differential inputs capable of 0-3.3V common mode and +/- 2.048V differential signals
* Software-adjustable resolution and conversion speed (16 bits @ 15 samples/sec, 14 bits @ 60 samples/sec, 12 bits @ 240 samples/sec)
* Software-controlled programmable gain amplifier can apply a gain of 1x, 2x, 4x, or 8x to the input signal to avoid external gain amplifiers
* Built-in 2.048V, 0.05% reference

The board comes equipped with:
* Two 4-position terminal blocks for easy analog input wiring
* Two [Qwiic-compatible](https://www.sparkfun.com/qwiic) JST connectors for easy connection to Qwiic-compatible Arduinos and peripherals
* Four solder jumpers for setting up to 9 different I2C addresses
* Four screw holes for easy mounting

-----

### Operation

The Golden Rule:  When in doubt, check [the MCP3427 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/22226a.pdf)  This guide is designed to tell you about the library, not be a comprehensive guide to the MCP3427.

-----

### Library Installation

The easiest way is to search for "I2C-MCP3427" in the Arduino Library manager.

-----

### Library Functions and Class Methods

This is the Arduino-compatible control library for the I2C-MCP3427.  

The board is represented by an MCP3427 class with the following methods available:

#### bool begin(uint8_t address = 0x68);
#### bool begin(TwoWire& i2cInterface, uint8_t address = 0x68);
#### bool begin(bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered );
#### bool begin(TwoWire& i2cInterface, bool jp0h_soldered, bool jp0l_soldered, bool jp1h_soldered, bool jp1l_soldered);

Before using the MCP3427 object, begin() must be called once. 

__Arguments:__
* If called with no arguments, it will initialize to use the Wire I2C object and the default address of 0x68 for the ADC.
* Optionally, it can be called with a different address for the MCP3427 directly.
* Optionally, it can be called with an alterate Arduino Wire interface, for microcontrollers having more than one
* Optionally, it can be called with the state of the solder jumpers, and the address will be calculated internally.

__Return values:__
* true - initialization was successful
* false - initialization failed

-----

#### int analogRead(uint8_t channel, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS)

Just like the built-in Arduino analogRead() function, this will read the channel set as an argument and return it as a signed integer between -32768 and +32767.

By default, the ADC reads at a gain of 1X and a bit depth of 16 bits.  You do not need to pass these arguments.  If you choose to, you can pass these two arguments 
to modify how the ADC operates.  Please see the datasheet for details.

__Arguments:__

channel must be 0 or 1, depending on the channel you want to read

gain is optional, defaulting to 1X, but can be one of:
* MCP3427::GAIN_1X - 1x input gain
* MCP3427::GAIN_2X - 2x input gain
* MCP3427::GAIN_4X - 4x input gain
* MCP3427::GAIN_8X - 8x input gain

bitDepth is optional, defaulting to 16 bits, but can be one of:
* MCP3427::ADC_16_BITS - Sample 16 bits @ 15 samples/second
* MCP3427::ADC_14_BITS - Sample 14 bits @ 30 samples/second
* MCP3427::ADC_12_BITS - Sample 12 bits @ 240 samples/second


__Return values:__
* -32768 to 32767 (for 16 bit depth)
* -8192 to 8191 (for 14 bit depth)
* -2048 to 2047 (for 12 bit depth)

-----

#### float analogReadVoltage(uint8_t channel, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS)

Will read the selected input channel and convert into an actual voltage based on the gain settings.

By default, the ADC reads at a gain of 1X and a bit depth of 16 bits.  You do not need to pass these arguments.  If you choose to, you can pass these two arguments 
to modify how the ADC operates.  Please see the datasheet for details.

__Arguments:__

channel must be 0 or 1, depending on the channel you want to read

gain is optional, defaulting to 1X, but can be one of:
* MCP3427::GAIN_1X - 1x input gain
* MCP3427::GAIN_2X - 2x input gain
* MCP3427::GAIN_4X - 4x input gain
* MCP3427::GAIN_8X - 8x input gain

bitDepth is optional, defaulting to 16 bits, but can be one of:
* MCP3427::ADC_16_BITS - Sample 16 bits @ 15 samples/second
* MCP3427::ADC_14_BITS - Sample 14 bits @ 30 samples/second
* MCP3427::ADC_12_BITS - Sample 12 bits @ 240 samples/second

__Return values:__
* -2.048 to +2.048 volts

-----

#### float convertToVoltage(int16_t adcVal, ADCGain gain=GAIN_1X, ADCBitDepth bitDepth=ADC_16_BITS)

Takes the signed integer returned by analogRead() and converts to a voltage.  Note that the gain and bit depth specified here
must match exactly the parameters used in analogRead() to get the initial reading.  The defaults are the same if not specified.

__Arguments:__

gain is optional, defaulting to 1X, but can be one of:
* MCP3427::GAIN_1X - 1x input gain
* MCP3427::GAIN_2X - 2x input gain
* MCP3427::GAIN_4X - 4x input gain
* MCP3427::GAIN_8X - 8x input gain

bitDepth is optional, defaulting to 16 bits, but can be one of:
* MCP3427::ADC_16_BITS - Sample 16 bits @ 15 samples/second
* MCP3427::ADC_14_BITS - Sample 14 bits @ 30 samples/second
* MCP3427::ADC_12_BITS - Sample 12 bits @ 240 samples/second


__Return values:__
* -2.048 to +2.048 volts

-----

### Hardware

The I2C-MCP3427 hardware design files can be found as part of the [ISE I2C-MCP3427 project](https://github.com/IowaScaledEngineering/i2c-mcp3427).  Assembled and tested parts can be purchased from the [ISE store](https://www.iascaled.com/store/I2C-MCP3427).

### License

Like all public Iowa Scaled Engineering designs, the I2C-MCP3427 is both open hardware and open source software.  Please refer to the hardware and software license files in this repository for details.
