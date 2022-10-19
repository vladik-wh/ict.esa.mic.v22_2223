#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

volatile bool TIMER1_COMPA_flag{false};

int main() {
    sei();
    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12);
    OCR1A = 6249;
    TCNT1 = 0;

    ADCSRA =  (1 << ADEN);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
//    ADMUX = 0x0;
    ADMUX |= (1 << REFS0);

    uint16_t adc_value{0};


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (TIMER1_COMPA_flag) {
            TIMER1_COMPA_flag = false;

            ADCSRA |= (1 << ADSC);
            while (ADCSRA & (1 << ADSC));
            adc_value = ADC;
            Serial.println(adc_value);
        }
    }
#pragma clang diagnostic pop
}

ISR(TIMER1_COMPA_vect) {
    TIMER1_COMPA_flag = true;
}
