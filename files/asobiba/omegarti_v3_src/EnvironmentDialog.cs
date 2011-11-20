
using System;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Zanetti.Config;

namespace Zanetti.Forms {
    internal class EnvironmentDialog : Form {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this._okButton = new System.Windows.Forms.Button();
            this._cancelButton = new System.Windows.Forms.Button();
            this._defaultProxy = new System.Windows.Forms.RadioButton();
            this._specifyProxy = new System.Windows.Forms.RadioButton();
            this._lAddress = new System.Windows.Forms.Label();
            this._addressBox = new System.Windows.Forms.TextBox();
            this._lPort = new System.Windows.Forms.Label();
            this._portBox = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
// 
// groupBox1
// 
            this.groupBox1.Controls.Add(this._portBox);
            this.groupBox1.Controls.Add(this._lPort);
            this.groupBox1.Controls.Add(this._addressBox);
            this.groupBox1.Controls.Add(this._lAddress);
            this.groupBox1.Controls.Add(this._specifyProxy);
            this.groupBox1.Controls.Add(this._defaultProxy);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(6, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(319, 157);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "プロキシ";
// 
// _okButton
// 
            this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._okButton.Location = new System.Drawing.Point(168, 166);
            this._okButton.Name = "_okButton";
            this._okButton.TabIndex = 1;
            this._okButton.Text = "OK";
            this._okButton.Click += new EventHandler(OnOK);
// 
// _cancelButton
// 
            this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._cancelButton.Location = new System.Drawing.Point(250, 166);
            this._cancelButton.Name = "_cancelButton";
            this._cancelButton.TabIndex = 2;
            this._cancelButton.Text = "キャンセル";
// 
// _defaultProxy
// 
            this._defaultProxy.Checked = true;
            this._defaultProxy.Location = new System.Drawing.Point(11, 19);
            this._defaultProxy.Name = "_defaultProxy";
            this._defaultProxy.Size = new System.Drawing.Size(150, 16);
            this._defaultProxy.TabIndex = 0;
            this._defaultProxy.TabStop = true;
            this._defaultProxy.Text = "IEの設定を使用(&I)";
// 
// _specifyProxy
// 
            this._specifyProxy.Location = new System.Drawing.Point(11, 42);
            this._specifyProxy.Name = "_specifyProxy";
            this._specifyProxy.Size = new System.Drawing.Size(150, 16);
            this._specifyProxy.TabIndex = 1;
            this._specifyProxy.Text = "以下の設定を使用(&C)";
            this._specifyProxy.CheckedChanged += new EventHandler(OnSpecifyProxyCheckedChanged);
// 
// _lAddress
// 
            this._lAddress.AutoSize = true;
            this._lAddress.Location = new System.Drawing.Point(30, 65);
            this._lAddress.Name = "_lAddress";
            this._lAddress.Size = new System.Drawing.Size(80, 14);
            this._lAddress.TabIndex = 2;
            this._lAddress.Text = "アドレス(&A)";
// 
// _addressBox
// 
            this._addressBox.Location = new System.Drawing.Point(107, 59);
            this._addressBox.Name = "_addressBox";
            this._addressBox.TabIndex = 3;
            this._addressBox.Enabled = false;
// 
// _lPort
// 
            this._lPort.AutoSize = true;
            this._lPort.Location = new System.Drawing.Point(30, 86);
            this._lPort.Name = "_lPort";
            this._lPort.Size = new System.Drawing.Size(49, 14);
            this._lPort.TabIndex = 4;
            this._lPort.Text = "ポート(&P)";
// 
// _portBox
// 
            this._portBox.Location = new System.Drawing.Point(107, 86);
            this._portBox.Name = "_portBox";
            this._portBox.TabIndex = 5;
            this._portBox.Enabled = false;
// 
// EnvironmentDialog
// 
            this.AcceptButton = this._okButton;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
            this.CancelButton = this._cancelButton;
            this.ClientSize = new System.Drawing.Size(337, 196);
            this.ControlBox = false;
            this.Controls.Add(this._cancelButton);
            this.Controls.Add(this._okButton);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EnvironmentDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "環境設定";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button _okButton;
        private System.Windows.Forms.Button _cancelButton;
        private System.Windows.Forms.RadioButton _defaultProxy;
        private System.Windows.Forms.RadioButton _specifyProxy;
        private System.Windows.Forms.Label _lAddress;
        private System.Windows.Forms.TextBox _addressBox;
        private System.Windows.Forms.Label _lPort;
        private System.Windows.Forms.TextBox _portBox;

        public EnvironmentDialog() {
            InitializeComponent();
            InitUI();
        }

        private void OnSpecifyProxyCheckedChanged(object sender, EventArgs args) {
            bool e = _specifyProxy.Checked;
            _addressBox.Enabled = e;
            _portBox.Enabled = e;
        }
        private void InitUI() {
            ProxyConfig c = Env.Options.ProxyConfig;
            _specifyProxy.Checked = !c.UseIESetting;
            _addressBox.Text = c.Address;
            _portBox.Text = c.Port.ToString();
        }
        private void OnOK(object sender, EventArgs args) {
            this.DialogResult = DialogResult.None;
            ProxyConfig c = Env.Options.ProxyConfig;
            if(_specifyProxy.Checked) {
                c.UseIESetting = false;
                c.Address = _addressBox.Text;
                bool port_ok = false;
                c.Port = Util.ParseInt(_portBox.Text, 0);
                port_ok = c.Port > 0 && c.Port < 65536;
                if(!port_ok) {
                    Util.Warning("ポートの指定が誤っています");
                    return;
                }
            } else
                c.UseIESetting = true;
            this.DialogResult = DialogResult.OK;
        }
    }
}