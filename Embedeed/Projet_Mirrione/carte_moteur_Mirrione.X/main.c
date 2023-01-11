#include <libpic30.h>

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "State.h"
#include "UART.h"
#include "cb_tx1.h"
#include "cb_rx1.h"
#include "UART_Protocol.h"

unsigned int *result;

int main(void) {

    InitOscillator();
    InitIO();

    InitTimer1();
    InitTimer23();
    InitTimer4();

    InitPWM();
    InitADC1();
    InitUART();
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    while (1) {


        int i;
        for (i = 0; i < CB_RX1_GetDataSize(); i++) { 
            unsigned char c = CB_RX1_Get();            
            //SendMessage(&c, 1);   // renvoie des trames recues 
            UartDecodeMessage(c);
        }

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int *result = ADCGetResult();
            float volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche2 = 34 / volts - 5;
            volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit2 = 34 / volts - 5;


            if (robotState.distanceTelemetreDroit2 < 30) {
                LED_ORANGE = 1;
            } else if (robotState.distanceTelemetreDroit < 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }

            if (robotState.distanceTelemetreCentre < 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }

            if (robotState.distanceTelemetreGauche2 < 30) {
                LED_BLANCHE = 1;    
            } else if (robotState.distanceTelemetreGauche < 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }


            //SendMessage("Bonjour", 7);
            //            SendMessageDirect((unsigned char*) "DAC", 3);
            //            __delay32(40000000);
            
            //unsigned char payload[] = {'B','o','n','j','o','u','r'};
            //    UartEncodeAndSendMessage(0x0080, 7, payload);
            unsigned char payload[] = {robotState.distanceTelemetreGauche,
            robotState.distanceTelemetreCentre,robotState.distanceTelemetreDroit};
                UartEncodeAndSendMessage(0x0030, 3, payload);
                

        }
    }
}
