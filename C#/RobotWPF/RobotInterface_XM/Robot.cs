using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterface_XM
{
public class Robot
    {
        public string receivedText = "";
        public float distanceTelemetreDroit;
        public float distanceTelemetreCentre;
        public float distanceTelemetreGauche;

        public int MotorSpeedGauche;
        public int MotorSpeedCentre;
        public Queue<byte> byteListReceived = new Queue<byte>();
        public Robot()
        {

        }
    }
}
