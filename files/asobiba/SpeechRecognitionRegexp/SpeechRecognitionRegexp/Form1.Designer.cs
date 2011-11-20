namespace SpeechRecognitionRegexp
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.RuleRebuildButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.RegexpTextBox = new System.Windows.Forms.TextBox();
            this.HypothesisTextBox = new System.Windows.Forms.TextBox();
            this.RecognitionTextBox = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "正規表現";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 140);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "マッチ中";
            // 
            // RuleRebuildButton
            // 
            this.RuleRebuildButton.Location = new System.Drawing.Point(27, 79);
            this.RuleRebuildButton.Name = "RuleRebuildButton";
            this.RuleRebuildButton.Size = new System.Drawing.Size(484, 23);
            this.RuleRebuildButton.TabIndex = 2;
            this.RuleRebuildButton.Text = "ルール構築";
            this.RuleRebuildButton.UseVisualStyleBackColor = true;
            this.RuleRebuildButton.Click += new System.EventHandler(this.RuleRebuildButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 177);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "最終マッチ";
            // 
            // RegexpTextBox
            // 
            this.RegexpTextBox.Location = new System.Drawing.Point(119, 33);
            this.RegexpTextBox.Name = "RegexpTextBox";
            this.RegexpTextBox.Size = new System.Drawing.Size(392, 19);
            this.RegexpTextBox.TabIndex = 4;
            // 
            // HypothesisTextBox
            // 
            this.HypothesisTextBox.Location = new System.Drawing.Point(119, 136);
            this.HypothesisTextBox.Name = "HypothesisTextBox";
            this.HypothesisTextBox.Size = new System.Drawing.Size(392, 19);
            this.HypothesisTextBox.TabIndex = 5;
            // 
            // RecognitionTextBox
            // 
            this.RecognitionTextBox.Location = new System.Drawing.Point(119, 173);
            this.RecognitionTextBox.Name = "RecognitionTextBox";
            this.RecognitionTextBox.Size = new System.Drawing.Size(392, 19);
            this.RecognitionTextBox.TabIndex = 6;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(28, 230);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(483, 154);
            this.textBox4.TabIndex = 7;
            this.textBox4.Text = resources.GetString("textBox4.Text");
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(546, 396);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.RecognitionTextBox);
            this.Controls.Add(this.HypothesisTextBox);
            this.Controls.Add(this.RegexpTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.RuleRebuildButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "正規表現で音声認識";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button RuleRebuildButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox RegexpTextBox;
        private System.Windows.Forms.TextBox HypothesisTextBox;
        private System.Windows.Forms.TextBox RecognitionTextBox;
        private System.Windows.Forms.TextBox textBox4;
    }
}

