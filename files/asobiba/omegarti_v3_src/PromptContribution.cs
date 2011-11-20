/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;

namespace Zanetti.Forms
{
	/// <summary>
	/// PromptContribution の概要の説明です。
	/// </summary>
	internal class PromptContribution : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.LinkLabel linkLabel1;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PromptContribution()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			Timer t = new Timer();
			t.Tick += new EventHandler(OnTimer);
			t.Interval = 20000;
			t.Start();
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this._okButton = new System.Windows.Forms.Button();
			this._message = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Enabled = false;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(176, 216);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			// 
			// _message
			// 
			this._message.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(420, 176);
			this._message.TabIndex = 1;
			// 
			// linkLabel1
			// 
			this.linkLabel1.Location = new System.Drawing.Point(0, 192);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.TextAlign = ContentAlignment.MiddleCenter;
			this.linkLabel1.Size = new System.Drawing.Size(424, 16);
			this.linkLabel1.TabIndex = 2;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "このメッセージを出す理由についてはこちらをクリックしてください。";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
			// 
			// PromptContribution
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(432, 248);
			this.ControlBox = false;
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this._message);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PromptContribution";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Contribution Certificateについて";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			_message.Text = String.Format("　このメッセージは、Contribution Certificateが登録されていない場合に、10回に１回の割合で登場します。\n\n" +
				"　OmegaChartは、皆さんの協力を通してよりよいツールを作っていこうという目的のプロジェクトです。そのため、" +
				"Contribution Certificate(貢献証明書)という仕組みによって、貢献があった人を表彰するようにしています。\n\n" +
				"　証明書を登録すると、このメッセージが出なくなるとともにバージョン情報に名前が登録され、希望者はOmegaChartのWebサイトにも名前が掲載されます。\n" +
				"　貢献活動には開発に参加する他、宣伝や寄付などいろいろあります。各自ができる協力をお願いします。\n" +
				"　なお、このメッセージは表示されてから20秒後に閉じることができるようになります。");
		}

		private void OnTimer(object sender, EventArgs args) {
			_okButton.Enabled = true;
		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e) {
			try {
				Process.Start("http://www.omegachart.org/prompt.html");
			}
			catch(Exception) {
			}
		}
	}

	internal class PromptDonation : System.Windows.Forms.Form {
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.LinkLabel linkLabel1;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PromptDonation() {
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if(components != null) {
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			this._okButton = new System.Windows.Forms.Button();
			this._message = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Enabled = true;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(176, 128);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			// 
			// _message
			// 
			this._message.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(420, 88);
			this._message.TabIndex = 1;
			// 
			// linkLabel1
			// 
			this.linkLabel1.Location = new System.Drawing.Point(0, 104);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.TextAlign = ContentAlignment.MiddleCenter;
			this.linkLabel1.Size = new System.Drawing.Size(424, 16);
			this.linkLabel1.TabIndex = 2;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "寄付についての詳しい案内はこちらです";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
			// 
			// PromptContribution
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(432, 160);
			this.ControlBox = false;
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this._message);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PromptDonation";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "寄付について";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			_message.Text = String.Format("　OmegaChartでは、常時寄付を募集中です。\n\n" +
				"　まだ寄付をしていない方は相場で成果を収めたときにでもぜひご検討ください。新機能の実現との交換条件をもちかけるなど、" +
				"お気軽にご相談ください。寄付をもらったり、機能についての意見を聞くのは作者と他の協力者のやる気につながります。");
		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e) {
			try {
				Process.Start("http://www.omegachart.org/donation.html");
			}
			catch(Exception) {
			}
		}
	}
}
