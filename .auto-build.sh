#/bin/bash

# Automatic build script
#
# To run locally, execute:
# sudo apt install doxygen graphviz texlive-latex-base texlive-latex-recommended texlive-pictures texlive-latex-extra

# Exit immediately if a command exits with a non-zero status.
set -e

echo "Starting auto-build script..."


function autobuild()
{
    # Set environment variables
    BOARDS_AVR="--board uno --board micro --board miniatmega328 --board nanoatmega328new --board pro16MHzatmega328 --board pro8MHzatmega328 --board megaatmega2560 --board leonardo"
    BOARDS_ESP="--board d1_mini --board nodemcuv2"

    echo "Installing library dependencies"
    # Install library dependency LowPower.h for example DHT22LowPower.ino
    platformio lib --global install https://github.com/Erriez/ErriezTM1638

    echo "Building examples..."
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Brightness/Brightness.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Buttons/Buttons.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Counter/Counter.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Date/Date.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Demo/Demo.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Temperature/Temperature.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/TestLEDs/TestLEDs.ino
    platformio ci --lib="." ${BOARDS_AVR} ${BOARDS_ESP} examples/Time/Time.ino
}

function generate_doxygen()
{
    echo "Generate Doxygen HTML..."

    DOXYGEN_PDF="ErriezLKM1638.pdf"

    # Cleanup
    rm -rf html latex

    # Generate Doxygen HTML and Latex
    doxygen Doxyfile

    # Allow filenames starting with an underscore    
    echo "" > html/.nojekyll

    # Generate PDF when script is not running on Travis-CI
    if [[ -z ${TRAVIS_BUILD_DIR} ]]; then
        # Generate Doxygen PDF
        make -C latex

        # Copy PDF to root directory
        cp latex/refman.pdf ./${DOXYGEN_PDF}
    fi
}

autobuild
generate_doxygen

