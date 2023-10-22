#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <util/delay.h>

#define LED PD2
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON PD3
#define BUTTON_DDR DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND

uint8_t circular_buffer[126];


void set_buffer(uint16_t ind)
{
    if(ind == 0) ind = 1001;
    else ind--;

    uint16_t ind_tab = ind >> 3;
    uint16_t ind_bite = 7 - (ind & 7);

    circular_buffer[ind_tab] |= (1 << ind_bite);
}

void light(uint16_t ind)
{
    uint16_t ind_tab = ind >> 3;
    uint16_t ind_bite = 7 - (ind & 7);

    if(bit_is_set(circular_buffer[ind_tab], ind_bite))
    {
        LED_PORT |= _BV(LED);
        circular_buffer[ind_tab] &= ~(1 << ind_bite);
    } else 
    {
        LED_PORT &= ~_BV(LED);
    }
}


int main()
{

    BUTTON_DDR &= ~ _BV(BUTTON);
    LED_DDR |= _BV(LED);

    BUTTON_PORT |= _BV(BUTTON);


    uint16_t time = 0;


    while(1) 
    {
        light(time);

        if(bit_is_clear(BUTTON_PIN, BUTTON))
        {
            set_buffer(time);
        }

        _delay_ms(1);

        if(time++ == 1001) time = 0;
        // printf("%s\n", buffer);
    }
}

