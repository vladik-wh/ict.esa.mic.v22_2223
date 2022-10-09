#include <avr/interrupt.h>
#include <avr/io.h>

#define HZ880 71

void delay(uint16_t ms) {
    TCCR0A |= (1 << WGM01);
    OCR0A = 250;
    TCCR0B |= (1 << CS00);

    uint8_t counter{0};
    while (ms > 0)
    {
        if (counter == 63) {
            counter = 0;
            ms--;
        }
        counter++;
        while (!(TIFR0 & (1 << OCF0A)));
        TIFR0 |= 1 << OCF0A;
    }
}

int main() {
    uint16_t counter{0};
    DDRB |= (1 << DDB5);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        delay(1000);
        counter++;

//        if (counter >= HZ880) {
//            counter = 0;
            PINB = (1 << PINB5);
//        }
    }
#pragma clang diagnostic pop
}
