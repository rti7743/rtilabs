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
using Zanetti.SystemTrading.AutoTrading;
using Zanetti.Config;	// for BookmarkFolder
//hacked by rti
using Zanetti.SystemTrading.MoneyPlanning;
using Zanetti.Commands;
using Zanetti.Indicators.Schema;
//-----------

namespace Zanetti.Forms {
	internal class AutoTradingDialog : System.Windows.Forms.Form {
		private static readonly int[] _signalCountValues = new int[] { 100,1000,Int32.MaxValue };
		private static AutoTradingExecutor _prevExecutor; //パラメータ保存用

		private AutoTradingExecutor _executor;
		private int _currentBrandIndex;

		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.GroupBox _filterGroup;
		private System.Windows.Forms.Label _messageLabel;
		private System.Windows.Forms.Label _filterLabel;
		private System.Windows.Forms.ComboBox _filterBox;
		private System.Windows.Forms.GroupBox _autoTradingItemGroup;
		private System.Windows.Forms.Label _lCondition;
		private System.Windows.Forms.Label _autoTradingItemLabel;
		private System.Windows.Forms.ComboBox _autoTradingItemBox;
		private System.Windows.Forms.GroupBox _dateGroup;
		private System.Windows.Forms.Label _startDateLabel;
		private System.Windows.Forms.DateTimePicker _startDatePicker;
		private System.Windows.Forms.Label _endDateLabel;
		private System.Windows.Forms.DateTimePicker _endDatePicker;
		private System.Windows.Forms.Label _signalCountLabel;
        private System.Windows.Forms.ComboBox _signalCountBox;
        private System.Windows.Forms.ProgressBar _progressBar;
        private System.Windows.Forms.ComboBox _favoriteBox;
		private System.Windows.Forms.Button _specBookmarkFolderButton;
		private System.Windows.Forms.TextBox _bookmarkFolderTextBox;
		private System.Windows.Forms.Label _filterBookmarkLabel;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
        private System.ComponentModel.Container components = null;
        private GroupBox groupBox1;
        private Label label1;
        private ComboBox _moneyPlanningItemBox;
        private CheckBox _autoReloadCheckBox;
        private Button _itemEditButton;
        private Button _moneyPlanningItemEditbutton;

		private BookmarkFolder _bookmarkFolder;

		public AutoTradingDialog() {
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			InitUI();
		}

