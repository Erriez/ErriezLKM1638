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

/* JY-LKM1638 board v1.1 test all LED's
 *
 * Required libraries:
 *   https://github.com/Erriez/TM1638
 *   https://github.com/Erriez/ErriezLKM1638
 */

#include <Arduino.h>
#include <ErriezLKM1638Board.h>

// Connect display pins to the Arduino DIGITAL pins
#if ARDUINO_ARCH_AVR
#define TM1638_CLK_PIN      2
#define TM1638_DIO_PIN      3
#define TM1638_STB0_PIN     4
#elif ARDUINO_ARCH_ESP8266
#define TM1638_CLK_PIN      D2
#define TM1638_DIO_PIN      D3
#define TM1638_STB0_PIN     D4
#elif ARDUINO_ARCH_ESP32
#define TM1638_CLK_PIN      0
#define TM1638_DIO_PIN      4
#define TM1638_STB0_PIN     5
#else
#error "May work, but not tested on this target"
#endif

// Create LKM1638Board object
LKM1638Board lkm1638(TM1638_CLK_PIN, TM1638_DIO_PIN, TM1638_STB0_PIN);


void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.println(F("JY-LKM1638 test LED example"));

    // Initialize TM1638
    lkm1638.begin();
    lkm1638.clear();
    lkm1638.setBrightness(2);
}

void loop()
{
    Serial.print(F("Testing LEDs..."));

    // Dual color LED's
    for (uint8_t count = 0; count < 3; count++) {
        for (uint8_t led = 0; led < 8; led++) {
            lkm1638.setColorLED(led, LedRed);
            delay(50);
        }

        for (uint8_t led = 0; led < 8; led++) {
            lkm1638.setColorLED(led, LedGreen);
            delay(50);
        }

        for (uint8_t led = 0; led < 8; led++) {
            lkm1638.setColorLED(led, LedOff);
            delay(50);
        }
        delay(400);
    }

    // 7-segments display dots from right to left and back
    for (uint8_t count = 0; count < 3; count++) {
        for (uint8_t digit = 0; digit < 8; digit++) {
            lkm1638.setDots((uint8_t)(1 << digit));
            delay(50);
        }
        for (int8_t digit = 7; digit >= 0; digit--) {
            lkm1638.setDots((uint8_t)(1 << digit));
            delay(50);
        }
        delay(400);
        lkm1638.setDots(0);
    }

    // 7-segments display all digits counting from 0..F
    for (uint8_t value = 0; value <= 0x10; value++) {
        for (uint8_t digit = 0; digit < 8; digit++) {
            lkm1638.setDigit(digit, value);
        }
        delay(400);
    }

    // 7-segments display shifting digits from right to left 0..F
    for (uint8_t value = 0; value <= 0x10; value++) {
        for (uint8_t digit = 0; digit < 8; digit++) {
            lkm1638.setDigit(digit, value);
            delay(80);
        }
    }

    // 7-segments display count individual digits
    for (uint8_t digit = 0; digit < 8; digit++) {
        for (uint8_t value = 0; value <= 0x10; value++) {
            lkm1638.setDigit(digit, value);
            delay(100);
        }
    }

    Serial.println(F("Done"));
}
