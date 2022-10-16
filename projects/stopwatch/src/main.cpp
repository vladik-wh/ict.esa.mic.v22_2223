#include <avr/interrupt.h>
#include <avr/io.h>
#include <Wire.h>

#pragma region Button

/**
 * The state of the button.
 * <br><br>
 * Is <code>true</code> as long as the button is held down.
 */
static bool button_status{false};

/**
 * Initializes the button.
 * <br><br>
 * Sets the interrupt pin 0 to input pull-up mode
 * and to trigger on logical change.
 */
void init_button();

/**
 * Enables the button.
 * <br><br>
 * Clears the external interrupt flag 0 and enables interrupt 0.
 */
void enable_button();

/**
 * Disables the button.
 * <br><br>
 * Disables interrupt 0.
 */
void disable_button();

/**
 * Returns the value of the button pin.
 *
 * @warning Should not be used to determine the button status.
 * Use get_button_status for that.
 * @see get_button_status
 * @return The value of interrupt 0 pin (PD2/INT0/digital pin 2)
 */
bool button_is_high();

/**
 * Returns the status of the button.
 *
 * @return <code>true</code> if the button is held down.
 */
bool get_button_status();

/**
 * Sets the status of the button.
 *
 * @param status Should be set to <code>true</code> if the button is held down.
 */
void set_button_status(bool status);

/**
 * Handler for an interrupt vector.
 * <br><br>
 * Used to determine the button status.
 * <br>
 * When the button status is set,
 * the button will be disabled and the debounce timer will start.
 * <br>
 * The debounce timer interrupt handler will enable the button again.
 *
 * @see debounce_timer_interrupt_handler
 */
void button_interrupt_handler();

#pragma endregion Button

#pragma region Debounce timer

/**
 * Initializes the debounce timer.
 * <ul>
 *   <li>Sets timer 0 to clear timer on compare mode (CTC).</li>
 *   <li>Sets timer 0 compare register A to 0.002 milliseconds</li>
 *   <li>Starts the debounce timer.</li>
 * </ul>
 */
void init_debounce_timer();

/**
 * Starts the beounce timer.
 * <br><br>
 * Enables timer 0 compare register A and sets the prescaler to 256
 * (which also starts the timer).
 */
void start_debounce_timer();

/**
 * Stops the debounce timer.
 * <br><br>
 * Disables the timer 0 compare register A and stops the timer.
 */
void stop_debounce_timer();

/**
 * Handler for an timer compare interrupt vector.
 * <br><br>
 * Used to debounce the button.
 * <br>
 * Cycles the compare interrupt as many times as necessary
 * to pass the debounce time of 30 milliseconds.
 * <br>
 * When the time is passed,
 * the button will be enabled again and the timer will be stopped.
 */
void debounce_timer_interrupt_handler();

#pragma endregion Debounce timer

#pragma region Centibeat timer

/**
 * Initializes the centibeat timer.
 * <br><br>
 * Sets timer 1 to clear timer on compare mode (CTC).
 * Sets timer 1 compare register A to 1 centibeat.
 */
void init_centibeat_timer();

/**
 * Starts the centibeat timer.
 * <br><br>
 * Enables the timer 1 compare A interrupt
 * and starts the timer 1 with 1024 prescaler.
 */
void start_centibeat_timer();

/**
 * Stops the centibeat timer.
 * <br><br>
 * Disables timer 1 compare A interrupt and stops the timer.
 */
void stop_centibeat_timer();

/**
 * Handler for timer compare vector.
 * <br><br>
 * Used to count centibeats by incrementing it every interrupt.
 */
void centibeat_timer_interrupt_handler();

#pragma endregion Centibeat timer

#pragma region Display

static uint8_t display_twi_address = 0x38;

