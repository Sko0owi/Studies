#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define BUFFER_SIZE 64
#define LONG_SPACE_TIME 300
#define CHAR_TIME 500

#define WORD_TIME 2000

#define LED PD2
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON PD3
#define BUTTON_DDR DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND

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
    0b01010000, // 'A'
    0b10001110, // 'B'
    0b10001010, // 'C'
    0b01101100, // 'D'
    0b00110000, // 'E'
    0b10011010, // 'F'
    0b01100100, // 'G'
    0b10011110, // 'H'
    0b01011000, // 'I'
    0b10010000, // 'J'
    0b01101000, // 'K'
    0b10010110, // 'L'
    0b01000000, // 'M'
    0b01001000, // 'N'
    0b01100000, // 'O'
    0b10010010, // 'P'
    0b10000100, // 'Q'
    0b01110100, // 'R'
    0b01111100, // 'S'
    0b00100000, // 'T'
    0b01111000, // 'U'
    0b10011100, // 'V'
    0b01110000, // 'W'
    0b10001100, // 'X'
    0b10001000, // 'Y'
    0b10000110 // 'Z'
};


char buffer[BUFFER_SIZE];
uint8_t ind = 0;

uint8_t curr_n = 0;
uint8_t curr_char = 0;



void process_char()
{
    // printf("%"PRId8"\n\r", curr_n);
    // printf("%"PRId8"\n\r", curr_char);

    curr_char |= (curr_n << 5);

    for(int i = 0; i < 26; i++)
    {
        if(morse_code[i] == curr_char)
        {
            buffer[ind++] = 'A' + i;
            // printf("%c\r\n", ('A' + i)); 
        }
    }

    curr_n = 0;
    curr_char = 0;
}

void process_buffer()
{
    printf("%s\r\n", buffer);

    memset(buffer, 0, BUFFER_SIZE); 
    ind = 0;
}

int main()
{
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    // program testowy

    BUTTON_DDR &= ~ _BV(BUTTON);
    LED_DDR |= _BV(LED);

    BUTTON_PORT |= _BV(BUTTON);

    uint16_t time_pushed = 0;
    uint16_t time_pulled = 0;

    uint8_t time_notify = 0;

    

    
    memset(buffer, 0, BUFFER_SIZE); 



    while(1) {
        
        if(bit_is_clear(BUTTON_PIN, BUTTON))
        {
            time_pulled = 0;
            time_pushed++;

            if(time_pushed == 5000) time_pushed = 5000;

            if(time_pushed >= LONG_SPACE_TIME)
            {
                // curr_char |= (1 << (4 - curr_n));   // LONG BOIIII
                time_notify = 1;
            } else 
            {
                // curr_char |= (1 << (4 - curr_n));
                                                    // SHORT BOIII
            }
            
        } else 
        {
            if(time_pushed >= 10)
            {
                if(time_pushed <= LONG_SPACE_TIME)
                {
                    curr_char |= (1 << (4 - curr_n));
                }
                
                curr_n++;
                if(curr_n == 5)
                {
                    process_char();
                    time_notify = 100;
                }
            }
            
            time_pushed = 0;
            time_pulled++;

            if(curr_n != 0 && time_pulled >= CHAR_TIME)
            {
                process_char();
                time_notify = 100;

            } else if(time_pulled >= WORD_TIME || ind == 64)
            {
                if(ind != 0) process_buffer();
                time_notify = 1;
            }
            
            
            if(time_pulled == 5000) time_pulled = 4999;

            // printf("PRZYCISK PUSZCZONY\n\r");
        }
        // printf("TIME PUSHED: %"PRId16 "TIME PULLED: %"PRId16"\r\n", time_pushed, time_pulled);
        // printf("\r\n");
        // // printf("SHORT CLICKED: %"PRId16 "LONG CLICKED: %"PRId16"\r\n", time_short, time_long);
        // printf("LONG CLICKED: %"PRId16 "SHORT CLICKED: %"PRId16"\r\n", time_long, time_short);
        if(time_notify)
        {
            LED_PORT |= _BV(LED);
            time_notify--;
        } else 
        {
            LED_PORT &= ~_BV(LED);
        }

        _delay_ms(1);
        // printf("%s\n", buffer);
    }
}

