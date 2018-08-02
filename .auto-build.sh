#!/bin/sh

################################################################################
# Title         : .auto-build.sh
# Date created  : 2 August 2018
__AUTHOR__="Erriez"
#
# This script will start PlatformIO build.
#
################################################################################

################################################################################
##### Setup this script and get the current gh-pages branch.               #####
echo 'Setting up the script...'

# Exit with nonzero exit code if anything fails
set -e

# Build sources
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Brightness/Brightness.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Buttons/Buttons.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Counter/Counter.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Date/Date.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Demo/Demo.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Temperature/Temperature.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/TestLEDs/TestLEDs.ino
platformio ci --lib="." --project-conf=platformio/platformio.ini examples/Time/Time.ino
