#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

ISR(INT0_vect) {
    EIMSK &= ~(1 << INT0);

    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCNT1 = 0;

    PINB = (1 << PINB5);
}

ISR(TIMER1_COMPA_vect) {
    TCCR1B = 0;

    EIFR |= (1 << INTF0);
    EIMSK |= (1 << INT0);
}

int main() {
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = 1563;

    PORTD |= (1 << PORTD2);
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);

    DDRB |= (1 << DDB5);

    EIFR |= (1 << INTF0);
    sei();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {}
#pragma clang diagnostic pop
}
