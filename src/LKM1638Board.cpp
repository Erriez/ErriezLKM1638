/*
 * MIT License
 *
 * Copyright (c) 2018 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* JY-LKM1638 board v1.1 library for Arduino
 *
 *   https://github.com/Erriez/ArduinoLibraryTM1638
 *   https://github.com/Erriez/ArduinoLibraryLKM1638
 */

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include "LKM1638Board.h"

/* 7-segment bits:
  - 0 -
   |   |
   5   1
   |   |
  - 6 -
   |   |
   4   2
   |   |
  - 3 -  .7
*/

// 7-segment display digit defines
static const PROGMEM uint8_t SEGMENT_DATA[] = {
  0b00111111, /* 0 */
  0b00000110, /* 1 */
  0b01011011, /* 2 */
  0b01001111, /* 3 */
  0b01100110, /* 4 */
  0b01101101, /* 5 */
  0b01111101, /* 6 */
  0b00000111, /* 7 */
  0b01111111, /* 8 */
  0b01101111, /* 9 */
  0b01110111, /* A */
  0b01111100, /* B */
  0b00111001, /* C */
  0b01011110, /* D */
  0b01111001, /* E */
  0b01110001, /* F */
};

LKM1638Board::LKM1638Board(uint8_t dioPin, uint8_t sclPin, uint8_t stbPin) :
  TM1638(dioPin, sclPin, stbPin), _pos(0), _dots(0)
{
  memset(_leds, 0, NUM_DIGITS);
}

//------------------------------------------------------------------------------
// Buttons
//------------------------------------------------------------------------------
uint8_t LKM1638Board::getButtons()
{
  uint32_t keys32;
  uint8_t keys = 0;

  /* Read 4 Byte key-scan registers */
  keys32 = getKeyScan();

  /* 8 buttons on the LKM1638 board are connected to K3 only
   * Sort the keys in BYTE1..BYTE4 bits 0 and 4 to a keys byte
   *
   * BIT: | 7 | 6 | 5 | 4  | 3 | 2 | 1 | 0  |
   * -----+---+---+---+----+---+---+---+----+------
   *    |   |   |   | S5 |   |   |   | S0 | BYTE1
   *    |   |   |   | S6 |   |   |   | S1 | BYTE2
   *    |   |   |   | S7 |   |   |   | S2 | BYTE3
   *    |   |   |   | S8 |   |   |   | S3 | BYTE4
   */

  for (uint8_t i = 0; i < 4; i++) {
    keys |= (keys32 >> (i * 7)) & 0xFF;
  }

  return swapBits(keys);
}

//------------------------------------------------------------------------------
// Turn all LED's off
//------------------------------------------------------------------------------
void LKM1638Board::clear()
{
  memset(_leds, 0, NUM_DIGITS);
  _dots = 0;

  TM1638::clear();
}

//------------------------------------------------------------------------------
// Dual color LED's
//------------------------------------------------------------------------------
void LKM1638Board::setColorLED(uint8_t led, LedColor color)
{
  /* led=7: Left, led=0: Right
   *
   * 8 two color LED's are connected to SEG8 and SEG9 pins at addresses:
   *
   *   LED1 	Address 0x01
   *   LED2	  Address 0x03
   *   ...  ...
   *   LED8 	Address 0x0F
   *
   *
   *   BIT0  | BIT1  |
   *  (SEG9) |(SEG10)|  COLOR
   * --------+-------+-------------
   *   0   |   0   |  OFF
   *   0   |   1   |  GREEN
   *   1   |   0   |  RED
   *   1   |   1   |  NOT ALLOWED
   */
  if (led < NUM_DIGITS) {
    writeDisplayRegister(0x01 + (swapLeds(led) << 1), (color & 0x03));
  }
}

void LKM1638Board::colorLEDsOn(uint8_t leds, LedColor color)
{
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if (leds & (1 << i)) {
      setColorLED(i, color);
    }
  }
}

void LKM1638Board::colorLEDsOff(uint8_t leds)
{
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if (leds & (1 << i)) {
      setColorLED(i, LedOff);
    }
  }
}

//------------------------------------------------------------------------------
// 7-segment display IO
//------------------------------------------------------------------------------
void LKM1638Board::writeDigit(uint8_t pos)
{
  if (pos < NUM_DIGITS) {
    uint8_t leds = _leds[pos];
    if (_dots & (1 << pos)) {
      leds |= 0x80;
    }
    writeDisplayRegister((swapPos(pos) << 1), leds);
  }
}

