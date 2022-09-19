#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-bool-literals"

#include <avr/io.h>
#include <util/delay.h>

void display_counter(uint8_t count) {
    PORTD = (count << 2) | (PORTD & 0x03);
}

int main() {
    DDRD |= ~((1 << PORTD0) | (1 << PORTD1));

    uint8_t counter = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        display_counter(counter);
        counter++;

        if (counter > 64)
            counter = 0;

        _delay_ms(250);
    }
#pragma clang diagnostic pop

}

#pragma clang diagnostic pop
