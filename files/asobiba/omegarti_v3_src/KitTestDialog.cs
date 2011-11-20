/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

using Zanetti.Arithmetic;
using Zanetti.Arithmetic.Series;
using Zanetti.Parser;
using Zanetti.UI;
using Zanetti.Data;
using Zanetti.Commands;
using Zanetti.Indicators;
using Travis.Util;

namespace Zanetti.Forms
{
	/// <summary>
	/// KitTestDialog の概要の説明です。
	/// </summary>
	internal class KitTestDialog : System.Windows.Forms.Form, IZModelessForm, IIndicatorCustomizer
	{
		private System.Windows.Forms.Label _lMessage;
		private System.Windows.Forms.GroupBox _elementGroup;
		private System.Windows.Forms.Label _lTarget;
		private System.Windows.Forms.ComboBox _targetBox;
		private System.Windows.Forms.Label _lName;
		private System.Windows.Forms.TextBox _nameBox;
		private System.Windows.Forms.Label _lExpression;
		private System.Windows.Forms.TextBox _expressionBox;
		private System.Windows.Forms.Label _lStyle;
		private System.Windows.Forms.ComboBox _lineStyleBox;
		private ColPickerComboBox _colorBox;
		private System.Windows.Forms.GroupBox _testGroup;
		private System.Windows.Forms.Label _lBrand;
		private System.Windows.Forms.Label _lDate;
		private System.Windows.Forms.TextBox _dateBox;
		private System.Windows.Forms.Label _lResult;
		private System.Windows.Forms.Label _resultBox;
		private System.Windows.Forms.Button _calcButton;
		private System.Windows.Forms.Button _exportButton;
		private System.Windows.Forms.Button _applyButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public KitTestDialog()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			InitUI();
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
			this._lMessage = new System.Windows.Forms.Label();
			this._elementGroup = new System.Windows.Forms.GroupBox();
			this._lTarget = new System.Windows.Forms.Label();
			this._targetBox = new System.Windows.Forms.ComboBox();
			this._lName = new System.Windows.Forms.Label();
			this._nameBox = new System.Windows.Forms.TextBox();
			this._lExpression = new System.Windows.Forms.Label();
			this._expressionBox = new System.Windows.Forms.TextBox();
			this._lStyle = new System.Windows.Forms.Label();
			this._lineStyleBox = new System.Windows.Forms.ComboBox();
			this._colorBox = new ColPickerComboBox();
			this._testGroup = new System.Windows.Forms.GroupBox();
			this._lBrand = new System.Windows.Forms.Label();
			this._lDate = new System.Windows.Forms.Label();
			this._dateBox = new System.Windows.Forms.TextBox();
			this._lResult = new System.Windows.Forms.Label();
			this._resultBox = new System.Windows.Forms.Label();
			this._calcButton = new System.Windows.Forms.Button();
			this._exportButton = new System.Windows.Forms.Button();
			this._applyButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this._elementGroup.SuspendLayout();
			this._testGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// _lMessage
			// 
			this._lMessage.Location = new System.Drawing.Point(8, 8);
			this._lMessage.Name = "_lMessage";
			this._lMessage.Size = new System.Drawing.Size(472, 24);
			this._lMessage.TabIndex = 0;
			this._lMessage.Text = "拡張キットのために書いた式の簡単な確認をここで行うことができます。\nチャート用の式はここで描画できますが、スクリーニング用や式の一部分のテストには「値のテスト」を使ってください。";
			this._lMessage.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _elementGroup
			// 
			this._elementGroup.Controls.Add(this._colorBox);
			this._elementGroup.Controls.Add(this._lineStyleBox);
			this._elementGroup.Controls.Add(this._lStyle);
			this._elementGroup.Controls.Add(this._expressionBox);
			this._elementGroup.Controls.Add(this._lExpression);
			this._elementGroup.Controls.Add(this._nameBox);
			this._elementGroup.Controls.Add(this._lName);
			this._elementGroup.Controls.Add(this._targetBox);
			this._elementGroup.Controls.Add(this._lTarget);
			this._elementGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._elementGroup.Location = new System.Drawing.Point(8, 48);
			this._elementGroup.Name = "_elementGroup";
			this._elementGroup.Size = new System.Drawing.Size(472, 152);
			this._elementGroup.TabIndex = 1;
			this._elementGroup.TabStop = false;
			this._elementGroup.Text = "拡張キットの内容の編集";
			// 
			// _lTarget
			// 
			this._lTarget.Location = new System.Drawing.Point(8, 24);
			this._lTarget.Name = "_lTarget";
			this._lTarget.Size = new System.Drawing.Size(100, 16);
			this._lTarget.TabIndex = 2;
			this._lTarget.Text = "対象(&T)";
			this._lTarget.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _targetBox
			// 
			this._targetBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this._targetBox.Location = new System.Drawing.Point(120, 24);
			this._targetBox.Name = "_targetBox";
			this._targetBox.Size = new System.Drawing.Size(121, 20);
			this._targetBox.TabIndex = 3;
			this._targetBox.SelectedIndexChanged += new EventHandler(OnTargetChanged);
			// 
			// _lName
			// 
			this._lName.Location = new System.Drawing.Point(8, 56);
			this._lName.Name = "_lName";
			this._lName.TabIndex = 6;
			this._lName.Text = "名前(&N)";
			this._lName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _nameBox
			// 
			this._nameBox.Location = new System.Drawing.Point(120, 56);
			this._nameBox.Name = "_nameBox";
			this._nameBox.Size = new System.Drawing.Size(120, 19);
			this._nameBox.TabIndex = 7;
			this._nameBox.Text = "";
			// 
			// _lExpression
			// 
			this._lExpression.Location = new System.Drawing.Point(8, 88);
			this._lExpression.Name = "_lExpression";
			this._lExpression.Size = new System.Drawing.Size(104, 23);
			this._lExpression.TabIndex = 8;
			this._lExpression.Text = "式(&E)";
			this._lExpression.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _expressionBox
			// 
			this._expressionBox.Location = new System.Drawing.Point(120, 88);
			this._expressionBox.Name = "_expressionBox";
			this._expressionBox.Size = new System.Drawing.Size(344, 19);
			this._expressionBox.TabIndex = 9;
			this._expressionBox.Text = "";
			// 
			// _lStyle
			// 
			this._lStyle.Location = new System.Drawing.Point(8, 120);
			this._lStyle.Name = "_lStyle";
			this._lStyle.TabIndex = 10;
			this._lStyle.Text = "スタイル(&S)";
			this._lStyle.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _lineStyleBox
			// 
			this._lineStyleBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this._lineStyleBox.Location = new System.Drawing.Point(120, 120);
			this._lineStyleBox.Name = "_lineStyleBox";
			this._lineStyleBox.Size = new System.Drawing.Size(96, 20);
			this._lineStyleBox.TabIndex = 11;
			// 
			// _colorBox
			// 
			this._colorBox.Location = new System.Drawing.Point(224, 120);
			this._colorBox.Name = "_colorBox";
			this._colorBox.Size = new System.Drawing.Size(121, 20);
			this._colorBox.TabIndex = 12;
			// 
			// _testGroup
			// 
			this._testGroup.Controls.Add(this._calcButton);
			this._testGroup.Controls.Add(this._resultBox);
			this._testGroup.Controls.Add(this._lResult);
			this._testGroup.Controls.Add(this._dateBox);
			this._testGroup.Controls.Add(this._lDate);
			this._testGroup.Controls.Add(this._lBrand);
			this._testGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._testGroup.Location = new System.Drawing.Point(8, 208);
			this._testGroup.Name = "_testGroup";
			this._testGroup.Size = new System.Drawing.Size(472, 104);
			this._testGroup.TabIndex = 13;
			this._testGroup.TabStop = false;
			this._testGroup.Text = "値のテスト - チャートに描画ができない式の実験や構文エラーのチェック目的です。";
			// 
			// _lBrand
			// 
			this._lBrand.Location = new System.Drawing.Point(8, 16);
			this._lBrand.Name = "_lBrand";
			this._lBrand.Size = new System.Drawing.Size(176, 23);
			this._lBrand.TabIndex = 14;
			this._lBrand.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _lDate
			// 
			this._lDate.Location = new System.Drawing.Point(224, 16);
			this._lDate.Name = "_lDate";
			this._lDate.TabIndex = 15;
			this._lDate.Text = "対象日付(&D)";
			this._lDate.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _dateBox
			// 
			this._dateBox.Location = new System.Drawing.Point(328, 16);
			this._dateBox.Name = "_dateBox";
			this._dateBox.Size = new System.Drawing.Size(136, 19);
			this._dateBox.TabIndex = 16;
			this._dateBox.Text = "";
			// 
			// _lResult
			// 
			this._lResult.Location = new System.Drawing.Point(8, 40);
			this._lResult.Name = "_lResult";
			this._lResult.Size = new System.Drawing.Size(56, 23);
			this._lResult.TabIndex = 17;
			this._lResult.Text = "結果";
			this._lResult.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _resultBox
			// 
			this._resultBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this._resultBox.Location = new System.Drawing.Point(64, 40);
			this._resultBox.Name = "_resultBox";
			this._resultBox.Size = new System.Drawing.Size(400, 24);
			this._resultBox.TabIndex = 18;
			this._resultBox.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _calcButton
			// 
			this._calcButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._calcButton.Location = new System.Drawing.Point(392, 72);
			this._calcButton.Name = "_calcButton";
			this._calcButton.TabIndex = 19;
			this._calcButton.Text = "計算(&C)";
			this._calcButton.Click += new EventHandler(OnTestExpression);
			// 
			// _exportButton
			// 
			this._exportButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._exportButton.Location = new System.Drawing.Point(8, 328);
			this._exportButton.Name = "_exportButton";
			this._exportButton.Size = new System.Drawing.Size(208, 23);
			this._exportButton.TabIndex = 20;
			this._exportButton.Text = "拡張キットの要素としてエクスポート...";
			this._exportButton.Click += new EventHandler(OnExportElement);
			// 
			// _applyButton
			// 
			this._applyButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._applyButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._applyButton.Location = new System.Drawing.Point(304, 328);
			this._applyButton.Name = "_applyButton";
			this._applyButton.TabIndex = 15;
			this._applyButton.Text = "描画";
			this._applyButton.Click += new EventHandler(OnDrawExpression);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(400, 328);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 16;
			this._cancelButton.Text = "閉じる";
			this._cancelButton.Click += new EventHandler(OnClose);
			// 
			// KitTestDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(488, 366);
			this.StartPosition = FormStartPosition.Manual;
			this.AcceptButton = _applyButton;
			this.CancelButton = _cancelButton;
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._applyButton);
			this.Controls.Add(this._exportButton);
			this.Controls.Add(this._testGroup);
			this.Controls.Add(this._elementGroup);
			this.Controls.Add(this._lMessage);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "KitTestDialog";
			this.ShowInTaskbar = false;
			this.Text = "拡張キットのテスト";
			this._elementGroup.ResumeLayout(false);
			this._testGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void InitUI() {
			_targetBox.Items.Add("価格");
			_targetBox.Items.Add("出来高");
			_targetBox.Items.Add("オシレータ");
			_targetBox.Items.Add("数値の計算のみ");
			_targetBox.SelectedIndex = 0;
			_nameBox.Text = "新しい指標";
			_lineStyleBox.Items.AddRange(EnumDescAttribute.For(typeof(IndicatorStyle)).DescriptionCollection());
			_lineStyleBox.Items.RemoveAt(0); //"表示しない"を除去
			_lineStyleBox.SelectedIndex = 0;
			_colorBox.Color = Color.White;
			_dateBox.Text = Util.DateToInt(Util.GuessLatestTradeDate()).ToString();
			UpdateBrandName();
		}
		public void UpdateBrandName() {
			_lBrand.Text = "銘柄 : " + Env.Frame.ChartCanvas.GetBrand().Name;
		}

