#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
#define BUFFER_SIZE  64

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

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

uint8_t morse_code[] = 
{
    80, 142, 138, 108, 48, 154, 100, 158, 88, 144, 104, 150, 64, 72, 96, 146, 132, 116, 124, 32, 120, 156, 112, 140, 136, 134
};

bool good_character(char c)
{
    return (c >= 'A' && c <= 'Z');
}

void morse_to_blink(char c)
{
    // printf("%c\n", c);
    int morse_from_char = morse_code[c - 'A'];
    int n = morse_from_char >> 5;
    
    int indeks = (1<<4);
    for(int i = 0; i < n; i++)
    {
        int bit = (morse_from_char & indeks) >> (4-i);
        if(bit == 1)
        {
            // printf("BLINK SHORT\n");
            LED_PORT |= _BV(LED);
            _delay_ms(100);
            LED_PORT &= ~_BV(LED);

        } else 
        {
            LED_PORT |= _BV(LED);
            _delay_ms(300);
            LED_PORT &= ~_BV(LED);
            // printf("BLINK LONG\n");
        }
        _delay_ms(500);
        // printf("ind: %d, bit: %d\n", indeks, (morse_from_char & indeks) >> (4-i));
        indeks >>= 1;
    }
    _delay_ms(500);
}



int main()
{
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    // program testowy

    LED_DDR |= _BV(LED);

    // printf("Hello world!\r\n");
    char buffer[BUFFER_SIZE];
    while(1) {
        memset(buffer, 0, BUFFER_SIZE); 
        printf("write text to encode: \r\n");       
        scanf("%s", &buffer);
        printf("Odczytano: %s\r\n", buffer);

        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            if(buffer[i] >= 'a' && buffer[i] <= 'z') buffer[i] += 'A' - 'a';
            if(!buffer[i] || !good_character(buffer[i])) break;
            morse_to_blink(buffer[i]);
            //printf("%d: %c ",i, buffer[i]);
        }
        // printf("%s\n", buffer);
    }
}

