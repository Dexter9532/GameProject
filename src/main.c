#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define CS 2   // PB2 (CS/LOAD)

uint8_t p1_points, p2_points;

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
void ADC_init() {
    ADMUX = (1 << REFS0); // Välj referensspänning AVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
    // Aktivera ADC och sätt prescaler till 128 (16MHz/128 = 125kHz ADC-klocka)
}
void spi_init(){
     
    PORTB |= (1 << CS); // CS börjar HIGH (inaktiv)
    // SPI inställningar: Enable SPI, Master mode, SCK = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR); // SPI på, master, stigande flank


     execute(0x09, 0xff); // Decode Mode 
     execute(0x0B, 0x08); // Vilka digits används
     execute(0x0A, 0x05); // Ljusstyrka (0-15)
     execute(0x0C, 0x01); // Slå på MAX7219
     execute(0x0F, 0x00); // Stäng av testläge
 
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
        while (!PINB & (1 << PB0))
        {
            // Pin 0 on PORTB is on
            execute(0x01, 0x01);
            execute(0x02, 0x02);
            execute(0x03, 0x03);
            execute(0x04, 0x04);
        }
    }
}