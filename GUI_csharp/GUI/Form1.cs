using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.Globalization;

namespace GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
            



            
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "Connect")
            {
                serialPort1.PortName = comboBox1.Text;
                try
                {
                    serialPort1.Open();
                    button2.Enabled = true;
                    button3.Enabled = true;
                    button4.Enabled = true;
                    button5.Enabled = true;
                    button1.Text = "Disconnect";
                }
                catch (Exception ex) { }
            }
            else
            {
                try
                {
                    serialPort1.Close();
                    button2.Enabled = false;
                    button3.Enabled = false;
                    button4.Enabled = false;
                    button5.Enabled = false;
                    button1.Text = "Connect";
                }
                catch (Exception ex) { }
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            string str = textBox1.Text;
            byte add1,add2,add3;
            string[] split = new string[str.Length / 2 + (str.Length % 2 == 0 ? 0 : 1)];
            for (int i = 0; i < split.Length; i++)
            {
                split[i] = str.Substring(i * 2, i * 2 + 2 > str.Length ? 1 : 2);
            }
            if (split.Length < 3) return;
            add1 = (Convert.ToByte(split[0], 16));
            add2 = (Convert.ToByte(split[1], 16));
            add3 = (Convert.ToByte(split[2], 16));
            //string hex_value = "12";
            //int int_value = Convert.ToInt32(hex_value, 16);


            int delay = 100;
            byte [] send = new byte[2];
            send[0] = 0xF1;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add1;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add2;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add3;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = 0x01;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            MessageBox.Show("OK");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string str = textBox1.Text;
            byte add1, add2, add3;
            string[] split = new string[str.Length / 2 + (str.Length % 2 == 0 ? 0 : 1)];
            for (int i = 0; i < split.Length; i++)
            {
                split[i] = str.Substring(i * 2, i * 2 + 2 > str.Length ? 1 : 2);
            }
            if (split.Length < 3) return;
            add1 = (Convert.ToByte(split[0], 16));
            add2 = (Convert.ToByte(split[1], 16));
            add3 = (Convert.ToByte(split[2], 16));
            
            int delay = 100;
            byte[] send = new byte[2];
            send[0] = 0xF1;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add1;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add2;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add3;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = 0x00;
            //serialPort1.WriteLine(send.ToString());
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            MessageBox.Show("OK");
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            char c = e.KeyChar;

            if (c != '\b' && !((c <= 0x66 && c >= 61) || (c <= 0x46 && c >= 0x41) || (c >= 0x30 && c <= 0x39)))
            {
                e.Handled = true;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            DateTime dt = dateTimePicker1.Value;
            string str = textBox1.Text;
            byte add1, add2, add3;
            byte hour, minute, second;
            string[] split = new string[str.Length / 2 + (str.Length % 2 == 0 ? 0 : 1)];
            for (int i = 0; i < split.Length; i++)
            {
                split[i] = str.Substring(i * 2, i * 2 + 2 > str.Length ? 1 : 2);
            }
            if (split.Length < 3) return;
            add1 = (Convert.ToByte(split[0], 16));
            add2 = (Convert.ToByte(split[1], 16));
            add3 = (Convert.ToByte(split[2], 16));
            
            hour = Convert.ToByte(dt.Hour);
            minute = Convert.ToByte(dt.Minute);
            second = Convert.ToByte(dt.Second);

            send_com(1, add1, add2, add3, hour);
            send_com(2, add1, add2, add3, minute);
            send_com(3, add1, add2, add3, second);
            MessageBox.Show("OK");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            DateTime dt = dateTimePicker2.Value;
            string str = textBox1.Text;
            byte add1, add2, add3;
            byte hour, minute, second;
            string[] split = new string[str.Length / 2 + (str.Length % 2 == 0 ? 0 : 1)];
            for (int i = 0; i < split.Length; i++)
            {
                split[i] = str.Substring(i * 2, i * 2 + 2 > str.Length ? 1 : 2);
            }
            if (split.Length < 3) return;
            add1 = (Convert.ToByte(split[0], 16));
            add2 = (Convert.ToByte(split[1], 16));
            add3 = (Convert.ToByte(split[2], 16));

            hour = Convert.ToByte(dt.Hour);
            minute = Convert.ToByte(dt.Minute);
            second = Convert.ToByte(dt.Second);

            send_com(4, add1, add2, add3, hour);
            send_com(5, add1, add2, add3, minute);
            send_com(6, add1, add2, add3, second);
            MessageBox.Show("OK");
        }
        private void send_com(int type,byte add1, byte add2, byte add3, byte value) {
            int delay = 200;
            byte[] send = new byte[2];
            if (type == 1) send[0] = 0xE1;
            else if (type == 2) send[0] = 0xE2;
            else if (type == 3) send[0] = 0xE3;
            else if (type == 4) send[0] = 0xD1;
            else if (type == 5) send[0] = 0xD2;
            else send[0] = 0xD3;
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add1;
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add2;
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = add3;
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            send[0] = value;
            serialPort1.Write(send, 0, 1);
            Thread.Sleep(delay);
            
        }
    }
}