/**
 * Port expander values that represent 7-segment display (common anode)
 * hexadecimal characters [0-9a-f].
 * <br><br>
 * <img width="500" src="https://lastminuteengineers.b-cdn.net/wp-content/uploads/arduino/7-Segment-Display-Number-Formation-Segment-Contol.png">
 * <br><br>
 * <img width="500" src="https://microcontrollerslab.com/wp-content/uploads/2020/03/7-segment-display-pic-microcontroller-tutorial.jpg">
 * <table>
  <tr>
    <th>Port expander pin</th>
    <th>Display pin</th>
  </tr>
  <tr><td>P0</td><td>7/A</td></tr>
  <tr><td>P1</td><td>6/B</td></tr>
  <tr><td>P2</td><td>4/C</td></tr>
  <tr><td>P3</td><td>2/D</td></tr>
  <tr><td>P4</td><td>1/E</td></tr>
  <tr><td>P5</td><td>9/F</td></tr>
  <tr><td>P6</td><td>10/G</td></tr>
</table>
 *
 */
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

/**
 * A converter array to turn numeric values into display characters.
 *
 * @note The compiler optimizes so the extra array doesn't cause overhead.
 */
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

display_char value_to_display_char(uint8_t value);

/**
 * Initializes the display.
 *
 * Begins the Wire library, if it hasn't already and clears the display.
 */
void init_display();

/**
 * Displays the pin values of the port expander on the display.
 *
 * @param value Port expander data to send (see the table from display_char).
 * @see display_enum
 */
void render_display(uint8_t value);

/**
 * Displays the <code>display_char</code>.
 *
 * @param character <code>display_enum</code> to display (see display_enum).
 * @see display_enum
 */
void render_display(display_char character);

/**
 * Renders <code>displau_char::num_0</code> on the display.
 */
void clear_display();

#pragma endregion Display

#pragma region Centibeat

/**
 * Used to track how many centibeats have passed.
 */
volatile static uint8_t centibeat{0};

/**
 * Increments the centibeat.
 * <br><br>
 * Masks it with 4 bits to avoid overflow.
 */
void increment_centibeat();

/**
 * Gets the centibeat.
 *
 * @return Current centibeat.
 */
uint8_t get_centibeat();

/**
 * Sets the centibeat.
 */
void set_centibeat(uint8_t _centibeat);

/**
 * Sets the centibeat to 0.
 */
void reset_centibeat();

/**
 * Renders the centibeat.
 * <br><br>
 * @note Renders only when the centibeat has changed.
 */
void render_centibeat();

#pragma endregion Centibeat

void init() {
    sei();
    init_button();
    init_debounce_timer();
    init_centibeat_timer();
    init_display();
}

void superloop() {
    render_centibeat();
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
    // Set digital pin 2 (PD2/INT0) to input mode (is input by default)
//    DDRD &= ~(DDD2);
    PORTD |= (1 << PORTD2); // Set digital pin 2  (PD2/INT0) to pull-up mode
    // Disable external interrupt 0 (is disabled by default)
//    EIMSK &= ~(1 << INT0);
    EICRA |= (1 << ISC00); // Set interrupt 0 to trigger on logical change
}

void enable_button() {
    EIFR = (1 << INTF0); // Clear the interrupt 0 flag
    EIMSK |= (1 << INT0); // Enable interrupt 0
}

void disable_button() {
    EIMSK &= ~(1 << INT0); // Disable interrupt 0
}

bool button_is_high() {
    return PIND & (1 << PIND2); // Read digital pin 2 (PD2/INT0) value
}

bool get_button_status() {
    return button_status;
}

void set_button_status(bool status) {
    button_status = status;

    if (status) {
        reset_centibeat();
        start_centibeat_timer();
    } else
        stop_centibeat_timer();
}

