#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#define CS 2   // PB2 (CS/LOAD)

uint8_t p1_points, p2_points;

uint8_t random_value;

bool game_state = false;

bool interrupt_flag = false;

ISR(PCINT2_vect) {

    interrupt_flag = true;

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

    execute(0x09, 0b00111111); // Aktiverar decode för DIG0 - DIG5, ingen decode på DIG6-7
    execute(0x0B, 0x07); // hur många används
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
uint8_t get_random_decimal() {
    uint16_t adc_value = ADC_read(); // Läs ADC (0 - 1023)
    
    int last_digit = (adc_value % 4) + 1; // Mappa till 1–4

    return last_digit;
}


void show_random_number (){


    switch (random_value)
    {
        case 1:
            execute(0x06, 0b10000000);
            break;
        case 2:
            execute(0x06, 0b01100000);
            break;
        case 3:
            execute(0x06, 0b10110000);
            break;
        case 4:
            execute(0x06, 0b11110000);
            break; 
    // Vänta på interrupt innan spelet fortsätter
    interrupt_flag = false; // Återställ flaggan
    while (!interrupt_flag); // Vänta på att en interrupt händer
    
}
void reset_game() {
    p1_points = 0;
    p2_points = 0;
    game_state = false;

    //fix this function so it does not restart directly
    blink_leds();
    _delay_ms(3000);
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

        //show p1 and p2
        execute(0x01
        execute(0x02
        execute(0x03
        execute(0x04
        
        //bestäm hur långt spelet ska vara


        // Check if pin 0 on PORTB is on
        while (!(PINB & (1 << PB0) || game_state == true))
        {
            game_state = true;

            if (PINB & (1 << PB0))
            {
                game_state = false;
                blink_leds();
                _delay_ms(3000);
            }
            random_value = get_random_decimal();
            show_random_number();
        }
    }
}