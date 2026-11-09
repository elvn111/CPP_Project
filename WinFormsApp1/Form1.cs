using System.Reflection.Emit;
using System.Windows.Forms;
namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        private void button1_Click(object sender, EventArgs e)
        {

            if (label1.Text == "")
            {
                label1.Text = "已点击";
            }
            else
            {
                label1.Text = "";
            }
        }


        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (label1.Text == "")
            {
                label1.Text = "已点击";
            }
            else
            {
                label1.Text = "";
            }
        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }
    }
}
