#include <avr/io.h>
#include <HardwareSerial.h>
#include <util/atomic.h>
#include <util/delay.h>

enum bstate {
    pressed,
    released,
};

void display_counter(uint8_t counter) {
    PORTC = (counter & 0x0F) | (PORTC & 0xF0);
}

enum bstate button_state() {
    // Pin should start high
    static bool state{true};

    // Store the pin status
    bool pin = PIND & (1 << PIND2);

    // Pin is high just as the previous high state
    if (pin && state)
        return bstate::released;

    // Pin is low just as the previous low state
    if (!pin && !state)
        return bstate::pressed;

    // Wait for debounce and set the state
    _delay_ms(50);
    state = PIND & (1 << PIND2);

    // Return released if the state is high
    if (state)
        return bstate::released;

    // Return pressed if the state is low
    return bstate::pressed;
}

void init_pins() {
    // Set LED pins to output
    DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3);

    // Set button pin to input
//    DDRD &= ~(1 << DDD2); // input by default

    // Set button pin to pull-up
    PORTD |= (1 << PORTD2);

    sei();
}

bool vehicle_passed() {
    static bool state{false};

    return false;
}

int main() {
    init_pins();
    Serial.begin(9600);
    uint8_t counter = 0;

    bool state{false};

    while (true) {
        if (!state && button_state() == bstate::pressed) {
            Serial.println("pressed");
            state = true;
        } else if (state && button_state() == bstate::released) {
            Serial.println("released");
            state = false;
        }
    }
}
