#include <avr/io.h>
#include <avr/interrupt.h>

volatile char toggle_timer;

ISR(TIMER0_OVF_vect)
{
    toggle_timer = !toggle_timer;
}

// Een delay-functie.

void my_delay(int sec)
{
    int teller = sec * 61;
    char old_toggle = toggle_timer;
    while(teller > 0)
        if(toggle_timer != old_toggle)
        {
            old_toggle = !old_toggle;
            teller--;
        }
}

int main(void)
{
    // init my_delay()
    TCCR0B |= (1 << CS02) | (1<<CS00);
    TIMSK0 |= (1<<TOIE0);
    sei();
    // blink
    DDRB |= (1<<DDB5);
    while(1)
    {
        PORTB |= (1<<PORTB5);
        my_delay(1);
        PORTB &= ~(1<<PORTB5);
        my_delay(1);
    }
}
