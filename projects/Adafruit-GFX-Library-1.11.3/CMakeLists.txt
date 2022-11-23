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

set(ARDUINO_PATH ../../arduino)

# the toolchain to use
set(CMAKE_TOOLCHAIN_FILE ${ARDUINO_PATH}/etc/toolchain-avr.cmake)

# include standard extra targets, like 'upload'
#set(CMAKE_PROJECT_INCLUDE ${ARDUINO_PATH}/etc/extratargets-avr.cmake)

# Project name (also the name of the target)
project(Adafruit_GFX)

# Create one target. To generate a list of source files: `find src -name \*.c\*`
add_library(${PROJECT_NAME} STATIC
        src/Adafruit_GrayOLED.cpp
        src/Adafruit_SPITFT.cpp
        src/fontconvert/fontconvert.c
        src/Adafruit_GFX.cpp
        glcdfont.c
        )

# Add arduino core library
target_include_directories(${PROJECT_NAME} PRIVATE
        "${ARDUINO_PATH}/include/arduino"
        )
target_link_directories(${PROJECT_NAME} PRIVATE
        "${ARDUINO_PATH}/lib"
        )
target_link_libraries(${PROJECT_NAME}
#        Adafruit_BusIO
#        ArduinoCore
        )

# Rename the output to .elf as we will create multiple files
#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
#install(FILES ${INCLUDES} DESTINATION include/arduino)
