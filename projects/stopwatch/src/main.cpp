#include <avr/interrupt.h>
#include <avr/io.h>

#pragma region Button

static bool button_status{false};

void init_button();

void enable_button();

void disable_button();

bool button_is_pressed();

bool get_button_status();

void set_button_status(bool status);

void button_interrupt_handler();

#pragma endregion Button

#pragma region Debounce timer

void init_debounce_timer();

void enable_debounce_timer();

void disable_debounce_timer();

void debounce_timer_interrupt_handler();

#pragma endregion Debounce timer

#pragma region Centibeat timer

void init_centibeat_timer();

#pragma endregion Centibeat timer

void init() {
    init_button();
    init_debounce_timer();
    init_centibeat_timer();
    sei();
}

void superloop() {
    if (get_button_status()) {
        PORTB |= (1 << PORTB5);
    } else
        PORTB &= ~(1 << PORTB5);
}

int main() {
    DDRB |= (1 << DDB5); // Pin 13 to output mode

    init();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        superloop();
    }
#pragma clang diagnostic pop
}

#pragma region Button

void init_button() {
    //    DDRD &= ~(DDD2); // Set digital pin 2 (PD2/INT0) to input mode (is input by default)
    PORTD |= (1 << PORTD2); // Set digital pin 2  (PD2/INT0) to pull up mode
//    EIMSK &= ~(1 << INT0); // Disable external interrupt 0 (is disabled by default)
    EICRA |= (1 << ISC00); // Set interrupt 0 to trigger on logical change
}

void enable_button() {
    EIMSK |= (1 << INT0); // Enable interrupt 0
}

void disable_button() {
    EIMSK &= ~(1 << INT0); // Disable interrupt 0
}

bool button_is_pressed() {
    return PIND & (1 << PIND2);
}

bool get_button_status() {
    return button_status;
}

void set_button_status(bool status) {
    button_status = status;
}

void button_interrupt_handler() {
    if (button_is_pressed()) {
        if (!get_button_status())
            return;
        set_button_status(false);
    } else if (!get_button_status())
        set_button_status(true);
    else
        return;

    disable_button();
    enable_debounce_timer();
}

#pragma endregion Button

#pragma region Debounce timer

void init_debounce_timer() {
    static constexpr uint8_t debounce_compare_value = 234;

    //    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 0 overflow compare A interrupt (disabled by default)
    TCCR0B |= (1 << CS02) | (1 << CS00); // Start timer 1 with 1024 prescaler
    OCR0A = debounce_compare_value; // Set the timer 0 compare register A to 30 ms
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 1 compare A interrupt
}

void enable_debounce_timer() {
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 1 compare A interrupt
    TCNT0 = 0; // Reset timer 1
}

void disable_debounce_timer() {
    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 1 compare A interrupt
}

void debounce_timer_interrupt_handler() {
    static constexpr uint8_t debounce_compare_interrupt_cycles = 2;
    static uint8_t count{0};

    count++;
    if (count < debounce_compare_interrupt_cycles)
        return;

    count = 0;

    enable_button();
    disable_debounce_timer();
}

#pragma endregion Debounce timer

#pragma region Centibeat timer

void init_centibeat_timer() {
    static constexpr uint16_t centibeat_compare_value = 54000;

    TCCR1B |= (1 << CS12); // Start timer 1 with 256 prescaler
    OCR1A = centibeat_compare_value; // Set the timer 1 compare register A to 0.864 seconds
}

#pragma endregion Centibeat timer

/**
 * Interrupt 0 Vector
 *
 * Used to set the button status.
 */
ISR(INT0_vect) {
    button_interrupt_handler();
}

/**
 * Timer 0 Output Compare Register A Vector
 *
 * Used for debouncing the button vor 30 milliseconds.
 *
 * The following formula is used to wait for specific amount of time:
 * (processor hz * seconds) / (prescaler * output compare register A value) = cycles
 */
ISR(TIMER0_COMPA_vect) {
    debounce_timer_interrupt_handler();
}
