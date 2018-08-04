# JY-LKM1638 7-segment display / button library for Arduino

This is a JY-MCU JY-LKM1638 library for Arduino.

![JY-LKM1638](https://raw.githubusercontent.com/Erriez/ErriezLKM1638/master/extras/LKM1638_board.jpg)

This board supports:
* 3-wire serial interface
* TM1638 LED driver and key-scan chip
* Power: 3.3V .. 5V
* 8 digits 7-segment display
* 8 dual color LEDs 
* 8 buttons

## Order number
[Google.com](https://www.google.nl/search?q=JY-LKM1638)  
[DX.com SKU: 81873](http://www.dx.com/p/8x-digital-tube-8x-key-8x-double-color-led-module-81873#.WmR3YTco9aR)  
[AliExpress.com](https://www.aliexpress.com/item/F71A-8-Digital-Tube-8-Key-8-Double-Color-LED-Module-TM1638-Can-be-Cascaded-Replace/2034831073.html?ws_ab_test=searchweb0_0,searchweb201602_1_10065_10344_10068_10342_10343_10340_10341_10084_10083_10618_10304_10615_10307_10301_10313_10059_10534_100031_10103_441_10624_442_10623_10622_10621_10620_10142,searchweb201603_36,ppcSwitch_5&algo_expid=539835e4-fcbf-4f84-870e-c972d12374fa-36&algo_pvid=539835e4-fcbf-4f84-870e-c972d12374fa&priceBeautifyAB=2)  
[eBay.com](https://www.ebay.com/itm/8xDigital-Tube-8x-Key-8x-Double-Color-LED-Module-TM1638-For-AVR-Arduino-NEW/172744309516?epid=814196876&hash=item28385cfb0c:g:bmsAAOSw-29ZS4-c)  
Many more...

**Note:** This library has not been tested with a different "LED&KEY" board.


## Hardware

Connect GND and +5V to the Arduino board.

Connect the following pins to the Arduino DIGITAL pins:
* DIO (Bi-directional data input/output)
* STB (Chip select)
* CLK (Clock) 

Note: Some Arduino boards cannot deliver enough 5V power to drive the LED's.


### Pins

| Pin  | LKM-1638 | Arduino UNO / Nano / Mega2560 / Leonardo / Pro Micro | Node MCU | LOLIN32 |
| :--: | :------: | :--------------------------------------------------: | :------: | :-----: |
|  1   |   VCC    |                     5V (or 3.3V)                     |   GND    |   GND   |
|  2   |   GND    |                         GND                          |   3V3    |   3V3   |
|  3   |   CLK    |                    Digital pin 2                     |    D2    |    0    |
|  4   |   DIO    |                    Digital pin 3                     |    D3    |    4    |
|  5   |   STB1   |                    Digital pin 4                     |    D4    |    5    |


## Examples
Examples | JY-LKM1638:
* [Brightness](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Brightness/Brightness.ino)  
* [Buttons](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Buttons/Buttons.ino)  
* [Counter](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Counter/Counter.ino)
* [Date](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Date/Date.ino)
* [Demo](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Demo/Demo.ino)  
* [Temperature](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Temperature/Temperature.ino)
* [TestLEDs](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/TestLEDs/TestLEDs.ino)  
* [Time](https://github.com/Erriez/ErriezLKM1638/blob/master/examples/Time/Time.ino)


## Documentation

* [Doxygen online HTML](https://erriez.github.io/ErriezLKM1638)
* [Doxygen PDF](https://github.com/Erriez/ErriezLKM1638/raw/gh-pages/latex/ErriezLKM1638.pdf)


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
LSB:       Most right digit, dual color LED8 or switch (SW8)
MSB:       Most left digit,  dual color LED1 or switch (SW1)
```


## Usage

### Initialization
```c++
#include <ErriezLKM1638Board.h>
  
// Connect display pins to the Arduino DIGITAL pins
#define TM1638_CLK_PIN   2
#define TM1638_DIO_PIN   3
#define TM1638_STB0_PIN  4
  
// Create LKM1638 board
LKM1638Board lkm1638(TM1638_CLK_PIN, TM1638_DIO_PIN, TM1638_STB0_PIN);

void setup()
{
    // Initialize LKM1638Board
    lkm1638.begin();
}
```

### Read 8 buttons
Buttons are 8-bit with bit 7 most left switch, bit 0 most right switch.

Note: The text on the board counts from S1 to S8!

```c++
uint8_t buttons = lkm1638.getButtons();
```

### Control 8 dual color LED's
Dual color LED 7 = most left (Text LED8)  
Dual color LED 0 = most right (Text LED0)

```c++
// Turn LED 0 red on (firt LED on the right)
lkm1638.setColorLED(0, LedRed);  
  
// Turn LED 0 green on
lkm1638.setColorLED(0, LedGreen);
  
// Turn LED 0 off
lkm1638.setColorLED(0, LedOff);
  
// Turn multiple LEDs on, color red
lkm1638.colorLEDsOn(0xA9, LedRed);
  
// Turn multiple LEDs off
lkm1638.colorLEDsOff(0x1F);
```

### Clear display
```c++
lkm1638.clear();
```

### Set/get print display position
The print position can be set from 0..7.  
7 = most left digit  
0 = most right digit

```c++
// Set postion 4
lkm1638.setPrintPos(4);
  
// Get print position
uint8_t pos = lkm1638.getPrintPos();
```

### Print variable on 7-segment display
Printing starts from digit right to left with an optional maximum number of 
digits. 

Minus '-' chars will be displayed when the value is out of range, or
does not fit on the display.

Optional padding can be used to display zero's. This is for example useful to 
print hours and minutes with fixed 2 digits.  

```c++
// Print int16_t on print position
lkm1638.print(1234);
  
// Print signed 32-bit value
lkm1638.print(-1234567);
  
// Print 16-bit unsigned casted value
lkm1638.print((uint16_t)65535);
  
// Print 16-bit hexadecimal unsigned value
uint16_t value = 0xBEEF;
lkm1638.print(value, HEX); 
  
// Print value with maximum 2 digits
uint8_t value = 99;
lkm1638.print(value++, DEC, 2);
  
// Print -- when value is greater than 2 digits 
lkm1638.print(value, DEC, 2); 
  
// Print 16-bit unsigned value with max 4 digits and 4 digits padding: 0009 
uint16_t value = 9;
lkm1638.print(value, DEC, 4, 4);
  
// Print 32-bit unsigned value
lkm1638.print(12345678UL);
  
// Print binary uint8_t 0xA9 = 10101001
uint8_t value = 0xA9;
lkm1638.print(value, BIN, 8, 8);
```

### Control 8 display dots
```c++
// Turn one dot on in digit 7 (most left)
lkm1638.dotOn(7);
  
// Turn one dot off in digit 0 (most right)
lkm1638.dotOff(0);
  
// Set multiple dots on and off
lkm1638.setDots(0x85);
```

### Display special characters
```c++
// Turn digit off
lkm1638.setSegmentsDigit(5, SEGMENTS_OFF);
  
// Display minus character
lkm1638.setSegmentsDigit(4, SEGMENTS_MINUS);
  
// Display degree selsius symbol + C
lkm1638.setSegmentsDigit(1, SEGMENTS_DEGREE);
lkm1638.setSegmentsDigit(0, SEGMENTS_C);
```

### Write a custom character to the display
```c++
// Display single LED in a digit
lkm1638.setSegmentsDigit(0, 0b0001000);
```


## Library dependencies

* [Erriez TM1638](https://github.com/Erriez/ErriezTM1638)


## Library installation

Please refer to the [Wiki](https://github.com/Erriez/ErriezArduinoLibrariesAndSketches/wiki) page.


## Other Arduino Libraries and Sketches from Erriez

* [Erriez Libraries and Sketches](https://github.com/Erriez/ErriezArduinoLibrariesAndSketches)
