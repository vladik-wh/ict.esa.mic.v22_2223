#include <avr/io.h>
#include <util/delay.h>

void init_pins() {
    DDRB |= (1 << DDB4) | (1 << DDB5);
}

void clock_toggle(uint8_t n) {
    PINB = 1 << PINB4;
    _delay_ms(500 / static_cast<double>(n));
    PINB = 1 << PINB4;
    _delay_ms(500 / static_cast<double>(n));
}

void send_bit(bool b, uint8_t repeat = 1) {
    do {
        if (b) {
            PORTB |= 1 << PORTB5;
        } else
            PORTB &= ~(1 << PORTB5);
        repeat--;
        clock_toggle(1);
    } while (repeat);
}

/**
 * Sends 1 start bit, 8 data bits, 1 parity bit and 2 stop bit = 12 bits
 * @param b 8-bit value to send
 */
void send_byte(uint8_t b) {
    // Start
    send_bit(false);

    for (uint8_t i {0}; i < 8; i++)
    {
        send_bit((b >> i) & 1);
    }

    // Even parity
    send_bit(__builtin_popcount(b) % 2);

    // Stop
    send_bit(false, 2);
}

void send_text(const char* t, uint8_t length)
{
    for (uint8_t i{0}; i < length; i++)
        send_byte(t[i]);
}

int main() {
    init_pins();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        send_bit(true, 4);
        send_text("Hi", 2);
        send_bit(true, 4);
    }
#pragma clang diagnostic pop
}
