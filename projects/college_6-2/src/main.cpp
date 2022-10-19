#include <avr/interrupt.h>
#include <avr/io.h>

int main() {
    sei();

    TCCR1B |= (1 << WGM12) | (1 << CS10);
    OCR1A = 36359; // 18184..36360
    TIMSK1 |= (1 << OCIE1A);
    TCNT1 = 0;

    ADCSRA = (1 << ADEN);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
//    ADMUX = 0x0;
    ADMUX |= (1 << ADLAR);
    ADMUX |= (1 << REFS0);

    uint16_t adc_value{0};

    DDRB |= (1 << DDB0);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        adc_value = ADCH;
        OCR1A = (uint16_t)(36359 - 18175 * ((float)adc_value / 255));
    }
#pragma clang diagnostic pop
}

ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << PORTB0);
}
