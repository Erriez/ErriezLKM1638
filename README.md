# JY-LKM1638 7-segment display / button library for Arduino

This is a JY-MCU JY-LKM1638 library for Arduino. 

![JY-LKM1638](https://raw.githubusercontent.com/Erriez/ArduinoLibraryLKM1638/master/extras/LKM1638_board.jpg)

This board supports:
* 3-wire serial interface
* TM1638 LED driver and key-scan chip
* 5V power +/- 10%
* 8 digits 7-segment display
* 8 dual color LEDs 
* 8 buttons

## Order number
[Google.com](https://www.google.nl/search?q=JY-LKM1638)  
[DX.com SKU: 81873](http://www.dx.com/p/8x-digital-tube-8x-key-8x-double-color-led-module-81873#.WmR3YTco9aR)  
[AliExpress.com](https://www.aliexpress.com/item/F71A-8-Digital-Tube-8-Key-8-Double-Color-LED-Module-TM1638-Can-be-Cascaded-Replace/2034831073.html?ws_ab_test=searchweb0_0,searchweb201602_1_10065_10344_10068_10342_10343_10340_10341_10084_10083_10618_10304_10615_10307_10301_10313_10059_10534_100031_10103_441_10624_442_10623_10622_10621_10620_10142,searchweb201603_36,ppcSwitch_5&algo_expid=539835e4-fcbf-4f84-870e-c972d12374fa-36&algo_pvid=539835e4-fcbf-4f84-870e-c972d12374fa&priceBeautifyAB=2)  
[eBay.com](https://www.ebay.com/itm/8xDigital-Tube-8x-Key-8x-Double-Color-LED-Module-TM1638-For-AVR-Arduino-NEW/172744309516?epid=814196876&hash=item28385cfb0c:g:bmsAAOSw-29ZS4-c)  
Many more...

**Note:**   
This library has not been tested with "LED&KEY" board.

## Hardware

Connect GND and +5V to the Arduino board.

Connect the following pins to the Arduino DIGITAL pins:
* DIO (Bi-directional data input/output)
* STB (Chip select)
* CLK (Clock) 

## Example
Examples | JY-LKM1638:
* [Brightness](https://github.com/Erriez/ArduinoLibraryLKM1638/blob/master/examples/Brightness/Brightness.ino)  
* [Buttons](https://github.com/Erriez/ArduinoLibraryLKM1638/blob/master/examples/Buttons/Buttons.ino)  
* [Counter](https://github.com/Erriez/ArduinoLibraryLKM1638/blob/master/examples/Counter/Counter.ino)  
* [Temperature](https://github.com/Erriez/ArduinoLibraryLKM1638/blob/master/examples/Temperature/Temperature.ino)  
* [Time](https://github.com/Erriez/ArduinoLibraryLKM1638/blob/master/examples/Time/Time.ino)

## Terms:
```
Segment:   One LED in a 7-segment display
Digit:     One 7-segment display (Value 0..9 and A..F)
Dot:       The dot LED in a 7-segment digit
Pos:       Print position 0...7 (MSB bit 7: left .. LB bit 0: right)
Radius:    DEC for decimal, HEX for hexadecimal, BIN for binary
MaxDigits: Reserve a number of digits to print a value
Pad:       Display fixed number of digits with 0 padding
Overflow:  Value does not fit on the display, display minus chars
```

## Usage

### Initialization
```c++
#include <LKM1638Board.h>
  
// Connect display pins to the Arduino DIGITAL pins
#define DIO_PIN   2
#define SCL_PIN   3
#define STB_PIN   4
  
// Create LKM1638 board
LKM1638Board lkm1638(DIO_PIN, SCL_PIN, STB_PIN);
```

## Installation
1. Start the Arduino IDE.
2. Download the latest version from:  
   https://github.com/Erriez/ArduinoLibraryLKM1638/archive/master.zip  
   and  
   https://github.com/Erriez/ArduinoLibraryTM1638/archive/master.zip
3. Click Sketch | Include Library | Add .ZIP Library... and select this ZIP.
5. Run the example.

## Library dependencies
* [TM1638](https://github.com/Erriez/ArduinoLibraryTM1638)  
```git clone https://github.com/Erriez/ArduinoLibraryTM1638.git```
