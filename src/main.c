#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
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
    DDRD = 255;
    // Ställ in SPI-pinnar som output (MOSI, SCK, CS)
    DDRB = (1 << PB3) | (1 << PB5) | (1 << CS);
    PORTB |= (1 << CS); // CS börjar HIGH (inaktiv)

    // SPI inställningar: Enable SPI, Master mode, SCK = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    // 🔧 Initiera MAX7221 korrekt
    execute(0x09, 0x00); // Decode mode av
    execute(0x0B, 0x00); // Endast DIG 0 aktiv (ändra till 0x07 om fler används)
    execute(0x0A, 0x0F); // Max ljusstyrka
    execute(0x0C, 0x01); // Slå på MAX7221
    execute(0x0F, 0x00); // Stäng av testläge

    while (1)
    {
        execute(0x01, 0b00111111); // Visa "0" på DIG 0
        _delay_ms(500);
        PORTD = 255;
    }
}
