using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace RobotInterface_XM
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        //string receivedText;
        Robot robot;

        public MainWindow()
        {
            InitializeComponent();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            serialPort1 = new ReliableSerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            robot = new Robot();
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            /*if (robot.receivedText != "")
            {
                textBoxReception.Text += robot.receivedText;
                robot.receivedText = "";
            }*/
            while (robot.byteListReceived.Count > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                //textBoxReception.Text += "0x" + c.ToString("X2") + " ";
                DecodeMessage(c);
            }

        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

            foreach (var lastw in e.Data)
            {
                robot.byteListReceived.Enqueue(lastw);
            }
        }


        bool toggle = false;
        private void ButtonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (toggle)
            {
                ButtonEnvoyer.Background = Brushes.Beige;
            }
            else
            {
                ButtonEnvoyer.Background = Brushes.RoyalBlue;
            }
            toggle = !toggle;
            SendMessage();
        }

        private void SendMessage()
        {
            // Envoie msg Emiss° à Recept°
            // string selectxt = textBoxEnvoi.Text;
            serialPort1.WriteLine(textBoxEnvoi.Text);
            textBoxEnvoi.Text = "";
        }

        private void textBoxEnvoi_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = "";
        }

        private void ButtonTest_Click(object sender, RoutedEventArgs e)
        {
            /*byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, 20);*/
            int msgFunction = (int)0x0080;
            string payload = "Bonjour";
            int msgPayloadLength = payload.Length;
            byte[] msgPayload = Encoding.ASCII.GetBytes(payload);
            UartEncodeAndSendMessage(msgFunction, msgPayloadLength, msgPayload);

            // LED
            msgPayload = new byte[] { 0, 1 };
            UartEncodeAndSendMessage((int)IdFunction.LED, 2, msgPayload);
            msgPayload = new byte[] { 1, 1 };
            UartEncodeAndSendMessage((int)IdFunction.LED, 2, msgPayload);
            msgPayload = new byte[] { 2, 1 };
            UartEncodeAndSendMessage((int)IdFunction.LED, 2, msgPayload);

            // IR
            msgPayload = new byte[] { 45, 67, 89 };
            UartEncodeAndSendMessage((int)IdFunction.DistanceTelemIR, 3, msgPayload);

            // MotorSpeed
            msgPayload = new byte[] { 45, 50 };
            UartEncodeAndSendMessage((int)IdFunction.MotorSpeed, 2, msgPayload);
        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;   // SOF du formatage
            checksum ^= (byte)(msgFunction >> 8); // >> 8 étant un décalage de 8 bits 
            checksum ^= (byte)(msgFunction >> 0); // pour homogéniser
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }
        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }
            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            serialPort1.Write(message, 0, msgPayloadLength + 6);

        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            Checksum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;
                    break;

                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;

                    if (msgDecodedPayloadLength == 0)
                        rcvState = StateReception.Checksum;
                    else
                    {
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                        rcvState = StateReception.Payload;
                    }
                    break;

                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    {
                        rcvState = StateReception.Checksum;
                    }
                    break;

                case StateReception.Checksum:
                    if (c == CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload))
                    {
                        textBoxReception.Text += " OK\n";
                        //Success, on a un message valide
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        textBoxReception.Text += " Not OK\n";
                    }
                    rcvState = StateReception.Waiting;
                    break;

                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        public enum IdFunction
        {
            TransmissionText = 0x0080,
            LED = 0x0020,
            DistanceTelemIR = 0x0030,
            MotorSpeed = 0x0040,
            State = 0x0050,
        }
        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15,
            STATE_RECULE_PAR_GAUCHE = 16,
            STATE_RECULE_PAR_GAUCHE_EN_COURS = 17,
            STATE_RECULE_PAR_DROITE = 18,
            STATE_RECULE_PAR_DROITE_EN_COURS = 19,
            STATE_RECULE_UN_PEU_GAUCHE = 20,
            STATE_RECULE_UN_PEU_GAUCHE_EN_COURS = 21,
            STATE_RECULE_UN_PEU_DROITE = 22,
            STATE_RECULE_UN_PEU_DROITE_EN_COURS = 23,
            STATE_TOURNE_UN_PEU_GAUCHE = 24,
            STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS = 25,
            STATE_TOURNE_UN_PEU_DROITE = 26,
            STATE_TOURNE_BCP_GAUCHE_EN_COURS = 29,
            STATE_TOURNE_BCP_DROITE = 30,
            STATE_TOURNE_BCP_DROITE_EN_COURS = 31 ,
            STATE_180 = 32
        }

        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch ((IdFunction)msgFunction)
            {
                case IdFunction.TransmissionText:
                    textBoxReception.Text += Encoding.ASCII.GetString(msgPayload);
                    break;

                case IdFunction.LED:
                    if(msgPayloadLength==2)
                    {
                        switch(msgPayload[0])
                        {
                            case 0:
                                LED1.IsChecked = (msgPayload[1] == 1);
                                break;
                            case 1:
                                LED2.IsChecked = (msgPayload[1] == 1);
                                break;
                            case 2:
                                LED3.IsChecked = (msgPayload[1] == 1);
                                break;
                        }
                    }
                    break;
                case IdFunction.DistanceTelemIR:
                    if (msgPayloadLength == 3)
                    {
                        robot.distanceTelemetreGauche = (float)msgPayload[0];
                        robot.distanceTelemetreCentre = (float)msgPayload[1];
                        robot.distanceTelemetreDroit = (float)msgPayload[2];
                        TelemtextBox.Text = "IR Gauche : " + robot.distanceTelemetreGauche.ToString("N2");
                        TelemtextBox1.Text = "IR Centre : " + robot.distanceTelemetreCentre.ToString("N2");
                        TelemtextBox2.Text = "IR Droit : " + robot.distanceTelemetreDroit.ToString("N2");
                    }

                    break;

                case IdFunction.MotorSpeed:
                    if (msgPayloadLength == 2)
                    {
                        robot.MotorSpeedGauche = (int)msgPayload[0];
                        robot.MotorSpeedCentre = (int)msgPayload[1];
                        MotortextBox.Text = "Vitesse Gauche : " + robot.MotorSpeedGauche.ToString("N2") + " %";
                        MotortextBox1.Text = "Vitesse Droit : " + robot.MotorSpeedCentre.ToString("N2") + " %";
                    }
                    break;
                case IdFunction.State:
                    int instant = (((int)msgPayload[1]) << 24) 
                        + (((int)msgPayload[2]) << 16)+ (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    textBoxReception.Text += "Nouvel état : "
                        + ((StateRobot)(msgPayload[0])).ToString() + "␣-␣" + instant.ToString() + "␣ms"; ;
                    break;
            }
        }

        private void checkBox_Checked(object sender, RoutedEventArgs e)
        {

        }
    }
}


// byte[] array = Encoding.ASCII.GetBytes(s);