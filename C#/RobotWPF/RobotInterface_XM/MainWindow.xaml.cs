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

            serialPort1 = new ReliableSerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
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
                textBoxReception.Text += "0x" + c.ToString("X2") + " ";
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
            UartEncodeAndSendMessage(msgFunction, msgPayloadLength,msgPayload);
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
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedFunction += c << 0;

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
                    msgDecodedPayload[msgDecodedPayloadIndex] = c ;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    {                        
                        rcvState = StateReception.Checksum;
                    }
                    break;

                case StateReception.Checksum:
                    if(c==CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload))
                    {
                        textBoxReception.Text = "OK";
                        //Success, on a un message valide
                    }
                    else
                    {
                        textBoxReception.Text = "NOK";
                    }
                    rcvState = StateReception.Waiting;
                    break;

                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        } 

    }
}


// byte[] array = Encoding.ASCII.GetBytes(s);