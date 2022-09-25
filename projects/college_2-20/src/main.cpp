#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY 200
#define LED_COUNT 5
#define BLINKER_PINS (1 << PIND2) | (1 << PIND3) | (1 << PIND4) | (1 << PIND5) | (1 << PIND6)

void init_pins() {
    DDRD |= BLINKER_PINS;
    DDRC |= (1 << DDC4) | (1 << DDC5);
    PINC |= 1 << PINC4;
}

void blink() {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        PIND = (1 << PIND2 << i);
        _delay_ms(BLINK_DELAY);
    }
    PIND = BLINKER_PINS;
}

int main() {
    init_pins();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        blink();
        PINC = (1 << PINC5) | (1 << PINC4);
    }
#pragma clang diagnostic pop
}
