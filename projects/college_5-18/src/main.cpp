#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>

volatile static bool int0_flag = false;
volatile static bool int0_enabled = false;

ISR(INT0_vect) {
    if (int0_enabled) {
        int0_enabled =  false;
        return;
    }

    EIMSK &= ~(1 << INT0);

    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCNT1 = 0;

    PINB = (1 << PINB5);

    int0_flag = true;
}

ISR(TIMER1_COMPA_vect) {
    TCCR1B = 0;

    EIMSK |= (1 << INT0);

    int0_enabled = true;
}

int main() {
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = 781;

    PORTD |= (1 << PORTD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01) | (1 << ISC00);

    DDRB |= (1 << DDB5);

    sei();

    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (int0_flag) {
            Serial.println("int0");
            Serial.flush();
            int0_flag = false;
        }
    }
#pragma clang diagnostic pop
}
