namespace DictationFilter
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.StringTextBox = new System.Windows.Forms.TextBox();
            this.RuleRebuildButton = new System.Windows.Forms.Button();
            this.HypothesisTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.RecognitionTextBox = new System.Windows.Forms.TextBox();
            this.DictationFilterCheckBox = new System.Windows.Forms.CheckBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.DictationRecognitioTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(74, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "認識する単語:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(105, 31);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 19);
            this.textBox1.TabIndex = 1;
            this.textBox1.Text = "コンピュータ";
            // 
            // StringTextBox
            // 
            this.StringTextBox.Location = new System.Drawing.Point(213, 32);
            this.StringTextBox.Name = "StringTextBox";
            this.StringTextBox.Size = new System.Drawing.Size(238, 19);
            this.StringTextBox.TabIndex = 2;
            this.StringTextBox.Text = "あしたのてんきは";
            // 
            // RuleRebuildButton
            // 
            this.RuleRebuildButton.Location = new System.Drawing.Point(105, 66);
            this.RuleRebuildButton.Name = "RuleRebuildButton";
            this.RuleRebuildButton.Size = new System.Drawing.Size(343, 23);
            this.RuleRebuildButton.TabIndex = 3;
            this.RuleRebuildButton.Text = "ルールの構築";
            this.RuleRebuildButton.UseVisualStyleBackColor = true;
            this.RuleRebuildButton.Click += new System.EventHandler(this.RuleRebuildButton_Click);
            // 
            // HypothesisTextBox
            // 
            this.HypothesisTextBox.Location = new System.Drawing.Point(109, 141);
            this.HypothesisTextBox.Name = "HypothesisTextBox";
            this.HypothesisTextBox.Size = new System.Drawing.Size(346, 19);
            this.HypothesisTextBox.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 144);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "マッチ中";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 203);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "最終マッチ";
            // 
            // RecognitionTextBox
            // 
            this.RecognitionTextBox.Location = new System.Drawing.Point(110, 200);
            this.RecognitionTextBox.Name = "RecognitionTextBox";
            this.RecognitionTextBox.Size = new System.Drawing.Size(346, 19);
            this.RecognitionTextBox.TabIndex = 6;
            // 
            // DictationFilterCheckBox
            // 
            this.DictationFilterCheckBox.AutoSize = true;
            this.DictationFilterCheckBox.Checked = true;
            this.DictationFilterCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.DictationFilterCheckBox.Location = new System.Drawing.Point(27, 105);
            this.DictationFilterCheckBox.Name = "DictationFilterCheckBox";
            this.DictationFilterCheckBox.Size = new System.Drawing.Size(182, 16);
            this.DictationFilterCheckBox.TabIndex = 8;
            this.DictationFilterCheckBox.Text = "Dictationフィルターを有効にする。";
            this.DictationFilterCheckBox.UseVisualStyleBackColor = true;
            this.DictationFilterCheckBox.CheckedChanged += new System.EventHandler(this.DictationFilterCheckBox_CheckedChanged);
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(12, 243);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(463, 282);
            this.textBox5.TabIndex = 9;
            this.textBox5.Text = resources.GetString("textBox5.Text");
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(25, 170);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(76, 12);
            this.label4.TabIndex = 11;
            this.label4.Text = "Dictationマッチ";
            // 
            // DictationRecognitioTextBox
            // 
            this.DictationRecognitioTextBox.Location = new System.Drawing.Point(110, 168);
            this.DictationRecognitioTextBox.Name = "DictationRecognitioTextBox";
            this.DictationRecognitioTextBox.Size = new System.Drawing.Size(346, 19);
            this.DictationRecognitioTextBox.TabIndex = 10;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 537);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.DictationRecognitioTextBox);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.DictationFilterCheckBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.RecognitionTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.HypothesisTextBox);
            this.Controls.Add(this.RuleRebuildButton);
            this.Controls.Add(this.StringTextBox);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "DictationFilter";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox StringTextBox;
        private System.Windows.Forms.Button RuleRebuildButton;
        private System.Windows.Forms.TextBox HypothesisTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox RecognitionTextBox;
        private System.Windows.Forms.CheckBox DictationFilterCheckBox;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox DictationRecognitioTextBox;
    }
}

