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

/* JY-LKM1638 board v1.1 brightness example
 *
 * Required libraries:
 *   https://github.com/Erriez/TM1638
 *   https://github.com/Erriez/ErriezLKM1638
 */

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

// Initialize brightness variable with value 0
static uint8_t brightness = 0;


void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.println(F("JY-LKM1638 brightness example"));

    // Initialize TM1638
    lkm1638.begin();
    lkm1638.clear();

    // Display a decimal unsigned value
    lkm1638.print(12345678UL);

    // Turn dual color LED's on
    lkm1638.colorLEDsOn(0xF0, LedRed);
    lkm1638.colorLEDsOn(0x0F, LedGreen);
}

void loop()
{
    // Set brightness
    lkm1638.setBrightness(brightness);

    Serial.print(F("Brightness: "));
    Serial.println(brightness);

    // Increase brightness
    if (brightness >= 7) {
        brightness = 0;
    } else {
        brightness++;
    }

    // Wait a second
    delay(1000);
}
