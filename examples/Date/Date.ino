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

/* JY-LKM1638 board v1.1 date example
 *
 * Required libraries:
 *   https://github.com/Erriez/ArduinoLibraryTM1638
 *   https://github.com/Erriez/ArduinoLibraryLKM1638
 */

#include <Arduino.h>
#include <LKM1638Board.h>

// Connect display pins to the Arduino DIGITAL pins
#define DIO_PIN   2
#define SCL_PIN   3
#define STB_PIN   4

LKM1638Board lkm1638(DIO_PIN, SCL_PIN, STB_PIN);

// Function prototype
void displayDate(uint8_t dayMonth, uint8_t month, uint8_t year);

void setup()
{
  Serial.begin(115200);
  Serial.println(F("JY-LKM1638 time example"));
  Serial.println(F("Display date..."));

  // Set brightness
  lkm1638.setBrightness(2);

  // Display a fixed date
  displayDate(31, 1, 18);
}

void loop()
{

}

void displayDate(uint8_t dayMonth, uint8_t month, uint8_t year)
{
  lkm1638.setPrintPos(6);
  lkm1638.print(dayMonth, DEC, 2, 2);
  lkm1638.setPrintPos(3);
  lkm1638.print(month, DEC, 2, 2);
  lkm1638.setPrintPos(0);
  lkm1638.print(year, DEC, 2, 2);

  lkm1638.setSegmentsDigit(2, SEGMENTS_MINUS);
  lkm1638.setSegmentsDigit(5, SEGMENTS_MINUS);
}
