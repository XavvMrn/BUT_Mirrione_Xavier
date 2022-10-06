/* 
 * File:   ADC.h
 * Author: TP_EO_6
 *
 * Created on 15 septembre 2022, 09:16
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
void ADC1StartConversionSequence(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);
unsigned int * ADCGetResult(void) ;


#endif	/* ADC_H */

