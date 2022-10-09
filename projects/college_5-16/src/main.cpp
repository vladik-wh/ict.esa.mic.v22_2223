#include <avr/interrupt.h>
#include <avr/io.h>

#define HZ880 71

volatile static uint8_t counter{0};

ISR(TIMER0_OVF_vect) {
        counter++;
}

int main() {
    TCCR0B |= (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
    DDRB |= (1 << DDB0);
    sei();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (counter >= HZ880) {
            counter = 0;
            PINB = (1 << PINB0);
        }
    }
#pragma clang diagnostic pop
}
