STM32F4 LCD LIBRARY
======
LCD Driver for use with HD44780 type displays. Major portions of the code were ported from the Arduino source code.

## Setup
To use the STM32F4 in the Linux environment, follow this [guide](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html).

Makefile and Scripts are written relative to their dependencies and references within the project. <br>
Please check the Makefile and Scripts to modify paths accordingly.

## Usage
See LCD.h file for list of functions. <br>
See examples to see their usage. <br>

LCD_4BitInit(..) is most likely the initialization you need if you are using only 4 wires for data lines. Does not matter if it is db0-3 or db5-7.

The [Arduino LiquidCrystal tutorials](http://arduino.cc/en/Tutorial/LiquidCrystal) may be helpful, although the syntax is slightly different.

## Commands
Make: compile program <br>
Make burn: flash the board <br>
Make clean: removes targets

ST-UTIL: starts utility server to talk to board <br>
DEBUG: starts debug console and hooks to server

## Contact
Maintainer: [Sharan ***REMOVED***](https://github.com/AtomicAntimatter)  ***REMOVED***

## LICENSE
STM32F4 LCD Library is available under the Creative Commmons Attribution-ShareAlike License.
See the [LICENSE](https://creativecommons.org/licenses/by-sa/4.0/legalcode) for more info.

## Copyright
Copyright &copy; 2014 Sharan ***REMOVED***
