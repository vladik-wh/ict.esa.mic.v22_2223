#include <avr/interrupt.h>
#include <util/delay.h>
#include <Wire.h>

#define ADDRESS 0x38

#define P0 0
#define P1 1
#define P4 4

int main() {
    constexpr uint8_t PCF = 0xFF ^ (1 << P4);
    bool toggle{false};

    sei();
    Wire.begin();

    Wire.beginTransmission(ADDRESS);
    Wire.write(PCF ^ (1 << P4));
    Wire.endTransmission();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        Wire.requestFrom(ADDRESS, 1);
        if (Wire.available())
            toggle = Wire.read() & (1 << P4);

        Wire.beginTransmission(ADDRESS);
        Wire.write(PCF ^ (1 << (toggle ? P1 : P0)));
        Wire.endTransmission();
        _delay_ms(500);

        Wire.beginTransmission(ADDRESS);
        Wire.write(PCF);
        Wire.endTransmission();
        _delay_ms(500);
    }
#pragma clang diagnostic pop
}
