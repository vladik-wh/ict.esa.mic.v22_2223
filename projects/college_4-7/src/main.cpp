#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <wiring_private.h>

//#define EXTERNAL_DEBOUNCE
#define EXTERNAL_PULL_UP_RESISTOR

void init_pins() {
    sbi(DDRB, DDB4);
    sbi(DDRB, DDB5);
    sbi(EIMSK, INT1);
    sbi(EICRA, ISC11);

#ifndef EXTERNAL_PULL_UP_RESISTOR
    sbi(PORTD, PORTD3);
#endif
}

void toggle_led() {
    sbi(PINB, PINB5);
    _delay_ms(500);
}

ISR(INT1_vect) {
#ifndef EXTERNAL_DEBOUNCE
    volatile static unsigned long last_interrupt_time{0};
    volatile unsigned long interrupt_time{millis()};

    if (interrupt_time - last_interrupt_time <= 200) {
        last_interrupt_time = interrupt_time;
        return;
    }

    last_interrupt_time = interrupt_time;
#endif

    sbi(PINB, PINB4);
}

int main() {
    init();
    init_pins();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        toggle_led();
        toggle_led();
    }
#pragma clang diagnostic pop
}
