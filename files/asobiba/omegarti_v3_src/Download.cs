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

using Zanetti.Commands;
using Zanetti.DataSource;
using Zanetti.Data;
using Zanetti.UI;

namespace Zanetti.Forms
{
	/// <summary>
	/// Download の概要の説明です。
	/// </summary>
	internal class DownloadWizard : System.Windows.Forms.Form
	{
		private DownloadOrder _currentOrder;
		private CombinedDataSource _dataSource;
		private int[] _downloadStartDates;
		private ArrayList _downloadErrorLogs;
		private long _downloadStartTime;
		private bool _runScreeningNow;
		private bool _initializeDataRequested;

		private ToolTip _toolTip;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.Button _donationButton;

		private System.Windows.Forms.GroupBox _orderGroup;
		private System.Windows.Forms.CheckBox _stockCheckBox;
		private System.Windows.Forms.GroupBox _stockGroup;
		private System.Windows.Forms.RadioButton _radioDailyStock;
		private System.Windows.Forms.RadioButton _radioAllStock;
		private System.Windows.Forms.Button _allStockButton;
#if KENMILLE
		private System.Windows.Forms.RadioButton _radioKenmilleStock;
#endif
		private System.Windows.Forms.ComboBox _dateBox;
		private System.Windows.Forms.CheckBox _indicesCheckBox;
		private System.Windows.Forms.CheckBox _sectorsCheckBox;
		private System.Windows.Forms.CheckBox _currenciesCheckBox;
		private System.Windows.Forms.CheckBox _commodityCheckBox;
		private System.Windows.Forms.CheckBox _namingCheckBox;
		private System.Windows.Forms.GroupBox _namingGroup;
		private System.Windows.Forms.Label _locationLabel;
		private System.Windows.Forms.ComboBox _locationBox;
		private System.Windows.Forms.Label _nameLabel;
		private System.Windows.Forms.TextBox _nameTextBox;
		private System.Windows.Forms.Label _progressLabel;
		private System.Windows.Forms.ProgressBar _progressBar;
        //hacked by rti
        private CheckBox _fundamentalCheckBox;
        private CheckBox _IsIgnoreObsoleteBox;
        //-----------------
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public DownloadWizard(DownloadOrder order) {
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			_toolTip = new ToolTip();
			_currentOrder = order;
			ShowOrderGroup();
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


		public bool RunScreeningNow {
			get {
				return _runScreeningNow;
			}
			set {
				_runScreeningNow = value;
			}
		}

		public bool InitializeDataRequested {
			get {
				return _initializeDataRequested;
			}
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
            this._okButton = new System.Windows.Forms.Button();
            this._cancelButton = new System.Windows.Forms.Button();
            this._donationButton = new System.Windows.Forms.Button();
            this._orderGroup = new System.Windows.Forms.GroupBox();
            this._IsIgnoreObsoleteBox = new System.Windows.Forms.CheckBox();
            this._fundamentalCheckBox = new System.Windows.Forms.CheckBox();
            this._currenciesCheckBox = new System.Windows.Forms.CheckBox();
            this._commodityCheckBox = new System.Windows.Forms.CheckBox();
            this._stockCheckBox = new System.Windows.Forms.CheckBox();
            this._sectorsCheckBox = new System.Windows.Forms.CheckBox();
            this._stockGroup = new System.Windows.Forms.GroupBox();
            this._dateBox = new System.Windows.Forms.ComboBox();
            this._radioDailyStock = new System.Windows.Forms.RadioButton();
            this._radioAllStock = new System.Windows.Forms.RadioButton();
            this._allStockButton = new System.Windows.Forms.Button();
            this._indicesCheckBox = new System.Windows.Forms.CheckBox();
            this._namingCheckBox = new System.Windows.Forms.CheckBox();
            this._namingGroup = new System.Windows.Forms.GroupBox();
            this._nameTextBox = new System.Windows.Forms.TextBox();
            this._nameLabel = new System.Windows.Forms.Label();
            this._locationBox = new System.Windows.Forms.ComboBox();
            this._locationLabel = new System.Windows.Forms.Label();
            this._progressBar = new System.Windows.Forms.ProgressBar();
            this._progressLabel = new System.Windows.Forms.Label();
            this._orderGroup.SuspendLayout();
            this._stockGroup.SuspendLayout();
            this._namingGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // _okButton
            // 
            this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._okButton.Location = new System.Drawing.Point(272, 338);
            this._okButton.Name = "_okButton";
            this._okButton.Size = new System.Drawing.Size(75, 23);
            this._okButton.TabIndex = 0;
            this._okButton.Text = "開始";
            this._okButton.Click += new System.EventHandler(this.OnOK);
            // 
            // _cancelButton
            // 
            this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._cancelButton.Location = new System.Drawing.Point(360, 338);
            this._cancelButton.Name = "_cancelButton";
            this._cancelButton.Size = new System.Drawing.Size(75, 23);
            this._cancelButton.TabIndex = 1;
            this._cancelButton.Text = "キャンセル";
            this._cancelButton.Click += new System.EventHandler(this.OnCancel);
            // 
            // _donationButton
            // 
            this._donationButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._donationButton.Location = new System.Drawing.Point(16, 338);
            this._donationButton.Name = "_donationButton";
            this._donationButton.Size = new System.Drawing.Size(80, 24);
            this._donationButton.TabIndex = 100;
            this._donationButton.Text = "寄付について...";
            this._donationButton.Click += new System.EventHandler(this.OnDonationButtonClick);
            // 
            // _orderGroup
            // 
            this._orderGroup.Controls.Add(this._IsIgnoreObsoleteBox);
            this._orderGroup.Controls.Add(this._fundamentalCheckBox);
            this._orderGroup.Controls.Add(this._currenciesCheckBox);
            this._orderGroup.Controls.Add(this._commodityCheckBox);
            this._orderGroup.Controls.Add(this._stockCheckBox);
            this._orderGroup.Controls.Add(this._sectorsCheckBox);
            this._orderGroup.Controls.Add(this._stockGroup);
            this._orderGroup.Controls.Add(this._indicesCheckBox);
            this._orderGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._orderGroup.Location = new System.Drawing.Point(8, 6);
            this._orderGroup.Name = "_orderGroup";
            this._orderGroup.Size = new System.Drawing.Size(440, 200);
            this._orderGroup.TabIndex = 2;
            this._orderGroup.TabStop = false;
            this._orderGroup.Text = "ダウンロードするデータの選択";
            // 
            // _IsIgnoreObsoleteBox
            // 
            this._IsIgnoreObsoleteBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._IsIgnoreObsoleteBox.Location = new System.Drawing.Point(223, 170);
            this._IsIgnoreObsoleteBox.Name = "_IsIgnoreObsoleteBox";
            this._IsIgnoreObsoleteBox.Size = new System.Drawing.Size(169, 24);
            this._IsIgnoreObsoleteBox.TabIndex = 10;
            this._IsIgnoreObsoleteBox.Text = "上場廃止銘柄を無視する";
            // 
            // _fundamentalCheckBox
            // 
            this._fundamentalCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._fundamentalCheckBox.Location = new System.Drawing.Point(15, 170);
            this._fundamentalCheckBox.Name = "_fundamentalCheckBox";
            this._fundamentalCheckBox.Size = new System.Drawing.Size(169, 24);
            this._fundamentalCheckBox.TabIndex = 9;
            this._fundamentalCheckBox.Text = "業績情報(&F)";
            // 
            // _currenciesCheckBox
            // 
            this._currenciesCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._currenciesCheckBox.Location = new System.Drawing.Point(224, 120);
            this._currenciesCheckBox.Name = "_currenciesCheckBox";
            this._currenciesCheckBox.Size = new System.Drawing.Size(168, 24);
            this._currenciesCheckBox.TabIndex = 6;
            this._currenciesCheckBox.Text = "為替(&C)";
            // 
            // _commodityCheckBox
            // 
            this._commodityCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._commodityCheckBox.Location = new System.Drawing.Point(224, 144);
            this._commodityCheckBox.Name = "_commodityCheckBox";
            this._commodityCheckBox.Size = new System.Drawing.Size(168, 24);
            this._commodityCheckBox.TabIndex = 8;
            this._commodityCheckBox.Text = "商品先物(&O)";
            this._commodityCheckBox.Visible = false;
            // 
            // _stockCheckBox
            // 
            this._stockCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._stockCheckBox.Location = new System.Drawing.Point(16, 15);
            this._stockCheckBox.Name = "_stockCheckBox";
            this._stockCheckBox.Size = new System.Drawing.Size(176, 24);
            this._stockCheckBox.TabIndex = 3;
            this._stockCheckBox.Text = "国内株式(&S)";
            this._stockCheckBox.CheckedChanged += new System.EventHandler(this.OnAdjustStockDownload);
            // 
            // _sectorsCheckBox
            // 
            this._sectorsCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._sectorsCheckBox.Location = new System.Drawing.Point(15, 144);
            this._sectorsCheckBox.Name = "_sectorsCheckBox";
            this._sectorsCheckBox.Size = new System.Drawing.Size(169, 24);
            this._sectorsCheckBox.TabIndex = 7;
            this._sectorsCheckBox.Text = "業種別指数(&E)";
            // 
            // _stockGroup
            // 
            this._stockGroup.Controls.Add(this._dateBox);
            this._stockGroup.Controls.Add(this._radioDailyStock);
            this._stockGroup.Controls.Add(this._radioAllStock);
            this._stockGroup.Controls.Add(this._allStockButton);
            this._stockGroup.Enabled = false;
            this._stockGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._stockGroup.Location = new System.Drawing.Point(8, 22);
            this._stockGroup.Name = "_stockGroup";
            this._stockGroup.Size = new System.Drawing.Size(424, 96);
            this._stockGroup.TabIndex = 4;
            this._stockGroup.TabStop = false;
            // 
            // _dateBox
            // 
            this._dateBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._dateBox.Enabled = false;
            this._dateBox.Location = new System.Drawing.Point(208, 16);
            this._dateBox.Name = "_dateBox";
            this._dateBox.Size = new System.Drawing.Size(137, 20);
            this._dateBox.TabIndex = 1;
            // 
            // _radioDailyStock
            // 
            this._radioDailyStock.Checked = true;
            this._radioDailyStock.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._radioDailyStock.Location = new System.Drawing.Point(16, 16);
            this._radioDailyStock.Name = "_radioDailyStock";
            this._radioDailyStock.Size = new System.Drawing.Size(152, 24);
            this._radioDailyStock.TabIndex = 0;
            this._radioDailyStock.TabStop = true;
            this._radioDailyStock.Text = "次の日付以降・全銘柄(&D)";
            this._radioDailyStock.CheckedChanged += new System.EventHandler(this.OnAdjustDailyStockCheckBox);
            // 
            // _radioAllStock
            // 
            this._radioAllStock.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._radioAllStock.Location = new System.Drawing.Point(16, 40);
            this._radioAllStock.Name = "_radioAllStock";
            this._radioAllStock.Size = new System.Drawing.Size(192, 24);
            this._radioAllStock.TabIndex = 2;
            this._radioAllStock.Text = "データの初期化(&A)";
            this._radioAllStock.CheckedChanged += new System.EventHandler(this.OnAllStockRadioButtonCheckedChanged);
            // 
            // _allStockButton
            // 
            this._allStockButton.Enabled = false;
            this._allStockButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._allStockButton.Location = new System.Drawing.Point(254, 40);
            this._allStockButton.Name = "_allStockButton";
            this._allStockButton.Size = new System.Drawing.Size(96, 24);
            this._allStockButton.TabIndex = 3;
            this._allStockButton.Text = "初期化(&S)...";
            this._allStockButton.Click += new System.EventHandler(this.OnInitializeButtonClicked);
            // 
            // _indicesCheckBox
            // 
            this._indicesCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._indicesCheckBox.Location = new System.Drawing.Point(16, 120);
            this._indicesCheckBox.Name = "_indicesCheckBox";
            this._indicesCheckBox.Size = new System.Drawing.Size(168, 24);
            this._indicesCheckBox.TabIndex = 5;
            this._indicesCheckBox.Text = "株価指数(&I)";
            // 
            // _namingCheckBox
            // 
            this._namingCheckBox.Checked = true;
            this._namingCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this._namingCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._namingCheckBox.Location = new System.Drawing.Point(24, 211);
            this._namingCheckBox.Name = "_namingCheckBox";
            this._namingCheckBox.Size = new System.Drawing.Size(384, 24);
            this._namingCheckBox.TabIndex = 9;
            this._namingCheckBox.Text = "このダウンロード設定を保存して次回以降再利用する(&P)";
            this._namingCheckBox.CheckedChanged += new System.EventHandler(this.OnAdjustNamingCheckBox);
            // 
            // _namingGroup
            // 
            this._namingGroup.Controls.Add(this._nameTextBox);
            this._namingGroup.Controls.Add(this._nameLabel);
            this._namingGroup.Controls.Add(this._locationBox);
            this._namingGroup.Controls.Add(this._locationLabel);
            this._namingGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._namingGroup.Location = new System.Drawing.Point(8, 218);
            this._namingGroup.Name = "_namingGroup";
            this._namingGroup.Size = new System.Drawing.Size(440, 56);
            this._namingGroup.TabIndex = 9;
            this._namingGroup.TabStop = false;
            // 
            // _nameTextBox
            // 
            this._nameTextBox.Location = new System.Drawing.Point(328, 24);
            this._nameTextBox.Name = "_nameTextBox";
            this._nameTextBox.Size = new System.Drawing.Size(104, 19);
            this._nameTextBox.TabIndex = 3;
            // 
            // _nameLabel
            // 
            this._nameLabel.Location = new System.Drawing.Point(240, 24);
            this._nameLabel.Name = "_nameLabel";
            this._nameLabel.Size = new System.Drawing.Size(48, 23);
            this._nameLabel.TabIndex = 2;
            this._nameLabel.Text = "名前(&N)";
            this._nameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _locationBox
            // 
            this._locationBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._locationBox.Location = new System.Drawing.Point(128, 24);
            this._locationBox.Name = "_locationBox";
            this._locationBox.Size = new System.Drawing.Size(104, 20);
            this._locationBox.TabIndex = 1;
            // 
            // _locationLabel
            // 
            this._locationLabel.Location = new System.Drawing.Point(16, 24);
            this._locationLabel.Name = "_locationLabel";
            this._locationLabel.Size = new System.Drawing.Size(104, 23);
            this._locationLabel.TabIndex = 0;
            this._locationLabel.Text = "保存場所(&L)";
            this._locationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _progressBar
            // 
            this._progressBar.Location = new System.Drawing.Point(8, 306);
            this._progressBar.Name = "_progressBar";
            this._progressBar.Size = new System.Drawing.Size(440, 23);
            this._progressBar.TabIndex = 10;
            // 
            // _progressLabel
            // 
            this._progressLabel.Location = new System.Drawing.Point(8, 282);
            this._progressLabel.Name = "_progressLabel";
            this._progressLabel.Size = new System.Drawing.Size(344, 16);
            this._progressLabel.TabIndex = 11;
            this._progressLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // DownloadWizard
            // 
            this.AcceptButton = this._okButton;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
            this.CancelButton = this._cancelButton;
            this.ClientSize = new System.Drawing.Size(465, 374);
            this.Controls.Add(this._orderGroup);
            this.Controls.Add(this._progressLabel);
            this.Controls.Add(this._progressBar);
            this.Controls.Add(this._namingCheckBox);
            this.Controls.Add(this._namingGroup);
            this.Controls.Add(this._cancelButton);
            this.Controls.Add(this._okButton);
            this.Controls.Add(this._donationButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "DownloadWizard";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "データのダウンロード";
            this._orderGroup.ResumeLayout(false);
            this._stockGroup.ResumeLayout(false);
            this._namingGroup.ResumeLayout(false);
            this._namingGroup.PerformLayout();
            this.ResumeLayout(false);

		}
		#endregion

		public DownloadOrder DownloadOrder {
			get {
				return _currentOrder;
			}
			set {
				_currentOrder = value;
			}
		}

		private const int PANEL_LEFT = 8;
		//各ページの初期化
		private void ShowOrderGroup() {
			if(_currentOrder==null) _currentOrder = DownloadOrder.CreateDefault();

			_stockCheckBox.Checked = _currentOrder.Stock!=DownloadOrder.StockDownload.None;
			//日付指定DLのみしか所期状態では選択できないようにする
			_indicesCheckBox.Checked = _currentOrder.ForeignIndices;
			_sectorsCheckBox.Checked = _currentOrder.Sectors;
			_currenciesCheckBox.Checked = _currentOrder.Currencies;
			_commodityCheckBox.Checked = _currentOrder.Commodities;
            //hacked by rti
            _fundamentalCheckBox.Checked = _currentOrder.Fundamental;
            _IsIgnoreObsoleteBox.Checked = _currentOrder.IsIgnoreObsolete;
            //--------------------------
#if DOJIMA
			//商品先物を有効化
			_commodityCheckBox.Visible = true;
#endif

			//日付リスト
			_dateBox.Items.Clear();
			_downloadStartDates = ListDates();
			FormatDateBox();

			//保存オプション
			int initial_index = 0;
			_locationBox.Items.Clear();
			for(int i=0; i<Env.Options.DownloadOrders.Count; i++) {
				DownloadOrder co = Env.Options.DownloadOrders[i];
				if(_currentOrder==co) initial_index= i;
				_locationBox.Items.Add(String.Format("設定{0} ({1})", i+1, Env.Options.DownloadOrders.GetDescription(i)));
			}
			_locationBox.SelectedIndex = initial_index;
			_namingCheckBox.Checked = _currentOrder.Name!=null;
			_nameTextBox.Text = _currentOrder.Name;
		}
		private bool CommitOrderGroup() {
			if(!_stockCheckBox.Checked)
				_currentOrder.Stock = DownloadOrder.StockDownload.None;
			else if(_radioDailyStock.Checked) {
				_currentOrder.Stock = DownloadOrder.StockDownload.Recent;
				_currentOrder.DateArray = CreateDateArray(_downloadStartDates, _dateBox.SelectedIndex);
			}
#if KENMILLE
			else if(_radioKenmilleStock.Checked) {
				_currentOrder.Stock = DownloadOrder.StockDownload.All;
			}
#endif
			else
				return false;

            _currentOrder.ForeignIndices = _indicesCheckBox.Checked;
			_currentOrder.DomesticIndices = _indicesCheckBox.Checked;
			_currentOrder.Sectors = _sectorsCheckBox.Checked;
			_currentOrder.Currencies = _currenciesCheckBox.Checked;
			_currentOrder.Commodities = _commodityCheckBox.Checked;
            //hacked by rti
            _currentOrder.Fundamental = _fundamentalCheckBox.Checked;
            _currentOrder.IsIgnoreObsolete = _IsIgnoreObsoleteBox.Checked;
            //-------------
			return true;
		}
		private bool CommitNamingGroup() {
			if(_namingCheckBox.Checked) {
				if(_nameTextBox.Text.Length==0) {
					Util.Warning(this, "保存先の名前が入力されていません。");
					return false;
				}
				_currentOrder.Name = _nameTextBox.Text; 
				Env.Options.DownloadOrders[_locationBox.SelectedIndex] = _currentOrder;
				Env.Frame.InitMRUDownloadMenu();
			}
			return true;
		}

		private void OnAdjustStockDownload(object sender, EventArgs args) {
			bool e = _stockCheckBox.Checked;
			_stockGroup.Enabled = e;
			_dateBox.Enabled = e && _radioDailyStock.Checked;
		}
		private void OnAdjustDailyStockCheckBox(object sender, EventArgs args) {
			bool e = _radioDailyStock.Checked;
			_dateBox.Enabled = e;
		}
		private void OnAdjustNamingCheckBox(object sender, EventArgs args) {
			bool e = _namingCheckBox.Checked;
			_locationBox.Enabled = e;
			_nameTextBox.Enabled = e;
		}
		private void OnAllStockRadioButtonCheckedChanged(object sender, EventArgs args) {
			_allStockButton.Enabled = _radioAllStock.Checked;
		}
		private void OnInitializeButtonClicked(object sender, EventArgs args) {
			this.DialogResult = DialogResult.Cancel;
			_initializeDataRequested = true;
			Close();
			Env.Command.Exec(CID.ShowInitializeDialog);
		}

		private void OnOK(object sender, EventArgs args) {
			this.DialogResult = DialogResult.None;
			//両方の内容をチェックできたらダウンロード開始
			if(CommitOrderGroup() && CommitNamingGroup()) StartDownload();
		}
		private void OnCancel(object sender, EventArgs args) {
			if(_dataSource!=null) _dataSource.Abort();
			this.Close();
		}

		private void StartDownload() {
			_dataSource = _currentOrder.ConvertToDataSource();
			if(_dataSource.Count==0) {
				Util.Warning(this, "ダウンロード対象が選択されていません。");
				return;
			}
			_okButton.Enabled = false;
			_downloadErrorLogs = new ArrayList();
			_downloadStartTime = DateTime.Now.Ticks;
            _orderGroup.Enabled = false;
            _namingGroup.Enabled = false;
            _namingCheckBox.Enabled = false;
            _progressBar.Enabled = true;
			_progressBar.Value = 0;
			_progressBar.Maximum = _dataSource.TotalStep;
			_dataSource.AsyncProcess(this.Handle);
		}
		private void ProgressStatus(int code, int wparam) {
			try {
				if(code==AsyncConst.LPARAM_PROGRESS_SUCCESSFUL || code==AsyncConst.LPARAM_PROGRESS_FAILURE) {
					if((wparam & DataSourceBase.DATE_MASK)!=0) {
						_progressLabel.Text = String.Format("{0}のデータを取得中", Util.FormatShortDate(wparam));
						if(code==AsyncConst.LPARAM_PROGRESS_FAILURE) {
							string msg = String.Format("{0}のデータが取得できませんでした。{1}", wparam, _dataSource.ErrorMessage);
							_downloadErrorLogs.Add(msg);
						}
					}
					else {
						if(_progressBar.Value < _progressBar.Maximum) _progressBar.Value++; //つまらないミスでバーの上限値を越えた値をセットしないためガード
						long left = (DateTime.Now.Ticks-_downloadStartTime) / _progressBar.Value * (_progressBar.Maximum-_progressBar.Value) / 10000000;
						string message = String.Format("{0}/{1}   残り 約 {2}分{3:D2}秒",_progressBar.Value, _progressBar.Maximum, left/60, left % 60);
						_progressLabel.Text = message;
						if(code==AsyncConst.LPARAM_PROGRESS_FAILURE) {
							string msg = String.Format("{0}({1})のデータが取得できませんでした。{2}", Env.BrandCollection.FindBrand(wparam).Name, wparam, _dataSource.ErrorMessage);
							_downloadErrorLogs.Add(msg);
						}
					}
				}
				else if(code==AsyncConst.LPARAM_FINISHED) {
					this.DialogResult = DialogResult.OK;
					//最新日付へ
					AbstractBrand b = Env.Frame.ChartCanvas.GetBrand();
					b.ReserveFarm().LoadFor(b);
					Env.Frame.ChartCanvas.LoadBrand(b, false);
					Env.Frame.ChartCanvas.MoveToLatest();
					Close();
				}
				else if(code==AsyncConst.LPARAM_ERROR) {
					Util.Warning(this, _dataSource.ErrorMessage);
                    this.DialogResult = DialogResult.Cancel;
                }
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				Util.Warning(this, ex.Message);
			}
		}

		private void AddHelpButton(int left, int top, Control parent, string text) {
			Button btn = new NotClickableButton();
			btn.Left = left;
			btn.Top = top;
			btn.Width = 19;
			btn.Height = 19;
			btn.FlatStyle = FlatStyle.Flat;
			btn.Image = Env.ImageList16.Images[IconConst.HELP];
			btn.Text = "";
			_toolTip.SetToolTip(btn, text);
			parent.Controls.Add(btn);
			btn.BringToFront();
		}
		private void FormatDateBox() {
			DataFarm f = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
			int latest = f.IsEmpty? Int32.MaxValue : f.LastDate;

			int sel = 0;
			for(int i=0; i<_downloadStartDates.Length; i++) {
				_dateBox.Items.Add(Util.FormatFullDate(_downloadStartDates[i]));
				if(_downloadStartDates[i] > latest) sel = i; //今見ているチャートの最新日付の次の日がセットされているようにする　EmptyのときはSelectedIndexは０．
			}
			_dateBox.SelectedIndex = sel;
		}

		protected override void WndProc(ref Message m) {
			base.WndProc (ref m);
			if(m.Msg==AsyncConst.WM_ASYNCPROCESS) {
				ProgressStatus(m.LParam.ToInt32(), m.WParam.ToInt32());
			}
		}

		private static int[] ListDates() {
			DateTime ld = Util.GuessLatestTradeDate();
			//市場が開いている日については、日本時間19時～24時(UTCでは10時～15時)まで取得可能。
			//無尽蔵でデータの準備ができるのが18～19時だった
			if(DateTime.Today==ld && (DateTime.UtcNow.Hour<10 || DateTime.UtcNow.Hour>=15)) ld = ld.AddDays(-1);

            //hacked by rti
            //せっかくだから、俺は多めにダウンロードできるようにするぜ
            DateTime start = ld.AddMonths(-100);
			//DateTime start = ld.AddMonths(-2);
            //---------------------------------
			ArrayList result = new ArrayList();
			while(start.CompareTo(ld)<0) {
				if(Util.IsMarketOpenDate(ld)) {
					result.Add(Util.DateToInt(ld));
				}
				ld = ld.AddDays(-1); 
			}

			return (int[])result.ToArray(typeof(int));
		}

		private static int[] CreateDateArray(int[] dates, int start) 
		{
			int[] t = new int[start+1];
			for(int i=start; i>=0; i--)
				t[start-i] = dates[i];
			return t;
		}
		

		private void OnDonationButtonClick(object sender, EventArgs args) {
			new PromptDonation().ShowDialog(this);
		}
	}
}
