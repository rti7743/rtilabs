using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using SpeechRecognitionRegexp;

namespace SpeechRecognitionRegexp
{
    public partial class Form1 : Form
    {
        SpeechRecognitionRegexp.SpeechRecognizer Reco = null;
        public Form1()
        {
            InitializeComponent();
            this.Reco = new SpeechRecognitionRegexp.SpeechRecognizer(); 
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Reco.Hypothesis +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult result) 
                {
                    string strText = result.PhraseInfo.GetText(0, -1, true);
                    this.HypothesisTextBox.Text = strText;
                };
            this.Reco.Recognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
                {
                    string strText = isrr.PhraseInfo.GetText(0, -1, true);
                    this.RecognitionTextBox.Text = strText;
                };
            this.Reco.StartStream +=
                delegate(int streamNumber, object streamPosition)
                {
                    this.HypothesisTextBox.Text = "";
                    this.RecognitionTextBox.Text = "";
                };
            this.Reco.FalseRecognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult isrr)
                {
                    this.RecognitionTextBox.Text = "--ERROR!--";
                };

            string regexpStr = "こんにちは(レモン|ばなな(みるく|ジュース)|なのは?)ください";
            this.Reco.SetRegexp(regexpStr);
            this.RegexpTextBox.Text = regexpStr;
        }

        private void RuleRebuildButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.Reco.SetRegexp(this.RegexpTextBox.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("エラー:" + ex.Message);
            }
        }
    }
}
