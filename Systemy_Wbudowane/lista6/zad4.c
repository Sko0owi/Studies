#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_MOSI PB3
#define SPI_SCK PB5
#define SPI_LA PB1
#define SPI_OE PB2

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

void timer_init() {
  TCCR2A = _BV(WGM22);                        
  TCCR2B = _BV(CS22) | _BV(CS20) | _BV(CS21); 
  OCR2A = 195;                                
  TIMSK2 |= _BV(OCIE2A);
}

volatile int8_t time = 0, ind = 0;

ISR(TIMER2_COMPA_vect) {
  if (++time >= 100) {
    time = 0;
    if(++ind == 10) ind = 0;
  }
}


void spi_init()
{
    SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_LA) | _BV(SPI_OE);
    SPI_PORT &= ~_BV(SPI_OE);

    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
}

void load_digit(uint8_t data)
{
    SPI_PORT |= _BV(SPI_LA);

    SPDR = data;
    while (!(SPSR & _BV(SPIF)));
    SPSR |= _BV(SPIF);

    SPI_PORT &= ~_BV(SPI_LA);
}


int main() {
    timer_init();
    spi_init();
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    load_digit(~numbers[ind]);

    while (1) {
        load_digit(~numbers[ind]);
        sleep_mode();

    }
}