void button_interrupt_handler() {
    if (button_is_high()) {
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
     * Equals to 0.002 seconds (0.2 milliseconds) on a 16 mhz processor
     * and timer set to 256 prescaling.
     * <br><br>
     * Because timer 0 counter is one byte,
     * the compare interrupt must be run multiple times.
     * Prescaling of 256 provides the least interrupt cycles
     * while maintaining accuracy.
     * <br><br>
     * 16 mhz * 0.03 seconds / 256 prescaling = 1875.
     * <br>
     * 1875 / 15 = 125
     * <br>
     * 215 - 1 (in CTC mode, counting starts form 0) = 124
     */
    static constexpr uint8_t debounce_compare_value = 124;

    TCCR0A |= (1 << WGM01); // Set timer 0 to clear timer on compare (CTC) mode
    // Disable the timer 0 overflow compare A interrupt (disabled by default)
//    TIMSK0 &= ~(1 << OCIE0A);
    // Set the timer 0 compare register A to 0.2 ms
    OCR0A = debounce_compare_value;
    start_debounce_timer();
}

void start_debounce_timer() {
    TIMSK0 |= (1 << OCIE0A); // Enable the timer 0 compare A interrupt
    TCNT0 = 0; // Reset timer 0
    TCCR0B |= (1 << CS02); // Start timer 0 with 256 prescaler
}

void stop_debounce_timer() {
    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer 0 compare A interrupt
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Stop timer 0
}

void debounce_timer_interrupt_handler() {
    /**
     * Equals to 0.03 seconds (30 milliseconds) on a 16 mhz processor
     * and timer set to 256 prescaling.
     * <br><br>
     * Because the compare value is set to 124 (see init_debounce_timer),
     * The compare interrupt must be cycled 15 times.
     * <br><br>
     * 16 mhz * 0.03 seconds / 256 prescaler = 1875
     * <br>
     * 1875 / 125 (duration of the compare) = 15 cycles
     *
     * @see init_debounce_timer
     */
    static constexpr uint8_t compare_interrupt_cycles = 15;
    static uint8_t count{0};

    count++;
    if (count < compare_interrupt_cycles)
        return;

    count = 0;

    enable_button();
    stop_debounce_timer();
}

#pragma endregion Debounce timer

#pragma region Centibeat timer

void init_centibeat_timer() {
    /**
     * Equals to 0.864 seconds (1 centibeat) on a 16 mhz processor
     * and timer set to 1024 prescaling.
     * <br><br>
     * Timer 1 compare value is 2 bytes (65565 max value).
     * A centibeat fits in one compare interrupt on 256 and 1024 prescaling.
     * <br><br>
     * 16 mhz * 0.864 seconds / 1024 = 13500
     * <br>Alternatively, 16 * 0.864 / 256 = 54000
     */
    static constexpr uint16_t centibeat_compare_value = 13499;

    TCCR1B |= (1 << WGM12);  // Set timer 1 to clear timer on compare (CTC) mode
    // Disable the timer 1 overflow compare A interrupt (disabled by default)
//    TIMSK1 &= ~(1 << OCIE1A);
    // Set the timer 1 compare register A to 0.864 seconds
    OCR1A = 13499;
}

void start_centibeat_timer() {
    TIMSK1 |= (1 << OCIE1A); // Enable the timer 1 compare A interrupt
    TCNT1 = 0; // Reset timer 1
    TCCR1B |= (1 << CS12) | (1 << CS10); // Start timer 1 with 1024 prescaler
}

void stop_centibeat_timer() {
    TIMSK1 &= ~(1 << OCIE1A); // Disable the timer 1 compare A interrupt
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Stop timer 1
}

void centibeat_timer_interrupt_handler() {
    increment_centibeat();
}

#pragma endregion Centibeat timer

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

display_char value_to_display_char(uint8_t value) {
    return display_chars[value];
}

#pragma endregion Display

#pragma region Centibeat

void increment_centibeat() {
    set_centibeat(get_centibeat() + 1);
}

uint8_t get_centibeat() {
    return centibeat;
}

void set_centibeat(uint8_t _centibeat) {
    centibeat = _centibeat;
    centibeat &= 0xF;
}

void reset_centibeat() {
    set_centibeat(0);
}

void render_centibeat() {
    static uint8_t previous_centibeat;

    if (previous_centibeat != get_centibeat()) {
        previous_centibeat = get_centibeat();
        render_display(value_to_display_char(get_centibeat()));
    }
}

#pragma endregion Centibeat

ISR(INT0_vect) {
    button_interrupt_handler();
}

ISR(TIMER0_COMPA_vect) {
    debounce_timer_interrupt_handler();
}

ISR(TIMER1_COMPA_vect) {
    centibeat_timer_interrupt_handler();
}
