#include <stdio.h>
#include "FreeRTOS.h"
#include "uart.h"
#include "queue.h"
#include "task.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

#define QUEUE_SIZE 128
xQueueHandle recvQueue, sendQueue;

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
  recvQueue = xQueueCreate(QUEUE_SIZE, sizeof(char));
  sendQueue = xQueueCreate(QUEUE_SIZE, sizeof(char));
}

int uart_transmit(char c, FILE *stream) {
  if (!uxQueueSpacesAvailable(sendQueue)) {
    UCSR0B |= _BV(UDRIE0);
    xQueueSend(sendQueue, &c, portMAX_DELAY);
  } else
    xQueueSend(sendQueue, &c, 0);

  if (c == '\n')
    UCSR0B |= _BV(UDRIE0);
  return 0;
}

int uart_receive(FILE *stream) {
  char c;
  if (!uxQueueMessagesWaiting(recvQueue)) {
    UCSR0B |= _BV(RXCIE0);
    xQueueReceive(recvQueue, &c, portMAX_DELAY);
  } else
    xQueueReceive(recvQueue, &c, 0);

  return c;
}

ISR(USART_RX_vect) {
  if (uxQueueMessagesWaitingFromISR(recvQueue) == QUEUE_SIZE)
    UCSR0B &= ~_BV(RXCIE0);
  else {
    char c = UDR0;
    xQueueSendToBackFromISR(recvQueue, &c, NULL);
  }
}

ISR(USART_UDRE_vect) {
  if (!uxQueueMessagesWaitingFromISR(sendQueue))
    UCSR0B &= ~_BV(UDRIE0);
  else {
    char c;
    if (xQueueReceiveFromISR(sendQueue, &c, NULL) == pdTRUE)
      UDR0 = c;
  }
}
