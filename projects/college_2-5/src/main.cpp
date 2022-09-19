#include <avr/io.h>

int main() {
    DDRB |= -1;

    PORTB = 0xAA;

    for (uint8_t i = 0; i < 200; i++) {
        PORTB ^= -1;
    }
}
