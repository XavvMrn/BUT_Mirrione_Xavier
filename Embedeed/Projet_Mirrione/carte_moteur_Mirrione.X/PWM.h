/* 
 * File:   PWM.h
 * Author: TP_EO_6
 *
 * Created on 7 septembre 2022, 16:22
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_GAUCHE 0
#define MOTEUR_DROIT 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int moteur);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, int moteur);

#endif	/* PWM_H */

