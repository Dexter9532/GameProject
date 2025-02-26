#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#define CS 2   // PB2 (CS/LOAD)

void execute(uint8_t cmd, uint8_t data)
{
    PORTB &= ~(1 << CS); // CS LOW (starta överföring)

    SPDR = cmd;  // Skicka kommando
    while (!(SPSR & (1 << SPIF))); // Vänta på överföring klar

    SPDR = data; // Skicka data
    while (!(SPSR & (1 << SPIF))); // Vänta på överföring klar

    PORTB |= (1 << CS); // CS HIGH (slut på överföring)
}

int main(void)
{
<<<<<<< HEAD
    EICRA |= (1 << ISC21);  // Fallande flank (ISC21 = 1, ISC20 = 0)
    EIMSK |= (1 << INT2);   // Aktivera INT2
    

    
    EICRA |= (1 << ISC01);  // Fallande flank (ISC01 = 1, ISC00 = 0)
    EIMSK |= (1 << INT0);   // Aktivera INT0
    
    EICRA |= (1 << ISC11);  // ISC11 = 1, ISC10 = 0, fallande flank
    EIMSK |= (1 << INT1);
    
    sei();
    
    DDRD = 255;
=======
>>>>>>> origin/main
    // Ställ in SPI-pinnar som output (MOSI, SCK, CS)
    DDRB = (1 << PB3) | (1 << PB5) | (1 << CS);
    PORTB |= (1 << CS); // CS börjar HIGH (inaktiv)

    // SPI inställningar: Enable SPI, Master mode, SCK = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR); // SPI på, master, stigande flank

<<<<<<< HEAD
    // 🔧 Initiera MAX7221 korrekt
    execute(0x09, 0xFF); // Decode 
    execute(0x0B, 0x07); // Endast DIG 0 aktiv (ändra till 0x07 om fler används)
    execute(0x0A, 0x05); // ljusstyrka
    execute(0x0C, 0x01); // Slå på MAX7221
=======
    // 🔧 Initiera MAX7219 korrekt för 4 displayer
    execute(0x09, 0xff); // Decode Mode för alla digits (0-3)
    execute(0x0B, 0x03); // Endast 4 digits aktiva (DIG 0-3)
    execute(0x0A, 0x05); // Ljusstyrka (0-15)
    execute(0x0C, 0x01); // Slå på MAX7219
>>>>>>> origin/main
    execute(0x0F, 0x00); // Stäng av testläge

    while (1)
    {
<<<<<<< HEAD
=======
        execute(0x01, 2); // Visa "8" på digit 1
        execute(0x02, 3); // Visa "8" på digit 2
        execute(0x03, 8); // Visa "8" på digit 3
        execute(0x04, 5); // Visa "8" på digit 4
        _delay_ms(500);
>>>>>>> origin/main
    }
}
