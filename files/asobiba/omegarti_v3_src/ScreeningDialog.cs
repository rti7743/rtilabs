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
using System.Text;
using Travis.Util;

using Zanetti.Data;
using Zanetti.Indicators;
using Zanetti.SystemTrading;
using Zanetti.SystemTrading.Screening;
using Zanetti.Config;	// for BookmarkFolder

namespace Zanetti.Forms
{
	/// <summary>
	/// ScreeningDialog の概要の説明です。
	/// </summary>
	internal class ScreeningDialog : System.Windows.Forms.Form
	{
		private ScreeningOrder _order;

		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.GroupBox _filterGroup;
		private System.Windows.Forms.Label _messageLabel;
		private System.Windows.Forms.Label _filterLabel;
		private System.Windows.Forms.ComboBox _filterBox;
		private System.Windows.Forms.GroupBox _primaryIndicatorGroup;
		private System.Windows.Forms.Label _lCondition;
		private System.Windows.Forms.Label _primaryIndicatorLabel;
		private System.Windows.Forms.ComboBox _primaryIndicatorBox;
		private System.Windows.Forms.Label _countLabel;
		private System.Windows.Forms.ComboBox _numberBox;
		private System.Windows.Forms.GroupBox _dateGroup;
		private System.Windows.Forms.DateTimePicker _datePicker;
		private System.Windows.Forms.ProgressBar _progressBar;
		private System.Windows.Forms.ComboBox _favoriteBox;
		private System.Windows.Forms.Button _specBookmarkFolderButton;
		private System.Windows.Forms.TextBox _bookmarkFolderTextBox;
		private System.Windows.Forms.Label _filterBookmarkLabel;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		private BookmarkFolder _bookmarkFolder;

		public ScreeningDialog()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			_filterBox.Items.AddRange(EnumDescAttribute.For(typeof(FilterType)).DescriptionCollection());
			_numberBox.Items.AddRange(EnumDescAttribute.For(typeof(ResultCountType)).DescriptionCollection());
			_filterBox.SelectedIndex = 0;
			_numberBox.SelectedIndex = 0;
			InitUI();
		}

