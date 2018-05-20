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

/* JY-LKM1638 board v1.1 button example
 *
 * Required libraries:
 *   https://github.com/Erriez/ArduinoLibraryTM1638
 *   https://github.com/Erriez/ArduinoLibraryLKM1638
 */

#include <Arduino.h>
#include <LKM1638Board.h>

// Connect display pins to the Arduino DIGITAL pins
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MICRO) || \
    defined(ARDUINO_AVR_PRO) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_LEONARDO)
#define TM1638_CLK_PIN      2
#define TM1638_DIO_PIN      3
#define TM1638_STB0_PIN     4
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI) || defined(ARDUINO_ESP8266_NODEMCU)
#define TM1638_CLK_PIN      D2
#define TM1638_DIO_PIN      D3
#define TM1638_STB0_PIN     D4
#elif defined(ARDUINO_LOLIN32)
#define TM1638_CLK_PIN      0
#define TM1638_DIO_PIN      4
#define TM1638_STB0_PIN     5
#else
#error "May work, but not tested on this target"
#endif

// Create LKM1638Board object
LKM1638Board lkm1638(TM1638_CLK_PIN, TM1638_DIO_PIN, TM1638_STB0_PIN);

// Static variables
static uint8_t keys = 0;
static uint8_t lastKeys = 0;


void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.println(F("JY-LKM1638 button example"));
    Serial.println(F("Press one or more buttons at the same time..."));

    // Initialize TM1638
    lkm1638.begin();
    lkm1638.clear();
}

void loop()
{
    // Get buttons
    keys = lkm1638.getButtons();

    // Check if button changed
    if (lastKeys != keys) {
        lastKeys = keys;

        // Display dual color LED
        for (uint8_t i = 0; i < 8; i++) {
            if (keys & (1 << i)) {
                // Button down

                // Upper 4 LED's red, lower 4 LED's green
                if (i >= 4) {
                    lkm1638.setColorLED(i, LedRed);
                } else {
                    lkm1638.setColorLED(i, LedGreen);
                }
            } else {
                // Turn LED off when button is up
                lkm1638.setColorLED(i, LedOff);
            }
        }

        // Print button serial
        Serial.print(F("0x"));
        if (keys < 0x10) {
            Serial.print(0);
        }
        Serial.println(keys, HEX);
    }
}
