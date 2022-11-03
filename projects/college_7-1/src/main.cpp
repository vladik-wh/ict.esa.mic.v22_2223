#include <avr/interrupt.h>
#include <avr/io.h>

int main() {
    sei();

    DDRD |= (1 << DDD5) | (1 << DDD6);

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0A |= (1 << COM0A1);
    TCCR0A |= (1 << COM0B1);
    TCCR0B |= (1 << CS02);
    TIMSK0 |= (1 << TOIE0);
    TCNT0 = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {

    }
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    static uint8_t speed{0};
    static uint8_t step{1};
    speed += step;

    if (speed == 255 || speed == 0)
        step = -step;

    OCR0A = speed;
    OCR0B = 255 - speed;
}
