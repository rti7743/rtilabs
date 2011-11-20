/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Diagnostics;
using System.ComponentModel;
using System.Windows.Forms;
using Travis.Util;

using Zanetti.Indicators;
using Zanetti.Indicators.Schema;
using Zanetti.UI;
using Zanetti.Commands;
using UtilityLibrary.WinControls;

namespace Zanetti.Forms
{
	/// <summary>
	/// CustomizeDialog の概要の説明です。
	/// </summary>
	internal class CustomizeDialog : System.Windows.Forms.Form {
		private const int MAX_ITEMS = 16;
		private static int _pageIndex;

		private string _fontName;
		private float _fontSize;
		private FontStyle _fontStyle;

        //hacked by rti
        //実質売買初回金額
        private TextBox _InitCachTextBox;
        //期間平均収益率を求める期間
        private TextBox _RTISisuuHeikinKikanTextBox;
        //本日買いつけに回させる金額
        private TextBox _TodayBuyCachTextBox;
        //--------------

		private class CustomizeTag {
			public SchemaItem schemaItem;
			public int index; //IndicatorではInstance番号、Screening/PredictionではParameterの番号
			public ComboBox styleBox;
			public ColPickerComboBox colorPicker;
			public TextBox dailyParams;
			public TextBox weeklyParams;
			public TextBox monthlyParams;
			public int parameterCount;
		}
		private ArrayList _tags;

		private ArrayList _indicators;
		private ArrayList _screeningItems;
		private ArrayList _predictionItems;
		private ArrayList _autoTradingItems;
        //hackedby rti
        private ArrayList _moneyPlanningItems;
        //------

		private Button _resetButton;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.TabControl _tabControl;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.Button _applyButton;
		private System.Windows.Forms.TabPage _genericPage;
		private System.Windows.Forms.TabPage _chartPage;
		private System.Windows.Forms.TabPage _screeningPage;
		private System.Windows.Forms.TabPage _theoremPage;
		private System.Windows.Forms.TabPage _autoTradingPage;
        //hacked by rti
        private System.Windows.Forms.TabPage _MoneyPlanningPage;
        //--------------
        private System.Windows.Forms.GroupBox _basicColorGroup;
		private System.Windows.Forms.GroupBox _scaleLineGroup;
		private Label _lFont;
		private Label _lCurrentFont;
		private Button _selectFont;
		private Label _lBackColor;
		private ColPickerComboBox _backColorBox;
		private Label _lTextColor;
		private ColPickerComboBox _textColorBox;
		private Label _lFushiColor;
		private ColPickerComboBox _fushiColorBox;
		private Label _lCandleColor;
		private ColPickerComboBox _candleColorBox;
		private CheckBox _candleShadowEffect;
		private Label _lInsenColor;
		private ColPickerComboBox _insenColorBox;
		private Label _lCreditLong;
		private ComboBox _creditLongStyleBox;
		private ColPickerComboBox _creditLongColorBox;
		private Label _lCreditShort;
		private ComboBox _creditShortStyleBox;
		private ColPickerComboBox _creditShortColorBox;
		private Label _lVolumeColor;
		private ColPickerComboBox _volumeColorBox;
		private Label _lAccumulativeVolumeLabel;
		private TextBox[] _accumulativeVolumeBox;
		private Label _lFreeLineColor;
		private ColPickerComboBox _freeLineColorBox;
		
		private Label _lMonthDiv;
		private ComboBox _monthDivStyleBox;
		private ColPickerComboBox _monthDivColorBox;
		private Label _lPriceScale;
		private ComboBox _priceScaleStyleBox;
		private ColPickerComboBox _priceScaleColorBox;
		private Label _lVolumeScale;
		private ComboBox _volumeScaleStyleBox;
		private ColPickerComboBox _volumeScaleColorBox;
		private Label _lOscillatorScale;
		private ComboBox _oscillatorScaleStyleBox;
		private ColPickerComboBox _oscillatorScaleColorBox;
		private Label _lMouseTracking;
		private ComboBox _mouseTrackingStyleBox;
		private ColPickerComboBox _mouseTrackingColorBox;
		private ComboBox _mouseTrackingModeBox;

		private ToolTip _toolTip;
		private System.ComponentModel.IContainer components;

