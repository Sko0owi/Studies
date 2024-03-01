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

void uart_init()
{ 
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */  
  UCSR0B |= _BV(RXCIE0);
  recvQueue = xQueueCreate(QUEUE_SIZE, sizeof(char));
  sendQueue = xQueueCreate(QUEUE_SIZE, sizeof(char));

}


int uart_transmit(char c, FILE *stream) {

  while((!uxQueueSpacesAvailable(sendQueue)));
  
  xQueueSend(sendQueue, &c, 0);

  UCSR0B |= _BV(UDRIE0);
  return 0;
}

int uart_receive(FILE *stream) {

  while(!uxQueueMessagesWaiting(recvQueue));
  char c;
  xQueueReceive(recvQueue, &c, 0);
  return c;
}

ISR(USART_RX_vect) {

  char c = UDR0;
  xQueueSendToBackFromISR(recvQueue, &c, NULL);
  
}

ISR(USART_UDRE_vect) {

  if (!uxQueueMessagesWaitingFromISR(sendQueue))
    UCSR0B &= ~_BV(UDRIE0);
  else 
  {
    char c;
    xQueueReceiveFromISR(sendQueue, &c, NULL);
    UDR0 = c;
  }
  
}
