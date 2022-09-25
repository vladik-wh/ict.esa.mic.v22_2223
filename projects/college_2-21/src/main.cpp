#include <avr/io.h>
#include <util/delay.h>

#define LED_COUNT 6
///*/
#define DELAY_US 100000
/*/
#define DELAY_US 3125
//*/

static constexpr uint8_t LEDON[LED_COUNT][2]{
        {(1 << DDB0) | (1 << DDB1), 1 << PORTB0},
        {(1 << DDB0) | (1 << DDB1), 1 << PORTB1},
        {(1 << DDB1) | (1 << DDB2), 1 << PORTB1},
        {(1 << DDB1) | (1 << DDB2), 1 << PORTB2},
        {(1 << DDB0) | (1 << DDB2), 1 << PORTB0},
        {(1 << DDB0) | (1 << DDB2), 1 << PORTB2},
};

void init_pins() {
    DDRB |= (1 << DDB0) | (1 << DDB2);
    PORTB |= (1 << PORTB2);
}

/**
 * Turns one of the 6 Charlieplexed LEDs on.
 * @param led LED number ranging from 0 to 5.
 */
void turnon(uint8_t led) {
    DDRB = LEDON[led][0];
    PORTB = LEDON[led][1];
}

int main() {
    init_pins();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            turnon(i);
            _delay_us(DELAY_US);
        }
    }
#pragma clang diagnostic pop
}
