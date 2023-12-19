#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "uart2.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#define mainADC1_TASK_PRIO 1
#define mainADC2_TASK_PRIO 1
#define mainADC3_TASK_PRIO 1

static void vADC_READ1(void *pvParameters);
static void vADC_READ2(void *pvParameters);
static void vADC_READ3(void *pvParameters);
FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

SemaphoreHandle_t semADC, semADCComplete, printSEM;
// inicjalizacja ADC
void adc_init() {
  ADMUX = _BV(REFS0); // referencja AVcc
  DIDR0 = _BV(ADC0D) | _BV(ADC1D) |
          _BV(ADC2D); // wyłącz wejście cyfrowe na ADC0
                      // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN);                           // włącz ADC
  ADCSRA |= _BV(ADIE);                           // włącz interrupt na ADC
}

int main(void) {
  uart_init();
  
  fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  adc_init();
  sei();
  
  xTaskHandle adc1, adc2, adc3;
  
  semADC = xSemaphoreCreateBinary();
  semADCComplete = xSemaphoreCreateBinary();
  printSEM = xSemaphoreCreateBinary();
  xSemaphoreGive(semADC);
  xSemaphoreGive(printSEM);
  // xTaskCreate(vADC_READ1, "adc1", configMINIMAL_STACK_SIZE, (void *)0,
  //             mainADC1_TASK_PRIO, &adc1);
  // xTaskCreate(vADC_READ2, "adc2", configMINIMAL_STACK_SIZE, (void *)(0xe),
  //             mainADC2_TASK_PRIO, &adc2);
  xTaskCreate(vADC_READ3, "adc3", configMINIMAL_STACK_SIZE, (void *)(_BV(MUX0)),
              mainADC3_TASK_PRIO, &adc3);

  vTaskStartScheduler();

  return 0;
}

ISR(ADC_vect) { xSemaphoreGive(semADCComplete); }

uint16_t readADC(uint8_t mux) {
  xSemaphoreTake(semADC, portMAX_DELAY);
  uint16_t adc_state;
  ADMUX &= 0xf0;
  ADMUX |= mux;
  ADCSRA |= _BV(ADSC);
  xSemaphoreTake(semADCComplete, portMAX_DELAY);
  adc_state = ADC;
  xSemaphoreGive(semADC);
  return adc_state;
}

void vApplicationIdleHook(void) {}

static void vADC_READ1(void *pvParameters) {
  uint8_t input = (uint8_t)pvParameters;
  uint16_t adc;
  char buf[10];
  while (1) {
    adc = readADC(input);
    xSemaphoreTake(printSEM, portMAX_DELAY);
    fputs("ADC1: ", stdout);
    itoa(adc, buf, 10);
    fputs(buf, stdout);
    fputs("\r\n", stdout);

    fflush(stdout);
    xSemaphoreGive(printSEM);

    vTaskDelay(600 / portTICK_PERIOD_MS);
  }
}

static void vADC_READ2(void *pvParameters) {
  uint8_t input = (uint8_t)pvParameters;
  uint16_t adc;
  char buf[10];
  while (1) {
    adc = readADC(input);
    xSemaphoreTake(printSEM, portMAX_DELAY);
    fputs("ADC2: ", stdout);
    itoa(adc, buf, 10);
    fputs(buf, stdout);
    fputs("\r\n", stdout);

    fflush(stdout);
    xSemaphoreGive(printSEM);
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }
}

static void vADC_READ3(void *pvParameters) {
  uint8_t input = (uint8_t)pvParameters;
  uint16_t adc;
  char buf[10];
  while (1) {
    adc = readADC(input);
    xSemaphoreTake(printSEM, portMAX_DELAY);
    fputs("ADC3: ", stdout);
    itoa(adc, buf, 10);
    fputs(buf, stdout);
    fputs("\r\n", stdout);

    fflush(stdout);
    xSemaphoreGive(printSEM);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}