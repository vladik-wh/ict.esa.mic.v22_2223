#include <avr/interrupt.h>
#include <util/delay.h>
#include <Wire.h>

#define ADDRESS 0x38

#define P0 0
#define P3 3
#define P7 7

static uint8_t PCF = 0xFF ^ (1 << P3);
static bool int0_flag = false;

ISR(INT0_vect) {
    int0_flag = true;
}

void init_pins()
{
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    PORTD |= (1 << PORTD2);

    Wire.begin();
    sei();
}

int main() {
    init_pins();

    Wire.beginTransmission(ADDRESS);
    Wire.write(PCF);
    Wire.endTransmission();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        _delay_ms(1);

        if (int0_flag) {
            int0_flag = false;

            Wire.requestFrom(ADDRESS, 1);
            if (Wire.available()) {
                Wire.beginTransmission(ADDRESS);
                if (Wire.read() & (1 << P0))
                    Wire.write(PCF ^ (1 << P7));
                else
                    Wire.write(PCF);
                Wire.endTransmission();
            }
        }
    }
#pragma clang diagnostic pop
}