		public CustomizeDialog() {
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();
			this.SetStyle(ControlStyles.DoubleBuffer|ControlStyles.AllPaintingInWmPaint, true);

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			if(!this.DesignMode)
				InitPages();
			_tabControl.SelectedIndex = _pageIndex;

			_toolTip.ShowAlways = true;
			_toolTip.InitialDelay = 1;
			_toolTip.AutoPopDelay = 60000;
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
			this.components = new System.ComponentModel.Container();
			this._resetButton = new Button();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this._applyButton = new System.Windows.Forms.Button();
			this._tabControl = new System.Windows.Forms.TabControl();
			this._genericPage = new System.Windows.Forms.TabPage();
			this._scaleLineGroup = new System.Windows.Forms.GroupBox();
			this._basicColorGroup = new System.Windows.Forms.GroupBox();
			this._chartPage = new System.Windows.Forms.TabPage();
			this._screeningPage = new System.Windows.Forms.TabPage();
			this._theoremPage = new System.Windows.Forms.TabPage();
			this._autoTradingPage = new System.Windows.Forms.TabPage();
            this._MoneyPlanningPage = new System.Windows.Forms.TabPage();
            this._toolTip = new System.Windows.Forms.ToolTip(this.components);
			this._tabControl.SuspendLayout();
			this._genericPage.SuspendLayout();
			this.SuspendLayout();
			// 
			// _resetButton
			// 
			this._resetButton.DialogResult = System.Windows.Forms.DialogResult.None;
			this._resetButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._resetButton.Location = new System.Drawing.Point(8, 400);
			this._resetButton.Name = "_resetButton";
			this._resetButton.Size = new System.Drawing.Size(96, 23);
			this._resetButton.TabIndex = 3;
			this._resetButton.Text = "リセット(&R)";
			this._resetButton.Click += new System.EventHandler(this.OnReset);
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(312, 400);
			this._okButton.Name = "_okButton";
			this._okButton.Size = new System.Drawing.Size(80, 23);
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			this._okButton.Click += new System.EventHandler(this.OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(400, 400);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.Size = new System.Drawing.Size(80, 23);
			this._cancelButton.TabIndex = 1;
			this._cancelButton.Text = "キャンセル";
			// 
			// _applyButton
			// 
			this._applyButton.Enabled = false;
			this._applyButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._applyButton.Location = new System.Drawing.Point(496, 400);
			this._applyButton.Name = "_applyButton";
			this._applyButton.Size = new System.Drawing.Size(80, 23);
			this._applyButton.TabIndex = 2;
			this._applyButton.Text = "適用(&A)";
			this._applyButton.Click += new System.EventHandler(this.OnApplyButtonClicked);
			// 
			// _tabControl
			// 
			this._tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this._tabControl.Controls.Add(this._genericPage);
			this._tabControl.Location = new System.Drawing.Point(0, 0);
			this._tabControl.Name = "_tabControl";
			this._tabControl.SelectedIndex = 0;
			this._tabControl.Size = new System.Drawing.Size(618, 392);
			this._tabControl.TabIndex = 4;
			// 
			// _genericPage
			// 
			this._genericPage.Controls.Add(this._scaleLineGroup);
			this._genericPage.Controls.Add(this._basicColorGroup);
			this._genericPage.Location = new System.Drawing.Point(4, 21);
			this._genericPage.Name = "_genericPage";
			this._genericPage.Size = new System.Drawing.Size(610, 346);
			this._genericPage.TabIndex = 0;
			this._genericPage.Text = "全般";
			// 
			// _scaleLineGroup
			// 
			this._scaleLineGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._scaleLineGroup.Location = new System.Drawing.Point(8, 256);
			this._scaleLineGroup.Name = "_scaleLineGroup";
			this._scaleLineGroup.Size = new System.Drawing.Size(592, 120);
			this._scaleLineGroup.TabIndex = 1;
			this._scaleLineGroup.TabStop = false;
			this._scaleLineGroup.Text = "区切り線";
			// 
			// _basicColorGroup
			// 
			this._basicColorGroup.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._basicColorGroup.Location = new System.Drawing.Point(8, 8);
			this._basicColorGroup.Name = "_basicColorGroup";
			this._basicColorGroup.Size = new System.Drawing.Size(592, 240);
			this._basicColorGroup.TabIndex = 0;
			this._basicColorGroup.TabStop = false;
			this._basicColorGroup.Text = "基本スタイル設定";
			// 
			// _chartPage
			// 
			this._chartPage.Location = new System.Drawing.Point(4, 21);
			this._chartPage.Name = "_chartPage";
			this._chartPage.Size = new System.Drawing.Size(584, 311);
			this._chartPage.TabIndex = 0;
			this._chartPage.Text = "チャート";
			// 
			// _screeningPage
			// 
			this._screeningPage.Location = new System.Drawing.Point(4, 21);
			this._screeningPage.Name = "_screeningPage";
			this._screeningPage.Size = new System.Drawing.Size(584, 311);
			this._screeningPage.TabIndex = 0;
			this._screeningPage.Text = "スクリーニング";
			// 
			// _theoremPage
			// 
			this._theoremPage.Location = new System.Drawing.Point(4, 21);
			this._theoremPage.Name = "_theoremPage";
			this._theoremPage.Size = new System.Drawing.Size(584, 311);
			this._theoremPage.TabIndex = 0;
			this._theoremPage.Text = "セオリー検証";
			// 
			// _autoTradingPage
			// 
			this._autoTradingPage.Location = new System.Drawing.Point(4, 21);
			this._autoTradingPage.Name = "_autoTradingPage";
			this._autoTradingPage.Size = new System.Drawing.Size(584, 311);
			this._autoTradingPage.TabIndex = 0;
			this._autoTradingPage.Text = "自動売買検証";
            // 
            // _MoneyPlanningPage
            // 
            this._MoneyPlanningPage.Location = new System.Drawing.Point(4, 21);
            this._MoneyPlanningPage.Name = "_MoneyPlanningPage";
            this._MoneyPlanningPage.Size = new System.Drawing.Size(584, 311);
            this._MoneyPlanningPage.TabIndex = 0;
            this._MoneyPlanningPage.Text = "実質売買検証";
            // 
			// CustomizeDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(618, 432);
			this.Controls.Add(this._tabControl);
			this.Controls.Add(this._applyButton);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._resetButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CustomizeDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "カスタマイズ";
			this._tabControl.ResumeLayout(false);
			this._genericPage.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private CustomizeTag FindCustomizeTag(ComboBox stylebox) {
			foreach(CustomizeTag tag in _tags)
				if(tag.styleBox==stylebox) return tag;
			return null;
		}
		private CustomizeTag FindCustomizeTagByTextBox(TextBox box) {
			foreach(CustomizeTag tag in _tags)
				if(tag.dailyParams==box || tag.weeklyParams==box || tag.monthlyParams==box) return tag;
			return null;
		}

		private TabPage InsertTabPage(int index, string caption) {
			TabPage page = new TabPage(caption);
			_tabControl.TabPages.Add(page);
			page.BackColor = ThemeUtil.TabPaneBackColor;
			page.Location = new System.Drawing.Point(4, 21);
			page.Size = new System.Drawing.Size(584, 311);
			return page;
		}


		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
		}
		protected override void OnClosed(EventArgs e) {
			base.OnClosed (e);
			_pageIndex = _tabControl.SelectedIndex;
		}


		private void InitPages() {
			_tags = new ArrayList();

			_indicators = new ArrayList();
			_screeningItems = new ArrayList();
			_predictionItems = new ArrayList();
			_autoTradingItems = new ArrayList();
            //hacked by rti
            _moneyPlanningItems = new ArrayList();
            //------------------
            Env.Schema.CollectIndicatorSchema(_indicators);
			Env.Schema.CollectScreeningSchema(_screeningItems);
			Env.Schema.CollectPredictionSchema(_predictionItems);
			Env.Schema.CollectAutoTradingSchema(_autoTradingItems);
            //hacked rti
            Env.Schema.CollectMoneyPlanningSchema(_moneyPlanningItems);
            //--------------

			//高さの決定
			int m = 0;
			foreach(SchemaItem i in _indicators)
				if(i.OwnsConfig) m += i.MaxInstanceCount;

			m = MAX_ITEMS; //Math.Max(m, Math.Max(_screeningItems.Count, _predictionItems.Count));
			_tabControl.Height = m*24 + 40;
			_okButton.Top = _tabControl.Bottom + 8;
			_cancelButton.Top = _tabControl.Bottom + 8;
			_resetButton.Top = _tabControl.Bottom + 8;
			_applyButton.Top = _tabControl.Bottom + 8;
			this.ClientSize = new Size(this.ClientSize.Width, _okButton.Bottom + 8);

			//!!オンデマンドでページ初期化できたほうがよい
			InitGenericPage();
			InitIndicatorPage(1, _indicators);
			InitScreeningPage(2, _screeningItems);
			InitPredictionPage(3, _predictionItems);
			InitAutoTradingPage(4, _autoTradingItems);
            //hacked by rti
            InitMoneyPlanningPage(5, _moneyPlanningItems);
            //--------------
        }

		//基本設定
		private void InitGenericPage() {
			_lFont = new Label();
			_lCurrentFont = new Label();
			_selectFont = new Button();
			_lBackColor = new Label();
			_backColorBox = new ColPickerComboBox();;
			_lTextColor = new Label();
			_textColorBox = new ColPickerComboBox(); 
			_lFushiColor = new Label(); 
			_fushiColorBox = new ColPickerComboBox(); 
			_lCandleColor = new Label(); 
			_candleColorBox = new ColPickerComboBox(); 
			_candleShadowEffect = new CheckBox(); 
			_lInsenColor = new Label(); 
			_insenColorBox = new ColPickerComboBox(); 
			_lVolumeColor = new Label();
			_volumeColorBox = new ColPickerComboBox(); 
			_lAccumulativeVolumeLabel = new Label();
			_accumulativeVolumeBox = new TextBox[3];
			_lCreditLong = new Label();
			_creditLongStyleBox = new ComboBox();
			_creditLongColorBox = new ColPickerComboBox();
			_lCreditShort = new Label();
			_creditShortStyleBox = new ComboBox();
			_creditShortColorBox = new ColPickerComboBox();
			_lFreeLineColor = new Label();
			_freeLineColorBox = new ColPickerComboBox();
			_lMonthDiv = new Label(); 
			_monthDivStyleBox = new ComboBox(); 
			_monthDivColorBox = new ColPickerComboBox(); 
			_lPriceScale = new Label(); 
			_priceScaleStyleBox = new ComboBox(); 
			_priceScaleColorBox = new ColPickerComboBox(); 
			_lVolumeScale = new Label(); 
			_volumeScaleStyleBox = new ComboBox(); 
			_volumeScaleColorBox = new ColPickerComboBox(); 
			_lOscillatorScale = new Label(); 
			_oscillatorScaleStyleBox = new ComboBox(); 
			_oscillatorScaleColorBox = new ColPickerComboBox(); 
			_lMouseTracking = new Label(); 
			_mouseTrackingStyleBox = new ComboBox(); 
			_mouseTrackingColorBox = new ColPickerComboBox(); 
			_mouseTrackingModeBox = new ComboBox();
			_basicColorGroup.SuspendLayout();
			_scaleLineGroup.SuspendLayout();
			_genericPage.BackColor = ThemeUtil.TabPaneBackColor;

			int top = 16;
			int left1 = 8;
			int left2 = _tabControl.Width / 2 - 8;
			int tabindex = 0;
			InitFontUI(left1, top, ref tabindex);
			top += 24;
			InitBasicColorConfigUI(_lBackColor, "背景色(&B)", _backColorBox, left1, top, ref tabindex, Env.Preference.BackBrush.Color);
			InitBasicColorConfigUI(_lTextColor, "テキスト色(&T)", _textColorBox, left2, top, ref tabindex, Env.Preference.TextColor);
			top += 24;
			InitBasicColorConfigUI(_lFushiColor, "直近高値・安値(&F)", _fushiColorBox, left1, top, ref tabindex, Env.Preference.FushiColor);

			//区切り線
			Label line = new Label();
			line.BorderStyle = BorderStyle.Fixed3D;
			line.Top = top + 29;
			line.Height = 3;
			line.Left = _basicColorGroup.Width / 10;
			line.Width = _basicColorGroup.Width*8 / 10;
			_basicColorGroup.Controls.Add(line);
			top += 38;

			InitBasicColorConfigUI(_lCandleColor, "ロウソク色(&C)", _candleColorBox, left1, top, ref tabindex, Env.Preference.CandlePen.Color);
			_candleShadowEffect.Text = "ロウソクの表示に影をつける(&S)";
			_candleShadowEffect.FlatStyle = FlatStyle.System;
			_candleShadowEffect.Top = top;
			_candleShadowEffect.Left = left2+16;
			_candleShadowEffect.Width = 160;
			_candleShadowEffect.TabIndex = tabindex++;
			_candleShadowEffect.Checked = Env.Preference.UseCandleEffect;
			_candleShadowEffect.CheckedChanged += new EventHandler(EnableApplyButton);
			_basicColorGroup.Controls.Add(_candleShadowEffect);
			top += 24;
			InitBasicColorConfigUI(_lInsenColor, "陰線の色(&I)", _insenColorBox, left1, top, ref tabindex, Env.Preference.InsenBrush.Color);
			
			//区切り線
			line = new Label();
			line.BorderStyle = BorderStyle.Fixed3D;
			line.Top = top + 29;
			line.Height = 3;
			line.Left = _basicColorGroup.Width / 10;
			line.Width = _basicColorGroup.Width*8 / 10;
			_basicColorGroup.Controls.Add(line);
			top += 38;

			InitBasicLineConfigUI(_lCreditLong, "信用買残(&L)", _creditLongStyleBox, _creditLongColorBox, left1, top, ref tabindex, Env.Preference.CreditLongAppearance.Pen);
			InitBasicColorConfigUI(_lVolumeColor, "出来高色(&V)", _volumeColorBox, left2, top, ref tabindex, Env.Preference.VolumeBrush.Color);
			top += 24;
			InitBasicLineConfigUI(_lCreditShort, "信用売残(&H)", _creditShortStyleBox, _creditShortColorBox, left1, top, ref tabindex, Env.Preference.CreditShortAppearance.Pen);
			InitAccumulativeVolumeConfigUI(_lAccumulativeVolumeLabel, "価格帯別出来高(&A)", left2, top, ref tabindex, Env.Preference.AccumulativeVolumePeriod);
			top += 24;
			InitBasicColorConfigUI(_lFreeLineColor, "自由直線(&K)", _freeLineColorBox, left1, top, ref tabindex, Env.Preference.FreeLineColor);
			_freeLineColorBox.Left += 36; //ugly adjustment

			top = 16;
			_scaleLineGroup.TabIndex = tabindex++;
			InitScaleLineConfigUI(_lMonthDiv  , "月/四半期(&M)", _monthDivStyleBox, _monthDivColorBox, left1, top, ref tabindex, Env.Preference.MonthDivPen);
			InitScaleLineConfigUI(_lMouseTracking  , "日付ルーラ(&D)", _mouseTrackingStyleBox, _mouseTrackingColorBox, left2, top, ref tabindex, Env.Preference.MouseTrackingLinePen);
			top += 24;
			InitScaleLineConfigUI(_lPriceScale, "価格(&P)", _priceScaleStyleBox, _priceScaleColorBox, left1, top, ref tabindex, Env.Preference.PriceScalePen);
			top += 24;
			InitScaleLineConfigUI(_lVolumeScale, "出来高(&E)", _volumeScaleStyleBox, _volumeScaleColorBox, left1, top, ref tabindex, Env.Preference.VolumeScalePen);
			top += 24;
			InitScaleLineConfigUI(_lOscillatorScale, "オシレータ(&O)", _oscillatorScaleStyleBox, _oscillatorScaleColorBox, left1, top, ref tabindex, Env.Preference.OscillatorScalePen);
			top += 24;

			_basicColorGroup.ResumeLayout();
			_scaleLineGroup.ResumeLayout();
		}
		private void InitBasicColorConfigUI(Label label, string caption, ColPickerComboBox cb, int left, int top, ref int tabindex, Color value) {
			label.Top = top;
			label.Left = left;
			label.Width = 120;
			label.Text = caption;
			label.TabIndex = tabindex++;
			label.TextAlign = ContentAlignment.MiddleLeft;

			cb.Top = top;
			cb.Left = label.Right;
			cb.Width = 120;
			cb.TabIndex = tabindex++;
			cb.Color = value;
			cb.NewColor += new UtilityLibrary.WinControls.ColorPickerDropDown.NewColorEventHandler(EnableApplyButton);

			_basicColorGroup.Controls.Add(label);
			_basicColorGroup.Controls.Add(cb);
		}
		private void InitBasicLineConfigUI(Label label, string caption, ComboBox stylebox, ColPickerComboBox colorbox, int left, int top, ref int tabindex, ZPen pen) {
			InitLineConfigUI(_basicColorGroup, label, caption, stylebox, colorbox, left, top, ref tabindex, pen);
		}
		private void InitScaleLineConfigUI(Label label, string caption, ComboBox stylebox, ColPickerComboBox colorbox, int left, int top, ref int tabindex, ZPen pen) {
			InitLineConfigUI(_scaleLineGroup, label, caption, stylebox, colorbox, left, top, ref tabindex, pen);
		}
		private void InitLineConfigUI(GroupBox parent, Label label, string caption, ComboBox stylebox, ColPickerComboBox colorbox, int left, int top, ref int tabindex, ZPen pen) {
			label.Top = top;
			label.Left = left;
			label.Width = 88;
			label.Text = caption;
			label.TabIndex = tabindex++;
			label.TextAlign = ContentAlignment.MiddleLeft;

			stylebox.DropDownStyle = ComboBoxStyle.DropDownList;
			stylebox.Top = top;
			stylebox.Left = label.Right;
			stylebox.Width = 52;
			stylebox.TabIndex = tabindex++;
			stylebox.Items.AddRange(EnumDescAttribute.For(typeof(ZPen.PenStyle)).DescriptionCollection());
			stylebox.SelectedIndex = (int)pen.Style;
			stylebox.SelectedIndexChanged += new EventHandler(EnableApplyButton);

			colorbox.Top = top;
			colorbox.Left = stylebox.Right+16;
			colorbox.Width = 120;
			colorbox.TabIndex = tabindex++;
			colorbox.Color = pen.Color;
			colorbox.NewColor += new UtilityLibrary.WinControls.ColorPickerDropDown.NewColorEventHandler(EnableApplyButton);

			parent.Controls.Add(label);
			parent.Controls.Add(stylebox);
			parent.Controls.Add(colorbox);
		}
		private void InitFontUI(int left, int top, ref int tabindex) {

			_fontName = Env.Preference.FontName;
			_fontSize = Env.Preference.FontSize;
			_fontStyle = Env.Preference.FontStyle;

			_lFont.Left = left;
			_lFont.Width = 120;
			_lFont.Top = top;
			_lFont.TabIndex = tabindex++;
			_lFont.Text = "フォント(&F)";
			_lFont.TextAlign = ContentAlignment.MiddleLeft;
			_basicColorGroup.Controls.Add(_lFont);

			_lCurrentFont.Left = left+_lFont.Width;
			_lCurrentFont.Width = 120;
			_lCurrentFont.Top = top;
			_lCurrentFont.TabIndex = tabindex++;
			_lCurrentFont.Text = String.Format("{0}/{1:F0}pt", _fontName, _fontSize);
			_lCurrentFont.TextAlign = ContentAlignment.MiddleLeft;
			_basicColorGroup.Controls.Add(_lCurrentFont);

			_selectFont.Left = _lCurrentFont.Right;
			_selectFont.Top = top;
			_selectFont.FlatStyle = FlatStyle.System;
			_selectFont.Text = "選択...";
			_selectFont.TabIndex = tabindex++;
			_selectFont.Click += new EventHandler(OnSelectFont);
			_basicColorGroup.Controls.Add(_selectFont);
		}

		//価格帯別出来高の設定
		private void InitAccumulativeVolumeConfigUI(Label label, string caption, int left, int top, ref int tabindex, ChartFormatSpecificValue value) {
			label.Text = caption;
			label.Left = left;
			label.Top = top;
			label.TabIndex = tabindex++;
			label.Width = 120;
			label.TextAlign = ContentAlignment.MiddleLeft;
			_basicColorGroup.Controls.Add(label);
	
			int x = label.Right;
			string[] descs = new string[] { "日","週","月" };
			for(int i=0; i<3; i++) {
				TextBox t = new TextBox();
				t.Left = x;
				t.Top = top;
				t.TabIndex = tabindex++;
				t.Width = 28;
				t.Text = value.GetValue((ChartFormat)i).ToString();
				t.Validating += new CancelEventHandler(OnValidatingNumericText);
				t.Validated += new EventHandler(EnableApplyButton);
				_basicColorGroup.Controls.Add(t);
				_accumulativeVolumeBox[i] = t;
				x += t.Width;

				Label l = new Label();
				l.Text = descs[i];
				l.Left = x;
				l.Top = top;
				l.Width = 24;
				l.TabIndex = tabindex++;
				l.TextAlign = ContentAlignment.MiddleLeft;
				_basicColorGroup.Controls.Add(l);
				x += l.Width;
			}
		}



		private void CommitPreference() {
			Preference pr = Env.Preference;

			pr.FontName = _fontName;
			pr.FontSize = _fontSize;
			pr.FontStyle = _fontStyle;
			pr.BackBrush.Update(_backColorBox.Color);
			pr.DefaultBrush.Update(_textColorBox.Color);
			pr.CandlePen.Update(_candleColorBox.Color, _backColorBox.Color);
			pr.InsenBrush.Update(_insenColorBox.Color);
			pr.UseCandleEffect = _candleShadowEffect.Checked;
			pr.VolumeBrush.Update(_volumeColorBox.Color);
			pr.FushiColor = _fushiColorBox.Color;
			pr.FreeLineColor = _freeLineColorBox.Color;
			
			pr.MonthDivPen.Update(_monthDivColorBox.Color, (ZPen.PenStyle)_monthDivStyleBox.SelectedIndex);
			pr.PriceScalePen.Update(_priceScaleColorBox.Color, (ZPen.PenStyle)_priceScaleStyleBox.SelectedIndex);
			pr.VolumeScalePen.Update(_volumeScaleColorBox.Color, (ZPen.PenStyle)_volumeScaleStyleBox.SelectedIndex);
			pr.OscillatorScalePen.Update(_oscillatorScaleColorBox.Color, (ZPen.PenStyle)_oscillatorScaleStyleBox.SelectedIndex);
			pr.MouseTrackingLinePen.Update(_mouseTrackingColorBox.Color, (ZPen.PenStyle)_mouseTrackingStyleBox.SelectedIndex);

			pr.CreditLongAppearance.Pen.Update(_creditLongColorBox.Color, (ZPen.PenStyle)_creditLongStyleBox.SelectedIndex);
			pr.CreditShortAppearance.Pen.Update(_creditShortColorBox.Color, (ZPen.PenStyle)_creditShortStyleBox.SelectedIndex);

			pr.AccumulativeVolumePeriod.Update(new int[3] {
															  Int32.Parse(_accumulativeVolumeBox[0].Text),
															  Int32.Parse(_accumulativeVolumeBox[1].Text),
															  Int32.Parse(_accumulativeVolumeBox[2].Text)});

			CommandExec.ResetLayout();
		}

		//Indicator
		private void InitIndicatorPage(int index, ArrayList items) {
			int top = 8;
			int tabIndex = 0;
			TabPage page = _chartPage;
			_tabControl.Controls.Add(page);
			page.BackColor = ThemeUtil.TabPaneBackColor;

			int item_count = 0;
			int page_count = 1;
			foreach(SchemaIndicatorItem item in items) {
				if(!item.OwnsConfig) continue;
				int ic = item.MaxInstanceCount;
				if(item_count + ic > MAX_ITEMS) { //はみ出しそうなら
					_chartPage.Text = String.Format("チャート - {0}", 1);
					page = InsertTabPage(page_count, String.Format("チャート - {0}", ++page_count));
					top = 8;
					tabIndex = 0;
					item_count = 0;
				}

				for(int i=0; i<ic; i++) {
					CustomizeTag tag = new CustomizeTag();
					tag.schemaItem = item;
					tag.index = i;
					if(i==0) {
						AddLabel(page, item.Title, tabIndex++, 8, top, 120);
						AddHelpButton(page, tabIndex++, 128, top, item.Description);
					}
					if(ic!=1)
						AddLabel(page, (i+1).ToString(), tabIndex++, 152, top, 16);
					
					if(item.Appearance.Owner==item) {
						tag.styleBox = AddStyleBox(page, item, i, tabIndex++, 176, top, 80);
						tag.colorPicker = AddColorPicker(page, item, i, tabIndex++, 264, top, 128);
					}

					SchemaParameter[] sp = item.OwnedParameters;
					if(sp.Length>0) {
						tag.dailyParams = AddParamBox(page, item, i, ChartFormat.Daily, tabIndex++, 400, top, 48);
						AddLabel(page, "日", tabIndex++, 448, top, 16);
						tag.weeklyParams = AddParamBox(page, item, i, ChartFormat.Weekly, tabIndex++, 472, top, 48);
						AddLabel(page, "週", tabIndex++, 520, top, 16);
						tag.monthlyParams = AddParamBox(page, item, i, ChartFormat.Monthly, tabIndex++, 544, top, 48);
						AddLabel(page, "月", tabIndex++, 592, top, 16);
						tag.parameterCount = sp.Length;
					}

					if(tag.styleBox!=null && (int)IndicatorStyle.None==tag.styleBox.SelectedIndex) {
						tag.colorPicker.Enabled = false;
						if(tag.dailyParams!=null) {
							tag.dailyParams.Enabled = false;
							tag.weeklyParams.Enabled = false;
							tag.monthlyParams.Enabled = false;
						}
					}


					item_count++;
					top += 24;
					_tags.Add(tag);
				}
			}
		}

		private Label AddLabel(TabPage parent, string text, int tabIndex, int left, int top, int width) {
			Label label = new Label();
			label.TextAlign = ContentAlignment.MiddleLeft;
			label.Text = text;
			label.TabIndex = tabIndex;
			label.Left = left;
			label.Top = top-2;
			label.Width = width;
			label.Height = 24;
			parent.Controls.Add(label);
			return label;
		}
		private ComboBox AddStyleBox(TabPage parent, SchemaIndicatorItem item, int index, int tabIndex, int left, int top, int width) {
			ComboBox box = new ComboBox();
			box.DropDownStyle = ComboBoxStyle.DropDownList;
			box.Items.AddRange(EnumDescAttribute.For(typeof(IndicatorStyle)).DescriptionCollection());
			IndicatorAppearance[] ap = item.Appearance.Appearances;
			box.SelectedIndex = ap.Length>index? (int)ap[index].Style : (int)IndicatorStyle.None;
			box.SelectedIndexChanged += new EventHandler(OnIndicatorStyleChanged);

			box.TabIndex = tabIndex;
			box.Left = left;
			box.Top = top;
			box.Width = width;
			box.Height = 24;
			parent.Controls.Add(box);
			return box;
		}
		private ColPickerComboBox AddColorPicker(TabPage parent, SchemaIndicatorItem item, int index, int tabIndex, int left, int top, int width) {
			ColPickerComboBox box = new ColPickerComboBox();
			IndicatorAppearance[] ap = item.Appearance.Appearances;
			box.Color = ap.Length>index? ap[index].Color : Color.Empty;

			box.NewColor += new UtilityLibrary.WinControls.ColorPickerDropDown.NewColorEventHandler(OnNewColor);
			box.TabIndex = tabIndex;
			box.Left = left;
			box.Top = top;
			box.Width = width;
			box.Height = 24;
			parent.Controls.Add(box);
			return box;
		}
		private TextBox AddParamBox(TabPage parent, SchemaItem item, int index, ChartFormat format, int tabIndex, int left, int top, int width) {
			TextBox box = new TextBox();
			string t = "";
			for(int i=0; i<item.ParameterCount; i++) {
				SchemaParameter sp = item.GetParameter(i);
				if(sp.Owner!=item) continue;

				if(t.Length>0) t += ",";
				double[] vs;
				switch(format){
					case ChartFormat.Daily:
					default:
						vs = sp.DailyValues;
						break;
					case ChartFormat.Weekly:
						vs = sp.WeeklyValues;
						break;
					case ChartFormat.Monthly:
						vs = sp.MonthlyValues;
						break;
				}
				double v = vs.Length>index? vs[index] : 1;
				t += v.ToString();
			}
			box.Text = t;

			box.TextChanged += new EventHandler(OnParameterTextChanged);
			box.Validating += new CancelEventHandler(OnValidatingParameterText);
			box.TabIndex = tabIndex;
			box.Left = left;
			box.Top = top;
			box.Width = width;
			box.Height = 24;
			parent.Controls.Add(box);
			return box;
		}
		private TextBox AddSingleParamBox(TabPage parent, SchemaItem item, int paramindex, ChartFormat format, int tabIndex, int left, int top, int width) {
			TextBox box = new TextBox();
			SchemaParameter sp = item.GetParameter(paramindex);
			switch(format){
				case ChartFormat.Daily:
				default:
					box.Text = sp.DailyValues[0].ToString();
					break;
				case ChartFormat.Weekly:
					box.Text = sp.WeeklyValues[0].ToString();
					break;
				case ChartFormat.Monthly:
					box.Text = sp.MonthlyValues[0].ToString();
					break;
			}
			
			box.TextChanged += new EventHandler(OnParameterTextChanged);
			box.Validating += new CancelEventHandler(OnValidatingParameterText);
			box.TabIndex = tabIndex;
			box.Left = left;
			box.Top = top;
			box.Width = width;
			box.Height = 24;
			parent.Controls.Add(box);
			return box;
		}
        //hacked by rti
        private TextBox AddSimpleInt64TextBox(TabPage parent, string def, int tabIndex, int left, int top, int width)
        {
            TextBox box = new TextBox();
            box.Text = def;

            box.TextChanged += new EventHandler(OnParameterTextChanged);
            box.Validating += new CancelEventHandler(OnValidatingInt64NumericText);
            box.TabIndex = tabIndex;
            box.Left = left;
            box.Top = top;
            box.Width = width;
            box.Height = 24;
            parent.Controls.Add(box);
            return box;
        }
        //------------------

		private void AddHelpButton(TabPage page, int tabIndex, int left, int top, string text) {
			if(text.Length==0) return;
			Button btn = new NotClickableButton();
			btn.Left = left;
			btn.Top = top;
			btn.Width = 19;
			btn.Height = 19;
			btn.FlatStyle = FlatStyle.Flat;
			btn.Image = Env.ImageList16.Images[IconConst.HELP];
			btn.Text = "";
			_toolTip.SetToolTip(btn, text);
			btn.TabIndex = tabIndex;
			page.Controls.Add(btn);
		}

		private void CommitIndicators() {
			foreach(CustomizeTag tag in _tags) {
				SchemaItem it = tag.schemaItem;
				if(!(it is SchemaIndicatorItem)) continue;
				if(tag.styleBox!=null) {
					it.Appearance.Appearances[tag.index].Style = (IndicatorStyle)tag.styleBox.SelectedIndex;
					it.Appearance.Appearances[tag.index].Color = tag.colorPicker.Color;
				}
				if(tag.dailyParams!=null) {
					double[] dp = Util.ParseDoubles(tag.dailyParams.Text);
					double[] wp = Util.ParseDoubles(tag.weeklyParams.Text);
					double[] mp = Util.ParseDoubles(tag.monthlyParams.Text);
					int j = 0;
					for(int i=0; i<it.ParameterCount; i++) {
						SchemaParameter p = it.GetParameter(i);
						if(p.Owner!=it) continue;
						p.DailyValues[tag.index] = dp[j];
						p.WeeklyValues[tag.index] = wp[j];
						p.MonthlyValues[tag.index] = mp[j];
						j++;
					}
				}
			}
		}

		//Screening
		private void InitScreeningPage(int index, ArrayList items) {
			int top = 8;
			int tabIndex = 0;
			TabPage page = _screeningPage;
			this._tabControl.Controls.Add(page);
			page.BackColor = ThemeUtil.TabPaneBackColor;

			int page_count = 1;
			int item_count = 0;
			foreach(SchemaScreeningItem item in items) {
				if(!item.OwnsConfig) continue;
				
				if(item_count + item.OwnedParameters.Length > MAX_ITEMS) { //はみ出しそうなら
					_screeningPage.Text = String.Format("スクリーニング - {0}", 1);
					page = InsertTabPage(page_count, String.Format("スクリーニング - {0}", ++page_count));
					top = 8;
					tabIndex = 0;
					item_count = 0;
				}
				int i = 0;
				foreach(SchemaParameter sp in item.OwnedParameters) {
					CustomizeTag tag = new CustomizeTag();
					tag.schemaItem = item;
					tag.index = sp.Index;
					tag.parameterCount = 1;
					if(i==0) {
						AddLabel(page, item.Title, tabIndex++, 8, top, 128);
						AddHelpButton(page, tabIndex++, 136, top, item.Description);
					}

					AddLabel(page, sp.Name, tabIndex++, 160, top, 64);
					tag.dailyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Daily, tabIndex++, 224, top, 48);
					AddLabel(page, sp.DailyUnit + " / ", tabIndex++, 272, top, 40);
					tag.weeklyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Weekly, tabIndex++, 312, top, 48);
					AddLabel(page, sp.WeeklyUnit + " / ", tabIndex++, 360, top, 40);
					tag.monthlyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Monthly, tabIndex++, 400, top, 48);
					AddLabel(page, sp.MonthlyUnit, tabIndex++, 448, top, 16);					
					item_count++;
					top += 24;
					i++;
					_tags.Add(tag);
				}
				

			}
		}
		private void CommitScreenings() {
			foreach(CustomizeTag tag in _tags) {
				SchemaItem it = tag.schemaItem;
				if(!(it is SchemaScreeningItem)) continue;

				double dp = Double.Parse(tag.dailyParams.Text);
				double wp = Double.Parse(tag.weeklyParams.Text);
				double mp = Double.Parse(tag.monthlyParams.Text);
				it.GetParameter(tag.index).DailyValues  = new double[] { dp };
				it.GetParameter(tag.index).WeeklyValues = new double[] { wp };
				it.GetParameter(tag.index).MonthlyValues = new double[] { mp };
			}
		}

		//Prediction
		private void InitPredictionPage(int index, ArrayList items) {
			int top = 8;
			int tabIndex = 0;
			TabPage page = _theoremPage;
			this._tabControl.Controls.Add(page);
			page.BackColor = ThemeUtil.TabPaneBackColor;

			int item_count = 0;
			int page_count = 1;
			foreach(SchemaPredictionItem item in items) {
				if(!item.OwnsConfig) continue;
				
				if(item_count + 1 > MAX_ITEMS/2) { //はみ出しそうなら
					_theoremPage.Text = String.Format("セオリー検証 - {0}", 1);
					page = InsertTabPage(page_count, String.Format("セオリー検証 - {0}", ++page_count));
					top = 8;
					tabIndex = 0;
					item_count = 0;
				}

				AddLabel(page, item.Title, tabIndex++, 8, top, 152);
				AddHelpButton(page, tabIndex++, 160, top, item.Description);
				AddLabel(page, item.HeaderString, tabIndex++, 184, top, 384);
				top += 24;
				int i = 0;

				int x = 184;
				foreach(SchemaParameter sp in item.OwnedParameters) {
					CustomizeTag tag = new CustomizeTag();
					tag.schemaItem = item;
					tag.index = sp.Index;
					tag.parameterCount = 1;

					AddLabel(page, String.Format("{{{0}}}=", sp.Index), tabIndex++, x, top, 28);
					x += 28;
					tag.dailyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Daily, tabIndex++, x, top, 32);
					x += 32;
					AddLabel(page, sp.DailyUnit, tabIndex++, x, top, 48);
					x += 48;
					i++;
					_tags.Add(tag);
				}
				top += 24;
				item_count++;

			}
		}
		private void CommitPredictions() {
			foreach(CustomizeTag tag in _tags) {
				SchemaItem it = tag.schemaItem;
				if(!(it is SchemaPredictionItem)) continue;

				double dp = Double.Parse(tag.dailyParams.Text);
				it.GetParameter(tag.index).DailyValues  = new double[] { dp };
			}
		}

		//AutoTrading
		private void InitAutoTradingPage(int index, ArrayList items) {
			int top = 8;
			int tabIndex = 0;
			TabPage page = _autoTradingPage;
			this._tabControl.Controls.Add(page);
			page.BackColor = ThemeUtil.TabPaneBackColor;

			int item_count = 0;
			int page_count = 1;
			foreach(SchemaAutoTradingItem item in items) {
				if(!item.OwnsConfig) continue;
				
				if(item_count + 1 > MAX_ITEMS/2) { //はみ出しそうなら
					_autoTradingPage.Text = String.Format("自動売買検証 - {0}", 1);
					page = InsertTabPage(page_count, String.Format("自動売買検証 - {0}", ++page_count));
					top = 8;
					tabIndex = 0;
					item_count = 0;
				}

				AddLabel(page, item.Title, tabIndex++, 8, top, 152);
				AddHelpButton(page, tabIndex++, 160, top, item.Description);
				AddLabel(page, item.HeaderString, tabIndex++, 184, top, 384);
				top += 24;
				int i = 0;

				int x = 184;
				foreach(SchemaParameter sp in item.OwnedParameters) {
					CustomizeTag tag = new CustomizeTag();
					tag.schemaItem = item;
					tag.index = sp.Index;
					tag.parameterCount = 1;

					AddLabel(page, String.Format("{{{0}}}=", sp.Index), tabIndex++, x, top, 28);
					x += 28;
					tag.dailyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Daily, tabIndex++, x, top, 32);
					x += 32;
					AddLabel(page, sp.DailyUnit, tabIndex++, x, top, 48);
					x += 48;
					i++;
					_tags.Add(tag);
				}
				top += 24;
				item_count++;
			}
		}
		private void CommitAutoTradings() {
			foreach(CustomizeTag tag in _tags) {
				SchemaItem it = tag.schemaItem;
				if(!(it is SchemaAutoTradingItem)) continue;

				double dp = Double.Parse(tag.dailyParams.Text);
				it.GetParameter(tag.index).DailyValues  = new double[] { dp };
			}
		}
        //hacked by rti
        //MoneyPlanning
        private void InitMoneyPlanningPage(int index, ArrayList items)
        {
            int top = 8;
            int tabIndex = 0;
            TabPage page = _MoneyPlanningPage;
            this._tabControl.Controls.Add(page);
            page.BackColor = ThemeUtil.TabPaneBackColor;

            int item_count = 0;
            int page_count = 1;

            //最初の一つ目は初期金額
            {
                AddLabel(page, "初期金額", tabIndex++, 8, top, 152);
                this._InitCachTextBox = AddSimpleInt64TextBox(page, Env.Preference.InitCach.ToString(), tabIndex++, 184, top, 100);
                AddLabel(page, "円", tabIndex++, 290, top, 30);
                item_count++;
                top += 24;
            }
            //二つ目は期間平均収益率を求める期間
            {

                AddLabel(page, "期間平均収益率を求める期間", tabIndex++, 8, top, 152);
                this._RTISisuuHeikinKikanTextBox = AddSimpleInt64TextBox(page, Env.Preference.RTISisuuHeikinKikan.ToString(), tabIndex++, 184, top, 100);
                AddLabel(page, "日", tabIndex++, 290, top, 30);
                item_count++;
                top += 24;
            }
            //三つ目は本日買いつけに回させる金額
            {
                AddLabel(page, "本日買い付けに回させる金額", tabIndex++, 8, top, 152);
                this._TodayBuyCachTextBox = AddSimpleInt64TextBox(page, Env.Preference.TodayBuyCach.ToString(), tabIndex++, 184, top, 100);
                AddLabel(page, "円", tabIndex++, 290, top, 30);
                item_count++;
                top += 24;
            }

            foreach (SchemaMoneyPlanningItem item in items)
            {
                if (!item.OwnsConfig) continue;

                if (item_count + 1 > MAX_ITEMS / 2)
                { //はみ出しそうなら
                    _MoneyPlanningPage.Text = String.Format("実質売買検証 - {0}", 1);
                    page = InsertTabPage(page_count, String.Format("実質売買検証 - {0}", ++page_count));
                    top = 8;
                    tabIndex = 0;
                    item_count = 0;

                }

                AddLabel(page, item.Title, tabIndex++, 8, top, 152);
                AddHelpButton(page, tabIndex++, 160, top, item.Description);
                AddLabel(page, item.HeaderString, tabIndex++, 184, top, 384);
                top += 24;
                int i = 0;

                int x = 184;
                foreach (SchemaParameter sp in item.OwnedParameters)
                {
                    CustomizeTag tag = new CustomizeTag();
                    tag.schemaItem = item;
                    tag.index = sp.Index;
                    tag.parameterCount = 1;

                    AddLabel(page, String.Format("{{{0}}}=", sp.Index), tabIndex++, x, top, 28);
                    x += 28;
                    tag.dailyParams = AddSingleParamBox(page, item, sp.Index, ChartFormat.Daily, tabIndex++, x, top, 32);
                    x += 32;
                    AddLabel(page, sp.DailyUnit, tabIndex++, x, top, 48);
                    x += 48;
                    i++;
                    _tags.Add(tag);
                }
                top += 24;
                item_count++;
            }
        }
        private void CommitMoneyPlannings()
        {
            //初期金額
            Env.Preference.InitCach = Util.ParseInt64(this._InitCachTextBox.Text, 10000000);
            //平均期間
            Env.Preference.RTISisuuHeikinKikan = Util.ParseInt(this._RTISisuuHeikinKikanTextBox.Text, 180);
            //本日買いつけに回させる金額
            Env.Preference.TodayBuyCach = Util.ParseInt64(this._TodayBuyCachTextBox.Text, 10000000);

            foreach (CustomizeTag tag in _tags)
            {
                SchemaItem it = tag.schemaItem;
                if (!(it is SchemaMoneyPlanningItem)) continue;

                double dp = Double.Parse(tag.dailyParams.Text);
                it.GetParameter(tag.index).DailyValues = new double[] { dp };
            }
        }
        //-------------

		private void OnIndicatorStyleChanged(object sender, EventArgs args) {
			ComboBox cb = (ComboBox)sender;
			CustomizeTag tag = FindCustomizeTag(cb);
			bool e = cb.SelectedIndex != (int)IndicatorStyle.None;
			if(tag.colorPicker!=null) tag.colorPicker.Enabled = e;
			if(tag.dailyParams!=null) tag.dailyParams.Enabled = e;
			if(tag.weeklyParams!=null) tag.weeklyParams.Enabled = e;
			if(tag.monthlyParams!=null) tag.monthlyParams.Enabled = e;
			
			_applyButton.Enabled = true;
		}
		private void OnValidatingParameterText(object sender, CancelEventArgs args) {
			string p = "";
			try {
				TextBox tb = (TextBox)sender;
				CustomizeTag tag = FindCustomizeTagByTextBox(tb);
				string[] t = tb.Text.Split(',');
				if(t.Length!=tag.parameterCount) {
					Util.Warning(this, String.Format("パラメータは{0}個の整数でなければいけません。", tag.parameterCount));
					args.Cancel = true;
					return;
				}

				foreach(string e in t) {
					p = e;
					//今はパラメータはすべて正整数として扱う。
					if(Int32.Parse(e)<=0) throw new FormatException();
				}
			}
			catch(Exception) {
				Util.Warning(this, String.Format("{0}はパラメータとして不正な形式です。", p));
				args.Cancel = true;
				return;
			}

			args.Cancel = false;
		}
		private void OnValidatingNumericText(object sender, CancelEventArgs args) {
			string p = ((TextBox)sender).Text;
			try {
				if(Int32.Parse(p)<=0) throw new FormatException();
			}
			catch(Exception) {
				Util.Warning(this, String.Format("{0}はパラメータとして不正な形式です。", p));
				args.Cancel = true;
				return;
			}

			args.Cancel = false;
		}
        //hacked by rti
        private void OnValidatingInt64NumericText(object sender, CancelEventArgs args)
        {
            string p = ((TextBox)sender).Text;
            try
            {
                if (Int64.Parse(p) <= 0) throw new FormatException();
            }
            catch (Exception)
            {
                Util.Warning(this, String.Format("{0}はパラメータとして不正な形式です。", p));
                args.Cancel = true;
                return;
            }

            args.Cancel = false;
        }
        //-------------

		private void OnSelectFont(object sender, EventArgs args) {
			FontDialog dlg = new FontDialog();
			dlg.Font = new Font(_fontName, _fontSize);
			dlg.AllowScriptChange = false;
			dlg.AllowVerticalFonts = false;
			dlg.FixedPitchOnly = true;
			dlg.MaxSize = 24;
			dlg.ShowEffects = false;
			
			if(dlg.ShowDialog(this)==DialogResult.OK) {
				_fontName = dlg.Font.Name;
				_fontSize = dlg.Font.Size;
				_fontStyle = dlg.Font.Style;
				_lCurrentFont.Text = String.Format("{0}/{1:F0}pt", _fontName, _fontSize);
				_applyButton.Enabled = true;
			}
		}

		private void OnReset(object sender, EventArgs args) {
			if(Util.AskUserYesNo(this, "すべての設定を初期化します。カスタマイズした内容はすべて失われますがよろしいですか？")==DialogResult.Yes) {
				Env.ResetWithoutConfig();
				this.DialogResult = DialogResult.OK;
				Close();
			}
		}

		private void OnOK(object sender, EventArgs args) {
			CommitConfigs();
		}
		private void OnApplyButtonClicked(object sender, EventArgs args) {
			CommitConfigs();
			CommandExec.RefreshChart();
			_applyButton.Enabled = false;
		}

		private void CommitConfigs() {
			CommitPreference();
			CommitIndicators();
			CommitScreenings();
			CommitPredictions();
			CommitAutoTradings();
            //hacked by rti
            CommitMoneyPlannings();
            //--------------
        }

		private void EnableApplyButton(object sender, NewColorArgs e) {
			_applyButton.Enabled = true;
		}
		private void EnableApplyButton(object sender, EventArgs args) {
			_applyButton.Enabled = true;
		}
		private void OnParameterTextChanged(object sender, EventArgs args) {
			_applyButton.Enabled = true;
		}
		private void OnNewColor(object sender, NewColorArgs args) {
			_applyButton.Enabled = true;
		}
	}
}
