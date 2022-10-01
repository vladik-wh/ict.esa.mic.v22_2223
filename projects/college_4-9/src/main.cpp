#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <HardwareSerial.h>

//#define EXTERNAL_PULL_UP_RESISTOR

void init_pins() {
    DDRB |= (1 << DDB5);

    DDRD |= (1 << DDD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    PORTD |= (1 << PORTD2);

#ifndef EXTERNAL_PULL_UP_RESISTOR

#endif
}

void toggle_led() {
    PIND = (1 << PIND2);
}

ISR(INT0_vect, ISR_NOBLOCK) {
    Serial.println(PIND & (1 << PIND2));
    Serial.flush();
}

int main() {
    init_pins();
    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();
    sei();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        _delay_ms(500);
        toggle_led();
        _delay_ms(500);
        toggle_led();
    }
#pragma clang diagnostic pop
}
