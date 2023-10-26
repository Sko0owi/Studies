#include <avr/io.h>
#include <util/delay.h>

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


//inicjalizowanie ADC
void adc_init(){
    ADMUX = _BV(REFS0);
    ADMUX |= 0b1110;
    //DIDR0 = _BV(ADC0D);
    ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
    ADCSRA |= _BV(ADEN);
}

int main(){
    // UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
    LED_DDR  |= _BV(LED);
    // LED_PORT |= _BV(LED);
    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    adc_init();
    while(1){
      // LED_PORT &= ~_BV(LED);
      ADCSRA |= _BV(ADSC);
      // LED_PORT |= _BV(LED);
    //   _delay_ms(100);
      while(!(ADCSRA & _BV(ADIF)));

      
      ADCSRA |= _BV(ADIF);
      uint32_t v = 112640 / ADC ; // 1.1 * 1024 = 1126.4
      // uint16_t v_2 = 1126 % ADC;
      printf("Odczytano: %"PRIu32"\r\n",v);
      }
}