#include <avr/interrupt.h>
#include <avr/io.h>

int main() {
    sei();

    DDRD |= (1 << DDD5);

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02);
    TCCR0A |= (1 << COM0B1);
    TCCR0B |= (1 << CS02);
    OCR0A = 142; // 440hz
    OCR0B = 71; // 50% duty cycle
    TCNT0 = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {

    }
#pragma clang diagnostic pop
}
