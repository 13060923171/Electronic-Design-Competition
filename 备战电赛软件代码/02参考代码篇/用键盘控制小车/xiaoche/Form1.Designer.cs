namespace xiaoche
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.button_OpenSerial = new System.Windows.Forms.Button();
            this.textBox_data = new System.Windows.Forms.TextBox();
            this.comboBox_PortName = new System.Windows.Forms.ComboBox();
            this.comboBox_Mode = new System.Windows.Forms.ComboBox();
            this.模式BindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.modeDataSet = new xiaoche.modeDataSet();
            this.button_Send = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.richTextBox_Received = new System.Windows.Forms.RichTextBox();
            this.button_Clear = new System.Windows.Forms.Button();
            this.模式TableAdapter = new xiaoche.modeDataSetTableAdapters.模式TableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.模式BindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.modeDataSet)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 19200;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // button_OpenSerial
            // 
            this.button_OpenSerial.Location = new System.Drawing.Point(43, 93);
            this.button_OpenSerial.Name = "button_OpenSerial";
            this.button_OpenSerial.Size = new System.Drawing.Size(75, 23);
            this.button_OpenSerial.TabIndex = 0;
            this.button_OpenSerial.Text = "打开串口";
            this.button_OpenSerial.UseVisualStyleBackColor = true;
            this.button_OpenSerial.Click += new System.EventHandler(this.button_OpenSerial_Click);
            // 
            // textBox_data
            // 
            this.textBox_data.Location = new System.Drawing.Point(57, 66);
            this.textBox_data.Name = "textBox_data";
            this.textBox_data.Size = new System.Drawing.Size(218, 21);
            this.textBox_data.TabIndex = 1;
            // 
            // comboBox_PortName
            // 
            this.comboBox_PortName.FormattingEnabled = true;
            this.comboBox_PortName.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4"});
            this.comboBox_PortName.Location = new System.Drawing.Point(57, 35);
            this.comboBox_PortName.Name = "comboBox_PortName";
            this.comboBox_PortName.Size = new System.Drawing.Size(67, 20);
            this.comboBox_PortName.TabIndex = 2;
            // 
            // comboBox_Mode
            // 
            this.comboBox_Mode.DataSource = this.模式BindingSource;
            this.comboBox_Mode.DisplayMember = "模式";
            this.comboBox_Mode.FormattingEnabled = true;
            this.comboBox_Mode.Location = new System.Drawing.Point(165, 35);
            this.comboBox_Mode.Name = "comboBox_Mode";
            this.comboBox_Mode.Size = new System.Drawing.Size(110, 20);
            this.comboBox_Mode.TabIndex = 3;
            this.comboBox_Mode.ValueMember = "值";
            // 
            // 模式BindingSource
            // 
            this.模式BindingSource.DataMember = "模式";
            this.模式BindingSource.DataSource = this.modeDataSet;
            // 
            // modeDataSet
            // 
            this.modeDataSet.DataSetName = "modeDataSet";
            this.modeDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // button_Send
            // 
            this.button_Send.Location = new System.Drawing.Point(124, 93);
            this.button_Send.Name = "button_Send";
            this.button_Send.Size = new System.Drawing.Size(75, 23);
            this.button_Send.TabIndex = 4;
            this.button_Send.Text = "发送";
            this.button_Send.UseVisualStyleBackColor = true;
            this.button_Send.Click += new System.EventHandler(this.button_Send_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "串口名";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(130, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "模式";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "命令";
            // 
            // richTextBox_Received
            // 
            this.richTextBox_Received.Location = new System.Drawing.Point(12, 122);
            this.richTextBox_Received.Name = "richTextBox_Received";
            this.richTextBox_Received.Size = new System.Drawing.Size(268, 132);
            this.richTextBox_Received.TabIndex = 8;
            this.richTextBox_Received.Text = "";
            this.richTextBox_Received.KeyDown += new System.Windows.Forms.KeyEventHandler(this.richTextBox_Received_KeyDown);
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(205, 93);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(75, 23);
            this.button_Clear.TabIndex = 9;
            this.button_Clear.Text = "清空";
            this.button_Clear.UseVisualStyleBackColor = true;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
            // 
            // 模式TableAdapter
            // 
            this.模式TableAdapter.ClearBeforeFill = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.richTextBox_Received);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button_Send);
            this.Controls.Add(this.comboBox_Mode);
            this.Controls.Add(this.comboBox_PortName);
            this.Controls.Add(this.textBox_data);
            this.Controls.Add(this.button_OpenSerial);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            ((System.ComponentModel.ISupportInitialize)(this.模式BindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.modeDataSet)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button button_OpenSerial;
        private System.Windows.Forms.TextBox textBox_data;
        private System.Windows.Forms.ComboBox comboBox_PortName;
        private System.Windows.Forms.ComboBox comboBox_Mode;
        private System.Windows.Forms.Button button_Send;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox richTextBox_Received;
        private System.Windows.Forms.Button button_Clear;
        private modeDataSet modeDataSet;
        private System.Windows.Forms.BindingSource 模式BindingSource;
        private xiaoche.modeDataSetTableAdapters.模式TableAdapter 模式TableAdapter;
    }
}

