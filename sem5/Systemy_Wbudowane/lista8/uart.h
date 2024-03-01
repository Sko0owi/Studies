#include <stdio.h>
#include "FreeRTOSConfig.h"
#include "queue.h"

#ifndef _UART_HDGYSTD
#define _UART_HDGYSTD

void uart_init(void);
int uart_transmit(char c, FILE *stream);
int uart_receive(FILE *stream);
#endif
