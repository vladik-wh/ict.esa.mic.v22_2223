#include <avr/interrupt.h>
#include <avr/io.h>

#define CLOCK_SPEED 16000000
#define NOTE_EXPONENT 1.059463094l
#define MIN_HZ 440
#define PRESCALAR 8

ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << PORTB0);
}

double pow(double a, int b) {
    double r = a;
    while (b) {
        r *= a;
        b--;
    }

    return r;
}

uint16_t tone(uint8_t note = 0) {
    static uint16_t notes[12]{
            4545,
            4290,
            4050,
            3822,
            3608,
            3405,
            3214,
            3034,
            2863,
            2703,
            2551,
            2408,
    };

    return notes[note];

    // Massive over head of 700 bytes;
    // return static_cast<uint16_t>(CLOCK_SPEED / (PRESCALAR * MIN_HZ * pow(NOTE_EXPONENT, note)));
}

void delay(uint16_t ms) {
    TCCR0A |= (1 << WGM01);
    OCR0A = 250;
    TCCR0B |= (1 << CS00);

    uint8_t counter{0};
    while (ms > 0) {
        if (counter == 63) {
            counter = 0;
            ms--;
        }
        counter++;
        while (!(TIFR0 & (1 << OCF0A)));
        TIFR0 |= 1 << OCF0A;
    }
}

int main() {
    uint8_t note = 0;

    TCCR1B |= (1 << WGM12) | (1 << CS11);
    OCR1A = tone(note);
    TIMSK1 |= (1 << OCIE1A);
    sei();

    DDRB |= (1 << DDB0);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        delay(500);
        note++;
        if (note > 11)
            note = 0;
        OCR1A = tone(note);
    }
#pragma clang diagnostic pop
}
