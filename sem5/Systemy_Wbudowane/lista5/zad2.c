#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600   //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem



#define BUTTON PD2
#define BUTTON_DDR DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND


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



static volatile float last_measurement = 0.0;
static volatile uint16_t current_time = 0;


//inicjalizowanie ADC
void adc_init(){

    ADMUX = _BV(REFS0);

    DIDR0 = _BV(ADC0D);

    ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2)   ;
    ADCSRA |= _BV(ADIE) | _BV(ADATE);

    ADCSRB |= _BV(ADTS1);
    ADCSRA |= _BV(ADEN);
}

void timer_init()
{

    TCCR0A |= _BV(WGM00);
    TCCR0B |= _BV(CS01) | _BV(CS00);
    TIMSK0 |= _BV(OCIE0A);
    OCR0A = 124;
}
void interrupt_init()
{
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
}

ISR(TIMER0_COMPA_vect) {

    if(current_time++ == 1000)
    {
        current_time = 0;
        printf("Last Measurement: %f\r\n", last_measurement);
    }
}
ISR(INT0_vect) {
    
    // ADCSRA |= _BV(ADSC);
    // printf("Pressed\r\n");

}
ISR(ADC_vect) 
{
    float R = 10000;
    float Vcc = 5;
    ADCSRA |= _BV(ADIF);
    float vin = (ADC * Vcc) / 1024; 
    last_measurement = R * ((Vcc - vin)/vin); 
}



int main(){
    UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    
    BUTTON_DDR &= ~_BV(BUTTON);
    BUTTON_PORT |= _BV(BUTTON);

    adc_init(); 
    timer_init();
    interrupt_init();
    sei();


    set_sleep_mode(SLEEP_MODE_IDLE);
    while(1) 
    {
        //sleep_mode();
    }
}