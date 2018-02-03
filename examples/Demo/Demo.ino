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

/* JY-LKM1638 board v1.1 demo
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

uint8_t led = 0;
uint8_t dir = 1;
LedColor color = LedRed;
uint8_t ledDelay = 0;
uint8_t ledDelayInit = 30;

bool  countEnable = false;
bool  countUp = true;
uint32_t countValue = 0;
uint8_t  countDelay = 0;
uint8_t  countDelayInit = 10;

uint8_t brightness = 2;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("JY-LKM1638 demo"));
  Serial.println(F("Press a button:"));
  Serial.println(F("  SW1: Start/stop counting"));
  Serial.println(F("  SW2: Reset counter"));
  Serial.println(F("  SW3: Toggle increment/decrement"));
  Serial.println(F("  SW4: Increment brightness"));
  Serial.println(F("  SW5: Color LED speed up"));
  Serial.println(F("  SW6: Color LED speed down"));
  Serial.println(F("  SW7: Toggle direction color LED"));
  Serial.println(F("  SW8: Toggle color LED red/green"));

  // Set brightness
  lkm1638.setBrightness(brightness);
}

void loop()
{
  handleButtons();
  handleLEDs();
  handleDisplay();

  delay(10);
}

void handleButtons(void)
{
  static uint8_t keysMask = 0;
  uint8_t keys;

  // Get keys
  keys = lkm1638.getButtons();

  // Mask keys to execute increment or decrement state only when the switch is
  // down the first time
  if (keys == 0) {
    keysMask = 0;
  } else {
    if (keysMask == 0) {
      if (keys & 0x01) {
        // Change color
        if (color == LedGreen) {
          color = LedRed;
        } else {
          color = LedGreen;
        }
      } else if (keys & 0x02) {
        // Change direction
        dir ^= 1;
      } else if (keys & 0x04) {
        // Speed down
        if (ledDelayInit < 20) {
          ledDelayInit += 2;
        }
      } else if (keys & 0x08) {
        // Speed up
        if (ledDelayInit > 2) {
          ledDelayInit -= 2;
        }
      } else if (keys & 0x10) {
        // Change display brightness
        if (brightness == 7) {
          brightness = 0;
        } else {
          brightness++;
        }
        lkm1638.setBrightness(brightness);
      } else if (keys & 0x20) {
        // Toggle count up/down
        countUp ^= true;
      } else if (keys & 0x40) {
        // Clear count
        countValue = 0;
      } else if (keys & 0x80) {
        // Toggle count enable
        countEnable ^= true;
      }

      keysMask = keys;
    }
  }
}

void handleLEDs(void)
{
  if (ledDelay <= 4) {
    ledDelay = ledDelayInit;

    lkm1638.colorLEDsOff(0xff);
    lkm1638.colorLEDsOn((uint8_t)(1<<led), color);

    if (dir) {
      if (led >= 7) {
        led = 0;
      } else {
        led++;
      }
    } else {
      if (led == 0) {
        led = 7;
      } else {
        led--;
      }
    }
  } else {
    ledDelay--;
  }
}

void handleDisplay(void)
{
  if (countEnable) {
    if (countDelay <= 5) {
      countDelay = countDelayInit;

      if (countUp) {
        if (countValue == 99999999) {
          countValue = 0;
        } else {
          countValue++;
        }
      } else {
        if (countValue == 0) {
          countValue = (uint32_t)99999999UL;
        } else {
          countValue--;
        }
      }
    } else {
      countDelay--;
    }
  }

  lkm1638.setPrintPos(0);
  lkm1638.print(countValue);
}
