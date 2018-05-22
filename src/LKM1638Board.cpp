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

/*!
 * \file LKM1638Board.h
 * \brief JY-LKM1638 board v1.1 library for Arduino
 * \details
 *   https://github.com/Erriez/ErriezTM1638
 *   https://github.com/Erriez/ErriezLKM1638
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

/*!
 * \brief LKM1638 constructor
 * \param clkPin Clock pin
 * \param dioPin Data pin (bi-directional)
 * \param stbPin Strobe pin (low is enable)
 */
LKM1638Board::LKM1638Board(uint8_t clkPin, uint8_t dioPin, uint8_t stbPin) :
        TM1638(clkPin, dioPin, stbPin), _pos(0), _dots(0)
{
    memset(_leds, 0, NUM_DIGITS);
}

//------------------------------------------------------------------------------
// Buttons
//------------------------------------------------------------------------------
/*!
 * \brief Read buttons
 * \return Value of 8 buttons
 */
uint8_t LKM1638Board::getButtons()
{
    uint32_t keys32;
    uint8_t keys = 0;

    /* Read 4 Byte key-scan registers */
    keys32 = getKeys();

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
/*!
 * \brief Turn all LED's off
 */
void LKM1638Board::clear()
{
    memset(_leds, 0, NUM_DIGITS);
    _dots = 0;

    TM1638::clear();
}

//------------------------------------------------------------------------------
// Dual color LED's
//------------------------------------------------------------------------------
/*!
 * \brief Set dual color LED
 * \param led LED number (0 = most right, 7 = most left)
 * \param color
 *      0: Off
 *      1: Green
 *      2: Red
 */
void LKM1638Board::setColorLED(uint8_t led, LedColor color)
{
    /* led=7: Left, led=0: Right
     *
     * 8 two color LED's are connected to SEG8 and SEG9 pins at addresses:
     *
     *   LED1 	  Address 0x01
     *   LED2	  Address 0x03
     *   ...  ...
     *   LED8 	  Address 0x0F
     *
     *
     *   BIT0  | BIT1  |
     *  (SEG9) |(SEG10)|  COLOR
     * --------+-------+-------------
     *     0   |   0   |  OFF
     *     0   |   1   |  GREEN
     *     1   |   0   |  RED
     *     1   |   1   |  NOT ALLOWED
     */
    if (led < NUM_DIGITS) {
        writeData((uint8_t)(0x01 + (swapLeds(led) << 1)), (color & 0x03));
    }
}

/*!
 * \brief Turn multiple color LED's on
 * \param leds Byte with 8 LED's
 * \param color
 *      0: Off
 *      1: Green
 *      2: Red
 */
void LKM1638Board::colorLEDsOn(uint8_t leds, LedColor color)
{
    for (uint8_t i = 0; i < NUM_COLOR_LEDS; i++) {
        if (leds & (1 << i)) {
            setColorLED(i, color);
        }
    }
}

/*!
 * \brief Turn multiple color LED's off
 * \param leds Byte with 8 LED's
 */
void LKM1638Board::colorLEDsOff(uint8_t leds)
{
    for (uint8_t i = 0; i < NUM_COLOR_LEDS; i++) {
        if (leds & (1 << i)) {
            setColorLED(i, LedOff);
        }
    }
}

//------------------------------------------------------------------------------
// 7-segment display IO
//------------------------------------------------------------------------------
/*!
 * \brief Write digit position
 * \param pos Digit number 0 is most right digit, 7 is most left digit
 */
void LKM1638Board::writeDigit(uint8_t pos)
{
    if (pos < NUM_DIGITS) {
        uint8_t leds = _leds[pos];
        if (_dots & (1 << pos)) {
            leds |= 0x80;
        }
        writeData((swapPos(pos) << 1), leds);
    }
}

/*!
 * \brief Write LED segments of a digit
 * \param pos Position 0..7
 * \param segments Segment LED's
 */
void LKM1638Board::setSegmentsDigit(uint8_t pos, uint8_t segments)
{
    if (pos < NUM_DIGITS) {
        _leds[pos] = segments;
        writeDigit(pos);
    }
}

/*!
 * \brief Write digit
 * \param pos Position 0..7
 * \param digit Value 0..9, A..F
 */
void LKM1638Board::setDigit(uint8_t pos, uint8_t digit)
{
    if (pos < NUM_DIGITS) {
        if (digit < sizeof(SEGMENT_DATA)) {
            setSegmentsDigit(pos, pgm_read_byte(&SEGMENT_DATA[digit]));
        } else {
            setSegmentsDigit(pos, 0x00);
        }
    }
}

/*!
 * \brief Refresh display
 */
void LKM1638Board::refresh()
{
    for (uint8_t pos = 0; pos < NUM_DIGITS; pos++) {
        writeDigit(pos);
    }
}

//------------------------------------------------------------------------------
// 7-segment display dots
//------------------------------------------------------------------------------
/*!
 * \brief Turn dot LED on
 * \param pos Position 0..7
 */
void LKM1638Board::dotOn(uint8_t pos)
{
    if (pos < NUM_DIGITS) {
        _dots |= (1 << pos);
        writeDigit(pos);
    }
}

/*!
 * \brief Turn dot LED off
 * \param pos Position 0..7
 */
void LKM1638Board::dotOff(uint8_t pos)
{
    if (pos < NUM_DIGITS) {
        _dots &= ~(1 << pos);
        writeDigit(pos);
    }
}

/*!
 * \brief Turn multiple dots on or off
 * \param dots Byte with dots
 */
void LKM1638Board::setDots(uint8_t dots)
{
    _dots = dots;
    refresh();
}

//------------------------------------------------------------------------------
// 7-segment display position
//------------------------------------------------------------------------------
/*!
 * \brief Set print position
 * \param pos Position 0..7
 */
void LKM1638Board::setPrintPos(uint8_t pos)
{
    if (pos < NUM_DIGITS) {
        _pos = pos;
    }
}

/*!
 * \brief Get print position
 * \return Position 0..7
 */
uint8_t LKM1638Board::getPrintPos()
{
    return _pos;
}

//------------------------------------------------------------------------------
// Display uint8_t
//------------------------------------------------------------------------------
/*!
 * \brief Print uint8_t value
 * \param value Display value 0..255
 */
void LKM1638Board::print(uint8_t value)
{
    writeUnsignedValue(value, DEC, 3, 1);
}

/*!
 * \brief Print uint8_t with radius
 * \param value Display value 0..255
 * \param radius Radius 2 for binary, 10 for decimal, 16 for HEX
 */
void LKM1638Board::print(uint8_t value, uint8_t radius)
{
    writeUnsignedValue(value, radius, 3, 1);
}

/*!
 * \brief Print uint8_t with radius and maximum number of digits
 * \param value Display value 0..255
 * \param radius Radius 2 for binary, 10 for decimal, 16 for HEX
 * \param maxDigits Maximum number of digits
 */
void LKM1638Board::print(uint8_t value, uint8_t radius, uint8_t maxDigits)
{
    writeUnsignedValue(value, radius, maxDigits, 1);
}

/*!
 * \brief Print uint8_t with radius, maximum number of digits and padding digits
 * \param value Display value 0..255
 * \param radius Radius 2 for binary, 10 for decimal, 16 for HEX
 * \param maxDigits Maximum number of digits
 * \param pad Number of digits starting with a 0
 */
void LKM1638Board::print(uint8_t value, uint8_t radius, uint8_t maxDigits, uint8_t pad)
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

void LKM1638Board::print(uint16_t value, uint8_t radius, uint8_t maxDigits, uint8_t pad)
{
    writeUnsignedValue(value, radius, maxDigits, pad);
}

//------------------------------------------------------------------------------
// Display unsigned long
//------------------------------------------------------------------------------
void LKM1638Board::print(unsigned long value)
{
    writeUnsignedValue((uint32_t)value, DEC, 8, 1);
}

void LKM1638Board::print(unsigned long value, uint8_t radius)
{
    writeUnsignedValue((uint32_t)value, radius, 8, 1);
}

void LKM1638Board::print(unsigned long value, uint8_t radius, uint8_t maxDigits)
{
    writeUnsignedValue((uint32_t)value, radius, maxDigits, 1);
}

void LKM1638Board::print(unsigned long value, uint8_t radius, uint8_t maxDigits, uint8_t pad)
{
    writeUnsignedValue((uint32_t)value, radius, maxDigits, pad);
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
// Display long
//------------------------------------------------------------------------------
void LKM1638Board::print(long value)
{
    writeSignedValue((int32_t)value, DEC, 8);
}

void LKM1638Board::print(long value, uint8_t radius)
{
    writeSignedValue((int32_t)value, radius, 8);
}

void LKM1638Board::print(long value, uint8_t radius, uint8_t maxDigits)
{
    writeSignedValue((int32_t)value, radius, maxDigits);
}

//------------------------------------------------------------------------------
// Position, pad and write value to 7-segment display
//------------------------------------------------------------------------------
/*!
 * \brief Write unsigned value to display
 * \param value Unsigned value 0..2^32
 * \param radius Radius 2 for binary, 10 for decimal, 16 for HEX
 * \param maxDigits Maximum number of digits
 * \param pad Number of digits starting with a 0
 */
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
            setDigit(pos, (uint8_t)(value % radius));
        }
        pos++;
        value /= radius;
    }
}

