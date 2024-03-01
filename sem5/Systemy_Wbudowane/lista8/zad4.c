#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>
#include <stdio.h>
#include "semphr.h"
#include "uart.h"
#include <avr/interrupt.h>


#define ADC_TASK_PRIORITY 1

static void vADC_read(void* pvParameters);
static void vADC_read_2(void* pvParameters);

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

SemaphoreHandle_t semADC, semADCComplete, printSEM;


void adc_init() {
  ADMUX = _BV(REFS0); // referencja AVcc
  DIDR0 = _BV(ADC0D) | _BV(ADC1D) |
          _BV(ADC2D); // wyłącz wejście cyfrowe na ADC0
                      // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN);                           // włącz ADC
  ADCSRA |= _BV(ADIE);                           // włącz interrupt na ADC
}

int main(void)
{
    uart_init();

    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    adc_init();
    sei();

    xTaskHandle adc_foto, adc_poten, adc_main;


    semADC = xSemaphoreCreateBinary();
    semADCComplete = xSemaphoreCreateBinary();
    printSEM = xSemaphoreCreateBinary();
    xSemaphoreGive(semADC);
    xSemaphoreGive(printSEM);

    xTaskCreate
        (
         vADC_read,
         "ADC_read_1",
         200,
         (void *)0,
         ADC_TASK_PRIORITY,
         &adc_poten
        );
    xTaskCreate
        (
         vADC_read,
         "ADC_read_2",
         200,
         (void *)1,
         ADC_TASK_PRIORITY,
         &adc_foto
        );
    xTaskCreate
        (
         vADC_read,
         "ADC_read_3",
         200,
         (void *)0xe,
         ADC_TASK_PRIORITY,
         &adc_main
        );

    vTaskStartScheduler();

    return 0;
}

ISR(ADC_vect) { xSemaphoreGive(semADCComplete); }


uint16_t readADC(uint8_t mux) 
{
  xSemaphoreTake(semADC, portMAX_DELAY);
  uint16_t adc_state;
  
  ADMUX = (ADMUX & 0xF0) | mux;
  ADCSRA |= _BV(ADSC);

  xSemaphoreTake(semADCComplete, portMAX_DELAY);
  adc_state = ADC;
  xSemaphoreGive(semADC);
  return adc_state;
}
void vApplicationIdleHook(void){}

static void vADC_read(void *pvParameters) {
    uint8_t input = (uint8_t)pvParameters;
    char index = 'c';
    if(input == 0)
    {
        index = 'b';
    }
    if(input == 1)
    {
        index = 'a';
    }
    while(1)
    {
        uint16_t adc;
        adc = readADC(input);
        xSemaphoreTake(printSEM, portMAX_DELAY);

        printf("ADC_%c: %"PRIu16"\r\n", index, adc);

        xSemaphoreGive(printSEM); 

        vTaskDelay(5000 / portTICK_PERIOD_MS);

    }
}