using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Globalization;

namespace xiaoche
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button_OpenSerial_Click(object sender, EventArgs e)
        {
            bool flag = true;
            while (flag == true)
            {
                flag = false;
                try
                {
                    if (serialPort1.IsOpen)
                    {
                        serialPort1.Close();
                        button_OpenSerial.Text = "打开串口";
                    }
                    else
                    {
                        if (comboBox_PortName.SelectedItem != null)
                            serialPort1.PortName = (string)comboBox_PortName.SelectedItem;
                        serialPort1.Open();
                        button_OpenSerial.Text = "关闭串口";
                    }
                }
                catch
                {
                    if (MessageBox.Show("无法打开串口！", "ERROR!", MessageBoxButtons.RetryCancel,
                        MessageBoxIcon.Error, MessageBoxDefaultButton.Button1) == DialogResult.Retry)

                        flag = true;
                }
            }
        }
        private Thread demoThread = null;
        delegate void SetTextCallback(string text);
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            //int temp;
            //while ((temp = serialPort1.ReadByte()) != -1)
            //{
            //    richTextBox_Received.AppendText(temp.ToString());
            //}
            //richTextBox_Received.AppendText("aa");
            //Thread.Sleep(100);
            this.demoThread =
                new Thread(new ThreadStart(this.ThreadProcSafe));

            this.demoThread.Start();
        }
        private void ThreadProcSafe()
        {
            int temp;
            if ((temp = serialPort1.ReadByte()) != -1)
                this.SetText(" " + Convert.ToString(temp, 16));
        }
        private void SetText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.richTextBox_Received.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.richTextBox_Received.AppendText(text);
            }
        }

        private void button_Clear_Click(object sender, EventArgs e)
        {
            richTextBox_Received.Clear();
        }

        private void button_Send_Click(object sender, EventArgs e)
        {

            if (serialPort1.IsOpen)
            {
                byte[] writebyte = { 0x55, 0xaa, 0x01 };
                List<byte> ww = new List<byte>(writebyte);
                string[] splitstr = new string[] { textBox_data.Text.Substring(0, 2), textBox_data.Text.Substring(2, 2) };
                switch (Convert.ToByte(comboBox_Mode.SelectedValue))
                {
                    case 1:
                        ww.Add(0x01);
                        ww.Add(0x01);
                        //writebyte
                        break;
                    case 2:
                        ww.Add(0x02);
                        ww.Add(0x02);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 3:
                        ww.Add(0x02);
                        ww.Add(0x03);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 4:
                        ww.Add(0x01);
                        ww.Add(0x04);
                        break;
                    case 5:
                        ww.Add(0x02);
                        ww.Add(0x05);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 6:
                        ww.Add(0x02);
                        ww.Add(0x06);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 7:
                        ww.Add(0x02);
                        ww.Add(0x07);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 8:
                        ww.Add(0x03);
                        ww.Add(0x07);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 9:
                        break;
                    default:
                        break;
                }
                try
                {
                    serialPort1.Write(ww.ToArray(), 0, ww.Count);
                }
                catch
                {
                    MessageBox.Show("ERROR!");
                }
            }
            else
            {
                MessageBox.Show("请打开串口！");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: 这行代码将数据加载到表“modeDataSet.模式”中。您可以根据需要移动或移除它。
            this.模式TableAdapter.Fill(this.modeDataSet.模式);
            try
            {
                textBox_data.Text = "01 d0 01 d0";
                comboBox_PortName.SelectedText = "COM1";
                serialPort1.PortName = "COM1";
                serialPort1.Open();
                button_OpenSerial.Text = "关闭串口";
            }
            catch
            {
                MessageBox.Show("无法打开COM1！");
            }
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            //switch(e.KeyValue)
            //    case 
            //serialPort1.Write(new byte[] { 0x55, 0xaa, 0x01, 0x02, 0x02, 0xf0 }, 0, 5); 
            //richTextBox_Received.AppendText(e.KeyValue.ToString());
        }

        private void richTextBox_Received_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                //richTextBox_Received.AppendText(e.KeyValue.ToString());
                byte[] writebyte = { 0x55, 0xaa, 0x01 };
                List<byte> ww = new List<byte>(writebyte);
                string[] splitstr = textBox_data.Text.Split(new char[]{' '}, StringSplitOptions.RemoveEmptyEntries);
                //textBox_data.Text.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

                switch (e.KeyValue)
                {
                    case 37:
                        ww.Add(0x05);
                        ww.Add(0x08);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                            ww.Add(Convert.ToByte(splitstr[2], 16));
                            ww.Add(Convert.ToByte(splitstr[3], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        //serialPort1.Write(new byte[] { 0x55, 0xaa, 0x01, 0x03, 0x08, 0xff, Convert.ToByte(textBox_data.Text, 16) }, 0, 7);
                        break;
                    case 38:
                        ww.Add(0x03);
                        ww.Add(0x03);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 39:
                        ww.Add(0x05);
                        ww.Add(0x08);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[2], 16));
                            ww.Add(Convert.ToByte(splitstr[3], 16));
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));

                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 40:
                        ww.Add(0x03);
                        ww.Add(0x02);
                        if (splitstr.Length >= 2)
                        {
                            ww.Add(Convert.ToByte(splitstr[0], 16));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 81:
                        ww.Add(0x03);
                        ww.Add(0x03);
                        if (splitstr.Length >= 2)
                        {
                            textBox_data.Text = Convert.ToString(Convert.ToInt32(splitstr[0], 16) + 1, 16) + splitstr[1];
                            ww.Add((byte)(Convert.ToByte(splitstr[0], 16) + 1));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 87:
                        ww.Add(0x03);
                        ww.Add(0x03);
                        if (splitstr.Length >= 2)
                        {
                            textBox_data.Text = Convert.ToString(Convert.ToInt32(splitstr[0], 16) - 1, 16) + splitstr[1];
                            ww.Add((byte)(Convert.ToByte(splitstr[0], 16) - 1));
                            ww.Add(Convert.ToByte(splitstr[1], 16));
                        }
                        else
                        {
                            MessageBox.Show("请输入PWM值");
                        }
                        break;
                    case 83:

                        //serialPort1.Write(new byte[] { 0x55, 0xaa, 0x01, 0x03, 0x02, 0x00, 0xee }, 0, 7);
                        //Thread.Sleep(250);
                        ww.Add(0x01);
                        ww.Add(0x04);
                        break;
                }
                serialPort1.Write(ww.ToArray(), 0, ww.Count);
            }
            catch
            {
                MessageBox.Show("ERROR!");
            }
        }

        private void richTextBox_Received_KeyUp(object sender, KeyEventArgs e)
        {

        }
    }
}