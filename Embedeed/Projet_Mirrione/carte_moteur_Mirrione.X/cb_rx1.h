/* 
 * File:   cb_rx1.h
 * Author: Table2
 *
 * Created on 9 d�cembre 2022, 14:22
 */

#ifndef CB_RX1_H
#define	CB_RX1_H


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
int CB_RX1_GetRemainingSize(void);
int CB_RX1_GetDataSize(void);


#endif	/* CB_RX1_H */