void LKM1638Board::setSegmentsDigit(uint8_t pos, uint8_t segments)
{
  if (pos < NUM_DIGITS) {
    _leds[pos] = segments;
    writeDigit(pos);
  }
}

void LKM1638Board::setDigit(uint8_t pos, uint8_t digit)
{
  if (pos < NUM_DIGITS) {
    setSegmentsDigit(pos, pgm_read_byte(&SEGMENT_DATA[digit]));
  }
}

void LKM1638Board::refresh()
{
  for (uint8_t pos = 0; pos < NUM_DIGITS; pos++) {
    writeDigit(pos);
  }
}

//------------------------------------------------------------------------------
// 7-segment display dots
//------------------------------------------------------------------------------
void LKM1638Board::dotOn(uint8_t pos)
{
  if (pos < NUM_DIGITS) {
    _dots |= (1 << pos);
    writeDigit(pos);
  }
}

void LKM1638Board::dotOff(uint8_t pos)
{
  if (pos < NUM_DIGITS) {
    _dots &= ~(1 << pos);
    writeDigit(pos);
  }
}

void LKM1638Board::setDots(uint8_t dots)
{
  _dots = dots;
  refresh();
}

//------------------------------------------------------------------------------
// 7-segment display position
//------------------------------------------------------------------------------
void LKM1638Board::setPrintPos(uint8_t pos)
{
  if (pos < NUM_DIGITS) {
    _pos = pos;
  }
}

uint8_t LKM1638Board::getPrintPos()
{
  return _pos;
}

//------------------------------------------------------------------------------
// Display uint8_t
//------------------------------------------------------------------------------
void LKM1638Board::print(uint8_t value)
{
  writeUnsignedValue(value, DEC, 3, 1);
}

void LKM1638Board::print(uint8_t value, uint8_t radius)
{
  writeUnsignedValue(value, radius, 3, 1);
}

void LKM1638Board::print(uint8_t value, uint8_t radius, uint8_t maxDigits)
{
  writeUnsignedValue(value, radius, maxDigits, 1);
}

void LKM1638Board::print(uint8_t value, uint8_t radius, uint8_t maxDigits,
       uint8_t pad)
{
  writeUnsignedValue(value, radius, maxDigits, pad);
}

//------------------------------------------------------------------------------
// Display uint16_t
//------------------------------------------------------------------------------
void LKM1638Board::print(uint16_t value)
{
  writeUnsignedValue(value, DEC, 5, 1);
}

void LKM1638Board::print(uint16_t value, uint8_t radius)
{
  writeUnsignedValue(value, radius, 5, 1);
}

void LKM1638Board::print(uint16_t value, uint8_t radius, uint8_t maxDigits)
{
  writeUnsignedValue(value, radius, maxDigits, 1);
}

void LKM1638Board::print(uint16_t value, uint8_t radius, uint8_t maxDigits,
       uint8_t pad)
{
  writeUnsignedValue(value, radius, maxDigits, pad);
}

//------------------------------------------------------------------------------
// Display uint32_t
//------------------------------------------------------------------------------
void LKM1638Board::print(uint32_t value)
{
  writeUnsignedValue(value, DEC, 8, 1);
}

void LKM1638Board::print(uint32_t value, uint8_t radius)
{
  writeUnsignedValue(value, radius, 8, 1);
}

void LKM1638Board::print(uint32_t value, uint8_t radius, uint8_t maxDigits)
{
  writeUnsignedValue(value, radius, maxDigits, 1);
}

void LKM1638Board::print(uint32_t value, uint8_t radius, uint8_t maxDigits,
       uint8_t pad)
{
  writeUnsignedValue(value, radius, maxDigits, pad);
}

//------------------------------------------------------------------------------
// Display int8_t
//------------------------------------------------------------------------------
void LKM1638Board::print(int8_t value)
{
  writeSignedValue(value, DEC, 4);
}

void LKM1638Board::print(int8_t value, uint8_t radius)
{
  writeSignedValue(value, radius, 4);
}

void LKM1638Board::print(int8_t value, uint8_t radius, uint8_t maxDigits)
{
  writeSignedValue(value, radius, maxDigits);
}

//------------------------------------------------------------------------------
// Display int16_t
//------------------------------------------------------------------------------
void LKM1638Board::print(int16_t value)
{
  writeSignedValue(value, DEC, 6);
}

