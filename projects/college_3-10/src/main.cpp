#include <avr/interrupt.h>
#include <HardwareSerial.h>
#include <util/delay.h>
#include <Wire.h>

#define ADDRESS 0x38

#define P0 0

int main() {
    sei();
    Wire.begin();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true)
    {
        Wire.beginTransmission(ADDRESS);
        Wire.write(0xFF ^ (1 << P0));
        Wire.endTransmission();
        _delay_ms(500);

        Wire.beginTransmission(ADDRESS);
        Wire.write(0xFF);
        Wire.endTransmission();
        _delay_ms(500);
    }
#pragma clang diagnostic pop
}
