#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600   //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem

#define LED PD4 //Dioda
#define LED_DDR DDRD    //dioda ddr
#define LED_PORT PORTD  //dioda port 


#define MAX_RES 59000
#define INF_RES 100000

#define BASE_LED_FREQ 10000

#define min(x, y) (x < y ? x : y)

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

    DIDR0 = _BV(ADC0D);

    ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2)   ;
    ADCSRA |= _BV(ADEN);
}



void my_delay_us(uint16_t value){
    while(value){
        _delay_us(1);
        value--;
    }
}

void timer_init()
{
  ICR1 = 35624;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS00); // | _BV(CS10) | _BV(CS12);
  DDRB |= _BV(PB1);
}

void update_led(uint32_t resValue) {

    resValue = min(resValue, MAX_RES);

    uint32_t brightness = (resValue * BASE_LED_FREQ) / INF_RES;
    OCR1A =  brightness;

    printf("%d\r\n", OCR1A);
}



int main(){
    // UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    adc_init();
    timer_init();


    float R = 10000;

    float Vcc = 5;

    while(1)
    {
      
        LED_PORT &= ~_BV(LED);

        ADCSRA |= _BV(ADSC);
        while(!(ADCSRA & _BV(ADIF)));
        ADCSRA |= _BV(ADIF);

        float vin = (ADC * Vcc) / 1024; 

        float val = R * ((Vcc - vin)/vin); 
        
        update_led(val);
        // printf("zmierzone światło: %f \r\n ", val);        
    }
}