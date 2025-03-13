#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>

#define CS 2   // PB2 (CS/LOAD)
#define PB0 0
#define PB3 3
#define PB5 5


// Variabler

uint8_t p1_points, p2_points, random_value, buttons_pressed, wheel;
uint8_t right_bin1 = 0b10000000, right_bin2 = 0b01100000, right_bin3 = 0b10110000, right_bin4 = 0b11110000;

uint16_t wait;

bool game_state = false, game_win = false;
bool interrupt_flag = false;

// Funktioner

ISR(PCINT2_vect) {

    buttons_pressed = ~PIND; // Läs in knapptryckningar

    switch (buttons_pressed) {
        case 0b00000100: buttons_pressed = 3; break;
        case 0b00001000: buttons_pressed = 4; break;
        case 0b00010000: buttons_pressed = 5; break;
        case 0b00100000: buttons_pressed = 6; break;
        case 0b01000000: buttons_pressed = 7; break;
        case 0b10000000: buttons_pressed = 8; break;
    }
    // Kontrollera vilken spelare och om svaret är rätt
    switch (buttons_pressed) {
        case 1: case 2: case 3: case 4: // Spelare 1 knappar
<<<<<<< HEAD
        if (buttons_pressed == random_value) {
            p1_points++;
            points();
            } else if (p1_points > 0) { // Se till att poängen inte blir negativ
            p1_points--;
            points();
        }
        execute(0x00, p1_points);
        break;

        case 5: case 6: case 7: case 8: // Spelare 2 knappar
        if (buttons_pressed == (random_value + 4)) {
            p2_points++;
            points();
            } else if (p2_points > 0) {
            p2_points--;
            points();
        }
        execute(0x03, p2_points);
        break;
=======
            if (buttons_pressed == random_value) {
                p1_points++;
                points();
            } else if (p1_points > 0) { // Se till att poängen inte blir negativ
                p1_points--;
                points();
            }
            execute(0x00, p1_points);
            break;

        case 5: case 6: case 7: case 8: // Spelare 2 knappar
            if (buttons_pressed == (random_value + 4)) {
                p2_points++;
                points();
            } else if (p2_points > 0) {
                p2_points--;
                points();
            }
            execute(0x03, p2_points);
            break;
>>>>>>> origin/main
    }

    interrupt_flag = true; // Flagga för att spelet vet att en knapp trycktes
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
uint8_t get_random_decimal_extended() {
    uint16_t adc_value = ADC_read(); // Läs ADC (0 - 1023)
    
    uint8_t mapped_value = (adc_value % 11) + 3; // Mappa till 3–13

    return mapped_value;
}
void random_wait() {

    uint8_t random_wait_time = get_random_decimal_extended();
    random_wait_time = (random_wait_time * 1000); // Omvandla till millisekunder
    
    for (uint16_t i = 0; i < random_wait_time; i++) {
        _delay_ms(1);
    }
}
void show_random_number (){
    
    random_wait();

    switch (random_value)
    {
        case 1:
        execute(0x06, right_bin1);
        break;
        case 2:
        execute(0x06, right_bin2);
        break;
        case 3:
        execute(0x06, right_bin3);
        break;
        case 4:
        execute(0x06, right_bin4);
        break;
        // Vänta på interrupt innan spelet fortsätter
    }
    sei();
    interrupt_flag = false; // Återställ flaggan
    while (!interrupt_flag){
        wheel++;
        if (wheel > 20) {
            wheel = 0;
        }
    } // Vänta på att en interrupt händer
<<<<<<< HEAD
=======
}
void points (){
    
    if (p1_points == 10){
        execute(0x00,1);
        execute(0x01,0);
        game_win = true;
    }  
    else if (p2_points == 10){
        execute(0x02,1)
        execute(0x03,0)
        game_win = true;
    }
    if (game_win == false){
    execute(0x01,p1_points);
    execute(0x03,p2_points);
>>>>>>> origin/main
}
void points (){
    
    if (p1_points == 10){
        execute(0x00,1);
        execute(0x01,0);
        game_win = true;
    }
    else if (p2_points == 10){
        execute(0x02,1);
        execute(0x03,0);
        game_win = true;
    }
    if (game_win == false){
        execute(0x01,p1_points);
        execute(0x03,p2_points);
    }
}    
    void reset_game() {
        p1_points = 0;
        p2_points = 0;
        game_state = false;

<<<<<<< HEAD
        //fix this function so it does not restart directly
        _delay_ms(3000);
    }
    int main(void)
=======
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
>>>>>>> origin/main
    {
        DDRD = 0;
        PORTD = 255;
        DDRB = (1 << PB3) | (1 << PB5) | (1 << CS);

        spi_init();
        ADC_init();
        setup_interrupts();

        while (1)
        {
            cli();
            execute(0x09, 0b00111010);
            //show p1 and p2
            execute(0x00,0b11100100);
            execute(0x01,1);
            execute(0x02,0b11100100);
            execute(0x03,2);

<<<<<<< HEAD
            //bestäm hur långt spelet ska vara

            // Check if pin 0 on PORTB is on
            while (!(PINB & (1 << PB0) || game_state == true))
=======
        while (game_win == true)
        {
            uint8_t first = counter / 10;  // Tiotal
            uint8_t second = counter % 10;   // Ental
            execute(0x04, first); // Skicka tiotal
            execute(0x05, second); // Skicka ental
            _delay_ms(10000);
            game_win = false;
            reset_game();
        }
            if (PINB & (1 << PB0))
>>>>>>> origin/main
            {
                cli();
                execute(0x09, 0b00111111);
                game_state = true;

                while (game_win == true)
                {
                    uint8_t first = wheel / 10;  // Tiotal
                    uint8_t second = wheel % 10;   // Ental
                    execute(0x04, first); // Skicka tiotal
                    execute(0x05, second); // Skicka ental
                    _delay_ms(10000);
                    game_win = false;
                    reset_game();
                }
                if (PINB & (1 << PB0))
                {
                    game_state = false;
                    _delay_ms(3000);
                }
                random_value = get_random_decimal();
                show_random_number();
            }
        }
    }