		public AutoTradingResult Result {
			get {
				return _executor==null? null : _executor.Result as AutoTradingResult;
			}
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
            this._filterGroup = new System.Windows.Forms.GroupBox();
            this._specBookmarkFolderButton = new System.Windows.Forms.Button();
            this._bookmarkFolderTextBox = new System.Windows.Forms.TextBox();
            this._filterBookmarkLabel = new System.Windows.Forms.Label();
            this._filterBox = new System.Windows.Forms.ComboBox();
            this._filterLabel = new System.Windows.Forms.Label();
            this._messageLabel = new System.Windows.Forms.Label();
            this._favoriteBox = new System.Windows.Forms.ComboBox();
            this._autoTradingItemGroup = new System.Windows.Forms.GroupBox();
            this._itemEditButton = new System.Windows.Forms.Button();
            this._lCondition = new System.Windows.Forms.Label();
            this._autoTradingItemBox = new System.Windows.Forms.ComboBox();
            this._autoTradingItemLabel = new System.Windows.Forms.Label();
            this._dateGroup = new System.Windows.Forms.GroupBox();
            this._startDateLabel = new System.Windows.Forms.Label();
            this._startDatePicker = new System.Windows.Forms.DateTimePicker();
            this._endDateLabel = new System.Windows.Forms.Label();
            this._endDatePicker = new System.Windows.Forms.DateTimePicker();
            this._signalCountLabel = new System.Windows.Forms.Label();
            this._signalCountBox = new System.Windows.Forms.ComboBox();
            this._progressBar = new System.Windows.Forms.ProgressBar();
            this._okButton = new System.Windows.Forms.Button();
            this._cancelButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this._moneyPlanningItemEditbutton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this._moneyPlanningItemBox = new System.Windows.Forms.ComboBox();
            this._autoReloadCheckBox = new System.Windows.Forms.CheckBox();
            this._filterGroup.SuspendLayout();
            this._autoTradingItemGroup.SuspendLayout();
            this._dateGroup.SuspendLayout();
            this.groupBox1.SuspendLayout();
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
            this._filterGroup.Location = new System.Drawing.Point(11, 10);
            this._filterGroup.Name = "_filterGroup";
            this._filterGroup.Size = new System.Drawing.Size(650, 120);
            this._filterGroup.TabIndex = 0;
            this._filterGroup.TabStop = false;
            this._filterGroup.Text = "１　対象設定";
            // 
            // _specBookmarkFolderButton
            // 
            this._specBookmarkFolderButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._specBookmarkFolderButton.Location = new System.Drawing.Point(515, 80);
            this._specBookmarkFolderButton.Name = "_specBookmarkFolderButton";
            this._specBookmarkFolderButton.Size = new System.Drawing.Size(112, 30);
            this._specBookmarkFolderButton.TabIndex = 1;
            this._specBookmarkFolderButton.Text = "フォルダ指定";
            this._specBookmarkFolderButton.Click += new System.EventHandler(this.OnSelectBookmarkFolder);
            // 
            // _bookmarkFolderTextBox
            // 
            this._bookmarkFolderTextBox.Location = new System.Drawing.Point(190, 80);
            this._bookmarkFolderTextBox.Name = "_bookmarkFolderTextBox";
            this._bookmarkFolderTextBox.ReadOnly = true;
            this._bookmarkFolderTextBox.Size = new System.Drawing.Size(314, 22);
            this._bookmarkFolderTextBox.TabIndex = 5;
            // 
            // _filterBookmarkLabel
            // 
            this._filterBookmarkLabel.Location = new System.Drawing.Point(34, 80);
            this._filterBookmarkLabel.Name = "_filterBookmarkLabel";
            this._filterBookmarkLabel.Size = new System.Drawing.Size(100, 20);
            this._filterBookmarkLabel.TabIndex = 4;
            this._filterBookmarkLabel.Text = "お気に入り(&T)";
            this._filterBookmarkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _filterBox
            // 
            this._filterBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._filterBox.Location = new System.Drawing.Point(190, 50);
            this._filterBox.Name = "_filterBox";
            this._filterBox.Size = new System.Drawing.Size(314, 23);
            this._filterBox.TabIndex = 0;
            this._filterBox.SelectedIndexChanged += new System.EventHandler(this.OnFilterBoxChanged);
            // 
            // _filterLabel
            // 
            this._filterLabel.Location = new System.Drawing.Point(34, 50);
            this._filterLabel.Name = "_filterLabel";
            this._filterLabel.Size = new System.Drawing.Size(89, 20);
            this._filterLabel.TabIndex = 2;
            this._filterLabel.Text = "対象(&F)";
            this._filterLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _messageLabel
            // 
            this._messageLabel.Location = new System.Drawing.Point(22, 20);
            this._messageLabel.Name = "_messageLabel";
            this._messageLabel.Size = new System.Drawing.Size(605, 20);
            this._messageLabel.TabIndex = 1;
            this._messageLabel.Text = "まず検索対象を決めます。ここでの対象銘柄数が少ないほど高速に結果が得られます。";
            // 
            // _favoriteBox
            // 
            this._favoriteBox.Location = new System.Drawing.Point(0, 0);
            this._favoriteBox.Name = "_favoriteBox";
            this._favoriteBox.Size = new System.Drawing.Size(121, 23);
            this._favoriteBox.TabIndex = 0;
            // 
            // _autoTradingItemGroup
            // 
            this._autoTradingItemGroup.Controls.Add(this._itemEditButton);
            this._autoTradingItemGroup.Controls.Add(this._lCondition);
            this._autoTradingItemGroup.Controls.Add(this._autoTradingItemBox);
            this._autoTradingItemGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._autoTradingItemGroup.Location = new System.Drawing.Point(11, 140);
            this._autoTradingItemGroup.Name = "_autoTradingItemGroup";
            this._autoTradingItemGroup.Size = new System.Drawing.Size(650, 103);
            this._autoTradingItemGroup.TabIndex = 4;
            this._autoTradingItemGroup.TabStop = false;
            this._autoTradingItemGroup.Text = "２　自動売買ルールの選択";
            // 
            // _itemEditButton
            // 
            this._itemEditButton.Location = new System.Drawing.Point(585, 57);
            this._itemEditButton.Name = "_itemEditButton";
            this._itemEditButton.Size = new System.Drawing.Size(48, 23);
            this._itemEditButton.TabIndex = 6;
            this._itemEditButton.Text = "編集";
            this._itemEditButton.UseVisualStyleBackColor = true;
            this._itemEditButton.Click += new System.EventHandler(this._itemEditButton_Click);
            // 
            // _lCondition
            // 
            this._lCondition.Location = new System.Drawing.Point(22, 20);
            this._lCondition.Name = "_lCondition";
            this._lCondition.Size = new System.Drawing.Size(583, 29);
            this._lCondition.TabIndex = 5;
            this._lCondition.Text = "自動売買ルールは拡張キットによって追加することができます。また売買は条件成立日の終値で行ったものとして計算されます。";
            this._lCondition.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _autoTradingItemBox
            // 
            this._autoTradingItemBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._autoTradingItemBox.Location = new System.Drawing.Point(22, 58);
            this._autoTradingItemBox.Name = "_autoTradingItemBox";
            this._autoTradingItemBox.Size = new System.Drawing.Size(557, 23);
            this._autoTradingItemBox.TabIndex = 0;
            // 
            // _autoTradingItemLabel
            // 
            this._autoTradingItemLabel.Location = new System.Drawing.Point(0, 0);
            this._autoTradingItemLabel.Name = "_autoTradingItemLabel";
            this._autoTradingItemLabel.Size = new System.Drawing.Size(100, 23);
            this._autoTradingItemLabel.TabIndex = 0;
            // 
            // _dateGroup
            // 
            this._dateGroup.Controls.Add(this._startDateLabel);
            this._dateGroup.Controls.Add(this._startDatePicker);
            this._dateGroup.Controls.Add(this._endDateLabel);
            this._dateGroup.Controls.Add(this._endDatePicker);
            this._dateGroup.Controls.Add(this._signalCountLabel);
            this._dateGroup.Controls.Add(this._signalCountBox);
            this._dateGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._dateGroup.Location = new System.Drawing.Point(11, 249);
            this._dateGroup.Name = "_dateGroup";
            this._dateGroup.Size = new System.Drawing.Size(650, 125);
            this._dateGroup.TabIndex = 10;
            this._dateGroup.TabStop = false;
            this._dateGroup.Text = "３　対象期間など";
            // 
            // _startDateLabel
            // 
            this._startDateLabel.Location = new System.Drawing.Point(11, 20);
            this._startDateLabel.Name = "_startDateLabel";
            this._startDateLabel.Size = new System.Drawing.Size(90, 30);
            this._startDateLabel.TabIndex = 0;
            this._startDateLabel.Text = "開始日";
            this._startDateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _startDatePicker
            // 
            this._startDatePicker.Location = new System.Drawing.Point(112, 20);
            this._startDatePicker.MaxDate = new System.DateTime(2010, 12, 31, 0, 0, 0, 0);
            this._startDatePicker.MinDate = new System.DateTime(1996, 1, 1, 0, 0, 0, 0);
            this._startDatePicker.Name = "_startDatePicker";
            this._startDatePicker.Size = new System.Drawing.Size(280, 22);
            this._startDatePicker.TabIndex = 0;
            this._startDatePicker.ValueChanged += new System.EventHandler(this.OnStartDateChanged);
            // 
            // _endDateLabel
            // 
            this._endDateLabel.Location = new System.Drawing.Point(11, 50);
            this._endDateLabel.Name = "_endDateLabel";
            this._endDateLabel.Size = new System.Drawing.Size(90, 30);
            this._endDateLabel.TabIndex = 2;
            this._endDateLabel.Text = "終了日";
            this._endDateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _endDatePicker
            // 
            this._endDatePicker.Location = new System.Drawing.Point(112, 50);
            this._endDatePicker.MaxDate = new System.DateTime(2010, 12, 31, 0, 0, 0, 0);
            this._endDatePicker.MinDate = new System.DateTime(1996, 1, 1, 0, 0, 0, 0);
            this._endDatePicker.Name = "_endDatePicker";
            this._endDatePicker.Size = new System.Drawing.Size(280, 22);
            this._endDatePicker.TabIndex = 1;
            this._endDatePicker.ValueChanged += new System.EventHandler(this.OnEndDateChanged);
            // 
            // _signalCountLabel
            // 
            this._signalCountLabel.Location = new System.Drawing.Point(11, 80);
            this._signalCountLabel.Name = "_signalCountLabel";
            this._signalCountLabel.Size = new System.Drawing.Size(258, 30);
            this._signalCountLabel.TabIndex = 4;
            this._signalCountLabel.Text = "検索を打ち切るシグナル発生数(&S)";
            this._signalCountLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _signalCountBox
            // 
            this._signalCountBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._signalCountBox.Location = new System.Drawing.Point(269, 80);
            this._signalCountBox.Name = "_signalCountBox";
            this._signalCountBox.Size = new System.Drawing.Size(123, 23);
            this._signalCountBox.TabIndex = 2;
            // 
            // _progressBar
            // 
            this._progressBar.Location = new System.Drawing.Point(11, 471);
            this._progressBar.Name = "_progressBar";
            this._progressBar.Size = new System.Drawing.Size(650, 29);
            this._progressBar.TabIndex = 14;
            // 
            // _okButton
            // 
            this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._okButton.Location = new System.Drawing.Point(437, 509);
            this._okButton.Name = "_okButton";
            this._okButton.Size = new System.Drawing.Size(105, 29);
            this._okButton.TabIndex = 0;
            this._okButton.Text = "OK";
            this._okButton.Click += new System.EventHandler(this.OnOK);
            // 
            // _cancelButton
            // 
            this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._cancelButton.Location = new System.Drawing.Point(560, 509);
            this._cancelButton.Name = "_cancelButton";
            this._cancelButton.Size = new System.Drawing.Size(105, 29);
            this._cancelButton.TabIndex = 1;
            this._cancelButton.Text = "キャンセル";
            this._cancelButton.Click += new System.EventHandler(this.OnCancel);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this._moneyPlanningItemEditbutton);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this._moneyPlanningItemBox);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(11, 379);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(650, 84);
            this.groupBox1.TabIndex = 17;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "４　実質売買ルールの選択";
            // 
            // _moneyPlanningItemEditbutton
            // 
            this._moneyPlanningItemEditbutton.Location = new System.Drawing.Point(585, 52);
            this._moneyPlanningItemEditbutton.Name = "_moneyPlanningItemEditbutton";
            this._moneyPlanningItemEditbutton.Size = new System.Drawing.Size(48, 23);
            this._moneyPlanningItemEditbutton.TabIndex = 7;
            this._moneyPlanningItemEditbutton.Text = "編集";
            this._moneyPlanningItemEditbutton.UseVisualStyleBackColor = true;
            this._moneyPlanningItemEditbutton.Click += new System.EventHandler(this._moneyPlanningItemEditbutton_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(22, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(583, 29);
            this.label1.TabIndex = 5;
            this.label1.Text = "売買シグナルにしたがって資産を運用する資産運用ルールを設定します";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _moneyPlanningItemBox
            // 
            this._moneyPlanningItemBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._moneyPlanningItemBox.Location = new System.Drawing.Point(22, 52);
            this._moneyPlanningItemBox.Name = "_moneyPlanningItemBox";
            this._moneyPlanningItemBox.Size = new System.Drawing.Size(557, 23);
            this._moneyPlanningItemBox.TabIndex = 1;
            // 
            // _autoReloadCheckBox
            // 
            this._autoReloadCheckBox.AutoSize = true;
            this._autoReloadCheckBox.Location = new System.Drawing.Point(14, 512);
            this._autoReloadCheckBox.Name = "_autoReloadCheckBox";
            this._autoReloadCheckBox.Size = new System.Drawing.Size(242, 19);
            this._autoReloadCheckBox.TabIndex = 18;
            this._autoReloadCheckBox.Text = "拡張キッドのリロードしてから実行する";
            this._autoReloadCheckBox.UseVisualStyleBackColor = true;
            // 
            // AutoTradingDialog
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(7, 15);
            this.ClientSize = new System.Drawing.Size(675, 543);
            this.Controls.Add(this._autoReloadCheckBox);
            this.Controls.Add(this._okButton);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this._dateGroup);
            this.Controls.Add(this._autoTradingItemGroup);
            this.Controls.Add(this._progressBar);
            this.Controls.Add(this._filterGroup);
            this.Controls.Add(this._cancelButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AutoTradingDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "自動売買ルール検証";
            this._filterGroup.ResumeLayout(false);
            this._filterGroup.PerformLayout();
            this._autoTradingItemGroup.ResumeLayout(false);
            this._dateGroup.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void InitUI() {

			if(_prevExecutor!=null) {
				_startDatePicker.Value = Util.IntToDate(_prevExecutor.StartDate);
				_endDatePicker.Value = Util.IntToDate(_prevExecutor.EndDate);
			}
			else {
				DateTime ld = Util.GuessLatestTradeDate();
				if(ld==DateTime.Today && DateTime.Now.Hour<17) {
					ld = ld.AddDays(-1);
					while(!Util.IsMarketOpenDate(ld)) ld = ld.AddDays(-1); //市場が開いている日の17時以前はデータ取れない
				}
				_endDatePicker.Value = ld;
				ld = ld.AddYears(-1);
				_startDatePicker.Value = ld;
			}

			AutoTradingItem[] target = Env.CurrentIndicators.AutoTradingItems;
			for(int i=0; i<target.Length; i++) {
				AutoTradingItem item = target[i];
				_autoTradingItemBox.Items.Add(item.Header);
				//if(o!=null && o.PrimaryItem==target[i])
				//	si = i;
			}

			if(_prevExecutor!=null) {
				_autoTradingItemBox.SelectedIndex = _autoTradingItemBox.FindStringExact(_prevExecutor.Item.Header);
			}
			else
				_autoTradingItemBox.SelectedIndex = 0;
            //hacked by rti
            MoneyPlanningItem[] targetMoneyPlanning = Env.CurrentIndicators.MoneyPlanningItems;
            for (int i = 0; i < targetMoneyPlanning.Length; i++)
            {
                MoneyPlanningItem item = targetMoneyPlanning[i];
                _moneyPlanningItemBox.Items.Add(item.Header);
                //if(o!=null && o.PrimaryItem==target[i])
                //	si = i;
            }

            if (_prevExecutor != null)
            {
                _moneyPlanningItemBox.SelectedIndex = _moneyPlanningItemBox.FindStringExact(_prevExecutor.MoneyPlanningItem.Header);
            }
            else
                _moneyPlanningItemBox.SelectedIndex = 0;
            //--------

			_filterBox.Items.AddRange(EnumDescAttribute.For(typeof(FilterType)).DescriptionCollection());
			_currentBrandIndex = _filterBox.Items.Count;
			_filterBox.Items.Add(String.Format("現在表示している銘柄({0})", Env.Frame.ChartCanvas.GetBrand().Name));
			if(_prevExecutor!=null) {
				FilteredBrandEnumerator e = _prevExecutor.BrandEnumerator as FilteredBrandEnumerator;
				_filterBox.SelectedIndex = e==null? _currentBrandIndex : (int)e.FilterType;
			}
			else
				_filterBox.SelectedIndex = _currentBrandIndex;

			for(int i=0; i<_signalCountValues.Length; i++) {
				_signalCountBox.Items.Add(_signalCountValues[i]==Int32.MaxValue? "無制限" : _signalCountValues[i].ToString());
				if(_prevExecutor!=null && _signalCountValues[i]==_prevExecutor.SignalCountLimit)
					_signalCountBox.SelectedIndex = i;
			}
			if(_signalCountBox.SelectedIndex==-1)
				_signalCountBox.SelectedIndex = 0;
        }

		private void OnOK(object sender, EventArgs args) {
			this.DialogResult = DialogResult.None;

            //hackedby rti
            //自動的にリロードするがチェックされていたら、開始する前に勝手にリロードする
            if (this._autoReloadCheckBox.Checked )
            {
                try
                {
                    Env.Command.Exec(CID.ReloadKit);
                }
                catch(Exception ex)
                {//文法エラーは例外らしい
    				Util.ReportCriticalError(ex);
                    return;
                }
            }
            //-----------------------

			_executor = CreateExecutor();
			if(_executor==null) return;

			_okButton.Enabled = false;
			_filterBox.Enabled = false;
			_autoTradingItemBox.Enabled = false;
            //hacked by rti
            _moneyPlanningItemBox.Enabled = false;
            //----------
			_startDatePicker.Enabled = false;
			_endDatePicker.Enabled = false;
			_signalCountBox.Enabled = false;

			//!!これでは範囲がはみ出てしまうことがあった。派生銘柄や為替の対応などをしているうちにこのあたりの管理がおかしくなっているみたいだ
			_progressBar.Maximum = _executor.BrandEnumerator.Count;
			_progressBar.Value = 0;
			_progressBar.Enabled = true;

			this.Cursor = Cursors.AppStarting;
			_executor.AsyncExecute(this.Handle);
		}

		private void OnCancel(object sender, EventArgs args) {
			if(_executor!=null) {
				_executor.Abort();
			}
		}

		private void OnStartDateChanged(object sender, EventArgs args) {
		}
		private void OnEndDateChanged(object sender, EventArgs args) {
		}

		private AutoTradingExecutor CreateExecutor() {
			if(_startDatePicker.Value.CompareTo(DateTime.Today)>0) {
				Util.Warning(this, "開始日が未来です。");
				return null;
			}
			if(_endDatePicker.Value.CompareTo(DateTime.Today)>0) {
				Util.Warning(this, "終了日が未来です。");
				return null;
			}
			if(_startDatePicker.Value.CompareTo(_endDatePicker.Value)>0) {
				Util.Warning(this, "終了日が開始日の先に設定されています。");
				return null;
			}

			AutoTradingItem[] items = Env.CurrentIndicators.AutoTradingItems;
			int pr = _autoTradingItemBox.SelectedIndex;
            //hacked by rti
            if (items.Length <= pr)
            {
                Util.Warning(this, "存在しないAutoTradingItemsをロードしようとしました");
                return null;
            }
            MoneyPlanningItem[] moneyPlanningItems = Env.CurrentIndicators.MoneyPlanningItems;
            int prMoneyPlanning = _moneyPlanningItemBox.SelectedIndex;
            if (moneyPlanningItems.Length <= prMoneyPlanning)
            {
                Util.Warning(this, "存在しないMoneyPlanningItemsをロードしようとしました");
                return null;
            }
            //AutoTradingExecutor ex = new AutoTradingExecutor(items[pr], Util.DateToInt(_startDatePicker.Value), Util.DateToInt(_endDatePicker.Value), _signalCountValues[_signalCountBox.SelectedIndex]);
            AutoTradingExecutor ex = new AutoTradingExecutor(items[pr], moneyPlanningItems[prMoneyPlanning], Util.DateToInt(_startDatePicker.Value), Util.DateToInt(_endDatePicker.Value), _signalCountValues[_signalCountBox.SelectedIndex]);
            //-----------------
			if(_filterBox.SelectedIndex == _currentBrandIndex) {
				ex.BrandEnumerator = new SingleBrandEnumerator(Env.Frame.ChartCanvas.GetBrand());
			}
			else {
				FilterType filter = (FilterType)_filterBox.SelectedIndex;
				if(filter == FilterType.Favorite){
					if(_bookmarkFolder==null){
						Util.Warning(this, "ブックマークのフォルダを指定してください。");
						return null;
					}
					ex.BrandEnumerator = new BookmarkedBrandEnumerator(_bookmarkFolder);
				}
				else
					ex.BrandEnumerator = new FilteredBrandEnumerator(filter);
			}

			return ex;
		}

		private void ExitSuccessFully() {
			this.DialogResult = DialogResult.OK;
			this.Cursor = Cursors.Default;
			AutoTradingResult r = _executor.Result as AutoTradingResult;
			_prevExecutor = _executor; //次回実行で使用

			StringBuilder msg = new StringBuilder();
			if(r.HasTooManuResults) {
				Util.Warning(this, String.Format("シグナル発生数が制限に達したため途中で打ち切りました。条件や範囲を見直してください。"));
			}
			else {
				if(r.CheckedBrandCount > 1) {
					if(r.DataErrorBrands.Count==0) {
						msg.AppendFormat("検証が完了しました。\n対象銘柄数は {0} でした。\n", r.CheckedBrandCount);
					}
					else {
						msg.AppendFormat("検証が完了しました。\n対象銘柄数 {0} のうち、\n対象日付のデータがなかった {1} 銘柄", r.CheckedBrandCount, r.DataErrorBrands.Count);
						if(r.DataErrorBrands.Count <= 10) {
							for(int i=0; i<r.DataErrorBrands.Count; i++) {
								if(i > 0) msg.Append(", ");
								msg.Append((r.DataErrorBrands[i] as BasicBrand).Code.ToString());
							}
						}
						msg.Append("は除外されました。");
					}

					Util.Information(this, msg.ToString());
				}
			}
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
					_executor.Abort();
					Util.Warning(this, _executor.ErrorMessage);
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

        //hacked by rti
        private void _itemEditButton_Click(object sender, EventArgs e)
        {
            AutoTradingItem[] AutoTradingItems = Env.CurrentIndicators.AutoTradingItems;
            int pr = _autoTradingItemBox.SelectedIndex;
            if (AutoTradingItems.Length <= pr)
            {
                Util.Warning(this, "存在しないAutoTradingItemをロードしようとしました" );
                return ;
            }
            //スキーマファイルを開く
            if ( ! SchemaFileOpen(AutoTradingItems[pr].Title))
            {
                return;
            }
            //勝手にチェックをつけます!! 余計なお世話かな、、
            this._autoReloadCheckBox.Checked = true;
        }
        private void _moneyPlanningItemEditbutton_Click(object sender, EventArgs e)
        {
            MoneyPlanningItem[] moneyPlanningItems = Env.CurrentIndicators.MoneyPlanningItems;
            int prMoneyPlanning = _moneyPlanningItemBox.SelectedIndex;
            if (moneyPlanningItems.Length <= prMoneyPlanning)
            {
                Util.Warning(this, "存在しないMoneyPlanningItemsをロードしようとしました");
                return;
            }
            //スキーマファイルを開く
            if ( ! SchemaFileOpen(moneyPlanningItems[prMoneyPlanning].Title))
            {
                return;
            }
            //勝手にチェックをつけます!! 余計なお世話かな、、
            this._autoReloadCheckBox.Checked = true;
        }

        //指定したタイトルのスキーマがあるファイルを開いてちょ
        //本当は、行数とかもとって移動させたかったんだけど、
        //xml の壁と、 omega の node の壁があった、、、
        //書き換えるの面倒だし、行数のスクロールはセルフサービスなりよキテレツ
        private bool SchemaFileOpen(string inSchemaTitle)
        {
            string filename = null;
            foreach (ExtensionKit ek in Env.Schema.ExtensionKits)
            {
                if (ek.FindSchemaItemByTitle(inSchemaTitle) != null)
                {
                    filename = ek.FileName;
                }
            }
            if (filename == null)
            {
                Util.Warning(this, String.Format( "スキーマ {0} のスキーマファイルを特定できませんでした" , inSchemaTitle ));
                return false; 
            }

            //面倒だから関連付けで開く
            try
            {
                System.Diagnostics.Process.Start(filename);
                return true;
            }
            catch (Win32Exception ex)
            {
                //関連付けが付けがされていないぐらいで例外とばすなボケ
                if (ex.NativeErrorCode != 1155)
                {
                    //あれ? 関連付け以外の例外らしいよ、それだったら、上に飛ばしとくか
                    Util.Warning(this, String.Format( "スキーマ {0} の\r\nスキーマファイル {1} を開けませんでした\r\nエラーメッセージ:{2}" , inSchemaTitle , filename , ex.Message ));
                    return false;
                }
            }
            catch(Exception ex)
            {
                Util.Warning(this, String.Format( "スキーマ {0} の\r\nスキーマファイル {1} を開けませんでした\r\nエラーメッセージ:{2}" , inSchemaTitle , filename , ex.Message ));
            }

            //関連付けがない場合はメモ帳で開く
            try
            {
                System.Diagnostics.Process.Start("Notepad", filename);
                return true;
            }
            catch (Exception ex)
            {
                Util.Warning(this, String.Format( "スキーマ {0} の\r\nスキーマファイル {1} を開けませんでした\r\nエラーメッセージ:{2}" , inSchemaTitle , filename , ex.Message ));
            }

            return false;
        }
        //------------------------------
	}
}
