#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>

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
    // Ställ in SPI-pinnar som output (MOSI, SCK, CS)
    DDRB = (1 << PB3) | (1 << PB5) | (1 << CS);
    PORTB |= (1 << CS); // CS börjar HIGH (inaktiv)

    // SPI inställningar: Enable SPI, Master mode, SCK = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR); // SPI på, master, stigande flank

    // 🔧 Initiera MAX7219 korrekt för 4 displayer
    execute(0x09, 0xff); // Decode Mode för alla digits (0-3)
    execute(0x0B, 0x03); // Endast 4 digits aktiva (DIG 0-3)
    execute(0x0A, 0x05); // Ljusstyrka (0-15)
    execute(0x0C, 0x01); // Slå på MAX7219
    execute(0x0F, 0x00); // Stäng av testläge

    while (1)
    {
        execute(0x01, 2); // Visa "8" på digit 1
        execute(0x02, 3); // Visa "8" på digit 2
        execute(0x03, 8); // Visa "8" på digit 3
        execute(0x04, 5); // Visa "8" på digit 4
        _delay_ms(500);
    }
}
