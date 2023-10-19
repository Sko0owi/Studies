#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

#define fst_digit PC0
#define snd_digit PC1
#define digits_DDR DDRC
#define digits_PORT PORTC

#define A (1 << 0)
#define B (1 << 1)
#define C (1 << 2)
#define D (1 << 3)
#define E (1 << 4)
#define F (1 << 5)
#define G (1 << 6)
#define DP (1 << 7)


uint8_t numbers[] = 
{
    (G), // 0
    (A | D | E | F | G), // 1
    (C | F), // 2
    (E | F), // 3
    (A | D | E), // 4
    (B | E), // 5
    (B), // 6
    (D | E | G), // 7
    (0), // 8
    (E) // 9
};

uint16_t time = 0;

void show_digits(uint8_t ind)
{
    uint8_t first_digit = ind / 10;
    uint8_t second_digit = ind - 10*first_digit;

    
    LED_PORT = numbers[first_digit];
    digits_PORT = ~_BV(fst_digit);

    _delay_ms(10);
    time += 10;

    LED_PORT = numbers[second_digit];
    digits_PORT = ~_BV(snd_digit);

    _delay_ms(10);
    time += 10;

}

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

    digits_DDR |= _BV(fst_digit) | _BV(snd_digit);

    uint8_t ind = 0;
    LED_DDR = 0xff;
    
    while (1) {

        show_digits(ind);
        
        if(time == 1000)
        {
            ind ++;
            if(ind == 60) ind = 0;  

            time = 0;  
        }
            
    }
}
