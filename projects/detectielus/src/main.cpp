#include <avr/io.h>

//#define EXTERNAL_DEBOUNCING
//#define EXTERNAL_PULL_UP_RESISTOR
#ifndef EXTERNAL_DEBOUNCING
#include <util/delay.h>

#define DEBOUNCE_TIME_MS 50
#endif

#define COUNTER_MASK 0x0F

/**
 * Button state.
 */
enum bstate {
    pressed,
    released,
};

/**
 * Displays a number ranging 0 < counter < 15 on the 4-bit LED display.
 * @param counter Number to represent on the display.
 */
void display_counter(uint8_t counter) {
    PORTC = (counter & COUNTER_MASK) | (PORTC & ~COUNTER_MASK);
}

/**
 * Gets the button of the PIND2 register (digital pin 2).
 * @return bstate State of the button.
 */
enum bstate button_state() {
    static bool state{true};
    bool pin = PIND & (1 << PIND2);

    if (pin == state)
        return static_cast<bstate>(state);

#ifndef EXTERNAL_DEBOUNCING
    _delay_ms(DEBOUNCE_TIME_MS);
#endif
    state = PIND & (1 << PIND2);

    return static_cast<bstate>(state);
}

/**
 * Initializes the pins.
 * <br><br>
 * Sets the DDC0, DDC1, DDC2, DDC3 to output for the 4-bit LED display (analog input pins 0-3).
 * <br><br>
 * Sets the PORTD2 pin to pull-up mode for the button.
 */
void init_pins() {
    // Set LED pins to output
    DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3);

    // Set button pin to input
//    DDRD &= ~(1 << DDD2); // input by default

#ifndef EXTERNAL_PULL_UP_RESISTOR
    // Set button pin to pull-up
    PORTD |= (1 << PORTD2); // Uncomment this line if there is no external pull-up resistor
#endif
}

/**
 * Returns true when a "vehicle passes".
 * @return <code>true</code> if the button has been released, otherwise <code>false</code>.
 */
bool vehicle_passed() {
    static bool state{false};

    if (state) {
        if (button_state() == bstate::released) {
            state = false;
            return true;
        }
    } else if (button_state() == bstate::pressed) {
        state = true;
    }

    return false;
}

int main() {
    init_pins();
    uint8_t counter{0};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (vehicle_passed())
            counter++;

        /*/ Counter reset according to design
        if (counter > COUNTER_MASK)
            counter = 0;
        /*/
        counter &= COUNTER_MASK;
        //*/

        display_counter(counter);
    }
#pragma clang diagnostic pop
}
