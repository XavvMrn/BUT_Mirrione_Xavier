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
            //throw new NotImplementedException();

            if (robot.receivedText != "")
            {
                textBoxReception.Text += robot.receivedText;
                robot.receivedText = "";
            }
                       
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

            //throw new NotImplementedException();
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
            if(e.Key == Key.Enter)
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
            byte[] byteList = new byte[20];
            for(int i = 0; i<20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, 20);
        }


    }
}
