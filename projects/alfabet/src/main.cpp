/*
 * alfabet program with Serial
 */
// keep intellisense satisfied
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <HardwareSerial.h>

int main() {
    Serial.begin(9600);

    while (true) {
        for (char c{'A'}; c < 'Z'; c++) {
            Serial.println(c);
            Serial.flush();
            _delay_ms(200);
        }

        Serial.println();
        Serial.flush();
    }
}
