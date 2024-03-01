#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600   //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem

#define LED PD4 //Dioda
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
    ADMUX |= _BV(MUX0);

    DIDR0 = _BV(ADC0D);

    ADCSRA = _BV(ADPS1) ;
    ADCSRA |= _BV(ADEN);
}



void my_delay_us(uint16_t value){
    while(value){
        _delay_us(1);
        value--;
    }
}


int main(){
    // UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

    LED_DDR |= _BV(LED);
    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    adc_init();

    float T0 = 298.15f;
    float R0 = 4700;
    float R2 = 10000;

    float Vcc = 5;
    float B = 3200;

    while(1){
      
        LED_PORT &= ~_BV(LED);

        ADCSRA |= _BV(ADSC);
        while(!(ADCSRA & _BV(ADIF)));
        ADCSRA |= _BV(ADIF);

        float vin = (ADC * Vcc) / 1024; 

        float R = R2 * ((Vcc - vin)/vin); 
        // printf("Odczytano: %"PRIu16" %"PRIu16" %"PRIu16"\r\n",ADC, my_log(ADC), my_exp(ADC/100)*1000);

        // printf("zmierzony woltarz hehe: %"PRIu32" %"PRIu32" \r\n ", vin, (ADC * Vcc)%1024);
        

        float T = (B / (log(R/R0) + (B/T0))) - 273.15;

        // printf("DEBUG: %"PRIu32" %"PRIu32" %"PRIu32" \r\n", (B / (T0 * (log(R) - 3))), R, log(R));

        printf("zmierzona temperatura: %fT0 \r\n ", T);

        _delay_ms(1000);
    //   printf("Odczytano: %"PRIu16"\r\n",v);
    //   migotanie(v);
    }
}