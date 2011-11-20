/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Forms;
using System.Xml;

using Travis.Storage;
using Zanetti.Indicators.Schema;

namespace Zanetti.Forms
{
	/// <summary>
	/// ExtensionKitList の概要の説明です。
	/// </summary>
	internal class ExtensionKitListDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListView _listView;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.ColumnHeader _titleHeader;
		private System.Windows.Forms.ColumnHeader _fileNameHeader;
		private System.Windows.Forms.ColumnHeader _authorHeader;
		private System.Windows.Forms.ColumnHeader _signatureHeader;
		private System.Windows.Forms.ColumnHeader _expirationHeader;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ExtensionKitListDialog()
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
			this._listView = new System.Windows.Forms.ListView();
			this._titleHeader = new System.Windows.Forms.ColumnHeader();
			this._fileNameHeader = new System.Windows.Forms.ColumnHeader();
			this._authorHeader = new System.Windows.Forms.ColumnHeader();
			this._signatureHeader = new System.Windows.Forms.ColumnHeader();
			this._expirationHeader = new System.Windows.Forms.ColumnHeader();
            this._okButton = new Button();
            this.SuspendLayout();
			// 
			// _listView
			// 
			this._listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						this._titleHeader,
																						this._fileNameHeader,
																						this._authorHeader,
																						this._signatureHeader,
																						this._expirationHeader});
			this._listView.FullRowSelect = true;
			this._listView.GridLines = true;
			this._listView.Location = new System.Drawing.Point(0, 0);
			this._listView.Name = "_listView";
			this._listView.Size = new System.Drawing.Size(440, 168);
			this._listView.TabIndex = 0;
			this._listView.View = System.Windows.Forms.View.Details;
			//this._listView.SelectedIndexChanged += new EventHandler(OnSelected);
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(448, 136);
			this._okButton.Name = "_okButton";
			this._okButton.Size = new System.Drawing.Size(72, 23);
			this._okButton.TabIndex = 3;
			this._okButton.Text = "OK";
			// 
			// _titleHeader
			// 
			this._titleHeader.Text = "タイトル";
			this._titleHeader.Width = 88;
			// 
			// _fileNameHeader
			// 
			this._fileNameHeader.Text = "ファイル名";
			this._fileNameHeader.Width = 103;
			// 
			// _authorHeader
			// 
			this._authorHeader.Text = "作者";
			this._authorHeader.Width = 99;
			// 
			// _signatureHeader
			// 
			this._signatureHeader.Text = "署名";
			// 
			// _expirationHeader
			// 
			this._expirationHeader.Text = "有効期限";
			this._expirationHeader.Width = 79;
			// 
			// ExtensionKitListDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._okButton;
			this.ClientSize = new System.Drawing.Size(522, 168);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._listView);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ExtensionKitListDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "拡張キット一覧";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnClosed(EventArgs e) {
			base.OnClosed (e);
		}


		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			foreach(ExtensionKit ek in Env.Schema.ExtensionKits) {
				AddItem(ek);
			}
		}
		private void AddItem(ExtensionKit ek) {
			ListViewItem li = new ListViewItem();
			li.Tag = ek;
			li.Text = ek.Title;
			li.SubItems.Add(Path.GetFileName(ek.FileName));
			li.SubItems.Add(ek.Author);
			li.SubItems.Add(ek.Signed? "あり" : "なし");
			DateTime dt = ek.Expiration;
			if(dt.Year<2100)
				li.SubItems.Add(String.Format("{0}/{1}/{2}", dt.Year, dt.Month, dt.Day));
			else
				li.SubItems.Add("無期限");
			_listView.Items.Add(li);
        }

#if false
		private void OnSelected(object sender, EventArgs args) {
			if(_listView.SelectedIndices.Count==0)
				_removeButton.Enabled = false;
			else
				_removeButton.Enabled = true;
		}

		private void OnRemove(object sender, EventArgs args) {
			ListViewItem li = _listView.SelectedItems[0];
			Env.Schema.ExtensionKits.Remove(li.Tag);
			File.Delete(((ExtensionKit)li.Tag).FileName);
			_listView.Items.Remove(li);
		}
		private void OnAdd(object sender, EventArgs args) {
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "拡張キットファイルの選択";
			dlg.Filter = "OmegaChart Extension Kit(*.omega)|*.omega|All Files|*";
			dlg.Multiselect = false;
			if(dlg.ShowDialog(this)==DialogResult.OK) {
				string filename = dlg.FileName;
				try {
					ExtensionKit nk = ExtensionKit.LoadFrom(filename, null);
					string destname = Env.GetAppDir()+"extension\\"+Path.GetFileName(filename);
					if(File.Exists(destname)) throw new IOException("すでに同名の拡張キットが登録されています。");
					if(Env.Schema.FindByID(nk.ID)!=null) throw new Exception(String.Format("同じID {0} を持つキットがすでに存在します。", nk.ID));

					nk.VerifyExpressions();
				
					Env.Schema.ExtensionKits.Add(nk);
					Env.Schema.Reorder();
					File.Copy(filename, destname, false);
					nk.FileName = destname;
					AddItem(nk);
					
				}
				catch(Exception ex) {
					Util.SilentReportCriticalError(ex);
					Util.Warning(this, ex.Message);
				}
			}
		}
#endif
		
	}
}
