using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

using Travis.Archive;

namespace Zanetti.Forms
{
	/// <summary>
	/// InitializeData の概要の説明です。
	/// </summary>
	internal class InitializeData : System.Windows.Forms.Form
	{
		private string _filename;
		private Thread _thread;

		private System.Windows.Forms.Label _messageLabel;
		private System.Windows.Forms.LinkLabel _linkLabel;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public InitializeData()
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
			this._messageLabel = new System.Windows.Forms.Label();
			this._linkLabel = new System.Windows.Forms.LinkLabel();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _messageLabel
			// 
			this._messageLabel.Location = new System.Drawing.Point(8, 8);
			this._messageLabel.Name = "_messageLabel";
			this._messageLabel.Size = new System.Drawing.Size(352, 112);
			this._messageLabel.TabIndex = 0;
			// 
			// _linkLabel
			// 
			this._linkLabel.Location = new System.Drawing.Point(8, 128);
			this._linkLabel.Name = "_linkLabel";
			this._linkLabel.Size = new System.Drawing.Size(344, 16);
			this._linkLabel.TabIndex = 1;
			this._linkLabel.TabStop = true;
			this._linkLabel.Text = "データの入手について";
			this._linkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this._linkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnLinkClicked);
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(176, 168);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 2;
			this._okButton.Text = "開始(&S)...";
			this._okButton.Click += new System.EventHandler(this.OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(264, 168);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 3;
			this._cancelButton.Text = "キャンセル";
			this._cancelButton.Click += new EventHandler(OnCancel);
			// 
			// InitializeData
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(362, 198);
			this.ControlBox = false;
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._linkLabel);
			this.Controls.Add(this._messageLabel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "InitializeData";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "データの初期化";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad(e);
			this._messageLabel.Text = "　データの初期化は主にOmegaChartインストール直後に行います。\n\n　この操作を行うと、初期化に使用したデータで既存のデータを上書きするため、より新しいデータが手元にあった場合でもそれは失われてしまいます。初期化完了後、必要に応じて残りのデータをダウンロードして補充してください。\n　また、この初期化作業には数分間程度の時間がかかります。\n\n　初期化データの入手については下記のリンクをご覧ください。";
		}
		private void OnLinkClicked(object sender, LinkLabelLinkClickedEventArgs args) {
			Process.Start("http://www.omegachart.org/init_data.html");
		}

		private void OnOK(object sender, EventArgs args) {
			this.DialogResult = DialogResult.None;
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "データファイルの選択";
			dlg.Filter = "File(*.zip)|*.zip|All Files|*";
			dlg.Multiselect = false;
			if(dlg.ShowDialog(this)==DialogResult.OK) {
				_okButton.Enabled = false;
				this.Cursor = Cursors.AppStarting;
				this.Text = String.Format("読み込んでいます...");
				_filename = dlg.FileName;
				_thread = new Thread(new ThreadStart(StartImport));
				_thread.Start();
			}
		}
		private void OnCancel(object sender, EventArgs args) {
			if(_thread!=null) {
				_thread.Abort();
				_thread = null;
			}
		}

		private delegate void ImportResult();
		private int _importCount;

		private void OnImportSucceeded() {
			this.DialogResult = DialogResult.OK;
			this.Close();
		}
		private void OnImportFailed() {
			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}
		private void OnImportProgress() {
			this.Text = String.Format("読み込んでいます...  {0}銘柄完了", _importCount);
		}

		private void StartImport() {
			try {
				ArrayList list = new ExtractZip().extract(_filename);
				_importCount = 0;
				foreach(Extract.FileBuffer e in list) {
					int code = Util.ParseInt(e.filename.Substring(5)); //ちょっとぬるいハンドリングだが
					FileStream s = new FileStream(Util.GetDailyDataFileName(code), FileMode.Create, FileAccess.Write);
					s.Write(e.buffer, 0, e.buffer.Length);
					s.Close();
					if((++_importCount % 100)==0)
						this.BeginInvoke(new ImportResult(OnImportProgress));
				}
				this.BeginInvoke(new ImportResult(OnImportSucceeded));
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Util.Warning(this, ex.Message);
				this.BeginInvoke(new ImportResult(OnImportFailed));
			}
		}

	}
}
