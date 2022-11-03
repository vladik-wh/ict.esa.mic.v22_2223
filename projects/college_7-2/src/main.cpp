#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint16_t adc_value{0};

int main() {
    sei();

    DDRD |= (1 << DDD5) | (1 << DDD6);

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0A |= (1 << COM0A1);
    TCCR0A |= (1 << COM0B1);
    TCCR0B |= (1 << CS02);
    TCNT0 = 0;

    ADCSRA = (1 << ADEN);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
//    ADMUX = 0x0;
    ADMUX |= (1 << REFS0);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        adc_value = ADC;
        TIMSK0 |= (1 << TOIE0);
    }
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    OCR0A = static_cast<uint8_t>((adc_value / 1023.0f) * 0xFF);
    TIMSK0 &= ~(1 << TOIE0);
}
