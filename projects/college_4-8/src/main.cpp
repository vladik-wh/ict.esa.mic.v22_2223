#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#define EXTERNAL_PULL_UP_RESISTOR

void init_pins() {
    DDRB |= (1 << DDB5);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4);
    DDRB &= ~((1 << PORTB0) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4));

#ifndef EXTERNAL_PULL_UP_RESISTOR
    PORTB |= (1 << PORTB0) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4);
#endif
}

void led(bool high = true) {
    if (high)
        PORTB |= (1 << PORTB5);
    else
        PORTB &= ~(1 << PORTB5);
}

ISR(PCINT0_vect) {
    static uint8_t previous_on_state{0};
    static uint8_t previous_off_state{0};
    uint8_t on_state = PINB & ((1 << PINB0) | (1 << PINB2) | (1 << PINB3));
    uint8_t off_state = PINB & (1 << PINB4);

    if (on_state != previous_on_state) {
        previous_on_state = on_state;
        led(true);
        return;
    }

    if (off_state != previous_off_state) {
        previous_off_state = off_state;
        led(false);
        return;
    }
}

int main() {
    sei();
    init_pins();
    PCINT0_vect(); // call once to initialize the interrupt

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
    }
#pragma clang diagnostic pop
}
