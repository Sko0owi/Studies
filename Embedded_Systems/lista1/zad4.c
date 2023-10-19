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

FILE uart_file;

int main() {

    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    printf("HELLO WORLD!\n\r");
    // program testowy
    while(1) {

        /* int8_t */
        int8_t x8,y8;
        scanf("%"SCNd8, &x8);
        scanf("%"SCNd8, &y8);

        printf("Odczytano x: %"PRId8"\n\r", x8);
        printf("Odczytano y: %"PRId8"\n\r", y8);

        int8_t sum8 = x8+y8;
        printf("SUM: %"PRId8"\r\n", sum8);
        int8_t mult8 = x8*y8;
        printf("MULT: %"PRId8"\r\n", mult8);
        int8_t div8 = x8/y8;
        printf("DIV: %"PRId8"\r\n", div8);

        /* int16_t */
        int16_t x16,y16;
        scanf("%"SCNd16, &x16);
        scanf("%"SCNd16, &y16);

        printf("Odczytano x: %"PRId16"\n\r", x16);
        printf("Odczytano y: %"PRId16"\n\r", y16);

        int16_t sum16 = x16+y16;
        printf("SUM: %"PRId16"\r\n", sum16);
        int16_t mult16 = x16*y16;
        printf("MULT: %"PRId16"\r\n", mult16);
        int16_t div16 = x16/y16;
        printf("DIV: %"PRId16"\r\n", div16);


        /* int32_t */
        int32_t x32,y32;
        scanf("%"SCNd32, &x32);
        scanf("%"SCNd32, &y32);

        printf("Odczytano x: %"PRId32"\n\r", x32);
        printf("Odczytano y: %"PRId32"\n\r", y32);

        int32_t sum32 = x32+y32;
        printf("SUM: %"PRId32"\r\n", sum32);
        int32_t mult32 = x32*y32;
        printf("MULT: %"PRId32"\r\n", mult32);
        int16_t div32 = x32/y32;
        printf("DIV: %"PRId32"\r\n", div32);


        /* int64_t oszukiwane*/
        int32_t x1_32, x2_32, y1_32, y2_32;
        scanf("%"SCNd32, &x1_32);
        scanf("%"SCNd32, &x2_32);
        scanf("%"SCNd32, &y1_32);
        scanf("%"SCNd32, &y2_32);


        printf("Odczytano x: %"SCNd32" %"SCNd32"\n\r", x1_32, x2_32);
        printf("Odczytano y: %"SCNd32" %"SCNd32"\n\r", y1_32, y2_32);

        int64_t x64, y64;

        x64 = ((int64_t)x1_32 << 31) + (int64_t)x2_32;
        y64 = ((int64_t)y1_32 << 31) + (int64_t)y2_32;

        

        int64_t sum64 = x64 + y64;
        printf("SUM: %"PRId32" %"PRId32"\r\n", (int32_t)(sum64>>32), (int32_t)sum64);
        int64_t mult64 = x64*y64;
        printf("MULT: %"PRId32" %"PRId32"\r\n", (int32_t)(mult64>>32), (int32_t)mult64);
        int64_t div64 = x64/y64;
        printf("DIV: %"PRId32" %"PRId32"\r\n", (int32_t)(div64>>32) , (int32_t)div64);

        float xfl, yfl;

        scanf("%f", &xfl);
        scanf("%f", &yfl);

        printf("Odczytano x: %f \n\r", xfl);
        printf("Odczytano y: %f \n\r", yfl);

        float sumfl = xfl + yfl;
        printf("SUM: %f\r\n", sumfl);
        float multfl = xfl * yfl;
        printf("MULT: %f\r\n", multfl);
        float divfl = xfl / yfl;
        printf("DIV: %f\r\n", divfl);



    }
}
