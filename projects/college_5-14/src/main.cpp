#include <avr/interrupt.h>
#include <avr/io.h>

#define SECOND 15624
#define TIMER1_MAX_VALUE 65535

ISR(TIMER1_COMPA_vect) {
    PORTB &= ~(1 << PORTB5);
}

ISR(TIMER1_OVF_vect) {
    PORTB = (1 << PORTB5);
}

int main() {
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = SECOND;
    TIMSK1 |= (1 << OCIE1A) | (1 << TOIE1);
    sei();

    DDRB |= (1 << DDB5);
    PORTB = (1 << PORTB5);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {}
#pragma clang diagnostic pop
}
