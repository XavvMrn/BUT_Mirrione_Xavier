#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "Robot.h"
#include "IO.h"


enum IdFunction {
    TransmissionText = 0x0080,
    LED = 0x0020,
    DistanceTelemIR = 0x0030,
    MotorSpeed = 0x0040,
    State = 0x0050
};

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char checksum = 0;
    checksum ^= 0xFE; // SOF du formatage
    checksum ^= (unsigned char) (msgFunction >> 8); // >> 8 étant un décalage de 8 bits 
    checksum ^= (unsigned char) (msgFunction >> 0); // pour homogéniser
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);
    int i;
    for (i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char message[msgPayloadLength + 6];
    int pos = 0;
    message[pos++] = 0xFE;
    message[pos++] = (unsigned char) (msgFunction >> 8);
    message[pos++] = (unsigned char) (msgFunction >> 0);
    message[pos++] = (unsigned char) (msgPayloadLength >> 8);
    message[pos++] = (unsigned char) (msgPayloadLength >> 0);
    
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        message[pos++] = msgPayload[i];
    }
    message[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    SendMessage(message, msgPayloadLength + 6);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

int rcvState;



void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entrée un octet et servant à reconstituer les trames
    switch (rcvState) {
        case Waiting:
            if (c == 0xFE)
                rcvState = FunctionMSB;
            break;

        case FunctionMSB:
            msgDecodedFunction = c << 8;
            rcvState = FunctionLSB;
            break;

        case FunctionLSB:
            msgDecodedFunction += c << 0;
            rcvState = PayloadLengthMSB;
            break;

        case PayloadLengthMSB:
            msgDecodedPayloadLength = c << 8;
            rcvState = PayloadLengthLSB;
            break;

        case PayloadLengthLSB:
            msgDecodedPayloadLength += c << 0;
            if (msgDecodedPayloadLength == 0)
                rcvState = Checksum;
            else if (msgDecodedPayloadLength >= 128) {
                rcvState = Waiting;
            } else {
                msgDecodedPayloadIndex = 0;
                rcvState = Payload;
            }
            break;

        case Payload:
            msgDecodedPayload[msgDecodedPayloadIndex] = c;
            msgDecodedPayloadIndex++;
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength) {
                rcvState = Checksum;
            }
            break;

        case Checksum:
            if (c == UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload)) {

                //Success, on a un message valide
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            }

            rcvState = Waiting;
            break;

        default:
            rcvState = Waiting;
            break;
    }
}


void UartProcessDecodedMessage(unsigned char function,
        unsigned char msgPayloadLength, unsigned char* msgPayload) {
    switch (function) {
        case TransmissionText :            
            break;

        case LED:
            if (msgPayloadLength == 2) {
                switch (msgPayload[0]) {
                    case 0:
                        LED_BLANCHE = msgPayload[1];
                        break;
                    case 1:
                        LED_BLEUE = msgPayload[1];
                        break;
                    case 2:
                        LED_ORANGE = msgPayload[1];
                        break;
                }
            }
            break;
        case DistanceTelemIR:            
            ;
            break;

        case MotorSpeed:
            ;
            break;
    }
}


//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