		public ScreeningOrder ScreeningOrder {
			get {
				return _order;
			}
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
			this._filterGroup = new System.Windows.Forms.GroupBox();
			this._specBookmarkFolderButton = new System.Windows.Forms.Button();
			this._bookmarkFolderTextBox = new System.Windows.Forms.TextBox();
			this._filterBookmarkLabel = new System.Windows.Forms.Label();
			this._filterBox = new System.Windows.Forms.ComboBox();
			this._filterLabel = new System.Windows.Forms.Label();
			this._messageLabel = new System.Windows.Forms.Label();
			this._favoriteBox = new System.Windows.Forms.ComboBox();
			this._primaryIndicatorGroup = new System.Windows.Forms.GroupBox();
			this._lCondition = new System.Windows.Forms.Label();
			this._numberBox = new System.Windows.Forms.ComboBox();
			this._countLabel = new System.Windows.Forms.Label();
			this._primaryIndicatorBox = new System.Windows.Forms.ComboBox();
			this._primaryIndicatorLabel = new System.Windows.Forms.Label();
			this._dateGroup = new System.Windows.Forms.GroupBox();
			this._datePicker = new System.Windows.Forms.DateTimePicker();
			this._progressBar = new System.Windows.Forms.ProgressBar();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this._filterGroup.SuspendLayout();
			this._primaryIndicatorGroup.SuspendLayout();
			this._dateGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// _filterGroup
			// 
			this._filterGroup.Controls.Add(this._specBookmarkFolderButton);
			this._filterGroup.Controls.Add(this._bookmarkFolderTextBox);
			this._filterGroup.Controls.Add(this._filterBookmarkLabel);
			this._filterGroup.Controls.Add(this._filterBox);
			this._filterGroup.Controls.Add(this._filterLabel);
			this._filterGroup.Controls.Add(this._messageLabel);
			this._filterGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._filterGroup.Location = new System.Drawing.Point(8, 8);
			this._filterGroup.Name = "_filterGroup";
			this._filterGroup.Size = new System.Drawing.Size(464, 96);
			this._filterGroup.TabIndex = 0;
			this._filterGroup.TabStop = false;
			this._filterGroup.Text = "１　フィルタの設定";
			// 
			// _specBookmarkFolderButton
			// 
			this._specBookmarkFolderButton.Location = new System.Drawing.Point(368, 64);
			this._specBookmarkFolderButton.Name = "_specBookmarkFolderButton";
			this._specBookmarkFolderButton.Size = new System.Drawing.Size(80, 24);
			this._specBookmarkFolderButton.TabIndex = 6;
			this._specBookmarkFolderButton.Text = "フォルダ指定";
			this._specBookmarkFolderButton.FlatStyle = FlatStyle.System;
			this._specBookmarkFolderButton.Click += new System.EventHandler(this.OnSelectBookmarkFolder);
			// 
			// _bookmarkFolderTextBox
			// 
			this._bookmarkFolderTextBox.Location = new System.Drawing.Point(136, 64);
			this._bookmarkFolderTextBox.Name = "_bookmarkFolderTextBox";
			this._bookmarkFolderTextBox.Size = new System.Drawing.Size(224, 19);
			this._bookmarkFolderTextBox.TabIndex = 5;
			this._bookmarkFolderTextBox.Text = "";
			this._bookmarkFolderTextBox.ReadOnly = true;
			// 
			// _filterBookmarkLabel
			// 
			this._filterBookmarkLabel.Location = new System.Drawing.Point(24, 64);
			this._filterBookmarkLabel.Name = "_filterBookmarkLabel";
			this._filterBookmarkLabel.Size = new System.Drawing.Size(72, 16);
			this._filterBookmarkLabel.TabIndex = 4;
			this._filterBookmarkLabel.Text = "お気に入り(&T)";
			this._filterBookmarkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _filterBox
			// 
			this._filterBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this._filterBox.Location = new System.Drawing.Point(136, 40);
			this._filterBox.Name = "_filterBox";
			this._filterBox.Size = new System.Drawing.Size(224, 20);
			this._filterBox.TabIndex = 3;
			this._filterBox.SelectedIndexChanged += new System.EventHandler(this.OnFilterBoxChanged);
			// 
			// _filterLabel
			// 
			this._filterLabel.Location = new System.Drawing.Point(24, 40);
			this._filterLabel.Name = "_filterLabel";
			this._filterLabel.Size = new System.Drawing.Size(64, 16);
			this._filterLabel.TabIndex = 2;
			this._filterLabel.Text = "フィルタ(&F)";
			this._filterLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _messageLabel
			// 
			this._messageLabel.Location = new System.Drawing.Point(16, 16);
			this._messageLabel.Name = "_messageLabel";
			this._messageLabel.Size = new System.Drawing.Size(432, 16);
			this._messageLabel.TabIndex = 1;
			this._messageLabel.Text = "まず検索対象を決めます。ここでの対象銘柄数が少ないほど高速に結果が得られます。";
			// 
			// _favoriteBox
			// 
			this._favoriteBox.Location = new System.Drawing.Point(0, 0);
			this._favoriteBox.Name = "_favoriteBox";
			this._favoriteBox.TabIndex = 0;
			// 
			// _primaryIndicatorGroup
			// 
			this._primaryIndicatorGroup.Controls.Add(this._lCondition);
			this._primaryIndicatorGroup.Controls.Add(this._numberBox);
			this._primaryIndicatorGroup.Controls.Add(this._countLabel);
			this._primaryIndicatorGroup.Controls.Add(this._primaryIndicatorBox);
			this._primaryIndicatorGroup.Controls.Add(this._primaryIndicatorLabel);
			this._primaryIndicatorGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._primaryIndicatorGroup.Location = new System.Drawing.Point(8, 112);
			this._primaryIndicatorGroup.Name = "_primaryIndicatorGroup";
			this._primaryIndicatorGroup.Size = new System.Drawing.Size(464, 96);
			this._primaryIndicatorGroup.TabIndex = 4;
			this._primaryIndicatorGroup.TabStop = false;
			this._primaryIndicatorGroup.Text = "２　ソート条件と件数の設定";
			// 
			// _lCondition
			// 
			this._lCondition.Location = new System.Drawing.Point(16, 16);
			this._lCondition.Name = "_lCondition";
			this._lCondition.Size = new System.Drawing.Size(432, 23);
			this._lCondition.TabIndex = 5;
			this._lCondition.Text = "ソート条件は拡張キットによって追加することができます。";
			this._lCondition.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _numberBox
			// 
			this._numberBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this._numberBox.Location = new System.Drawing.Point(136, 64);
			this._numberBox.Name = "_numberBox";
			this._numberBox.Size = new System.Drawing.Size(224, 20);
			this._numberBox.TabIndex = 8;
			// 
			// _countLabel
			// 
			this._countLabel.Location = new System.Drawing.Point(24, 64);
			this._countLabel.Name = "_countLabel";
			this._countLabel.TabIndex = 9;
			this._countLabel.Text = "表示件数(&N)";
			this._countLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _primaryIndicatorBox
			// 
			this._primaryIndicatorBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this._primaryIndicatorBox.Location = new System.Drawing.Point(136, 40);
			this._primaryIndicatorBox.Name = "_primaryIndicatorBox";
			this._primaryIndicatorBox.Size = new System.Drawing.Size(224, 20);
			this._primaryIndicatorBox.TabIndex = 7;
			// 
			// _primaryIndicatorLabel
			// 
			this._primaryIndicatorLabel.Location = new System.Drawing.Point(24, 40);
			this._primaryIndicatorLabel.Name = "_primaryIndicatorLabel";
			this._primaryIndicatorLabel.TabIndex = 6;
			this._primaryIndicatorLabel.Text = "ソート条件(&S)";
			this._primaryIndicatorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// _dateGroup
			// 
			this._dateGroup.Controls.Add(this._datePicker);
			this._dateGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._dateGroup.Location = new System.Drawing.Point(8, 216);
			this._dateGroup.Name = "_dateGroup";
			this._dateGroup.Size = new System.Drawing.Size(464, 48);
			this._dateGroup.TabIndex = 10;
			this._dateGroup.TabStop = false;
			this._dateGroup.Text = "３　スクリーニングの基準日";
			// 
			// _datePicker
			// 
			this._datePicker.Location = new System.Drawing.Point(16, 16);
			this._datePicker.MaxDate = new System.DateTime(2010, 12, 31, 0, 0, 0, 0);
			this._datePicker.MinDate = new System.DateTime(1996, 1, 1, 0, 0, 0, 0);
			this._datePicker.Name = "_datePicker";
			this._datePicker.TabIndex = 12;
			this._datePicker.ValueChanged += new System.EventHandler(this.OnDateChanged);
			// 
			// _progressBar
			// 
			this._progressBar.Location = new System.Drawing.Point(8, 272);
			this._progressBar.Name = "_progressBar";
			this._progressBar.Size = new System.Drawing.Size(464, 23);
			this._progressBar.TabIndex = 14;
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(312, 304);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 15;
			this._okButton.Text = "OK";
			this._okButton.Click += new System.EventHandler(this.OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(400, 304);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 16;
			this._cancelButton.Text = "キャンセル";
			this._cancelButton.Click += new System.EventHandler(this.OnCancel);
			// 
			// ScreeningDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(482, 336);
			this.Controls.Add(this._dateGroup);
			this.Controls.Add(this._primaryIndicatorGroup);
			this.Controls.Add(this._progressBar);
			this.Controls.Add(this._filterGroup);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ScreeningDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "スクリーニング条件設定";
			this._filterGroup.ResumeLayout(false);
			this._primaryIndicatorGroup.ResumeLayout(false);
			this._dateGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/*
		private void InitIndicatorAppearance() {
			ScreeningItem[] target = Env.CurrentIndicators.GetScreeningItems();
			_secondaryIndicatorBoxes = new CheckBox[target.Length];
			for(int i=0; i<target.Length; i++) {
				ScreeningItem item = target[i];
				_primaryIndicatorBox.Items.Add(item.Header);

				CheckBox ch = new CheckBox();
				ch.FlatStyle = FlatStyle.System;
				ch.Text = item.Header;
				ch.Left = 8 + 150 * (i % 3);
				ch.Width = 150;
				ch.Top  = 12 + 24 * (i / 3);
				_secondaryIndicatorGroup.Controls.Add(ch);
				_secondaryIndicatorBoxes[i] = ch;
			}
			_primaryIndicatorBox.SelectedIndex = 0;
		}
		*/
		private void InitUI() {
			ScreeningOrder o = Env.Options.PrevScreeningOrder;
			if(o!=null) {
				FilteredBrandEnumerator fbe = o.BrandEnumerator as FilteredBrandEnumerator;
				FilterType filter = fbe==null? FilterType.Favorite : fbe.FilterType;
				_filterBox.SelectedIndex = (int)filter;
				_numberBox.SelectedIndex = (int)o.PreferredResultCountType;
				_datePicker.Value = Util.IntToDate(o.OriginDate);
			}
			else {
				DateTime ld = Util.GuessLatestTradeDate();
				if(ld==DateTime.Today && DateTime.Now.Hour<17) {
					ld = ld.AddDays(-1);
					while(!Util.IsMarketOpenDate(ld)) ld = ld.AddDays(-1); //市場が開いている日の17時以前はデータ取れない
				}
				_datePicker.Value = ld;
			}

			int si = 0;
			ScreeningItem[] target = Env.CurrentIndicators.GetScreeningItems();
			for(int i=0; i<target.Length; i++) {
				ScreeningItem item = target[i];
				_primaryIndicatorBox.Items.Add(item.Header);
				if(o!=null && o.PrimaryItem==target[i])
					si = i;
			}
			_primaryIndicatorBox.SelectedIndex = si;

		}

		private void OnOK(object sender, EventArgs args) {
			this.DialogResult = DialogResult.None;
			_order = CreateScreeningOrder();
			if(_order==null) return;

			_okButton.Enabled = false;
			_filterBox.Enabled = false;
			_numberBox.Enabled = false;
			_primaryIndicatorBox.Enabled = false;
			_datePicker.Enabled = false;
			/*
			foreach(CheckBox ch in _secondaryIndicatorBoxes)
				ch.Enabled = false;
			*/

			_progressBar.Maximum = _order.BrandEnumerator.Count;
			_progressBar.Value = 0;
			_progressBar.Enabled = true;
			Env.Options.PrevScreeningOrder = _order;

			this.Cursor = Cursors.AppStarting;
			_order.AsyncExecute(this.Handle);
		}

		private void OnCancel(object sender, EventArgs args) {
			if(_order!=null) {
				_order.Abort();
			}
		}

		private void OnDateChanged(object sender, EventArgs args) {
			DateTime dt = _datePicker.Value;
		}

		private ScreeningOrder CreateScreeningOrder() {
			if(_datePicker.Value.CompareTo(DateTime.Today)>0) {
				Util.Warning(this, "基準日が未来です。");
				return null;
			}

			ScreeningItem[] items = Env.CurrentIndicators.GetScreeningItems();
			int pr = _primaryIndicatorBox.SelectedIndex;
			ScreeningOrder so = new ScreeningOrder(items[pr].Header, items[pr]);
			FilterType filter = (FilterType)_filterBox.SelectedIndex;
			so.OriginDate = Util.DateToInt(_datePicker.Value);

			if(filter == FilterType.Favorite){
				if(_bookmarkFolder==null){
					Util.Warning(this, "お気に入りのフォルダを指定してください。");
					return null;
				}
				so.BrandEnumerator = new BookmarkedBrandEnumerator(_bookmarkFolder);
			}
			else
				so.BrandEnumerator = new FilteredBrandEnumerator(filter);

			return so;
		}

		private void ExitSuccessFully() {
			this.DialogResult = DialogResult.OK;
			this.Cursor = Cursors.Default;
			ScreeningResult r = _order.Result as ScreeningResult;
			StringBuilder msg = new StringBuilder();
			if(r.DataErrorBrands.Count==0) {
				msg.AppendFormat("スクリーニングが完了しました。\n対象銘柄数は {0} でした。\n", r.CheckedBrandCount);
				if(_order.PrimaryItem.Filter!=null)
					msg.AppendFormat("ただしフィルタで除外された {0} 銘柄は除外されました。", r.FilteredCount);
			}
			else {
				msg.AppendFormat("スクリーニングが完了しました。\n対象銘柄数 {0} のうち、\n対象日付のデータがなかった {1} 銘柄", r.CheckedBrandCount, r.DataErrorBrands.Count);
				if(r.DataErrorBrands.Count <= 10) {
					for(int i=0; i<r.DataErrorBrands.Count; i++) {
						if(i > 0) msg.Append(", ");
						msg.Append((r.DataErrorBrands[i] as BasicBrand).Code.ToString());
					}
				}
				for(int i=0; i<r.DataErrorBrands.Count; i++) {
					Debug.WriteLine("DataError "+(r.DataErrorBrands[i] as BasicBrand).Code);
				}
				if(_order.PrimaryItem.Filter!=null)
					msg.AppendFormat("、\nフィルタで除外された {0} 銘柄", r.FilteredCount);
				msg.Append("は除外されました。");
			}

			Util.Information(this, msg.ToString());

			_order.PreferredResultCountType = (ResultCountType)_numberBox.SelectedIndex;
			Close();
		}

		protected override void WndProc(ref Message m) {
			base.WndProc (ref m);
			if(m.Msg==AsyncConst.WM_ASYNCPROCESS) {
				if(m.LParam.ToInt32()==AsyncConst.LPARAM_PROGRESS_SUCCESSFUL || m.LParam.ToInt32()==AsyncConst.LPARAM_PROGRESS_FAILURE) {
					if(_progressBar.Value<_progressBar.Maximum) _progressBar.Value++;
				}
				else if(m.LParam.ToInt32()==AsyncConst.LPARAM_FINISHED)
					ExitSuccessFully();
				else if(m.LParam.ToInt32()==AsyncConst.LPARAM_ERROR) {
					this.Cursor = Cursors.Default;
					_order.Abort();
					Util.Warning(this, _order.ErrorMessage);
					this.DialogResult = DialogResult.Abort;
					Close();
				}
			}
		}

		private void OnSelectBookmarkFolder(object sender, System.EventArgs e){
			SelectBookmarkFolder dlg = new SelectBookmarkFolder();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK){
				_bookmarkFolder = dlg.ResultFolder;
				_bookmarkFolderTextBox.Text = _bookmarkFolder.Name;
			}
		}

		private void OnFilterBoxChanged(object sender, System.EventArgs e){
			bool en = ((FilterType)_filterBox.SelectedIndex == FilterType.Favorite);
			_bookmarkFolderTextBox.Enabled = en;
			_specBookmarkFolderButton.Enabled = en;
		}
	}
}
