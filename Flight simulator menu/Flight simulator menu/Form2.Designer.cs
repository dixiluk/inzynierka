namespace Flight_simulator_menu
{
    partial class mapDownloader
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.runButton = new System.Windows.Forms.Button();
            this.qualityTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.latitude1TextBox = new System.Windows.Forms.TextBox();
            this.longitude1TextBox = new System.Windows.Forms.TextBox();
            this.longitude2TextBox = new System.Windows.Forms.TextBox();
            this.latitude2TextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // runButton
            // 
            this.runButton.Location = new System.Drawing.Point(147, 497);
            this.runButton.Name = "runButton";
            this.runButton.Size = new System.Drawing.Size(221, 111);
            this.runButton.TabIndex = 21;
            this.runButton.Text = "URUCHOM";
            this.runButton.UseVisualStyleBackColor = true;
            this.runButton.Click += new System.EventHandler(this.runButton_Click);
            // 
            // qualityTextBox
            // 
            this.qualityTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.qualityTextBox.Location = new System.Drawing.Point(162, 437);
            this.qualityTextBox.Name = "qualityTextBox";
            this.qualityTextBox.Size = new System.Drawing.Size(196, 35);
            this.qualityTextBox.TabIndex = 20;
            this.qualityTextBox.Text = "0";
            this.qualityTextBox.TextChanged += new System.EventHandler(this.qualityTextBox_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label4.Location = new System.Drawing.Point(127, 380);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(278, 29);
            this.label4.TabIndex = 19;
            this.label4.Text = "MAKSYMALNA JAKOSC";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label3.Location = new System.Drawing.Point(157, 204);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(201, 58);
            this.label3.TabIndex = 18;
            this.label3.Text = "SZEROKOŚĆ\r\nGEOGRAFICZNA";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label2.Location = new System.Drawing.Point(157, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(201, 58);
            this.label2.TabIndex = 17;
            this.label2.Text = "DŁUGOŚĆ\r\nGEOGRAFICZNA";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // latitude1TextBox
            // 
            this.latitude1TextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.latitude1TextBox.Location = new System.Drawing.Point(30, 302);
            this.latitude1TextBox.Name = "latitude1TextBox";
            this.latitude1TextBox.Size = new System.Drawing.Size(196, 35);
            this.latitude1TextBox.TabIndex = 16;
            this.latitude1TextBox.Text = "0";
            this.latitude1TextBox.TextChanged += new System.EventHandler(this.latitude1TextBox_TextChanged);
            // 
            // longitude1TextBox
            // 
            this.longitude1TextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.longitude1TextBox.Location = new System.Drawing.Point(30, 144);
            this.longitude1TextBox.Name = "longitude1TextBox";
            this.longitude1TextBox.Size = new System.Drawing.Size(196, 35);
            this.longitude1TextBox.TabIndex = 15;
            this.longitude1TextBox.Text = "0";
            this.longitude1TextBox.TextChanged += new System.EventHandler(this.longitude1TextBox_TextChanged);
            // 
            // longitude2TextBox
            // 
            this.longitude2TextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.longitude2TextBox.Location = new System.Drawing.Point(290, 144);
            this.longitude2TextBox.Name = "longitude2TextBox";
            this.longitude2TextBox.Size = new System.Drawing.Size(196, 35);
            this.longitude2TextBox.TabIndex = 22;
            this.longitude2TextBox.Text = "0";
            this.longitude2TextBox.TextChanged += new System.EventHandler(this.longitude2TextBox_TextChanged);
            // 
            // latitude2TextBox
            // 
            this.latitude2TextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.latitude2TextBox.Location = new System.Drawing.Point(290, 302);
            this.latitude2TextBox.Name = "latitude2TextBox";
            this.latitude2TextBox.Size = new System.Drawing.Size(196, 35);
            this.latitude2TextBox.TabIndex = 23;
            this.latitude2TextBox.Text = "0";
            this.latitude2TextBox.TextChanged += new System.EventHandler(this.latitude2TextBox_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label1.Location = new System.Drawing.Point(107, 99);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 29);
            this.label1.TabIndex = 24;
            this.label1.Text = "OD";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label5.Location = new System.Drawing.Point(107, 260);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(49, 29);
            this.label5.TabIndex = 25;
            this.label5.Text = "OD";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label6.Location = new System.Drawing.Point(368, 260);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(49, 29);
            this.label6.TabIndex = 26;
            this.label6.Text = "DO";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label7.Location = new System.Drawing.Point(368, 99);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(49, 29);
            this.label7.TabIndex = 27;
            this.label7.Text = "DO";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // mapDownloader
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(516, 628);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.latitude2TextBox);
            this.Controls.Add(this.longitude2TextBox);
            this.Controls.Add(this.runButton);
            this.Controls.Add(this.qualityTextBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.latitude1TextBox);
            this.Controls.Add(this.longitude1TextBox);
            this.Name = "mapDownloader";
            this.Text = "Pobieranie mapy";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button runButton;
        private System.Windows.Forms.TextBox qualityTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox latitude1TextBox;
        private System.Windows.Forms.TextBox longitude1TextBox;
        private System.Windows.Forms.TextBox longitude2TextBox;
        private System.Windows.Forms.TextBox latitude2TextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
    }
}