#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "Robot.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
 
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
unsigned char* message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (unsigned char)(msgFunction >> 8);
            message[pos++] = (unsigned char)(msgFunction >> 0);
            message[pos++] = (unsigned char)(msgPayloadLength >> 8);
            message[pos++] = (unsigned char)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }
            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            serialPort1.Write(message, 0, msgPayloadLength + 6);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c)
{
//Fonction prenant en entrée un octet et servant à reconstituer les trames
 switch (rcvState)
            {
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
                        rcvState = StateReception.Checksum;
                    else
                    {
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                        rcvState = Payload;
                    }
                    break;

                case Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    {
                        rcvState = Checksum;
                    }
                    break;

                case Checksum:
                    if (c == CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload))
                    {
                        
                        //Success, on a un message valide
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                   
                    rcvState = Waiting;
                    break;

                default:
                    rcvState = Waiting;
                    break;
            }
}

void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload)
{
//Fonction appelée après le décodage pour exécuter l?action
//correspondant au message reçu
?
}

 

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
