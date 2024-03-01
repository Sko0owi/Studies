#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);


    uint8_t direction = 0; // 1 -> left, 0 -> right
    uint8_t setup = -1;
    uint8_t currLaser = (1<<7) | (1<<6);
    // printf("%d\n", test);
    LED_DDR |= setup;
    LED_PORT = 0;
    
    while (1) {

        direction &= ~((currLaser & (1 << 7)) >> 7);
        direction |= (currLaser & 1);

        LED_PORT = currLaser;
        _delay_ms(50);

        if (direction)
        {
            currLaser <<= 1;
        } else 
        {
            currLaser >>= 1;
        }
        
    }
}
