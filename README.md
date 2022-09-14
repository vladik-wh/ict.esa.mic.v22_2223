## Installation
1. Download latest release of ArduinoCore from `https://github.com/arduino/ArduinoCore-avr/releases`
2. Extract `cores` and `variants` to `arduino-core-lib/src`

From the `arduino-core-lib` open a terminal and run the following commands.
1. `cmake -B build`
2. `cmake --build build`
3. `cmake --install build`

The `arduino` folder should then have 2 new folders named `include` and `lib` with content.
