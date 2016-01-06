namespace Flight_simulator_menu
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.editConfigButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.longitudeTextBox = new System.Windows.Forms.TextBox();
            this.latitudeTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.hightTextBox = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.GdańskButton = new System.Windows.Forms.Button();
            this.himalajeButton = new System.Windows.Forms.Button();
            this.ColoradoButton = new System.Windows.Forms.Button();
            this.runButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(74, 4);
            // 
            // editConfigButton
            // 
            this.editConfigButton.Location = new System.Drawing.Point(212, 22);
            this.editConfigButton.Name = "editConfigButton";
            this.editConfigButton.Size = new System.Drawing.Size(221, 111);
            this.editConfigButton.TabIndex = 1;
            this.editConfigButton.Text = "EDYTUJ PLIK KONFIGURACYJNY";
            this.editConfigButton.UseVisualStyleBackColor = true;
            this.editConfigButton.Click += new System.EventHandler(this.editConfigButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label1.Location = new System.Drawing.Point(164, 172);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(381, 29);
            this.label1.TabIndex = 2;
            this.label1.Text = "WSPÓŁRZĘDNE POCZATKOWE:";
            // 
            // longitudeTextBox
            // 
            this.longitudeTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.longitudeTextBox.Location = new System.Drawing.Point(111, 302);
            this.longitudeTextBox.Name = "longitudeTextBox";
            this.longitudeTextBox.Size = new System.Drawing.Size(196, 35);
            this.longitudeTextBox.TabIndex = 3;
            this.longitudeTextBox.Text = "0";
            this.longitudeTextBox.TextChanged += new System.EventHandler(this.longitudeTextBox_TextChanged);
            // 
            // latitudeTextBox
            // 
            this.latitudeTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.latitudeTextBox.Location = new System.Drawing.Point(111, 437);
            this.latitudeTextBox.Name = "latitudeTextBox";
            this.latitudeTextBox.Size = new System.Drawing.Size(196, 35);
            this.latitudeTextBox.TabIndex = 4;
            this.latitudeTextBox.Text = "0";
            this.latitudeTextBox.TextChanged += new System.EventHandler(this.latitudeTextBox_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label2.Location = new System.Drawing.Point(106, 229);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(201, 58);
            this.label2.TabIndex = 5;
            this.label2.Text = "DŁUGOŚĆ\r\nGEOGRAFICZNA";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label3.Location = new System.Drawing.Point(106, 362);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(201, 58);
            this.label3.TabIndex = 6;
            this.label3.Text = "SZEROKOŚĆ\r\nGEOGRAFICZNA";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label4.Location = new System.Drawing.Point(47, 497);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(329, 29);
            this.label4.TabIndex = 7;
            this.label4.Text = "WYSOKOSC POCZATKOWA";
            // 
            // hightTextBox
            // 
            this.hightTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.hightTextBox.Location = new System.Drawing.Point(111, 545);
            this.hightTextBox.Name = "hightTextBox";
            this.hightTextBox.Size = new System.Drawing.Size(196, 35);
            this.hightTextBox.TabIndex = 8;
            this.hightTextBox.Text = "0";
            this.hightTextBox.TextChanged += new System.EventHandler(this.hightTextBox_TextChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(450, 280);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(211, 57);
            this.button1.TabIndex = 9;
            this.button1.Text = "ZAKOPANE";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.zakopaneButton_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label5.Location = new System.Drawing.Point(461, 229);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(200, 29);
            this.label5.TabIndex = 10;
            this.label5.Text = "PREFEROWANE";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // GdańskButton
            // 
            this.GdańskButton.Location = new System.Drawing.Point(450, 343);
            this.GdańskButton.Name = "GdańskButton";
            this.GdańskButton.Size = new System.Drawing.Size(211, 57);
            this.GdańskButton.TabIndex = 11;
            this.GdańskButton.Text = "GDAŃSK";
            this.GdańskButton.UseVisualStyleBackColor = true;
            this.GdańskButton.Click += new System.EventHandler(this.GdańskButton_Click);
            // 
            // himalajeButton
            // 
            this.himalajeButton.Location = new System.Drawing.Point(450, 406);
            this.himalajeButton.Name = "himalajeButton";
            this.himalajeButton.Size = new System.Drawing.Size(211, 57);
            this.himalajeButton.TabIndex = 12;
            this.himalajeButton.Text = "HIMALAJE";
            this.himalajeButton.UseVisualStyleBackColor = true;
            this.himalajeButton.Click += new System.EventHandler(this.himalajeButton_Click);
            // 
            // ColoradoButton
            // 
            this.ColoradoButton.Location = new System.Drawing.Point(450, 469);
            this.ColoradoButton.Name = "ColoradoButton";
            this.ColoradoButton.Size = new System.Drawing.Size(211, 57);
            this.ColoradoButton.TabIndex = 13;
            this.ColoradoButton.Text = "KANION COLORADO";
            this.ColoradoButton.UseVisualStyleBackColor = true;
            this.ColoradoButton.Click += new System.EventHandler(this.ColoradoButton_Click);
            // 
            // runButton
            // 
            this.runButton.Location = new System.Drawing.Point(212, 638);
            this.runButton.Name = "runButton";
            this.runButton.Size = new System.Drawing.Size(221, 111);
            this.runButton.TabIndex = 14;
            this.runButton.Text = "URUCHOM";
            this.runButton.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(739, 788);
            this.Controls.Add(this.runButton);
            this.Controls.Add(this.ColoradoButton);
            this.Controls.Add(this.himalajeButton);
            this.Controls.Add(this.GdańskButton);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.hightTextBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.latitudeTextBox);
            this.Controls.Add(this.longitudeTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.editConfigButton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.Button editConfigButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox longitudeTextBox;
        private System.Windows.Forms.TextBox latitudeTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox hightTextBox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button GdańskButton;
        private System.Windows.Forms.Button himalajeButton;
        private System.Windows.Forms.Button ColoradoButton;
        private System.Windows.Forms.Button runButton;
    }
}

