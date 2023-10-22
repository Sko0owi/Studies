#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

uint8_t numbers[] = 
{
    191, 134, 219, 207, 230, 237, 253, 167, 255, 239
};

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

    int8_t ind = 0;
    LED_DDR = -1;
    
    while (1) {

        if (ind == 10) ind = 0;

        LED_PORT = ~numbers[ind++];

        _delay_ms(1000);
        
    }
}
