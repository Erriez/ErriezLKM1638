cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

#-------------------------------------------------------------------------------
# User configuration
#-------------------------------------------------------------------------------
# Project name
set(PROJECT_NAME    LKM1638)

# Set Arduino library directory
if(WIN32)
    set(LIBRARY_DIR     C:/Users/$ENV{USERNAME}/Documents/Arduino/libraries)
elseif(UNIX)
    set(LIBRARY_DIR     $ENV{HOME}/Arduino/libraries)
endif()

# Configure board, default Arduino UNO
set(BOARD               uno)
#set(BOARD              nano)

if(${BOARD} STREQUAL nano)
    set(nano.upload.maximum_size 30720)
    set(nano.upload.maximum_data_size 2048)
    set(nano.upload.speed 57600)
    set(nano.build.mcu atmega328p)
endif()

# Configure sketch
set(SKETCH              examples/TestLEDs/TestLEDs.ino)

# Configure serial port
if(WIN32)
    set(SERIAL_PORT     COM1)
elseif(UNIX)
    set(SERIAL_PORT     /dev/ttyUSB0)
endif()

#-------------------------------------------------------------------------------
# Project configuation
#-------------------------------------------------------------------------------
# Configure project
project(${PROJECT_NAME})

# print_board_list()

set(TM1638_LIBRARY_DIR      ${LIBRARY_DIR}/ArduinoLibraryTM1638/src)
set(LKM1638_LIBRARY_DIR     ${LIBRARY_DIR}/ArduinoLibraryLKM1638/src)

include_directories(
        ${TM1638_LIBRARY_DIR}
        ${LKM1638_LIBRARY_DIR}
)

generate_arduino_library(
        TM1638Lib
        SRCS
        ${TM1638_LIBRARY_DIR}/TM1638.cpp
        HDRS
        ${TM1638_LIBRARY_DIR}/TM1638.h
)

generate_arduino_library(
        LKM1638Lib
        SRCS
            ${LKM1638_LIBRARY_DIR}/LKM1638Board.cpp
        HDRS
            ${LKM1638_LIBRARY_DIR}/LKM1638Board.h
)

set(${CMAKE_PROJECT_NAME}_SKETCH    ${SKETCH})
set(${CMAKE_PROJECT_NAME}_LIBS      TM1638Lib LKM1638Lib)
set(${CMAKE_PROJECT_NAME}_BOARD     ${BOARD})
set(${CMAKE_PROJECT_NAME}_PORT      ${SERIAL_PORT})

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
