#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>


#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON_RESET PC3
#define BUTTON_NEXT PC2
#define BUTTON_PREV PC1

#define BUTTON_DDR DDRC 
#define BUTTON_PIN PINC 
#define BUTTON_PORT PORTC 

#define DEBOUNCE_TIME 800



void show_led(int ind) 
{
    LED_PORT = (ind ^ (ind >> 1)) << 4;
}

uint8_t debounce(uint8_t button)
{
    if(bit_is_clear(BUTTON_PIN, button))
    {
        _delay_us(DEBOUNCE_TIME);
        if(bit_is_clear(BUTTON_PIN, button))
        {
            return 1;
        }
    }
    return 0;
}

int main() {

    BUTTON_DDR &= ~(_BV(BUTTON_RESET) | _BV(BUTTON_NEXT) | _BV(BUTTON_PREV));
    BUTTON_PORT |= (_BV(BUTTON_RESET) | _BV(BUTTON_NEXT) | _BV(BUTTON_PREV));


    LED_DDR = -1;

    uint8_t button_reset_pressed = 0;
    uint8_t button_next_pressed = 0;
    uint8_t button_prev_pressed = 0;

    int8_t ind = 0;

    while (1) {

        if(debounce(BUTTON_RESET))
        {
            if(button_reset_pressed == 0)
            {
                ind = 0;
                button_reset_pressed = 1;
            }
        } else 
        {
            button_reset_pressed = 0;
        }

        if(debounce(BUTTON_NEXT))
        {
            if(button_next_pressed == 0)
            {
                ind ++;
                if(ind == 16) ind = 0;
                button_next_pressed = 1;
            }
        } else 
        {
            button_next_pressed = 0;
        }

        if(debounce(BUTTON_PREV))
        {
            if(button_prev_pressed == 0)
            {
                ind --;
                if(ind == -1) ind = 15;
                button_prev_pressed = 1;
            }
        } else 
        {
            button_prev_pressed = 0;
        }

        show_led(ind);


        
    }
}