void LKM1638Board::print(int16_t value, uint8_t radius)
{
  writeSignedValue(value, radius, 6);
}

void LKM1638Board::print(int16_t value, uint8_t radius, uint8_t maxDigits)
{
  writeSignedValue(value, radius, maxDigits);
}

//------------------------------------------------------------------------------
// Display int32_t
//------------------------------------------------------------------------------
void LKM1638Board::print(int32_t value)
{
  writeSignedValue(value, DEC, 8);
}

void LKM1638Board::print(int32_t value, uint8_t radius)
{
  writeSignedValue(value, radius, 8);
}

void LKM1638Board::print(int32_t value, uint8_t radius, uint8_t maxDigits)
{
  writeSignedValue(value, radius, maxDigits);
}

//------------------------------------------------------------------------------
// Position, pad and write value to 7-segment display
//------------------------------------------------------------------------------
void LKM1638Board::writeUnsignedValue(uint32_t value, uint8_t radius,
              uint8_t maxDigits, uint8_t pad)
{
  uint8_t numDigits;
  uint8_t pos;

  // Get number of digits of the value
  numDigits = getNumDigits(value, radius);

  // Check if the value fits in the reserved area on the display
  if ((numDigits > maxDigits) || ((_pos + numDigits) > NUM_DIGITS)) {
    displayOverflow(maxDigits);
    return;
  }

  // Display every digit by dividing the value with the radius:
  // 10 for DEC, 16 for HEX and 2 for BIN
  pos = _pos;
  for (uint8_t i = 0; i < maxDigits; i++) {
    if ((value == 0) && (i >= pad)) {
      setSegmentsDigit(pos, 0x00);
    } else {
      setDigit(pos, (value % radius));
    }
    pos++;
    value /= radius;
  }
}

void LKM1638Board::writeSignedValue(int32_t value, uint8_t radius,
                                    uint8_t maxDigits)
{
  uint8_t numDigits;
  uint8_t pos;
  bool negative = false;

  if (value < 0) {
    negative = true;
    value *= -1;
  }

  // Get number of digits of the value including one minus char
  numDigits = getNumDigits((uint32_t)value, radius) + 1;

  // Check if the value fits in the reserved area on the display
  if ((numDigits > maxDigits) || ((_pos + numDigits) > NUM_DIGITS)) {
    displayOverflow(maxDigits);
    return;
  }

  // Display every digit by dividing the value with the radius:
  // 10 for DEC, 16 for HEX and 2 for BIN
  pos = _pos;
  for (uint8_t i = 0; i < maxDigits; i++) {
    if ((value == 0) && (i >= 1)) {
      setSegmentsDigit(pos, 0x00);
    } else {
      setDigit(pos, (value % radius));
    }
    pos++;
    value /= radius;
  }

  // Display or hide minus char
  if (negative) {
    pos = _pos + numDigits - 1;
    setSegmentsDigit(pos, SEGMENTS_MINUS);
  } else {
    setSegmentsDigit(pos, SEGMENTS_OFF);
  }
}

uint8_t LKM1638Board::getNumDigits(uint32_t value, uint8_t radius)
{
  uint8_t numDigits = 1;

  // Calculate the number of digits of a value by dividing with radius
  for (uint8_t i = 0; i < NUM_DIGITS; i++) {
    value /= radius;
    if (value == 0) {
      break;
    } else {
      numDigits++;
    }
  }

  return numDigits;
}

void LKM1638Board::displayOverflow(uint8_t numDigits)
{
  // Display minus char on value area
  for (uint8_t i = 0; i < numDigits; i++) {
    setSegmentsDigit(_pos + i, SEGMENTS_MINUS);
  }
}

uint8_t LKM1638Board::swapPos(uint8_t pos)
{
  // Swap position LSB - MSB
  return NUM_DIGITS - 1 - pos;
}

uint8_t LKM1638Board::swapLeds(uint8_t led)
{
  // Swap LED's LSB - MSB
  return NUM_LEDS - 1 - led;
}

uint8_t LKM1638Board::swapBits(uint8_t data)
{
  uint8_t result = 0;

  // Swap all bits in a Byte (most significant and least significant bit)
  for (uint8_t i = 0; i < 8; i++) {
    result <<= 1;
    result |= data & 1;
    data >>= 1;
  }

  return result;
}
