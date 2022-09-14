/*
 * small blink program
 */
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /**
     * 1. In disassembly staat:
     * 80:	25 9a       	sbi	0x04, 5	; 4
     *
     * 2. De instructie is dus 25 9a
     *
     * 3. Als we de AVR instruction set manual erbij pakken:
     * http://ww1.microchip.com/downloads/en/devicedoc/atmel-0856-avr-instruction-set-manual.pdf
     * Hoofdstuk 99. SBI – Set Bit in I/O Register
     *
     * Opcode: 1001 1010 AAAA Abbb
     * Omgedraait: AAAA Abbb 1001 1010 (1010 1001 is 9a in hex)
     * Dit zet de bbb bit in de AAAA A adres van de I/O Register.
     * Deze instructie werkt bij de laagste 32 adressen.
     * AAAA A = 5 bits, 2 ^ 5  = 32. Dus 5 bits repersenteert het adres van de I/O register.
     * bbb representeert welke bit naar 1 gezet moet worden.
     *
     * 4. Als we de datasheet erbij pakken:
     * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
     *
     * Hoofdstuk 30. Register Summary
     * DDRB staat op adres 0x04
     * Het bit dat wij willen zetten is 5 dus 0x05
     * Als we 0x04 omzetten naar AAAA A = 0010 0
     * Hier staat ook bij dat DDB5 bit nummer 5 is.
     *
     * Hoofdstuk 13.4.3 DDRB – The Port B Data Direction Register
     * DDB5 is is de 5e bit
     * Ook hier staat het adres erbij 0x04
     * Als we 0x05 omzetten naar bbb = 101
     * 0010 0 + 101 = 0010 0101 = 0x25
     *
     * Alles bij elkaar kom je inderdaad uit op instructie:
     * 9a 25 (Bij disassembly is het om een of andere reden omgedraait naar 25 9a)
     * Verder beschrijft de regel: sbi	0x04, 5	; 4
     * sbi is de instructie zoals die precies overeenkomt met opcode beschrijving in de instruction set manual
     * 0x04 is het adres in 32-bit
     * 5 is welke bit
     * ; is een comment of een schijdingsteken voor leesbaarheid
     * 4 is nog even de 0x04 omgezet naar decimaal voor gemak
     *
     * Als het goed is de hele regel nu verklaard.
     */
    DDRB |= (1<<DDB5);		// pin 13 output

    while(1)			// endless loop
    {
        /**
         * Zelfde redenatie als bij de vorige comment maar dan in het kort:
         *
         * 82:	90 e2       	ldi	r25, 0x20	; 32
         * 84:	85 b1       	in	r24, 0x05	; 5
         * 86:	89 27       	eor	r24, r25
         * 88:	85 b9       	out	0x05, r24	; 5
         *
         * 82:	90 e2       	ldi	r25, 0x20	; 32
         * Dit zet gewoon de 5 bit aan en de rest uit in r25.
         * LDI = 73. LDI – Load Immediate
         * Opcode: 1110 KKKK dddd KKKK (k = 0-255, d = 0-15)
         * 1110 = 0x0e
         * r25 = 1001 = 0x09
         * 0x20 = 0010 0000 = 0x02 0x00
         * Instructie = 0x0e 0x02 0x09 0x00 = e2 90
         * Opvallend hier is dat de constante hier in twee delen uit elkaar staan
         *
         * 84:	85 b1       	in	r24, 0x05	; 5
         * Dit leest de register van de pin 13 en zet die in r24.
         * IN = 64. IN - Load an I/O Location to Register
         * Opcode: 1011 0AAd dddd AAAA (A = 0-63, d = 0-31)
         * r24 = 0001 1000 = xxxx xxx1 1000 xxxx
         * 5   = 0000 0101 = xxxx x00x xxxx 0101
         * Instructie = 1011 0001 1000 0101 = b1 85
         * Waarom zijn de constanten zoals PORTB5 niet gelijk de bit varianten zoals 0x00010000
         * Ik weet al dat het een 5 is... het zit in de naam PORTB5...? Alle constanten hebben dit.
         * Dan zijn dingen zoals 1<<PORTB5 niet meer nodig.
         *
         * 86:	89 27       	eor	r24, r25
         * Dit is een exclusive or (XOR) operatie.
         * Opcode = 0010 01rd dddd rrrr (d xor r)
         * 24 = 1 1000
         * 25 = 1 1001
         * 0010 0111 1000 1001 = 27 89
         * in r24 zit xx1x xxxx of xx0x xxxx
         * in r25 zit 0010 0000
         * Aan de hand van r24 wordt die weer aan of uit gezet door xor, hier ga ik verder niet op in.
         *
         * 85 b9       	out	0x05, r24	; 5
         * Dit zet de pin 13 weer uit of aan aan de hand van de uitkomst van r24 door de xor operatie.
         * OUT = 88. OUT – Store Register to I/O Location
         * Opcode: 1011 1AAr rrrr AAAA
         * 24 = 1 1000
         * 5 = 00 0101
         * 1011 1001 1000 0101 = b9 85
         * Wat er in r24 zit wordt naar pin 5 gestuurd.
         */
        PORTB ^= (1<<PORTB5);	// toggle pin
        _delay_ms(1000);		// wait a while
    }
}

/*
 1-6  Verken de datasheet van de ATmega328

 Hoeveel machineinstructies kent de AVR ATMega328?
 124 op basis van de instruction set manual

 Welke features ondersteunt de ATMega328?
 Ligt aan de definitie van features
 Kijkend naar de peripheral features op de voorpagina:
 - 2 8-bit timer
 - 1 16-bit timer
 - real time counter
 - 6 pwm channels
 - 10-bit ADC
 - USART
 - SPI
 - 2-wire serial interface
 - watchdog timer
 - analog comparator
 - interrupt and wake-up on pin change

 Binnen welke temperatuur range werkt de ATMega328?
 -40 tot 125 degrees celsius

 Welke alternatieve functies heeft pin PB4?
 MISO (SPI bus master output/slave input)
 PCINT4 (pin change interrupt 4)

 Wat betekent bit 2 (N) in het Status register?
 Dat is de negative flag. Dat geeft aan dat het resultaat in een arithmische operatie negatief is.

 Wat betekent bit 3 (EESAVE) in het high fuse byte?
 Als het 1 is, dan wordt de EEPROM geheugen niet gehouden na een chip erase (standaard is de waarde 1).

 Op welk geheugenadres bevindt zich het PORTC register in het data geheugen?
 0x28 (0x08 relatief)

 Op welk geheugenadres bevindt zich de INT1 interrupt in het programmageheugen?
 0x3D (bit 1)

 Wat doet 'PUD' in figuur 18.2 general Digital I/O?
 Het woord PUD komt niet voor in figure 18-2 Counter Unit Block Diagram.
 PUD staat voor Pull Up Disable. Dit zou de pull-up functie uitschakelen van iets.

 In welk register staat dit bit op welke plek?
 MCUCR register op adres 0x35 (0x55 absoluut) bit nummer 4

 */
