#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>
#define SIGRD 5
#include <avr/boot.h>

volatile uint16_t adc_value{0};
volatile bool sample_ready{false};

int main() {
    sei();

    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);
    OCR1A = 15624;
    OCR1B = 0;
    TCNT1 = 0;

    ADMUX |= (1 << MUX3); // ADC8 (temperature sensor)
    ADMUX |= (1 << REFS1) | (1 << REFS0); // Internal 1.1V reference
    ADCSRA |= (1 << ADEN); // ADC Enable
    ADCSRA |= (1 << ADATE); // ADC Auto Trigger Enable
    ADCSRA |= (1 << ADIE); // ADC Interrupt Enable
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 Division Factor
    ADCSRB |= (1 << ADTS2) | (1 << ADTS0); // Timer/Counter1 compare match B

    const uint8_t TS_GAIN = boot_signature_byte_get(0x0003);
    const int8_t TS_OFFSET = boot_signature_byte_get(0x0002);

    Serial.println(TS_GAIN);
    Serial.println(TS_OFFSET);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (sample_ready) {
            sample_ready = false;

            Serial.println(adc_value);
            Serial.flush();
        }
    }
#pragma clang diagnostic pop
}

ISR(TIMER1_COMPA_vect) {

}

ISR(TIMER1_COMPB_vect) {

}

ISR(ADC_vect) {
    adc_value = ADC;
    sample_ready = true;
}
