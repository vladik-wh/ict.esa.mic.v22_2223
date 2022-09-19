#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-bool-literals"

#include <avr/io.h>

/*
 * Program that turns a LED HIGH when the switch is LOW.
 */
int main() {
    // Set the LED pin on Port C5 (A5) on output mode
    DDRC |= (1 << PORTC5);

    // Set the switch pin on Port B1 (D9) pull-up mode
    PORTB |= (1 << PORTB1);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        // Check if the switch is HIGH
        if (PINB & (1 << PINB1)) {
            // Set the LED LOW
            PORTC &= ~(1 << PORTC5);
        } else {
            // Set the LED HIGH
            PORTC |= (1 << PORTC5);
        }
    }
#pragma clang diagnostic pop

}

#pragma clang diagnostic pop