/*!
 * \brief Write signed value to display
 * \param value signed value -2^31..2^31
 * \param radius Radius 2 for binary, 10 for decimal, 16 for HEX
 * \param maxDigits Maximum number of digits
 */
void LKM1638Board::writeSignedValue(int32_t value, uint8_t radius, uint8_t maxDigits)
{
    uint8_t numDigits;
    uint8_t pos;
    bool negative = false;

    if (value < 0) {
        negative = true;
        value *= -1;
    }

    // Get number of digits of the value including one minus char
    numDigits = (uint8_t)(getNumDigits((uint32_t)value, radius) + 1);

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
            setDigit(pos, (uint8_t)(value % radius));
        }
        pos++;
        value /= radius;
    }

    // Display or hide minus char
    if (negative) {
        pos = (uint8_t)(_pos + numDigits - 1);
        setSegmentsDigit(pos, SEGMENTS_MINUS);
    } else {
        setSegmentsDigit(pos, SEGMENTS_OFF);
    }
}

/*!
 * \brief Get number of digits of a signed 32-bit value
 * \param value 32-bit signed value
 * \param radius Radius
 * \return Number of digits
 */
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

/*!
 * \brief Display overflow with - characters
 * \param numDigits Number of digits to display
 */
void LKM1638Board::displayOverflow(uint8_t numDigits)
{
    // Display minus char on value area
    for (uint8_t i = 0; i < numDigits; i++) {
        setSegmentsDigit(_pos + i, SEGMENTS_MINUS);
    }
}

/*!
 * \brief Swap digit position
 * \param pos Position
 * \return Swapped position
 */
uint8_t LKM1638Board::swapPos(uint8_t pos)
{
    // Swap position LSB - MSB
    return (uint8_t)(NUM_DIGITS - 1 - pos);
}

/*!
 * \brief Swap dual color LED's
 * \param led LED's
 * \return Swapped LED bits
 */
uint8_t LKM1638Board::swapLeds(uint8_t led)
{
    // Swap LED's LSB - MSB
    return (uint8_t)(NUM_COLOR_LEDS - 1 - led);
}

/*!
 * \brief Swap bits
 * \param data 9-bit unsigned value
 * \return Swapped bits
 */
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
