#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

ISR(INT0_vect) {
    bool pinB5 = PINB & (1 << PINB5);

    if (PIND & (1 << PIND2)) {
        if (!pinB5)
            return;
        PORTB &= ~(1 << PORTB5);
    } else if (!pinB5)
        PORTB |= (1 << PORTB5);
    else
        return;

    EIMSK &= ~(1 << INT0); // Disable interrupt 0
    TIMSK1 |= (1 << OCIE1A); // Enable the timer 1 compare A interrupt
    TCNT1 = 0; // Reset timer 1
}

ISR(TIMER1_COMPA_vect) {
    bool pinB5 = PINB & (1 << PINB5);

    if (PIND & (1 << PIND2)) {
        if (pinB5)
            PORTB &= ~(1 << PORTB5);
    } else if (!pinB5)
        PORTB |= (1 << PORTB5);


    EIMSK |= (1 << INT0); // Enable interrupt 0
    TIMSK1 &= ~(1 << OCIE1A); // Disable the timer 1 compare A interrupt
}

int main() {
    DDRB |= (1 << DDB5); // Pin 13 to output mode

    PORTD |= (1 << PORTD2); // Digital pin 2  (PD2/INT0) to pull up mode
    EIMSK |= (1 << INT0); // Enable external interrupt 0
    EICRA |= (1 << ISC00); // Set interrupt 0 to trigger on logical change
    TCCR1B |= (1 << CS12) | (1 << CS10); // Start timer 1
    OCR1A = 469; // Set the timer 1 compare register A to 30 ms

    EIFR |= (1 << INTF0); // Clear the external interrupt flag 0
    sei();

    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {}
#pragma clang diagnostic pop
}
