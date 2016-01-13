using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Globalization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Flight_simulator_menu
{
    public partial class mapDownloader : Form
    {
        public mapDownloader()
        {
            InitializeComponent();
        }

        private void runButton_Click(object sender, EventArgs e)
        {
            string parameters = latitude1TextBox.Text + " " + longitude1TextBox.Text + " " + latitude2TextBox.Text + " " + longitude2TextBox.Text + " " + qualityTextBox.Text;
            System.Diagnostics.Process.Start("Flight simulator map.exe", parameters);
            Application.Exit();
        }

        private bool doubleTextChenge(string Text)
        {
            try
            {
                NumberFormatInfo format = new NumberFormatInfo();
                format.NumberGroupSeparator = ",";
                format.NumberDecimalSeparator = ".";
                double value = Double.Parse(Text, format);
                return true;
            }
            catch (Exception)
            {
                return false;
                throw;
            }
        }

        private void longitude1TextBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(longitude1TextBox.Text))
                longitude1TextBox.Text = "0";
        }

        private void longitude2TextBox_TextChanged(object sender, EventArgs e)
        {

            if (!doubleTextChenge(longitude2TextBox.Text))
                longitude2TextBox.Text = "0";
        }

        private void latitude1TextBox_TextChanged(object sender, EventArgs e)
        {

            if (!doubleTextChenge(latitude1TextBox.Text))
                latitude1TextBox.Text = "0";
        }

        private void latitude2TextBox_TextChanged(object sender, EventArgs e)
        {

            if (!doubleTextChenge(latitude2TextBox.Text))
                latitude2TextBox.Text = "0";
        }

        private void qualityTextBox_TextChanged(object sender, EventArgs e)
        {

            if (!doubleTextChenge(qualityTextBox.Text))
                qualityTextBox.Text = "0";
        }
    }
}
