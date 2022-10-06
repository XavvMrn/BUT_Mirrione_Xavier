/* 
 * File:   timer.h
 * Author: TP_EO_6
 *
 * Created on 7 septembre 2022, 13:44
 */

#ifndef TIMER_H
#define	TIMER_H

extern unsigned long timestamp;

void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);
#endif	/* TIMER_H */

