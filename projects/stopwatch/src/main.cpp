#include <avr/interrupt.h>
#include <avr/io.h>
#include <Wire.h>

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

void start_debounce_timer();

void stop_debounce_timer();

void debounce_timer_interrupt_handler();

#pragma endregion Debounce timer

#pragma region Centibeat timer

void init_centibeat_timer();

void start_centibeat_timer();

void stop_centibeat_timer();

void centibeat_timer_interrupt_handler();

#pragma endregion Centibeat timer

#pragma region Display

static uint8_t display_twi_address = 0x38;

enum class display_char : uint8_t {
    num_0 = 0b01000000,
    num_1 = 0b01111001,
    num_2 = 0b00100100,
    num_3 = 0b00110000,
    num_4 = 0b00011001,
    num_5 = 0b00010010,
    num_6 = 0b00000010,
    num_7 = 0b01111000,
    num_8 = 0b00000000,
    num_9 = 0b00010000,
    A = 0b00001000,
    B = 0b00000011,
    C = 0b01000110,
    D = 0b00100001,
    E = 0b00000110,
    F = 0b00001110,
};

static display_char display_chars[16] = {
        display_char::num_0,
        display_char::num_1,
        display_char::num_2,
        display_char::num_3,
        display_char::num_4,
        display_char::num_5,
        display_char::num_6,
        display_char::num_7,
        display_char::num_8,
        display_char::num_9,
        display_char::A,
        display_char::B,
        display_char::C,
        display_char::D,
        display_char::E,
        display_char::F,
};

volatile static uint8_t centibeat{0};

void init_display();

void render_display(uint8_t value);

void render_display(display_char character);

void clear_display();

#pragma endregion Display

void init() {
    sei();
    init_button();
    init_debounce_timer();
    init_centibeat_timer();
    init_display();
}

void superloop() {
    static uint8_t previous_centibeat;

    if (previous_centibeat != centibeat) {
        previous_centibeat = centibeat;
        render_display(display_chars[centibeat]);
    }
//    render_display(display_chars[centibeat]);
}

int main() {
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
    EIFR = (1 << INTF0);
    EIMSK |= (1 << INT0); // Enable interrupt 0
}

void disable_button() {
    EIMSK &= ~(1 << INT0); // Disable interrupt 0
}

bool button_is_pressed() {
    return PIND & (1 << PIND2); // Read digital pin 2 (PD2/INT0) value
}

bool get_button_status() {
    return button_status;
}

void set_button_status(bool status) {
    button_status = status;

    if (status) {
        centibeat = 0;
        start_centibeat_timer();
    } else
        stop_centibeat_timer();
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
    start_debounce_timer();
}

#pragma endregion Button

#pragma region Debounce timer

void init_debounce_timer() {
    /**
     * @var Equals to 0.03 seconds (30 milliseconds) on a 16 mhz processor and timer set to 1024 prescaling.
     */
    static constexpr uint8_t debounce_compare_value = 234;

    TCCR0A = (1 << WGM01); // Set timer 0 to clear timer on compare (CTC) mode
//    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 0 overflow compare A interrupt (disabled by default)
    OCR0A = debounce_compare_value; // Set the timer 0 compare register A to 30 ms
    start_debounce_timer();
}

void start_debounce_timer() {
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 0 compare A interrupt
    TCNT0 = 0; // Reset timer 0
    TCCR0B |= (1 << CS02) | (1 << CS00); // Start timer 0 with 1024 prescaler
}

void stop_debounce_timer() {
    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 0 compare A interrupt
    TCCR0B = 0; // Stop timer 0
}

void debounce_timer_interrupt_handler() {
    static constexpr uint8_t debounce_compare_interrupt_cycles = 2;
    static uint8_t count{0};

    count++;
    if (count < debounce_compare_interrupt_cycles)
        return;

    count = 0;

    enable_button();
    stop_debounce_timer();
}

#pragma endregion Debounce timer

#pragma region Centibeat timer

void init_centibeat_timer() {
    /**
     * Equals to 0.864 seconds (1 centibeat) on a 16 mhz processor and timer set to 256 prescaling.
     */
    static constexpr uint16_t centibeat_compare_value = 54000;

    TCCR1B = (1 << WGM12);  // Set timer 1 to clear timer on compare (CTC) mode
//    TIMSK1 &= ~(1 << OCIE1A); // Disable the timer 1 overflow compare A interrupt (disabled by default)
//    TCCR1B |= (1 << CS12); // Start timer 1 with 256 prescaler
    OCR1A = centibeat_compare_value; // Set the timer 1 compare register A to 0.864 seconds
}

void start_centibeat_timer() {
    TIMSK1 |= (1 << OCIE1A); // Enable the timer 1 compare A interrupt
    TCNT1 = 0; // Reset timer 1
    TCCR1B |= (1 << CS12); // Start timer 1 with 1024 prescaler
}

void stop_centibeat_timer() {
    TIMSK1 &= ~(1 << OCIE1A); // Disable the timer 1 compare A interrupt
    TCCR1B = 0; // Stop timer 1
}

void centibeat_timer_interrupt_handler() {
    centibeat++;
    centibeat &= 0xF;
}

#pragma region Display

void init_display() {
#ifndef WIRE_BEGIN_CALLED
    Wire.begin();
#endif

    clear_display();
}

void render_display(uint8_t value) {
    Wire.beginTransmission(display_twi_address);
    Wire.write(value);
    Wire.endTransmission();
}

void render_display(display_char character) {
    render_display(static_cast<uint8_t>(character));
}

void clear_display() {
    render_display(display_char::num_0);
}

#pragma endregion Display

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

/**
 * Timer 1 Output Compare Register A Vector
 *
 * Used for counting centibeat.
 *
 * The following formula is used to determine when a centibeat passes:
 * (processor hz * centibeat in seconds) / (prescaler) = centibeat
 */
ISR(TIMER1_COMPA_vect) {
    centibeat_timer_interrupt_handler();
}
