#include "HardwareSerial.h"
#include <util/delay.h>
#include <util/atomic.h>

#define UPPERCASE_TO_LOWERCASE_DIFFERENCE 0x20

char rot13(char chr) {
    bool capital = chr < 'a';

    chr -= 13;

    if (chr < ('a' - (UPPERCASE_TO_LOWERCASE_DIFFERENCE * capital)))
        chr += 26;

    return chr;
}

int main() {
    sei();
    Serial.begin(9600);

    while (true) {
        char *messageBuffer = new char[32];

        while (!Serial.available());

        uint8_t charactersRead = Serial.readBytesUntil('\n', messageBuffer, 31);
        messageBuffer[charactersRead] = '\0';

        for (uint8_t i = 0; i < charactersRead; i++)
            Serial.print(rot13(messageBuffer[i]));

        Serial.println();
        Serial.flush();
    }
}
