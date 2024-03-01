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

const uint8_t rtc_addr = 0xd0;

// inicjalizacja UART
void uart_init()
{
    UBRR0 = UBRR_VALUE;

    UCSR0A = 0;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = data;
    return 0;
}

int uart_receive(FILE *stream)
{
    while (!(UCSR0A & _BV(RXC0)))
        ;
    return UDR0;
}

FILE uart_file;

const char set_token[] PROGMEM = "set\0";

const char date_token[] PROGMEM = "date\0";
const char time_token[] PROGMEM = "time\0";

char instruction[12];

uint8_t ACTUAL_CENTURY = 20;

void write_byte(uint8_t addr, uint8_t data)
{
    i2cStart();
    i2cCheck(0x08, "I2C write start");

    i2cSend(rtc_addr);
    i2cCheck(0x18, "I2C RTC write request");

    i2cSend(addr);
    i2cCheck(0x28, "I2C RTC set address to write");

    i2cSend(data);
    i2cCheck(0x28, "I2C RTC data transmitted");

    i2cStop();
    i2cCheck(0xf8, "I2C stop");
}

uint8_t read_byte(uint8_t addr)
{

    // dummy write

    i2cStart();
    i2cCheck(0x08, "I2C dummy write start");

    i2cSend(rtc_addr);
    i2cCheck(0x18, "I2C RTC dummy write request");

    i2cSend(addr);
    i2cCheck(0x28, "I2C RTC set address to dummy write");

    // restart

    i2cStart();
    i2cCheck(0x10, "I2C repeated read start");

    i2cSend(rtc_addr | 1);
    i2cCheck(0x40, "I2C RTC read request");

    uint8_t byte = i2cReadNoAck();
    i2cCheck(0x58, "I2C RTC read");

    i2cStop();
    i2cCheck(0xf8, "I2C stop");

    return byte;
}

uint8_t calc_sec(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t calc_min(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t calc_hour(uint8_t byte)
{
    uint8_t format = byte & 0x40;
    if(format)
    {
        if(byte & 0x20)
        {
            return 10 * ((byte & 0x10) >> 4) + (byte & 0x0F) + 12;
        } else 
        {
            return 10 * ((byte & 0x10) >> 4) + (byte & 0x0F);
        }
    } else 
    {
        return 20 * ((byte & 0x20) >> 5) + 10 * ((byte & 0x10) >> 4) + (byte & 0x0F);
    }
}


uint8_t get_day(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}

uint8_t get_month(uint8_t byte)
{
    return ((10 * (byte >> 4)) & 0x3) + (byte & 0x0F);
}
uint8_t get_year(uint8_t byte)
{
    return (10 * (byte >> 4)) + (byte & 0x0F);
}
uint8_t get_century(uint8_t byte)
{
    return (byte & 0x40) >> 7;
}


void read_current_time()
{
    uint8_t curr_sec, curr_min, curr_hour;
    curr_sec = calc_sec(read_byte(0));
    curr_min = calc_min(read_byte(1));
    curr_hour = calc_hour(read_byte(2));
    
    printf("Current time: %.2d:%.2d:%.2d\r\n", curr_hour, curr_min, curr_sec);

}


void read_current_date()
{
    uint8_t curr_day, curr_month, curr_year;

    curr_day = get_day(read_byte(4));
    curr_month = get_month(read_byte(5));
    curr_year = get_year(read_byte(6));
    
    printf("Current date: %.2d%.2d-%.2d-%.2d\r\n", ACTUAL_CENTURY, curr_year, curr_month, curr_day);

}

uint8_t convertToUint(char x, char y)
{
    return (x - '0') * 10 + (y - '0');
}

void write_time(const char *str)
{
    uint8_t hour = convertToUint(str[0], str[1]);
    uint8_t minute = convertToUint(str[3], str[4]);
    uint8_t second = convertToUint(str[6], str[7]);

    write_byte(0,((second / 10) << 4) | (second % 10));
    write_byte(1,((minute / 10) << 4) | (minute % 10));
    if(hour >= 20)
    {
        write_byte(2, ((hour / 20 << 5)) | (hour % 10));
    } else if (hour >= 10)
    {
        write_byte(2, ((1 << 4)) | (hour % 10));
    } else 
    {
        write_byte(2, hour % 10);
    }
}
void write_date(const char *str)
{
    uint8_t century = convertToUint(str[0], str[1]);
    uint8_t year = convertToUint(str[2], str[3]);
    uint8_t month = convertToUint(str[5], str[6]);
    uint8_t day = convertToUint(str[8], str[9]);

    write_byte(4,((day / 10) << 4) | (day % 10));
    write_byte(5,((month / 10) << 4) | (month % 10));
    write_byte(6,((year / 10) << 4) | (year % 10));
    ACTUAL_CENTURY = century;
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
        printf("%s\r\n", &instruction);

        if (!strcmp_P(&instruction, &set_token))
        {
            scanf("%s", &instruction);
            printf("%s\r\n", &instruction);


            if (!strcmp_P(&instruction, &time_token))
            {
                char time[20];
                scanf("%s", &time);
                printf("%s\r\n", &time);

                write_time(time);

            }
            else if (!strcmp_P(&instruction, &date_token))
            {
                char date[20];
                scanf("%s", &date);
                printf("%s\r\n", &date);

                write_date(date);
            }
        }
        else if (!strcmp_P(&instruction, &time_token))
        {
            read_current_time();
        }
        else if (!strcmp_P(&instruction, &date_token))
        {
            read_current_date();
        }
    }
}