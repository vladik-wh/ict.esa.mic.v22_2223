#include <avr/interrupt.h>
#include <avr/io.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Nunchuk.h>

#define DISPLAY_ADDRESS 0x38
#define NUNCHUK_ADDRESS 0x52

volatile bool timer1_flag{false};

int main() {
    sei();
    Serial.begin(9600);
    Serial.println("start");
    Serial.flush();

    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS02) | (1 << CS00);
    OCR1A = 2000;
    TCNT1 = 0;

    Wire.begin();
    Nunchuk.begin(NUNCHUK_ADDRESS);

    uint8_t display_value = ~0x1;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        if (!timer1_flag)
            continue;
        timer1_flag = false;

        Wire.beginTransmission(DISPLAY_ADDRESS);
        Wire.write(~display_value);
        Wire.endTransmission();

        display_value <<= 1;
        if (display_value > (0x1 << 5))
            display_value = 0x1;

        Nunchuk.getState(NUNCHUK_ADDRESS);
        OCR1A = static_cast<uint16_t>(2000 - Nunchuk.state.joy_x_axis / 255.0 * 1500);

    }
#pragma clang diagnostic pop
}

ISR(TIMER1_COMPA_vect) {
    timer1_flag = true;
}
