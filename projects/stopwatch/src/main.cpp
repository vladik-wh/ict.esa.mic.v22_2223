#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

#define OUTPUT_COMPARE_INTERRUPT_CYCLES 2
#define OUTPUT_COMPARE_REGISTER_A_VALUE 234

volatile static bool button_status{false};

/**
 * Interrupt 0 Vector
 *
 * Used to set the button status.
 */
ISR(INT0_vect) {
    if (PIND & (1 << PIND2)) {
        if (!button_status)
            return;
        button_status = false;
    } else if (!button_status)
        button_status = true;
    else
        return;

    EIMSK &= ~(1 << INT0); // Disable interrupt 0
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 1 compare A interrupt
    TCNT0 = 0; // Reset timer 1
}

/**
 * Timer 0 Output Compare Register A Vector
 *
 * Used for debouncing the button vor 30 milliseconds.
 *
 * The following formula is used to wait for specific amount of time:
 * (processor hz * seconds) / (prescaler * output compare register A value) = cycles
 */
ISR(TIMER0_COMPA_vect) {
    static uint8_t count{0};

    count++;
    if (count < OUTPUT_COMPARE_INTERRUPT_CYCLES)
        return;

    count = 0;

    if (PIND & (1 << PIND2)) {
        if (button_status)
            button_status = false;
    } else if (!button_status)
        button_status = true;

    EIMSK |= (1 << INT0); // Enable interrupt 0
    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 1 compare A interrupt
}

int main() {
    DDRB |= (1 << DDB5); // Pin 13 to output mode

    // Setup Interrupt 0 and disable it at first
//    DDRD &= ~(DDD2); // Set digital pin 2 (PD2/INT0) to input mode (is input by default)
    PORTD |= (1 << PORTD2); // Set digital pin 2  (PD2/INT0) to pull up mode
//    EIMSK &= ~(1 << INT0); // Disable external interrupt 0 (is disabled by default)
    EICRA |= (1 << ISC00); // Set interrupt 0 to trigger on logical change

    // Setup and start the debounce timer
    TCCR0B |= (1 << CS02) | (1 << CS00); // Start timer 1 with 1024 prescaler
    OCR0A = OUTPUT_COMPARE_REGISTER_A_VALUE; // Set the timer 0 compare register A to 30 ms
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 1 compare A interrupt
    TCNT0 = 0; // Reset timer 1

    sei();

    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    // "Superloop"
    while (true) {
        if (button_status)
            PORTB |= (1 << PORTB5);
        else
            PORTB &= ~(1 << PORTB5);
    }
#pragma clang diagnostic pop
}
