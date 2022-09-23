/* 
 * File:   pwm.h
 * Author: Table2
 *
 * Created on 20 septembre 2022, 08:15
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 0

void InitPWM(void);

void PWMUpdateSpeed();
//void PWMSetSpeed(float vitesseEnPourcents, int moteur);

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);

#endif	/* PWM_H */

