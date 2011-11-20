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

using Zanetti.UI;

namespace Zanetti.Forms
{
	/// <summary>
	/// CodeInput の概要の説明です。
	/// </summary>
	internal class CodeInput : System.Windows.Forms.Form
	{
		private CodeBox _codeBox;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public CodeInput()
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
			this._codeBox = new CodeBox();
			this.SuspendLayout();
			// 
			// _codeBox
			// 
			this._codeBox.Font = new System.Drawing.Font("MS UI Gothic", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(128)));
			this._codeBox.ImeMode = System.Windows.Forms.ImeMode.Off;
			this._codeBox.Location = new System.Drawing.Point(0, 0);
			this._codeBox.MaxLength = 4;
			this._codeBox.Name = "_codeBox";
			this._codeBox.StockCode = 0;
			this._codeBox.TabIndex = 0;
			this._codeBox.Text = "";
			this._codeBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this._codeBox.CodeComplete += new System.EventHandler(this.OnCodeComplete);
			// 
			// CodeInput
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(98, 24);
			this.Controls.Add(this._codeBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CodeInput";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "コード入力";
			this.ResumeLayout(false);

		}
		#endregion

		public int StockCode {
			get {
				return _codeBox.StockCode;
			}
			set {
				_codeBox.StockCode = value;
			}
		}

		private void OnCodeComplete(object sender, EventArgs args) {
			this.DialogResult = DialogResult.OK;
			Close();
		}

		protected override bool ProcessDialogKey(Keys keyData) {
			if(keyData==Keys.Escape) {
				this.DialogResult = DialogResult.Cancel;
				Close();
				return true;
			}
			else
				return base.ProcessDialogKey (keyData);
		}



	}
}
