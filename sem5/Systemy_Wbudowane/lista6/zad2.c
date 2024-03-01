#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define BUFFER_SIZE 128


volatile char sending[BUFFER_SIZE], receiving[BUFFER_SIZE];
volatile uint8_t curr_sending = 0, last_sending = 0;
volatile uint8_t curr_receiving = 0, last_receiving = 0;




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
    // włączanie interruptów
    UCSR0B |= _BV(RXCIE0);

}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{      
    while(curr_sending == (last_sending + 1)%BUFFER_SIZE) sleep_mode();
    
    sending[last_sending++] = data;
    last_sending %= BUFFER_SIZE;

    UCSR0B |= _BV(UDRIE0);
    return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    while(curr_receiving == last_receiving) sleep_mode();

    char return_val = receiving[curr_receiving++];
    curr_receiving %= BUFFER_SIZE;

    return return_val;

}

ISR(USART_RX_vect)
{
    receiving[last_receiving++] = UDR0;
    last_receiving %= BUFFER_SIZE;
}

ISR(USART_UDRE_vect)
{
    UDR0 = sending[curr_sending++];
    curr_sending %= BUFFER_SIZE;

    if(curr_sending == last_sending)
    {
        UCSR0B &= ~_BV(UDRIE0);
    }
}



FILE uart_file;

int main()
{
    // zainicjalizuj UART
    uart_init();
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);

    
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
 
    printf("TARCZA SZM*TO\r\n");

    while(1) {
        
        char a = 'a';
        scanf("%c", &a);
        printf("Odczytano : %c\r\n", a);
    }
}

