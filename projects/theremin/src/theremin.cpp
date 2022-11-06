/**
 * @file theremin.cpp
 * @brief Theremin Synthesizer
 *
 * A fake theremin synthesizer that uses a light dependent resistor to generate
 * tones from the piezoelectric speaker. The potentiometer controls the volume.
 *
 * @author Vladik Packo
 * <br>
 * Student nr.: 1160265
 * <br>
 * Contact: s1160265&#64;student.windesheim.nl
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#define ADC0 0x0
#define ADC1 (1 << MUX0)
#define MUX ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) |(1 << MUX0))
#define LDR ADC0
#define POT ADC1
#define BUZZ (1 << DDB3)

/**
 * Initializes timer/counter0.
 * <br><br>
 * Used to modulate OC2A mode between non-inverting and disconnected. Which will
 * generate and modulate a tone.
 */
void timer0_init();

/**
 * Initializes timer/counter2.
 * <br><br>
 * Used to modulate the duty cycle of OC2A. Which will modulate the volume.
 */
void timer2_init();

/**
 * Initializes the ADC.
 * <br><br>
 * Used to toggle the input channel and set the volume and tone levels.
 */
void adc_init();

/**
 * ADC interrupt flag.
 */
volatile bool adc_flag{false};

int main() {
    // Digital pin 11 (OC2A) to output mode (piezoelectric speaker)
    DDRB |= BUZZ;

    timer0_init();
    timer2_init();
    adc_init();

    // Enable global interrupts
    sei();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    bool swap{true};

    while (true) {
        if (!adc_flag)
            continue;

        // ADC Control and Status Register A start ADC conversion
        ADCSRA |= (1 << ADSC);

        // ADC Multiplex clear input channel
        ADMUX &= ~MUX;

        if (swap) {
            // Timer/Counter0 output compare register A set to ADC value
            OCR0A = ADCH;
            // ADC Multiplex set input channel to ADC0
            ADMUX |= LDR;
        } else {
            // Timer/Counter2 output compare register A set to ADC value
            OCR2A = ADCH;
            // ADC Multiplex set input channel to ADC1
            ADMUX |= POT;
        }

        swap ^= true;
        adc_flag = false;
    }
#pragma clang diagnostic pop
}

void timer0_init() {
    // Timer/Counter0 Control Register A
    // Set to mode 3 (CTC)
    TCCR0A |= (1 << WGM01);
    // Clock select prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // Timer/Counter0 Interrupt Mask Register
    // Output Compare Match B Interrupt Enable
    TIMSK0 |= (1 << OCIE0A);

    // Timer/Counter0 Output Compare Register A
    // Default value to the lowest tone
    OCR0A = 255;
}

void timer2_init() {
    // Timer/Counter2 Control Register A
    // Waveform generation mode 3 (Fast PWM, 0xFF)
    TCCR2A |= (1 << WGM21) | (1 << WGM20);

    // Timer/Counter2 Control Register A
    // Clock select prescaler
    TCCR2B |= (1 << CS20);

    // Timer/Counter2 Output Compare Register A
    // Default value to maximum volume
    OCR2A = 255;
}

void adc_init() {
    // ADC Control and Status Register A
    // ADC Enable
    ADCSRA |= (1 << ADEN);
    // ADC prescaler select division factor 32
    ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
    // ADC trigger source select free running mode (default)
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
    // ADC Interrupt Enable
    ADCSRA |= (1 << ADIE);

    // ADC Multiplexer Selection Register
    // ADC left adjust result
    ADMUX |= (1 << ADLAR);
    // ADC voltage reference select AVcc
    ADMUX |= (1 << REFS0);

    // ADC start conversion
    ADCSRA |= (1 << ADSC);
}

/**
 * Interrupt Service Routine of Analog-to-Digital Converter
 * <br><br>
 * Used to switch the ADC input channels.
 */
ISR(ADC_vect) {
    if (adc_flag)
        return;

    adc_flag = true;
}

/**
 * Interrupt Service Routine of Timer/Counter0 Compare A
 * <br><br>
 * Toggles the PB3/OC2A/D11 pin to generate a tone.
 */
ISR(TIMER0_COMPA_vect) {
    // Timer/Counter2 Control Register A toggle OC2A non-inverting mode
    TCCR2A ^= (1 << COM2A1);
}
