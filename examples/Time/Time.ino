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

/* JY-LKM1638 board v1.1 time example
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

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Time_t;

Time_t tm = { 9, 59, 00 };

void setup()
{
  Serial.begin(115200);
  Serial.println(F("JY-LKM1638 time example"));
  Serial.println(F("Display time..."));

  lkm1638.setBrightness(2);
}

void loop()
{
  incrementTime();
  displayTime();

  delay(1000);
}

void displayTime()
{
  lkm1638.setPrintPos(4);
  lkm1638.print(tm.hours, DEC, 2);
  lkm1638.setPrintPos(2);
  lkm1638.print(tm.minutes, DEC, 2, 2);
  lkm1638.setPrintPos(0);
  lkm1638.print(tm.seconds, DEC, 2, 2);
  lkm1638.dotOn(2);
  if (tm.seconds & 1) {
    lkm1638.dotOn(4);
  } else {
    lkm1638.dotOff(4);
  }
}

void incrementTime()
{
  if (tm.seconds >= 59) {
    tm.seconds = 0;
    if (tm.minutes >= 59) {
      tm.minutes = 0;
      if (tm.hours >= 23) {
        tm.hours = 0;
      } else {
        tm.hours++;
      }
    } else {
      tm.minutes++;
    }
  } else {
    tm.seconds++;
  }
}
