/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include <util/delay.h>
#include "uart.h"

#define blink_TASK_PRIORITY 1

#define readUART_TASK_PRIORITY 2

#define LED PB3
#define LED_DDR DDRB
#define LED_PORT PORTB


static void vReadUART(void* pvParameters);
static void vBlink(void* pvParameters);
FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

QueueHandle_t xQueue1;

int main(void)
{
    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle read_handle;

    xTaskCreate
        (
            vBlink,
            "Blink according to queue",
            200,
            NULL,
            blink_TASK_PRIORITY,
            &blink_handle
        );
    xTaskCreate
        (
            vReadUART,
            "read from uart",
            700,
            NULL,
            readUART_TASK_PRIORITY,
            &read_handle
        );

    

    xQueue1 = xQueueCreate( 20, sizeof( uint16_t ) );

    xQueueReset(xQueue1);

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

void vApplicationIdleHook(void) {}


static void vBlink(void* pvParameters)
{
    uint16_t time;
    LED_DDR |= _BV(LED);

    for (;;) {
        xQueueReceive(xQueue1, &time, portMAX_DELAY);

        LED_PORT |= _BV(LED);
        vTaskDelay(time / portTICK_PERIOD_MS);
        LED_PORT &= ~_BV(LED);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}


static void vReadUART(void* pvParameters)
{
    uart_init();
    stdin = stdout = stderr = &uart_file;
    xQueueReset(xQueue1);

    uint16_t input;

    for (;;) {
        char buff[10], c;
        uint8_t cnt = 0;
        while(c = getchar())
        {
            if (c == '\r') break;
            buff[cnt++] = c;
        }
        input = atoi(buff);
        
        for(int i = 0; i < 10; i++) buff[i] = '\0';
        
        printf("Podano %d\r\n", input);
        xQueueSend(xQueue1, &input, portMAX_DELAY);
    }
}