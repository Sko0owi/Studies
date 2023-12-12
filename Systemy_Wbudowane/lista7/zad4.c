#define __AVR_ATmega328P__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c.h"

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem


#define SS PD4
#define MOSI PD5
#define MISO PD6
#define SCK PD7

// inicjalizacja UART
void uart_init()
{
    // ustaw baudrate
    UBRR0 = UBRR_VALUE;
    // wyczyść rejestr UCSR0A
    UCSR0A = 0;
    // włącz odbiornik i nadajnik
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    // ustaw format 8n1
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
    // czekaj aż transmiter gotowy
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = data;
    return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    // czekaj aż znak dostępny
    while (!(UCSR0A & _BV(RXC0)))
        ;
    return UDR0;
}

FILE uart_file;

void setup_spi(){
    SPCR |= _BV(SPIE) | _BV(SPE);

    SPSR |= _BV(SPIF);
}

ISR(SPI_STC_vect){
    uint8_t value = SPDR;

    printf("Received\n\r");
    for(int i = 0; i < 8; i++)
    {
        if(value & (1 << i)) putchar('1');
        else putchar('0');
    }
    printf("\n\r");
}


void send_byte(uint8_t byte){

    PORTD &= ~_BV(SS);

    for(int i = 0; i<8; i++){

        if(byte & (1 << i))
            PORTD |= _BV(MOSI); 
        else
            PORTD &= ~_BV(MOSI); 

        PORTD |= _BV(SCK);

        PORTD &= ~_BV(SCK);
    }

    PORTD |= _BV(SS);

}

int main()
{
    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    DDRD |= _BV(SCK) | _BV(MOSI) | _BV(SS);
    PORTD |= _BV(SS);

    sei();
    setup_spi();

    uint8_t counter = 0;

    while(1){
        send_byte(counter++);

        _delay_ms(1000);
    }
}