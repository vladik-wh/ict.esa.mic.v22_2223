#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define EXTERNAL_DEBOUNCING
#define EXTERNAL_PULL_UP_RESISTOR
#define USING_INTERRUPT
#ifndef EXTERNAL_DEBOUNCING
#include <Arduino.h>

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
    _delay_ms(DEBOUNCE_TIME_MS);        _delay_ms(1);
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

#ifdef USING_INTERRUPT
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01);
#endif

    // Set button pin to input
//    DDRD &= ~(1 << DDD2); // input by default

#ifndef EXTERNAL_PULL_UP_RESISTOR
    // Set button pin to pull-up
    PORTD |= (1 << PORTD2); // Uncomment this line if there is no external pull-up resistor
#endif

    sei();
}

#ifdef USING_INTERRUPT
static bool int0_flag = false;

ISR(INT0_vect) {
#ifndef EXTERNAL_DEBOUNCING
    volatile static unsigned long last_interrupt_time{0};
    volatile unsigned long interrupt_time{millis()};

    if (interrupt_time - last_interrupt_time <= DEBOUNCE_TIME_MS) {
        last_interrupt_time = interrupt_time;
        return;
    }

    last_interrupt_time = interrupt_time;
#endif
    int0_flag = true;
}

#endif

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
#ifndef EXTERNAL_DEBOUNCING
    init();
#endif
    init_pins();
    uint8_t counter{0};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
#ifdef USING_INTERRUPT
        _delay_ms(1);

        if (int0_flag) {
            int0_flag = false;
            counter++;
            counter &= COUNTER_MASK;
            display_counter(counter);
        }
#else
        if (vehicle_passed())
            counter++;

        counter &= COUNTER_MASK;
        display_counter(counter);
#endif
    }
#pragma clang diagnostic pop
}
