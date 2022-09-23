#ifndef ADC_C
#define	ADC_C

void InitADC1(void);
void ADC1StartConversionSequence();
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);


#endif	/* ADC_C */
