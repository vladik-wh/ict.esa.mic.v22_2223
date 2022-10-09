#include <avr/interrupt.h>
#include <avr/io.h>

#define HZ880 71

int main() {
    uint16_t counter{0};

    TCCR0B |= (1 << CS00);
    DDRB |= (1 << DDB0);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        while (!(TIFR0 & (1 << TOV0)));
        TIFR0 |= 1 << TOV0;
        counter++;

        if (counter >= HZ880) {
            counter = 0;
            PINB = (1 << PINB0);
        }
    }
#pragma clang diagnostic pop
}
