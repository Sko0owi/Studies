#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600   //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem

#define LED PD6 //Dioda
#define LED_DDR DDRD    //dioda ddr
#define LED_PORT PORTD  //dioda port 

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
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

void clock_init()
{
    TCCR1A |= _BV(WGM12) | _BV(WGM13);
    TCCR1B |= _BV(CS11);
    TIMSK1 |= _BV(ICIE1);
}


volatile uint16_t prev = 0, val = 0, current = 0;

volatile uint8_t changeEdge = 0;
ISR(TIMER1_CAPT_vect) {

    changeEdge ^= 1;
    prev = current;
    current = ICR1;

    if(changeEdge)
    {
        val = (current - prev);
        val = (F_CPU >> 3) / val;
        printf("KURRENT_VAL: %"PRIu16"\r\n", val);        
    }
            
}

int main(){
    UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    clock_init();
    sei();

    set_sleep_mode(SLEEP_MODE_IDLE);

    while(1)
    {
        sleep_mode();
    }
}