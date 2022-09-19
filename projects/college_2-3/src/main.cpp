#include <avr/io.h>

int main() {
//    DDRC &= ~(1 << DDC5); // input by default
    DDRB |= (1 << DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB3);

    PORTC |= (1 << PORTC5);

    while (true) {
        /*/ one-liner 220 bytes
        PINB ^= (!(PINC & (1 << PINC5)) << PINB0 | (1 - !(PINC & (1 << PINC5))) << PINB1 |
                 (1 - !(PINC & (1 << PINC5))) << PINB2 | !(PINC & (1 << PINC5)) << PINB3);
        /*/
        if (PINC & (1 << PINC5)) {
//            asm volatile ("ldi  r24,    0x06;");
//            asm volatile ("out  0x03,   r24;");
            PINB &= ~(1 << PINB0 | 1 << PINB3);
            PINB |= (1 << PINB1 | 1 << PINB2);
        } else {
//            asm volatile ("ldi  r24,    0x09;");
//            asm volatile ("out  0x03,   r24;");
            PINB |= (1 << PINB0 | 1 << PINB3);
            PINB &= ~(1 << PINB1 | 1 << PORTB2);
        }
        //*/
    }
}