		private void OnTargetChanged(object sender, EventArgs args) {
			bool calc_only = ForLibrary();
			_applyButton.Enabled = !calc_only;
			_lineStyleBox.Enabled = !calc_only;
			_colorBox.Enabled = !calc_only;
		}
		private bool ForLibrary() {
			return _targetBox.SelectedIndex==3;
		}
		private void OnTestExpression(object sender, EventArgs args) {
			try {
				string r_expr = _expressionBox.Text;
				if(r_expr.Length==0) {
					Util.Warning(this, "式が入力されていません。");
					return;
				}
				DataFarm f = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
				if(f==null || f.IsEmpty) {
					Util.Warning(this, "テストをするためには何かの銘柄が表示されていないといけません。");
					return;
				}

				int date = Int32.Parse(_dateBox.Text);
				if(f.FirstDate > date || date > f.LastDate) {
					Util.Warning(this, "日付が有効範囲にありません。");
					return;
				}

				Expression expr = ParseExpression(r_expr);
				if(expr==null) return;
				Evaluator ev = new Evaluator(_nameBox.Text);
				ev.Farm = f;
				ev.BaseIndex = f.DateToIndex(date);
				EvalResult result = (EvalResult)expr.Apply(ev);
				PrintResult(result);
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				Util.Warning(this, ex.Message);
				_expressionBox.Focus();
			}
		}
		private void OnExportElement(object sender, EventArgs args) {
			DisplayKitTemplateDialog dlg = new DisplayKitTemplateDialog();
			if(ForLibrary())
				dlg.FormatLibrary(_nameBox.Text, _expressionBox.Text);
			else
				dlg.FormatIndicator(_nameBox.Text, (IndicatorTarget)_targetBox.SelectedIndex, _expressionBox.Text, (IndicatorStyle)(_lineStyleBox.SelectedIndex+1), _colorBox.Color);
			dlg.ShowDialog(Env.Frame);
		}
		private void OnDrawExpression(object sender, EventArgs args) {
			this.Cursor = Cursors.WaitCursor;
			CommandExec.RefreshChart(this);
			this.Cursor = Cursors.Default;
		}
		private void OnClose(object sender, EventArgs args) {
			this.Visible = false;
			Env.Frame.CurrentModelessDialog = null;
		}
		protected override void OnClosing(CancelEventArgs e) {
			e.Cancel = true;
			this.Visible = false;
		}


		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			Env.KitTestDialog = this;
			_dateBox.Text = Util.DateToInt(Util.GuessLatestTradeDate()).ToString();
			UpdateBrandName();
		}

		private Expression ParseExpression(string expr) {
			ExpressionConstructor ec = new ExpressionConstructor();
			try {
				new ZPredicationParser(new StringReader(expr), ec).Parse();
			}
			catch(Exception ex) {
				Util.Warning(this, "式の構文エラーです。" + ex.Message);
				return null;
			}

			Expression e = ec.Result;
			if(ec.NumOfParams > 0) {
				throw new FormatException("テスト時にはパラメータを含めることはできません");
			}

			return e;
		}

		private void PrintResult(EvalResult r) {
			string t = "";
			if(r.IsBool)
				t = r.BoolVal? "true" : "false";
			else if(r.IsDouble)
				t = r.DoubleVal.ToString();
			else if(r.IsTimeSeries) {
				StringBuilder bld = new StringBuilder();
				bld.Append("TimeSeries Length=");
				TimeSeries ts = r.TimeSeriesVal;
				bld.Append(ts.Count.ToString());
				bld.Append(" (");
				int count = ts.Count;
				TimeSeries.Cursor cur = ts.CreateCursor();
				int i = 0;
				while(cur.HasNext) {
					double x = cur.Next;
					if(count>5 && i>0 && cur.HasNext) continue;
					if(i>0) bld.Append(", ");
					bld.Append(x.ToString("F2"));
					if(count>5 && i==0) {
						bld.Append(", ..."); //途中省略
					}
					i++;
				}
				bld.Append(")");
				t = bld.ToString();
			}

			_resultBox.Text = t;

		}

		public void ZAbort() {
		}
		public void ZShow() {
			this.Visible = true;
			Env.Frame.CurrentModelessDialog = this;
		}

		//動的な描画
		public void ModifyIndicatorSet(IndicatorSet indicators) {
			try {
				string r_expr = _expressionBox.Text;
				if(r_expr.Length==0) {
					Util.Warning(this, "式が入力されていません。");
					return;
				}
				DataFarm f = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
				if(f==null || f.IsEmpty) {
					Util.Warning(this, "テストをするためには何かの銘柄が表示されていないといけません。");
					return;
				}

				Expression expr = ParseExpression(r_expr);
				if(expr==null) return;

				//値の計算をここで行う必要はないが、未定義関数の参照の検出のためには必要
				try {
					Evaluator ev = new Evaluator(_nameBox.Text);
					ev.Farm = f;
					ev.BaseIndex = f.FilledLength-1;
					EvalResult result = (EvalResult)expr.Apply(ev);
					if(!result.IsDouble) {
						Util.Warning(this, "描画のためには数値を返す式を書かなければいけません。");
						return;
					}
				}
				catch(Exception ex) {
					Util.Warning(this, ex.Message);
					return;
				}

				Indicator ind = indicators.AddExpressionIndicator(r_expr, null, _nameBox.Text, null);
				ind.Display = IndicatorDisplay.Value|IndicatorDisplay.Chart;
				ind.Target = (IndicatorTarget)_targetBox.SelectedIndex;
				ind.Appearance = new IndicatorAppearance((IndicatorStyle)(_lineStyleBox.SelectedIndex+1), _colorBox.Color);

				/*
				 //!!ちとさぼり
				if(_targetBox.SelectedIndex==2) { //oscillator
					OscillatorGroup gr = new OscillatorGroup(_nameBox.Text, ValueRange.Default);
					indicators.AddOscillatorGroup(gr);
					gr.AddIndicator(ind);
					indicators.ActiveOscillatorGroupIndex = indicators.OscillatorGroupCount-1;
				}
				*/
			}
			catch(FormatException ex) {
				Util.Warning(this, ex.Message);
				_expressionBox.Focus();
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				_expressionBox.Focus();
			}

		}

	}
}
