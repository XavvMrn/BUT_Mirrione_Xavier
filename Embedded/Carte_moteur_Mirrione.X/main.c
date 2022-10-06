//Partie PWM
#include <xc.h> // library xc.h inclut tous les uC
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "toolbox.h"
#include "adc.h"
#include "main.h"


unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

unsigned char stateRobot;
unsigned long timestamp;
unsigned long startingActionTimestamp;

int main(void) {
   InitOscillator();

    InitIO();
    
    InitTimer23();
    InitTimer1();
    InitTimer4();
    InitPWM();
    InitADC1();   
     
    
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    //PWMSetSpeed(20,MOTEUR_DROITE);

    while (1) {
        if (ADCIsConversionFinished()) 
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            //ADCValue0 = result[0];
            //ADCValue1 = result[1];
            //ADCValue2 = result[2];
            float volts = ((float) result [4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche2 = 34 / volts - 5;
            volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit2 = 34 / volts - 5;
            
              if (robotState.distanceTelemetreDroit2< 30){
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreCentre < 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }
            if (robotState.distanceTelemetreGauche2< 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }
        }
        
    } // fin main
    
}


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
            PWMSetSpeedConsigne(40, MOTEUR_DROIT);
            PWMSetSpeedConsigne(40, MOTEUR_GAUCHE);
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
        case STATE_RECULE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_DROITE_EN_COURS;
            break;
        case STATE_RECULE_UN_PEU_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_UN_PEU_GAUCHE_EN_COURS;
            break;
        case STATE_RECULE_UN_PEU_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(22, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(22, MOTEUR_DROIT);
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
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-25, MOTEUR_GAUCHE);
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
    if (robotState.distanceTelemetreCentre < 20) {
        nextStateRobot = STATE_180;
    } else if (robotState.distanceTelemetreGauche < 20) {
        //Recule par la droite
        nextStateRobot = STATE_RECULE_PAR_DROITE;
    } else if (robotState.distanceTelemetreDroit < 20) {
        //Recule par la gauche
        nextStateRobot = STATE_RECULE_PAR_GAUCHE;
    } else if (robotState.distanceTelemetreDroit < 40 &&
            robotState.distanceTelemetreGauche > 20 &&
            robotState.distanceTelemetreCentre > 20) {
        //Tourne a droite car aucun obstacle(main)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    } else if (robotState.distanceTelemetreGauche < 40 &&
            robotState.distanceTelemetreDroit > 20 &&
            robotState.distanceTelemetreCentre > 20) {
        //Tourne a gauche car aucun obstacle (main)
        nextStateRobot = STATE_TOURNE_DROITE;
        //Tourne un peu vers la gauche
    } else if (robotState.distanceTelemetreCentre < 40 &&
            robotState.distanceTelemetreDroit > 20 &&
            robotState.distanceTelemetreGauche > 20) {
        if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
            nextStateRobot = STATE_TOURNE_BCP_GAUCHE;
        } else {
            nextStateRobot = STATE_TOURNE_BCP_DROITE;
        }
    }
    else if(robotState.distanceTelemetreCentre < 40){
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



