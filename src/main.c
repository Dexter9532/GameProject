#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#define CS 2   // PB2 (CS/LOAD)

uint8_t p1_points, p2_points;

uint8_t random_value;

ISR(PCINT2_vect) {


}
void execute(uint8_t cmd, uint8_t data)
{
    PORTB &= ~(1 << CS); // CS LOW (starta överföring)

    SPDR = cmd;  // Skicka kommando
    while (!(SPSR & (1 << SPIF))); // Vänta på överföring klar

    SPDR = data; // Skicka data
    while (!(SPSR & (1 << SPIF))); // Vänta på överföring klar

    PORTB |= (1 << CS); // CS HIGH (slut på överföring)
}
void setup_interrupts() {
    cli();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= 0xFF;
    sei();
}

void spi_init(){
     
    PORTB |= (1 << CS); // CS börjar HIGH (inaktiv)
    // SPI inställningar: Enable SPI, Master mode, SCK = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR); // SPI på, master, stigande flank

    execute(0x09, 0xFF); // Decode 
    execute(0x0B, 0x07); // Endast DIG 0 aktiv (ändra till 0x07 om fler används)
    execute(0x0A, 0x05); // ljusstyrka
    execute(0x0C, 0x01); // Slå på MAX7221
}
void ADC_init() {
    ADMUX = (1 << REFS0); // Välj referensspänning AVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
    // Aktivera ADC och sätt prescaler till 128 (16MHz/128 = 125kHz ADC-klocka)
}
uint16_t ADC_read() {
    ADCSRA |= (1 << ADSC); // Starta ADC-konvertering
    while (ADCSRA & (1 << ADSC)); // Vänta tills konvertering är klar
    return ADC; // Returnera 10-bitars ADC-värde (0–1023)
}
float ADC_getVoltage() {
    uint16_t adc_value = ADC_read();
    float voltage = (adc_value * 5.0) / 1023.0; // Beräkna spänning i volt
    return voltage;
}
uint16_t get_random_decimal() {
    uint16_t adc_value = ADC_read(); // Läs ADC (0 - 1023)
    
    int last_digit = (adc_value % 4) + 1; // Mappa till 1–4

    return last_digit;
}


void game_starting (){

}
void reset_game() {
    p1_points = 0;
    p2_points = 0;
}
int main(void)
{
    DDRD = 0;
    PORTD = 255;
    DDRB = (1 << PB3) | (1 << PB5) | (1 << CS);

    spi_init();
    ADC_init();
    setup_interrupts();

    while (1)
    {
        reset_game();

        // Check if pin 0 on PORTB is on
        while (!(PINB & (1 << PB0)))
        {
            
            execute(0x01, 0x01);
            execute(0x02, 0x02);
            execute(0x03, 0x03);
            execute(0x04, 0x04);
        }
    }
}