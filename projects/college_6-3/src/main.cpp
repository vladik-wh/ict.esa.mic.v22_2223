#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

volatile uint16_t ticks{0};
volatile uint8_t adc_value{0};
volatile bool sample_ready{false};
volatile bool knocked{false};

int main() {
    sei();

    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

    TCCR0B |= (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
    TCNT0 = 0;

    ADMUX |= (1 << REFS0); // Internal VREF turned off
    ADCSRA |= (1 << ADEN); // ADC Enable
    ADCSRA |= (1 << ADATE); // ADC Auto Trigger Enable
    ADCSRA |= (1 << ADIE); // ADC Interrupt Enable
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 Division Factor
    ADCSRB |= (1 << ADTS2); // Timer/Counter0 overflow

    DDRB |= (1 << DDB5);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (sample_ready) {
            sample_ready = false;

            if (adc_value > 2) {
                Serial.println(adc_value);
                Serial.flush();
                PORTB |= (1 << PORTB5);
                knocked = true;
            }
        }

        if (ticks > 305) {
            PORTB &= ~(1 << PORTB5);
            ticks = 0;
            knocked = false;
        }
    }
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    if (knocked)
        ticks++;
}

ISR(ADC_vect) {
    adc_value = ADC;
    sample_ready = true;
}
