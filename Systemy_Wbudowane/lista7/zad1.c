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

void write()
{
    scanf("%" SCNx16 "%" SCNx8, &addr, &data);
    printf("Addr: %.3x Data: %" PRId8 "\r\n", addr, data);
    i2cStart();
    i2cCheck(0x08, "I2C write start");
    i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
    i2cCheck(0x18, "I2C EEPROM write request");
    i2cSend(addr & 0xff);
    i2cCheck(0x28, "I2C EEPROM set address");
    i2cSend(data);
    i2cCheck(0x28, "I2C EEPROM data transmitted");
    i2cStop();
    i2cCheck(0xf8, "I2C stop");
}

void read()
{
    scanf("%" SCNx16, &addr);
    printf("Addr: %.3x\r\n", addr);
    i2cStart();
    i2cCheck(0x08, "I2C read start");
    i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
    i2cCheck(0x18, "I2C EEPROM write request");
    i2cSend(addr & 0xff);
    i2cCheck(0x28, "I2C EEPROM set address");
    i2cStart();
    i2cCheck(0x10, "I2C second start");
    i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
    i2cCheck(0x40, "I2C EEPROM read request");
    data = i2cReadNoAck();
    i2cCheck(0x58, "I2C EEPROM read");
    i2cStop();
    printf("At addr: %.3x: Lies data: %"PRId8"\r\n", addr, data);
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