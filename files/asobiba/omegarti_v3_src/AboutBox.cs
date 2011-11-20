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

namespace Zanetti.Forms
{
	/// <summary>
	/// AboutBox の概要の説明です。
	/// </summary>
	internal class AboutBox : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox _pictureBox;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Label _lVersion;
		private System.Windows.Forms.Label _lUser;
		private System.Windows.Forms.Label _lAcknowledge;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public AboutBox()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(AboutBox));
			this._pictureBox = new System.Windows.Forms.PictureBox();
			this._okButton = new System.Windows.Forms.Button();
			this._lVersion = new System.Windows.Forms.Label();
			this._lUser = new System.Windows.Forms.Label();
			this._lAcknowledge = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// _pictureBox
			// 
			this._pictureBox.Image = ((System.Drawing.Image)(resources.GetObject("_pictureBox.Image")));
			this._pictureBox.Location = new System.Drawing.Point(0, 0);
			this._pictureBox.Name = "_pictureBox";
			this._pictureBox.Size = new System.Drawing.Size(376, 115);
			this._pictureBox.TabIndex = 0;
			this._pictureBox.TabStop = false;
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(152, 280);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 1;
			this._okButton.Text = "OK";
			// 
			// _lVersion
			// 
			this._lVersion.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this._lVersion.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this._lVersion.Location = new System.Drawing.Point(8, 128);
			this._lVersion.Name = "_lVersion";
			this._lVersion.Size = new System.Drawing.Size(360, 32);
			this._lVersion.TabIndex = 2;
			this._lVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _lUser
			// 
			this._lUser.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this._lUser.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this._lUser.Location = new System.Drawing.Point(8, 168);
			this._lUser.Name = "_lUser";
			this._lUser.Size = new System.Drawing.Size(360, 32);
			this._lUser.TabIndex = 3;
			this._lUser.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _lAcknowledge
			// 
			this._lAcknowledge.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this._lAcknowledge.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._lAcknowledge.Location = new System.Drawing.Point(8, 208);
			this._lAcknowledge.Name = "_lAcknowledge";
			this._lAcknowledge.Size = new System.Drawing.Size(360, 64);
			this._lAcknowledge.TabIndex = 4;
			this._lAcknowledge.Text = "【謝辞】\nOmegaChartは、Carlos H. Perez 氏の各種GUIコンポーネント、Chew Keong TAN氏の長整数演算ライブラリ、Crownw" +
				"ood Consulting社のMagicLibrary、Per Cederberg氏のパーサジェネレータ\"Grammatica\"の各製品およびコンポーネントを" +
				"利用しています。この場を借りて感謝を表します。";
			// 
			// AboutBox
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.BackColor = System.Drawing.Color.White;
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(376, 312);
			this.Controls.Add(this._lAcknowledge);
			this.Controls.Add(this._lUser);
			this.Controls.Add(this._lVersion);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._pictureBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AboutBox";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "OmegaChart バージョン情報";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			string modifier = "";
#if KENMILLE
			modifier += "-K";
#endif
#if BUILD_INITIAL_DATA
			modifier += "-I";
#endif
			this._lVersion.Text = String.Format("Copyright(c) 2003-2006 Daisuke OKAJIMA\nVersion {0}{1}", ZanettiVersion.VERSION, modifier);
#if DOJIMA
			_lUser.Visible = false;
			this.Text = Dojima.DojimaUtil.AppTitle;
#else
			if(!Env.Options.Certificate.IsValid)
				_lUser.Text = "Contribution Certificate - 未登録(詳細はOmegaChartのWebをご覧ください)";
			else {
				try {
					_lUser.Text = String.Format("Contribution Certificate は {0} 様の名前で登録済み - ありがとうございます。", Env.Options.Certificate.UserName);
				}
				catch(Exception) {
					_lUser.Text = "Contribution Certificate - 検証エラー";
				}
			}
#endif
		}

	}
}
