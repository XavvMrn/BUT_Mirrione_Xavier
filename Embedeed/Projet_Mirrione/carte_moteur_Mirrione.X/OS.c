#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "OS.h"

#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_DROITE 6
#define STATE_TOURNE_DROITE_EN_COURS 7
#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
#define STATE_TOURNE_SUR_PLACE_DROITE 10
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
#define STATE_RECULE_PAR_GAUCHE 16
#define STATE_RECULE_PAR_GAUCHE_EN_COURS 17
#define STATE_RECULE_PAR_DROITE 18
#define STATE_RECULE_PAR_DROITE_EN_COURS 19
#define STATE_ARRET 12
#define STATE_ARRET_EN_COURS 13
#define STATE_RECULE 14
#define STATE_RECULE_EN_COURS 15

#define STATE_RECULE_UN_PEU_GAUCHE 20
#define STATE_RECULE_UN_PEU_GAUCHE_EN_COURS 21
#define STATE_RECULE_UN_PEU_DROITE 22
#define STATE_RECULE_UN_PEU_DROITE_EN_COURS 23
#define STATE_TOURNE_UN_PEU_GAUCHE 24
#define STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS 25
#define STATE_TOURNE_UN_PEU_DROITE 26
#define STATE_TOURNE_UN_PEU_DROITE_EN_COURS 27

#define STATE_TOURNE_BCP_GAUCHE 28
#define STATE_TOURNE_BCP_GAUCHE_EN_COURS 29
#define STATE_TOURNE_BCP_DROITE 30
#define STATE_TOURNE_BCP_DROITE_EN_COURS 31 
#define STATE_180 32
#define STATE_180_EN_COURS 33

#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_EN_FACE 3

unsigned long timestamp;
unsigned long startingActionTimestamp;
unsigned char stateRobot;


void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

            
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

            
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

            
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
           
        case STATE_RECULE_PAR_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_GAUCHE_EN_COURS;
            break;
        case STATE_RECULE_PAR_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_PAR_DROITE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_DROITE_EN_COURS;
            break;
        case STATE_RECULE_PAR_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
           
        case STATE_TOURNE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(12, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(12, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_UN_PEU_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
       
            
        case STATE_TOURNE_BCP_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_BCP_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_BCP_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        
        case STATE_TOURNE_BCP_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_BCP_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_BCP_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_180:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            startingActionTimestamp = timestamp;
            stateRobot = STATE_180_EN_COURS;
            break;
        case STATE_180_EN_COURS:
            if((timestamp-startingActionTimestamp)>500)
                stateRobot = STATE_AVANCE;
            //SetNextRobotStateInAutomaticMode();
            break;
            
            

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}


unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreCentre < 15) {
        nextStateRobot = STATE_180;
    } 
    else if (robotState.distanceTelemetreGauche < 25) {
        //Recule par la droite
        nextStateRobot = STATE_RECULE_PAR_DROITE;
    } else if (robotState.distanceTelemetreDroit < 25) {
        //Recule par la gauche
        nextStateRobot = STATE_RECULE_PAR_GAUCHE; 
    } 
    else if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreGauche > 20 &&
            robotState.distanceTelemetreCentre > 20) {
        //Tourne a droite car aucun obstacle(main)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    } else if (robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreDroit > 20 &&
            robotState.distanceTelemetreCentre > 20) {
        //Tourne a gauche car aucun obstacle (main)
        nextStateRobot = STATE_TOURNE_DROITE;
        //Tourne un peu vers la gauche
        
    } else if (robotState.distanceTelemetreCentre < 35 &&
            robotState.distanceTelemetreDroit > 20 &&
            robotState.distanceTelemetreGauche > 20) {
        if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
            nextStateRobot = STATE_TOURNE_BCP_GAUCHE;
        } else {
            nextStateRobot = STATE_TOURNE_BCP_DROITE;
        }
    }
    else if (robotState.distanceTelemetreGauche2 > 30 &&
            robotState.distanceTelemetreDroit2 < 35 ) {
            nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
        } 
    else if (robotState.distanceTelemetreDroit2 > 30 &&
            robotState.distanceTelemetreGauche2 < 35 ) {
            nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
        }
    
    
    else if(robotState.distanceTelemetreCentre < 35){
        if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
        } else {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        }
    }

    else {
        //Avance
        nextStateRobot = STATE_AVANCE;
    }

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }
}

