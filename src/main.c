#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>

#define CS 2   // PB2 (CS/LOAD)

uint8_t digit = 0;
uint8_t digit2 = 0;
uint8_t digit3 = 0;
uint8_t digit4 = 0;
uint8_t digit5 = 0;
uint8_t digit6 = 0;
uint8_t digit7 = 0;
uint8_t digit8 = 0;

uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;
uint8_t counter4 = 0;
uint8_t counter5 = 0;
uint8_t counter6 = 0;
uint8_t counter7 = 0;
uint8_t counter8 = 0;


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
    execute(0x09, 0x01); // Decode 
    execute(0x0B, 0x07); // Endast DIG 0 aktiv (ändra till 0x07 om fler används)
    execute(0x0A, 0x05); // ljusstyrka
    execute(0x0C, 0x01); // Slå på MAX7221
    execute(0x0F, 0x00); // Stäng av testläge

    while (1)
    {


        counter1 ++;
        digit ++;

        if (digit == 10 ){
            counter2 ++;
            digit = 0;
            counter1 = 0;
            digit2 ++;
        }
        if (digit2 == 10)
        {
            counter3 ++;
            digit2 = 0;
            counter2 = 0;
            digit3 ++;
        }
        if (digit3 == 10)
        {
            counter4 ++;
            digit3 = 0;
            counter3 = 0;
            digit4 ++;
        }
        if (digit4 == 10)
        {
            counter5 ++;
            digit4 = 0;
            counter4 = 0;
            digit5 ++;
        }
        if (digit5 == 10)
        {
            counter6 ++;
            digit5 = 0;
            counter5 = 0;
            digit6 ++;
        }
        if (digit6 == 10)
        {
            counter7 ++;
            digit6 = 0;
            counter6 = 0;
            digit7 ++;
        }
        if (digit7 == 10)
        {
            counter8 ++;
            digit7 = 0;
            counter7 = 0;
            digit8 ++;
        }
        execute(0x01, counter1);
        execute(0x02, counter2);
        execute(0x03, counter3);
        execute(0x04, counter4);
        execute(0x05, counter5);
        execute(0x06, counter6);
        execute(0x07, counter7);
        execute(0x08, counter8);

    }
}
