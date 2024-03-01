/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include <util/delay.h>
#include "uart.h"

#define buttonBlink_TASK_PRIORITY 1

#define knightRider_TASK_PRIORITY 2

#define LED_BUTTON PB3
#define LED_DDR_BUTTON DDRB
#define LED_PORT_BUTTON PORTB

#define BUTTON PB2
#define BUTTON_DDR DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN PINB


#define LED_DDR DDRD
#define LED_PORT PORTD


static void vButtonBlink(void* pvParameters);
static void vKnightRider(void* pvParameters);
FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

int main(void)
{
    // Create task.
    xTaskHandle buttonBlink_handle;
    xTaskHandle knightRider_handle;

    xTaskCreate
        (
            vButtonBlink,
            "Button Blink",
            configMINIMAL_STACK_SIZE,
            NULL,
            buttonBlink_TASK_PRIORITY,
            &buttonBlink_handle
        );
    xTaskCreate
        (
            vKnightRider,
            "Knight Rider",
            configMINIMAL_STACK_SIZE,
            NULL,
            knightRider_TASK_PRIORITY,
            &knightRider_handle
        );

    

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

void vApplicationIdleHook(void) {}


uint8_t circular_buffer[126];

void set_buffer(uint16_t ind)
{
    if(ind == 0) ind = 1001;
    else ind--;

    uint16_t ind_tab = ind >> 3;
    uint16_t ind_bite = 7 - (ind & 7);

    circular_buffer[ind_tab] |= (1 << ind_bite);
}

void light(uint16_t ind)
{
    uint16_t ind_tab = ind >> 3;
    uint16_t ind_bite = 7 - (ind & 7);

    if(bit_is_set(circular_buffer[ind_tab], ind_bite))
    {
        LED_PORT_BUTTON |= _BV(LED_BUTTON);
        circular_buffer[ind_tab] &= ~(1 << ind_bite);
    } else 
    {
        LED_PORT_BUTTON &= ~_BV(LED_BUTTON);
    }
}

void button_init()
{
    BUTTON_DDR &= ~ _BV(BUTTON);
    LED_DDR_BUTTON |= _BV(LED_BUTTON);

    BUTTON_PORT |= _BV(BUTTON);
}

uint16_t time = 0;


static void vButtonBlink(void* pvParameters)
{
    button_init();

    while(1) 
    {
        light(time);

        if(bit_is_clear(BUTTON_PIN, BUTTON))
        {
            set_buffer(time);
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);

        if(time++ == 1001) time = 0;
        // printf("%s\n", buffer);
    }
}

static void vKnightRider(void* pvParameters)
{

    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);


    uint8_t direction = 0; // 1 -> left, 0 -> right
    uint8_t setup = -1;
    uint8_t currLaser = (1<<7) | (1<<6);

    LED_DDR |= setup;
    LED_PORT = 0;
    
    while (1) {

        direction &= ~((currLaser & (1 << 7)) >> 7);
        direction |= (currLaser & 1);

        LED_PORT = currLaser;
        vTaskDelay(50 / portTICK_PERIOD_MS);

        if (direction)
        {
            currLaser <<= 1;
        } else 
        {
            currLaser >>= 1;
        }
        
    }

}

