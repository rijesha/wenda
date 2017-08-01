// uart.h
// Functions for reading and writing to the device's serial port.

#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <sys/types.h>
#include <string.h>

int uartOpen();
int uartInit();
uint8_t uartGetByte();
int uartBytesAvailable();

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
