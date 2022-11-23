#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <HardwareSerial.h>

//#define SEND_DEBUG
#define SEND_PAUSE 1

#define RECEIVE_DEBUG

// 2.245 ms hoog start
// 1.124 ms laag start

volatile bool timer0_compa_flag{false};
volatile bool int0_flag{false};

volatile uint8_t send_mock{0b10101010};
volatile uint8_t send_pos{0};
volatile uint8_t send_current_bit{0};

volatile uint8_t receive_mock{0};
volatile uint8_t receive_pos{0};
volatile uint8_t receive_current_bit{0};

volatile uint8_t send_pause{0};

bool getbit(uint8_t byte, uint8_t pos) {
    return (byte >> pos) & 0x1;
}

ISR(TIMER0_COMPA_vect) {
        timer0_compa_flag = true;
}

ISR(TIMER0_COMPB_vect) {
        PORTB &= ~(1 << PORTB5);
}

ISR(INT0_vect) {
        int0_flag = true;
}

void init_pins() {
    DDRB |= (1 << DDB5);

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01) | (1 << ISC00);
    PORTD |= (1 << PORTD2);

    sei();
}

int main() {
    init_pins();

    DDRB |= (1 << DDB5);

    OCR1A = 0;
    TCNT1 = 0;
    TCCR1B |= (1 << CS10);

    TCCR0A |= (1 << WGM01);
    OCR0A = 255;
    OCR0B = 24;
    TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);
    TCNT0 = 0;
    TCCR0B |= (1 << CS02) | (1 << CS00);

    Serial.begin(9800);
    Serial.println("start");
    Serial.flush();
    // 10110010
    // start 01001101
    int counter{0};

    uint8_t count{0};

    uint16_t times[10]{};
    bool values[10]{};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
//        if (timer0_compa_flag) {
//            timer0_compa_flag = false;
//            Serial.println(1);
//            Serial.flush();
//        }

//        if (timer0_compa_flag) {
//            timer0_compa_flag = false;
//
//            if (!send_pause) {
//#ifdef SEND_DEBUG
//                Serial.print(send_current_bit);
//                Serial.print(' ');
//                Serial.print(send_pos);
//                Serial.print(' ');
//#endif
//                PORTB |= (1 << PORTB5);
//
//                if (send_current_bit >= 1 && send_current_bit <= 8) {
//                    if (getbit(send_mock, send_pos)) {
//                        OCR0A = 192;
//                    } else
//                        OCR0A = 48;
//
//                    if (send_current_bit >= 1 && send_current_bit <= 8) {
//                        send_pos++;
//                        send_pos &= 0b111;
//                    }
//                } else {
//                    OCR0A = 192;
//                }
//
//#ifdef SEND_DEBUG
//                Serial.print(OCR0A);
//                Serial.println();
//                Serial.flush();
//#endif
//                send_current_bit++;
//                send_pause--;
//
//                if (send_current_bit == 10) {
//                    send_pause = 4;
//                    send_current_bit = 0;
//                }
//            } else {
//                send_pause++;
//            }
//        }

        uint8_t data{0};
        uint8_t pos{0};

        if (int0_flag) {
            int0_flag = false;

#ifdef RECEIVE_DEBUG
//            Serial.print(receive_current_bit, DEC);
//            Serial.print(' ');
//            Serial.print(receive_pos); //TCNT1
//            Serial.print(' ');
//            uint16_t timer1 = TCNT1;
//
//            if (timer1 > 15200 && timer1 < 16000) {
//                start = true;
//                pos = 0;
//            }
//
//            if (start) {
//                if (timer1 > 6720 && timer1 < 9600) {
//                    data &= 1;
//                    data <<= 1;
//                } else if (timer1 < 6720 && timer1 > 4000) {
//                    data <<= 1;
//                } else {
//                    start = false;
//                    pos = 0;
//                }
//                pos++;
//
//                if (pos == 7) {
//                    Serial.println(data, BIN);
//                    Serial.flush();
//                    start = false;
//                }
//            }

            values[count] = PIND & (1 << PIND2);
            times[count] = TCNT1;

            if (count == 1) {
                uint16_t t = times[1];
                if (t < 14900 || t > 16000)
                    count = 0;
            }

            count++;

            if (count == 10) {
                data = 0;
                count = 0;

                if (times[1] > 14900 && times[1] < 16000)
                    for (uint8_t i = 2; i < 10; i++) {
                        uint16_t t = times[i];

                        if (t >= 6720) {
                            if (t < 9600) {
                                data |= 1;
                            }
                        } else if (t < 3900) {
                            Serial.println("e1");
                            Serial.println(t);
                            Serial.println("ee");
                            data = 0;
                            break;
                        }

//                    Serial.println(t);
//                    if (t >= 6720)
//                        data |= 1;

                        if (i < 9)
                            data <<= 1;

//                    if (t > 30000) Serial.print(1);
//                    else Serial.print(0);
//                    Serial.print(values[i]);
//                    Serial.print(' ');
//                    Serial.print(' ');
//                    Serial.print(times[i] / 16);
//                    Serial.print("μs");

//                    Serial.print(times[i]);
//                    Serial.println();
                    }
                else {
                    Serial.println("e2");
                    Serial.println(times[1]);
                    Serial.println(times[2]);
                    Serial.println("ee");
                    data = 0;
                    count = 0;
                }

                Serial.println(data, BIN);
                Serial.flush();
            }

#endif
            TCNT1 = 0;

//            if (receive_current_bit >= 1 && receive_current_bit <= 8) {
//                receive_pos++;
//                receive_pos &= 0b111;
//            }



//            receive_current_bit++;
//
//            if (receive_current_bit == 10) {
//                receive_current_bit = 0;
//            }
        }
    }
#pragma clang diagnostic pop
}

#pragma clang diagnostic pop
