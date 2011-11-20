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

using Zanetti.Prediction;
using Zanetti.Indicators;

namespace Zanetti.Forms
{
	/// <summary>
	/// PredictionDialog の概要の説明です。
	/// </summary>
	internal class PredictionDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label _message;
		private System.Windows.Forms.Button _okButton;
		//private System.Windows.Forms.Button _quizButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		private Hashtable _buttonToPredictionItem;
		private PredictionItem _selectedPrediction;
		private VerificationResult _result;

		public PredictionDialog()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			LoadRadioButtons();
		}

		private void LoadRadioButtons() {
			_buttonToPredictionItem = new Hashtable();
			int i = 0;

			int y = _message.Top+_message.Height+8;
			foreach(PredictionItem pi in Env.CurrentIndicators.PredictionItems) {
				RadioButton rb = new RadioButton();
				rb.Text = pi.Title;
				rb.FlatStyle = FlatStyle.System;
				rb.Width = this.ClientSize.Width-16;
				rb.Left = 8;
				rb.Top = y + i*24;
				rb.Height = 24;
				rb.CheckedChanged += new EventHandler(OnCheckedChanged);
				_buttonToPredictionItem[rb] = pi;
				rb.Checked = (i==0);
				this.Controls.Add(rb);
				i++;
			}

			_okButton.Top = y + i*24;
			_cancelButton.Top = y + i*24;
			this.ClientSize = new Size(this.ClientSize.Width, _okButton.Top+32);
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
			this._message = new System.Windows.Forms.Label();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _message
			// 
			this._message.Location = new System.Drawing.Point(8, 8);
			this._message.Name = "_message";
			this._message.Size = new System.Drawing.Size(320, 23);
			this._message.TabIndex = 0;
			this._message.Text = "　表示中の銘柄について、指定した法則がどの程度成立するかを検証します。";
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.Location = new System.Drawing.Point(160, 40);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 1;
			this._okButton.Text = "OK";
			this._okButton.Click += new EventHandler(OnOK);
			this._okButton.FlatStyle = FlatStyle.System;
			/*
			// 
			// _quizButton
			// 
			this._quizButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;	// quiz終わったらダイアログ閉じる
			this._quizButton.Location = new System.Drawing.Point(208, 40);
			this._quizButton.Name = "_quizButton";
			this._quizButton.TabIndex = 2;
			this._quizButton.Text = "クイズ";
			this._quizButton.Click += new EventHandler(OnQuiz);
			this._quizButton.FlatStyle = FlatStyle.System;
			*/
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.Location = new System.Drawing.Point(240, 40);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 2;
			this._cancelButton.Text = "キャンセル";
			this._cancelButton.FlatStyle = FlatStyle.System;
			// 
			// PredictionDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(362, 70);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._message);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "PredictionDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "セオリーの検証";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			if(_buttonToPredictionItem.Count==0) {
				Util.Warning(this, "セオリー検証が拡張キット内に登録されていません。");
				Close();
			}
		}


		public VerificationResult Result {
			get {
				return _result;
			}
		}

		private void OnCheckedChanged(object sender, EventArgs args) {
			_selectedPrediction = (PredictionItem)_buttonToPredictionItem[sender];
		}

		private void OnOK(object sender, EventArgs args) {
			try {
				VerificationResult vr = new VerificationResult(_selectedPrediction);
				vr.Verify(Env.Frame.ChartCanvas.GetBrand().ReserveFarm());
				_result = vr;
			}
			catch(ArgumentException ex) {
				Util.Warning(this, ex.Message);
				this.DialogResult = DialogResult.Abort;
			}

		}
		private void OnQuiz(object sender, EventArgs args) 
		{
			try 
			{
				int seikai = 0;
				int machigai = 0;
				
				this.Visible = false;
				VerificationResult vr = new VerificationResult(_selectedPrediction);
				vr.Verify(Env.Frame.ChartCanvas.GetBrand().ReserveFarm());
				// お尻から頭の順番で入っているので逆にする　高速化：配列にして
				Stack s = new Stack();
				foreach(VerificationResultEntry e in vr.Entries)
				{
					s.Push(e);
				}				
				foreach(VerificationResultEntry e in s) 
				{
					Zanetti.Data.AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
					Env.Frame.ChartCanvas.ForceVisibleDate(e.Date,true);
					DialogResult ans = Util.AskUserYesNo(this,String.Format("{0}年{1}月{2}日 ： セオリーが成立していると予想しますか？",e.Date / 10000,e.Date / 100 % 100,e.Date % 100));
					Env.Frame.ChartCanvas.ForceVisibleDate(e.Date,false);
					
					if((ans == DialogResult.Yes && e.Result == PredictionResult.True) ||
						(ans == DialogResult.No && e.Result == PredictionResult.False)){
						// 正解
						Util.Information(this,"正解！");
						seikai++;
					}
					else{
						// 間違い
						Util.Information(this,"不正解！");
						machigai++;
					}					
				}
				Util.Information(this,String.Format("クイズが終了しました。\n正解数：{0}  不正解数：{1}  正解率：{2:0.##}です。",seikai,machigai,(double)seikai/(seikai+machigai)));
				
			}
			catch(ArgumentException ex) 
			{
				Util.Warning(this, ex.Message);
				this.DialogResult = DialogResult.Abort;
			}

		}
	}
}
