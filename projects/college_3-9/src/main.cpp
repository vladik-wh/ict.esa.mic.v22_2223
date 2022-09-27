#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#include <util/setbaud.h>
#include <usart.h>

void init_pins() {
    DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
}

int main(void) {
    init_pins();
    USART_Init();
    uint8_t counter = 0;

    while (true) {
        USART_Transmit('0');
        USART_Transmit('o');
        USART_Transmit((counter / 8) | 0x30);
        USART_Transmit((counter % 8) | 0x30);
        USART_Transmit('\n');

        PORTD &= 0x03;
        PORTD |= counter << 2;

        counter++;
        counter &= 0x3F;
        _delay_ms(250);
    }

    // never reached
    return (0);
}
