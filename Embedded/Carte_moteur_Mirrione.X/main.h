/* 
 * File:   main.h
 * Author: Table2
 *
 * Created on 3 octobre 2022, 15:11
 */

#ifndef MAIN_H
#define	MAIN_H

// Configuration des param�tres du chip
#define FCY 40000000

#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_EXGAUCHE 5
#define STATE_TOURNE_GAUCHE_EN_COURS 6
#define STATE_TOURNE_EXGAUCHE_EN_COURS 7
#define STATE_TOURNE_DROITE 8
#define STATE_TOURNE_EXDROITE 9
#define STATE_TOURNE_DROITE_EN_COURS 10
#define STATE_TOURNE_EXDROITE_EN_COURS 11
#define STATE_TOURNE_SUR_PLACE_GAUCHE 12
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 13
#define STATE_TOURNE_SUR_PLACE_DROITE 14
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 15
#define STATE_ARRET 16
#define STATE_ARRET_EN_COURS 17
#define STATE_RECULE 18
#define STATE_RECULE_EN_COURS 19

#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_EXGAUCHE 2 
#define OBSTACLE_A_DROITE 3
#define OBSTACLE_A_EXDROITE 4
#define OBSTACLE_EN_FACE 5

void SetNextRobotStateInAutomaticMode();
void OperatingSystemLoop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

