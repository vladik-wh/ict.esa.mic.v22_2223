# Just a recent number
cmake_minimum_required(VERSION 3.18)

# Determine the Arduino path, with all Arduino related libraries and generic cmake stuff
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
#    set(ARDUINO_PATH $ENV{HOME}/Arduino)
    set(AVR_PORT /dev/ttyACM0)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
#    set(ARDUINO_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}/Arduino")
    set(AVR_PORT COM6)
endif()

set(ARDUINO_PATH ../arduino)

# the toolchain to use
set(CMAKE_TOOLCHAIN_FILE ${ARDUINO_PATH}/etc/toolchain-avr.cmake)

# we need the assembler this time
enable_language(ASM)

# Project name
project("ArduinoCore")

# Where to install
set(CMAKE_INSTALL_PREFIX ${ARDUINO_PATH})

# Target: the Arduino core library with all core source files to compile
# To get a list of .c and cpp files in Linux:
#       cd <project directory>
#       find src/cores/arduino -name \*.[cSs]\*
add_library(${PROJECT_NAME} STATIC
        src/cores/arduino/wiring_pulse.S
        src/cores/arduino/abi.cpp
        src/cores/arduino/CDC.cpp
        src/cores/arduino/HardwareSerial0.cpp
        src/cores/arduino/HardwareSerial1.cpp
        src/cores/arduino/HardwareSerial2.cpp
        src/cores/arduino/HardwareSerial3.cpp
        src/cores/arduino/HardwareSerial.cpp
        src/cores/arduino/hooks.c
        src/cores/arduino/IPAddress.cpp
        src/cores/arduino/main.cpp
        src/cores/arduino/new.cpp
        src/cores/arduino/PluggableUSB.cpp
        src/cores/arduino/Print.cpp
        src/cores/arduino/Stream.cpp
        src/cores/arduino/Tone.cpp
        src/cores/arduino/USBCore.cpp
        src/cores/arduino/WInterrupts.c
        src/cores/arduino/wiring_analog.c
        src/cores/arduino/wiring.c
        src/cores/arduino/wiring_digital.c
        src/cores/arduino/wiring_pulse.c
        src/cores/arduino/wiring_shift.c
        src/cores/arduino/WMath.cpp
        src/cores/arduino/WString.cpp
        )

# Here the compiler finds include files (-I flags)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cores/arduino
        ${CMAKE_CURRENT_SOURCE_DIR}/src/variants/standard
        )

# All .h files of Arduino core
# To get a list of .h files (in Linux):
#       cd <project directory>
#       find src/cores/arduino -name \*.h
# Also include the pins_arduino.h file
#       src/variants/standard/pins_arduino.h
set(INCLUDES
        src/variants/standard/pins_arduino.h
        src/cores/arduino/Arduino.h
        src/cores/arduino/binary.h
        src/cores/arduino/Client.h
        src/cores/arduino/HardwareSerial.h
        src/cores/arduino/HardwareSerial_private.h
        src/cores/arduino/IPAddress.h
        src/cores/arduino/new.h
        src/cores/arduino/PluggableUSB.h
        src/cores/arduino/Printable.h
        src/cores/arduino/Print.h
        src/cores/arduino/Server.h
        src/cores/arduino/Stream.h
        src/cores/arduino/Udp.h
        src/cores/arduino/USBAPI.h
        src/cores/arduino/USBCore.h
        src/cores/arduino/USBDesc.h
        src/cores/arduino/WCharacter.h
        src/cores/arduino/wiring_private.h
        src/cores/arduino/WString.h
        )

# Install the library here (INSTALL_PREFIX_PATH is prepended)
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/arduino)