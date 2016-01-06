using System;
using System.Collections.Generic;
using System.Globalization;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Flight_simulator_menu
{
    public partial class Config : Form
    {
        public Config()
        {
            InitializeComponent();
        }

        private void Config_Load(object sender, EventArgs e)
        {
            reloadButton_Click(sender, e);
        }

        private void reloadButton_Click(object sender, EventArgs e)
        {
            string[] lines = System.IO.File.ReadAllLines("config.txt");
            string[] parameters = new string[30];
            string[] values = new string[30];
            int cout = 0;
            foreach (string line in lines)
            {
                parameters[cout] = "";
                values[cout] = "";
                bool value = false;
                for(int i = 0; i < line.Length; i++)
                {
                    if (line[i] != '=')
                    {
                        if (value)
                            values[cout] += line[i];
                        else
                            parameters[cout] += line[i];
                    }
                    else
                        value = true;
                }
                cout++;
            }
            for (int i = 0; i < 30; i++)
            {
                switch (parameters[i])
                {
                    case "elevationRows":
                        {
                            elevationRowsTextBox.Text = values[i];
                            break;
                        }
                    case "elevationCols":
                        {
                            elevationColsTextBox.Text = values[i];
                            break;
                        }
                    case "dataServer":
                        {
                            dataServerTextBox.Text = values[i];
                            break;
                        }
                    case "licenceKey":
                        {
                            licenceKeyTextBox.Text = values[i];
                            break;
                        }
                    case "saveDataOnDrive":
                        {
                            if (values[i] == "false")
                                saveDataOnDriveCheckBox.Checked = false;
                            else
                                saveDataOnDriveCheckBox.Checked = true;
                            break;

                        }
                    case "prefImageSizeX":
                        {
                            prefImageSizeXTextBox.Text = values[i];
                            break;
                        }
                    case "prefImageSizeY":
                        {
                            prefImageSizeYTextBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail16":
                        {
                            LevelOfDetail16textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail15":
                        {
                            LevelOfDetail15textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail14":
                        {
                            LevelOfDetail14textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail13":
                        {
                            LevelOfDetail13textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail12":
                        {
                            LevelOfDetail12textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail11":
                        {
                            LevelOfDetail11textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail10":
                        {
                            LevelOfDetail10textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail9":
                        {
                            LevelOfDetail9textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail8":
                        {
                            LevelOfDetail8textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail7":
                        {
                            LevelOfDetail7textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail6":
                        {
                            LevelOfDetail6textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail5":
                        {
                            LevelOfDetail5textBox.Text = values[i];
                            break;
                        }
                    case "LevelOfDetail4":
                        {
                            LevelOfDetail4textBox.Text = values[i];
                            break;
                        }                   
                    case "levelOfDetailCheckAccuracy":
                        {
                            levelOfDetailCheckAccuracyTextBox.Text = values[i];
                            break;
                        }
                    case "cameraSpeed":
                        {
                            cameraSpeedTextBox.Text = values[i];
                            break;
                        }
                    case "maxCameraHight":
                        {
                            maxCameraHightTextBox.Text = values[i];
                            break;
                        }
                    case "minCameraHight":
                        {
                            minCameraHightTextBox.Text = values[i];
                            break;
                        }
                    case "downloadThread":
                        {
                            downloadThreadTextBox.Text = values[i];
                            break;
                        }
                    default:
                        break;
                }


            }
                    
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
        private bool intTextChenge(string Text)
        {
            try
            {
                NumberFormatInfo format = new NumberFormatInfo();
                format.NumberGroupSeparator = ",";
                format.NumberDecimalSeparator = ".";
                int value = int.Parse(Text, format);
                return true;
            }
            catch (Exception)
            {
                return false;
                throw;
            }
        }
        private void LevelOfDetail16textBox_TextChanged(object sender, EventArgs e)
        {
            if(!doubleTextChenge(LevelOfDetail16textBox.Text))
            LevelOfDetail16textBox.Text = "0";
        }

        private void LevelOfDetail15textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail15textBox.Text))
                LevelOfDetail15textBox.Text = "0";
        }

        private void LevelOfDetail14textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail14textBox.Text))
                LevelOfDetail14textBox.Text = "0";
        }

        private void LevelOfDetail13textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail13textBox.Text))
                LevelOfDetail13textBox.Text = "0";
        }

        private void LevelOfDetail12textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail12textBox.Text))
                LevelOfDetail12textBox.Text = "0";
        }

        private void LevelOfDetail11textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail11textBox.Text))
                LevelOfDetail11textBox.Text = "0";
        }

        private void LevelOfDetail10textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail10textBox.Text))
                LevelOfDetail10textBox.Text = "0";
        }

        private void LevelOfDetail9textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail9textBox.Text))
                LevelOfDetail9textBox.Text = "0";
        }

        private void LevelOfDetail8textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail8textBox.Text))
                LevelOfDetail8textBox.Text = "0";
        }

        private void LevelOfDetail7textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail7textBox.Text))
                LevelOfDetail7textBox.Text = "0";
        }

        private void LevelOfDetail6textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail6textBox.Text))
                LevelOfDetail6textBox.Text = "0";
        }

        private void LevelOfDetail5textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail5textBox.Text))
                LevelOfDetail5textBox.Text = "0";
        }

        private void LevelOfDetail4textBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(LevelOfDetail4textBox.Text))
                LevelOfDetail4textBox.Text = "0";
        }

        private void levelOfDetailCheckAccuracyTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!intTextChenge(levelOfDetailCheckAccuracyTextBox.Text))
                levelOfDetailCheckAccuracyTextBox.Text = "0";
        }

        private void minCameraHightTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(minCameraHightTextBox.Text))
                minCameraHightTextBox.Text = "0";
        }

        private void maxCameraHightTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(maxCameraHightTextBox.Text))
                maxCameraHightTextBox.Text = "0";
        }

        private void cameraSpeedTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!doubleTextChenge(cameraSpeedTextBox.Text))
                cameraSpeedTextBox.Text = "0";
        }

        private void prefImageSizeYTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!intTextChenge(prefImageSizeYTextBox.Text))
                prefImageSizeYTextBox.Text = "0";
        }

        private void prefImageSizeXTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!intTextChenge(prefImageSizeXTextBox.Text))
                prefImageSizeXTextBox.Text = "0";
        }

        private void elevationColsTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!intTextChenge(elevationColsTextBox.Text))
                elevationColsTextBox.Text = "0";
        }

        private void elevationRowsTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!intTextChenge(elevationRowsTextBox.Text))
                elevationRowsTextBox.Text = "0";
        }
        
        private void downloadThreadTextBox_TextChanged(object sender, EventArgs e)
        {

            if (!intTextChenge(downloadThreadTextBox.Text))
                downloadThreadTextBox.Text = "0";
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            string[] lines = System.IO.File.ReadAllLines("config.txt");
            string[] parameters = new string[30];
            string[] values = new string[30];
            int cout = 0;
            foreach (string line in lines)
            {
                parameters[cout] = "";
                values[cout] = "";
                bool value = false;
                for (int i = 0; i < line.Length; i++)
                {
                    if (line[i] != '=')
                    {
                        if (value)
                            values[cout] += line[i];
                        else
                            parameters[cout] += line[i];
                    }
                    else
                        value = true;
                }
                cout++;
            }
            for (int i = 0; i < 30; i++)
            {
                switch (parameters[i])
                {
                    case "elevationRows":
                        {
                            values[i] = elevationRowsTextBox.Text;
                            break;
                        }
                    case "elevationCols":
                        {
                            values[i] = elevationColsTextBox.Text;
                            break;
                        }
                    case "dataServer":
                        {
                            values[i] = dataServerTextBox.Text;
                            break;
                        }
                    case "licenceKey":
                        {
                            values[i] = licenceKeyTextBox.Text;
                            break;
                        }
                    case "saveDataOnDrive":
                        {
                            if (saveDataOnDriveCheckBox.Checked)
                                values[i] = "true";
                            else
                                values[i] = "false";
                            break;

                        }
                    case "prefImageSizeX":
                        {
                            values[i] = prefImageSizeXTextBox.Text;
                            break;
                        }
                    case "prefImageSizeY":
                        {
                            values[i] = prefImageSizeYTextBox.Text;
                            break;
                        }
                    case "LevelOfDetail16":
                        {
                            values[i] = LevelOfDetail16textBox.Text;
                            break;
                        }
                    case "LevelOfDetail15":
                        {
                            values[i] = LevelOfDetail15textBox.Text;
                            break;
                        }
                    case "LevelOfDetail14":
                        {
                            values[i] = LevelOfDetail14textBox.Text;
                            break;
                        }
                    case "LevelOfDetail13":
                        {
                            values[i] = LevelOfDetail13textBox.Text;
                            break;
                        }
                    case "LevelOfDetail12":
                        {
                            values[i] = LevelOfDetail12textBox.Text;
                            break;
                        }
                    case "LevelOfDetail11":
                        {
                            values[i] = LevelOfDetail11textBox.Text;
                            break;
                        }
                    case "LevelOfDetail10":
                        {
                            values[i] = LevelOfDetail10textBox.Text;
                            break;
                        }
                    case "LevelOfDetail9":
                        {
                            values[i] = LevelOfDetail9textBox.Text;
                            break;
                        }
                    case "LevelOfDetail8":
                        {
                            values[i] = LevelOfDetail8textBox.Text;
                            break;
                        }
                    case "LevelOfDetail7":
                        {
                            values[i] = LevelOfDetail7textBox.Text;
                            break;
                        }
                    case "LevelOfDetail6":
                        {
                            values[i] = LevelOfDetail6textBox.Text;
                            break;
                        }
                    case "LevelOfDetail5":
                        {
                            values[i] = LevelOfDetail5textBox.Text;
                            break;
                        }
                    case "LevelOfDetail4":
                        {
                            values[i] = LevelOfDetail4textBox.Text;
                            break;
                        }
                    case "levelOfDetailCheckAccuracy":
                        {
                            values[i] = levelOfDetailCheckAccuracyTextBox.Text;
                            break;
                        }
                    case "cameraSpeed":
                        {
                            values[i] = cameraSpeedTextBox.Text;
                            break;
                        }
                    case "maxCameraHight":
                        {
                            values[i] = maxCameraHightTextBox.Text;
                            break;
                        }
                    case "minCameraHight":
                        {
                            values[i] = minCameraHightTextBox.Text;
                            break;
                        }
                    case "downloadThread":
                        {
                            values[i] = downloadThreadTextBox.Text;
                            break;
                        }
                    default:
                        break;
                }
            }

            using (System.IO.StreamWriter file = new System.IO.StreamWriter("config.txt"))
            {
                for (int i = 0; i < 30; i++){
                    {
                        file.WriteLine(parameters[i]+"="+values[i]);
                    }
                }
            }
        }
    }
}
