/* 
 * File:   UART.h
 * Author: Table2
 *
 * Created on 7 décembre 2022, 08:55
 */

#ifndef UART_H
#define	UART_H

void InitUART(void);
void SendMessageDirect (unsigned char* message, int length);

#endif	/* UART_H */

