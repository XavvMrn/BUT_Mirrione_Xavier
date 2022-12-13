/* 
 * File:   cb_tx1.h
 * Author: Table2
 *
 * Created on 7 d�cembre 2022, 10:37
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
unsigned char CB_TX1_Get(void);
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
void SendOne();
unsigned char CB_TX1_IsTranmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_RemainingSize(void);


#endif	/* CB_TX1_H */

