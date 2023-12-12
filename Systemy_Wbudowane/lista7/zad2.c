#define __AVR_ATmega328P__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c.h"

#define BAUD 9600                            // baudrate
#define UBRR_VALUE ((F_CPU) / 16 / (BAUD)-1) // zgodnie ze wzorem

#define i2cCheck(code, msg)                                     \
    if ((TWSR & 0xf8) != (code))                                \
    {                                                           \
        printf(msg " failed, status: %.2x\r\n", (TWSR & 0xf8)); \
        i2cReset();                                             \
    }

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
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = data;
    return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    // czekaj aż znak dostępny
    while (!(UCSR0A & _BV(RXC0)))
        ;
    return UDR0;
}

FILE uart_file;

const char write_instr[] PROGMEM = "write\0";
const char read_instr[] PROGMEM = "read\0";

const uint8_t eeprom_addr = 0xa0;

uint16_t addr = 0;
uint8_t data = 0;
char instruction[8];

uint8_t hexToByte(char x, char y){
    uint8_t res = 0;
    if( 'a' <= x && x <= 'f')
        res += ((x - 'a') + 10) << 4;
    else if( '0' <= x && x <= '9')
        res += ((x - '0')) << 4;
    
    if( 'a' <= y && y <= 'f')
        res += ((y - 'a') + 10);
    else if( '0' <= y && y <= '9')
        res += ((y - '0'));
    
    return res;
}

void byteToHex(uint8_t b, char *resX, char *resY){
    uint8_t res = 0;
    char x = ( b & 0xf0 ) >> 4;
    char y = ( b & 0x0f );

    if( 0 <= x && x <= 9)
        x += '0';
    else if( 10 <= x && x <= 15)
        x = (x - 10) + 'a';
    
    if( 0 <= y && y <= 9)
        y += '0';
    else if( 10 <= y && y <= 15)
        y = (y - 10) + 'a';
    
    *resX = x;
    *resY = y;
}

void write()
{
    char line[32];

    while(1)
    {
        scanf("%s", &line);
        printf("%s\n\r", line);
        uint8_t length = hexToByte(line[0], line[1]);
        uint16_t addr = (hexToByte(line[2],line[3]) << 8) | hexToByte(line[4],line[5]);
        uint8_t record_type = hexToByte(line[6], line[7]);
        printf("%" PRId8 ", %" PRId16", %"PRId8"\n\r", length, addr, record_type);

        if(record_type == 1) break;

        i2cStart();
        i2cCheck(0x08,"I2C write start");

        i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
        i2cCheck(0x18,"I2C EEPROM write request");

        i2cSend(addr&0xff);
        i2cCheck(0x28,"I2C EEPROM set address");

        for(int i = 0; i < 2*length; i += 2)
        {
            uint8_t data = hexToByte(line[8+i], line[8+i+1]);
            i2cSend(data);
            i2cCheck(0x28,"I2C EEPROM data transmitted");
            printf("DATA: %"PRId8"\n\r", data);
        }
    }
    i2cStop();
    i2cCheck(0xf8,"I2C stop");
}

void read()
{
    uint16_t addr;
    uint8_t length;

    scanf("%" SCNx16, &addr);
    scanf("%" SCNd8, &length);

    i2cStart();
    i2cCheck(0x08,"I2C read start");

    i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
    i2cCheck(0x18,"I2C EEPROM write request");

    i2cSend(addr&0xff);
    i2cCheck(0x28,"I2C EEPROM set address");

    i2cStart();
    i2cCheck(0x10,"I2C second start");


    i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));

    char x,y;
    
    byteToHex(length, &x, &y);
    printf("%c%c", x,y);
    byteToHex(addr>>8, &x, &y);
    printf("%c%c", x,y);
    byteToHex(addr, &x, &y);
    printf("%c%c", x,y);
    printf("%c%c", '0','0');


    uint8_t sum_check = length + (addr >> 8) + (uint8_t)(addr) + 0; // 0 for record type


    for(uint16_t i = 0; i<length - 1; i++){
        data = i2cReadAck();
        sum_check += data;

        byteToHex(data,&x,&y);
        printf("%c%c", x,y);
    }
    data = i2cReadNoAck();
    i2cCheck(0x58,"I2C EEPROM read");


    byteToHex(data, &x, &y);
    sum_check += data;

    printf("%c%c", x,y);

    sum_check = 256 - sum_check;
    byteToHex(sum_check, &x,&y);
    printf("%c%c", x,y);
    printf("\r\n");

    printf("%"PRId8, sum_check);


    i2cStop();
    i2cCheck(0xf8,"I2C stop");
}

int main()
{
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    i2cInit();

    while (1)
    {
        scanf("%s", &instruction);
        if (!(strcmp_P(instruction, &write_instr)))
        {
            printf("Instruction: %s \r\n", instruction);
            write();
        }
        else if (!(strcmp_P(instruction, &read_instr)))
        {
            printf("Instruction: %s \r\n", instruction);
            read();
        } else 
        {
            printf("Invalid Instruction \r\n");
        }
    }
}