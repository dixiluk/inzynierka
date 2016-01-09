using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Flight_simulator_menu
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void editConfigButton_Click(object sender, EventArgs e)
        {
            Config config = new Config();
            config.ShowDialog();
        }

        private double doubleTextChenge(string Text)
        {
            try
            {
                NumberFormatInfo format = new NumberFormatInfo();
                format.NumberGroupSeparator = ",";
                format.NumberDecimalSeparator = ".";
                double value = Double.Parse(Text, format);
                return value;
            }
            catch (Exception)
            {
                return 0;
                throw;
            }
        }

        private void longitudeTextBox_TextChanged(object sender, EventArgs e)
        {
            NumberFormatInfo format = new NumberFormatInfo();
            format.NumberGroupSeparator = ",";
            format.NumberDecimalSeparator = ".";
            double value = doubleTextChenge(longitudeTextBox.Text);
            if (value > 180) value = value = 180;
            if (value < -180) value = value = -180;
            longitudeTextBox.Text = value.ToString(format);
        }

        private void latitudeTextBox_TextChanged(object sender, EventArgs e)
        {
            NumberFormatInfo format = new NumberFormatInfo();
            format.NumberGroupSeparator = ",";
            format.NumberDecimalSeparator = ".";
            double value = doubleTextChenge(latitudeTextBox.Text);
            if (value > 90) value = value = 90;
            if (value < -90) value = value = -90;
            latitudeTextBox.Text = value.ToString(format);
        }

        private void hightTextBox_TextChanged(object sender, EventArgs e)
        {
            NumberFormatInfo format = new NumberFormatInfo();
            format.NumberGroupSeparator = ",";
            format.NumberDecimalSeparator = ".";
            double value = doubleTextChenge(hightTextBox.Text);
            if (value < 0) value = value = 0;
            hightTextBox.Text = value.ToString(format);
        }

        private void zakopaneButton_Click(object sender, EventArgs e)
        {
            latitudeTextBox.Text = "49.25";
            longitudeTextBox.Text = "19.95";
            hightTextBox.Text = "2";
        }

        private void himalajeButton_Click(object sender, EventArgs e)
        {
            latitudeTextBox.Text = "27.95";
            longitudeTextBox.Text = "86.95";
            hightTextBox.Text = "8";

        }

        private void GdańskButton_Click(object sender, EventArgs e)
        {
            latitudeTextBox.Text = "54.36";
            longitudeTextBox.Text = "18.64";
            hightTextBox.Text = "2";

        }

        private void ColoradoButton_Click(object sender, EventArgs e)
        {
            latitudeTextBox.Text = "36.11";
            longitudeTextBox.Text = "-113.93";
            hightTextBox.Text = "2";
        }

        private void runButton_Click(object sender, EventArgs e)
        {

        System.Diagnostics.Process.Start("Flight simulator map.exe", latitudeTextBox.Text+" "+ longitudeTextBox.Text + " " + hightTextBox.Text);
        Application.Exit();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            mapDownloader downloader = new mapDownloader();
            downloader.ShowDialog();
        }
    }
}
