#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BUZZ PD5
#define BUZZ_DDR DDRD
#define BUZZ_PORT PORTD

void my_delay_us(uint16_t value){
    while(value){
        _delay_us(1);
        value--;
    }
}

static inline void play(uint16_t step, uint16_t delay){
    // uint16_t step2 = step;
    if(step == 0){
        BUZZ_PORT &= ~_BV(BUZZ);
        my_delay_us(2*delay);
        return;
    }
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++){
        BUZZ_PORT |= _BV(BUZZ);
        my_delay_us(step);
        BUZZ_PORT &= ~_BV(BUZZ);
        my_delay_us(step);
    }
}

// _delay_us()
#define A 20
#define B 23
#define C 26
#define D 29
#define E 33
#define F 35
#define G 39

#define P 0

#define f 255   
#define h 75
#define q 50
#define o 25

// #define e 125
static const uint8_t notes[]  PROGMEM = {
    C,D,E,C,C,D,E,C,E,F,G,E,F,G,G,A,G,F,E,C,G,A,G,F,E,C,D,G,C,P,D,G,C
};
static const uint8_t length[] PROGMEM = {
    q,q,q,q,q,q,q,q,q,q,h,q,q,h,o,o,o,o,q,q,o,o,o,o,q,q,q,q,h,f,q,q,h
};

int main(){
    BUZZ_DDR |= _BV(BUZZ);
    uint16_t value,len;
    while(1){
        for(int8_t i = 0; i < 32; i++){
            value = (uint16_t)pgm_read_byte(&notes[i]) * 10;
            len = (uint16_t)pgm_read_byte(&length[i]) * 10;
            play(value,len);
        }
        //return 0;
    }
}