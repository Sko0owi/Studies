#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

volatile uint16_t ticks = 0;

static void tick_uint8() {
    volatile uint8_t a = 5, b = 15, c;

    TCNT1 = 0;
    c = a+b;
    ticks = TCNT1;

    printf("Addition: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a*b;
    ticks = TCNT1;

    printf("Multiplication: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a/b;
    ticks = TCNT1;

    printf("Division: %"PRIu16" ticks.\r\n", ticks);
}
static void tick_uint16() {
    volatile uint16_t a = 5,b=15, c;

    TCNT1 = 0;
    c = a+b;
    ticks = TCNT1;

    printf("Addition: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a*b;
    ticks = TCNT1;

    printf("Multiplication: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a/b;
    ticks = TCNT1;

    printf("Division: %"PRIu16" ticks.\r\n", ticks);
}
static void tick_uint32() {

    volatile uint32_t a = 5,b=15, c;

    TCNT1 = 0;
    c = a+b;
    ticks = TCNT1;

    printf("Addition: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a*b;
    ticks = TCNT1;

    printf("Multiplication: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a/b;
    ticks = TCNT1;

    printf("Division: %"PRIu16" ticks.\r\n", ticks);
}
static void tick_uint64() {
    volatile uint64_t a = 5,b=15, c;

    TCNT1 = 0;
    c = a+b;
    ticks = TCNT1;

    printf("Addition: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a*b;
    ticks = TCNT1;

    printf("Multiplication: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a/b;
    ticks = TCNT1;

    printf("Division: %"PRIu16" ticks.\r\n", ticks);
}
static void tick_float() {
    volatile float a = 5,b=15, c;

    TCNT1 = 0;
    c = a+b;
    ticks = TCNT1;

    printf("Addition: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a*b;
    ticks = TCNT1;

    printf("Multiplication: %"PRIu16" ticks.\r\n", ticks);

    TCNT1 = 0;
    c = a/b;
    ticks = TCNT1;

    printf("Division: %"PRIu16" ticks.\r\n", ticks);
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  while(1) {
    
    printf("\r\nTicks of uint8_t.\r\n\r\n");
    tick_uint8();
    _delay_ms(2000);

    printf("\r\nTicks of uint16_t.\r\n\r\n");
    tick_uint16();
    _delay_ms(2000);

    printf("\r\nTicks of uint32_t.\r\n\r\n");
    tick_uint32();
    _delay_ms(2000);

    printf("\r\nTicks of uint64_t.\r\n\r\n");
    tick_uint64();
    _delay_ms(2000);

    printf("\r\nTicks of float.\r\n\r\n");
    tick_float();
    _delay_ms(2000);
    break;
  }

}

