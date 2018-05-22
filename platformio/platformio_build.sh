#!/bin/sh

# Install Python platformio 
# pip install -U platformio

# Update library
# git fetch
# git pull

# Build example(s)
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Brightness/Brightness.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Buttons/Buttons.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Counter/Counter.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Date/Date.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Demo/Demo.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Temperature/Temperature.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/TestLEDs/TestLEDs.ino
platformio ci --lib=".." --project-conf=platformio.ini ../examples/Time/Time.ino
