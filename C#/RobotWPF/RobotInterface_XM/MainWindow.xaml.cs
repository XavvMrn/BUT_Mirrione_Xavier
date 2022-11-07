using System;
using System.Collections.Generic;
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


namespace RobotInterface_XM
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        string receivedText;

        public MainWindow()
        {
            InitializeComponent();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
            }

        //if (robot.receivedText != "")
        //    textBoxReception.Text += robot.receivedText;
        //robot.receivedText = "";

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
            string selectxt = textBoxEnvoi.Text;
            textBoxReception.Text = "Recu : " + selectxt;
            textBoxEnvoi.Text = "";
        }

        private void textBoxEnvoi_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {                
                SendMessage();
            }
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            textBoxReception.Text += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

    }
}
