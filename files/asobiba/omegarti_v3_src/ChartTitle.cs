/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
#if KENMILLE
#define USE_UPDATE_BUTTON
#endif

using System;
using System.Collections;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;

using Travis.Util;
using Zanetti.Commands;
using Zanetti.Data;
using Zanetti.DataSource;
using Zanetti.Indicators;


namespace Zanetti.UI
{
	internal class ChartTitle : UserControl
	{
		private string   _titleText;
		//private CustomComboBox _formatBox;
		private ButtonEx   _backButton;
		private ButtonEx   _frontButton;
#if USE_UPDATE_BUTTON
		private CombinedDataSource _dataSource;
		private ButtonEx   _updateButton;
#endif
		private ButtonEx   _chartFormatButton;

		private ToggleButtonEx _priceButton;
		private ToggleButtonEx _volumeButton;
		private ToggleButtonEx[] _oscillatorButtons;
		private ToggleButtonEx _accumulativeVolumeButton;

		private ToggleButtonEx _splitButton;
		private ToggleButtonEx _lockScaleButton;
		private ButtonEx       _candleWidthButton;
		private ToggleButtonEx _inverseButton;
		private ToggleButtonEx _logButton;

		private ContextMenu _chartFormatMenu;
		private ContextMenu _volumeMenu;
		private ContextMenu _candleWidthMenu;
		private ContextMenu _oscillatorMenu;
		private int _currentOscillatorPreferenceIndex;
		private int _tabWidth;

		private ZMenuItem _dailyMenu;
		private ZMenuItem _weeklyMenu;
		private ZMenuItem _monthlyMenu;
#if DOJIMA
		private ZMenuItem _halfDailyMenu;
#endif
		private ToolTip _toolTip;
		


		public string TitleText {
			get {
				return _titleText;
			}
			set {
				_titleText = value;
			}
		}


		public ChartTitle() {
			_toolTip = new ToolTip();
			_tabWidth = 180;
			this.BackColor = SystemColors.Control;
			this.Dock = DockStyle.Top;
			this.SetStyle(ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true);

			_backButton = new ButtonEx();
			_backButton.Size = new Size(36, 24);
			_backButton.Image = Env.ImageList16.Images[IconConst.BACK];
			_backButton.Click += new EventHandler(OnBackButtonClicked);
			_backButton.PopupMenu += new EventHandler(OnBackButtonPopupMenu);
			_backButton.Style = ButtonEx.BStyle.ButtonAndMenu;
			_toolTip.SetToolTip(_backButton, "Webブラウザのように、直前に見た銘柄に戻ります。");

			_frontButton = new ButtonEx();
			_frontButton.Size = new Size(36, 24);
			_frontButton.Image = Env.ImageList16.Images[IconConst.GO];
			_frontButton.Click += new EventHandler(OnFrontButtonClicked);
			_frontButton.PopupMenu += new EventHandler(OnFrontButtonPopupMenu);
			_frontButton.Style = ButtonEx.BStyle.ButtonAndMenu;
			_toolTip.SetToolTip(_frontButton, "Webブラウザのように、一つ先の銘柄を見ます。");
		
#if USE_UPDATE_BUTTON
			_updateButton = new ButtonEx();
			_updateButton.Size = new Size(24, 24);
			_updateButton.Image = Env.ImageList16.Images[IconConst.UPDATE];
			_updateButton.Click += new EventHandler(OnUpdateButtonClicked);
			_toolTip.SetToolTip(_updateButton, "現在表示中の銘柄のデータを最新のものに更新します。F5でも同じ動作です。");
#endif

			_chartFormatButton = new ButtonEx();
			_chartFormatButton.Size = new Size(36, 24);
			_chartFormatButton.Image = Env.ImageList16.Images[IconConst.CHARTFORMAT];
			_chartFormatButton.Click += new EventHandler(OnChartFormatButtonClicked);
			_chartFormatButton.Style = ButtonEx.BStyle.ShowTriangle;
			_toolTip.SetToolTip(_chartFormatButton, "日足・週足・月足の表示を切り替えます。");

			_priceButton = new ToggleButtonEx();
			_priceButton.Size = new Size(24, 24);
			_priceButton.Image = Env.ImageList16.Images[IconConst.VALUEWINDOW];
			_priceButton.Click += new EventHandler(OnTogglePrice);
			_toolTip.SetToolTip(_priceButton, "指標と凡例の表示をするかどうかを指定します。");

			_volumeButton = new ToggleButtonEx();
			_volumeButton.Size = new Size(36, 24);
			_volumeButton.Image = Env.ImageList16.Images[IconConst.VOLUME];
			_volumeButton.ShowComboStyle = true;
			_volumeButton.Click += new EventHandler(OnVolumeButtonClicked);
			_toolTip.SetToolTip(_volumeButton, "出来高の表示をするかどうかを指定します。");

			_oscillatorButtons = new ToggleButtonEx[OscillatorPreference.LENGTH];
			for(int i=0; i<_oscillatorButtons.Length; i++) {
				ToggleButtonEx b = new ToggleButtonEx();
				b.Size = new Size(36, 24);
				b.Image = Env.ImageList16.Images[IconConst.OSCILLATOR1 + i];
				b.ShowComboStyle = true;
				b.Click += new EventHandler(OnToggleOscillator);
				_toolTip.SetToolTip(b, "オシレータの表示をするかどうかを指定します。");
				_oscillatorButtons[i] = b;
			}

			_accumulativeVolumeButton = new ToggleButtonEx();
			_accumulativeVolumeButton.Size = new Size(24, 24);
			_accumulativeVolumeButton.Image = Env.ImageList16.Images[IconConst.ACCUMULATIVE];
			_accumulativeVolumeButton.Click += new EventHandler(OnToggleAccumulativeVolume);
			_toolTip.SetToolTip(_accumulativeVolumeButton, "価格帯別出来高の表示をするかどうかを指定します。対象日数はカスタマイズダイアログで指定できます。");

			_lockScaleButton = new ToggleButtonEx();
            _lockScaleButton.Size = new Size(24, 24);
            _lockScaleButton.Image = Env.ImageList16.Images[IconConst.GRIDLOCK];
            _lockScaleButton.Click += new EventHandler(OnToggleScaleLock);
			_toolTip.SetToolTip(_lockScaleButton, "価格のスケールを固定します。");

            _splitButton = new ToggleButtonEx();
            _splitButton.Size = new Size(24, 24);
            _splitButton.Image = Env.ImageList16.Images[IconConst.SPLIT];
            _splitButton.Click += new EventHandler(OnToggleSplitAdjuster);
			_toolTip.SetToolTip(_splitButton, "チャートが分割を考慮するかどうかを指定します。");

			_candleWidthButton = new ButtonEx();
			_candleWidthButton.Size = new Size(36, 24);
			_candleWidthButton.Image = Env.ImageList16.Images[IconConst.CANDLEWIDTH];
			_candleWidthButton.Click += new EventHandler(OnCandleWidthButtonClicked);
			_candleWidthButton.Style = ButtonEx.BStyle.ShowTriangle;
			_toolTip.SetToolTip(_candleWidthButton, "ロウソクの幅を指定します。");

			_inverseButton = new ToggleButtonEx();
			_inverseButton.Size = new Size(24, 24);
			_inverseButton.Image = Env.ImageList16.Images[IconConst.INVERSE];
			_inverseButton.Click += new EventHandler(OnToggleInverseStyle);
			_toolTip.SetToolTip(_inverseButton, "チャートの上下逆転表示を指定します。");

			_logButton = new ToggleButtonEx();
			_logButton.Size = new Size(24, 24);
			_logButton.Image = Env.ImageList16.Images[IconConst.LOG];
			_logButton.Click += new EventHandler(OnToggleLogStyle);
			_toolTip.SetToolTip(_logButton, "チャートの対数表示を指定します。");

			this.Controls.Add(_backButton);
			this.Controls.Add(_frontButton);
#if USE_UPDATE_BUTTON
			this.Controls.Add(_updateButton);
#endif
			this.Controls.Add(_chartFormatButton);
			this.Controls.Add(_priceButton);
			this.Controls.Add(_volumeButton);
			this.Controls.AddRange(_oscillatorButtons);
			this.Controls.Add(_accumulativeVolumeButton);
			this.Controls.Add(_lockScaleButton);
            this.Controls.Add(_splitButton);
			this.Controls.Add(_candleWidthButton);
			this.Controls.Add(_inverseButton);
			this.Controls.Add(_logButton);

			InitUI();
			InitMenu();
		}

		protected override void OnResize(EventArgs e) {
			base.OnResize (e);
			
			const int TOP = 2;
            int x = _tabWidth + 24;
			_backButton.Location = new Point(x, TOP);
			x += _backButton.Width;
			_frontButton.Location = new Point(x, TOP);
			x += _frontButton.Width;
#if USE_UPDATE_BUTTON
			_updateButton.Location = new Point(x, TOP);
            x += _updateButton.Width;
#endif
			_chartFormatButton.Location = new Point(x, TOP);
			x += _chartFormatButton.Width;
			x += 8;

			_priceButton.Location = new Point(x, TOP);
			x += _priceButton.Width;
			_volumeButton.Location = new Point(x, TOP);
			x += _volumeButton.Width;
			_accumulativeVolumeButton.Location = new Point(x, TOP);
			x += _accumulativeVolumeButton.Width;
			for(int i=0; i<_oscillatorButtons.Length; i++) {
				_oscillatorButtons[i].Location = new Point(x, TOP);
				x += _oscillatorButtons[i].Width;
			}
			x += 8;
			
			_lockScaleButton.Location = new Point(x, TOP);
			x += _lockScaleButton.Width;
			_splitButton.Location = new Point(x, TOP);
			x += _splitButton.Width;
			_candleWidthButton.Location = new Point(x, TOP);
			x += _candleWidthButton.Width;
			_inverseButton.Location = new Point(x, TOP);
			x += _inverseButton.Width;
			_logButton.Location = new Point(x, TOP);
			x += _logButton.Width;
			x += 8;
		}

		protected override void OnPaint(PaintEventArgs e) {
			base.OnPaint (e);
			DrawTitleTab(e.Graphics);
		}

		private void DrawTitleTab(Graphics g) {
			AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
			IntPtr hdc = g.GetHdc();
			try {
				Win32.RECT rect = new Win32.RECT();
				const int TAB_LEFT =  6;
				const int TAB_TOP  =  3;
				rect.left = TAB_LEFT+1;
				rect.top = TAB_TOP+1;
				rect.right = rect.left + _tabWidth;
				rect.bottom = this.Height;
				ZBrush back = new ZBrush(SystemColors.ControlLight);
				Win32.FillRect(hdc, ref rect, back.Handle);

				ZPen black  = new ZPen(Color.Black, ZPen.PenStyle.Normal);
				ZPen frame  = new ZPen(Color.DarkGray, ZPen.PenStyle.Normal);
				Win32.SelectObject(hdc, frame.Handle);
				Win32.POINT dummy = new Win32.POINT();
				Win32.MoveToEx(hdc, 0, this.Height-1, out dummy);
				Win32.LineTo(hdc, TAB_LEFT, this.Height-1);
				Win32.LineTo(hdc, TAB_LEFT, TAB_TOP);
				Win32.LineTo(hdc, rect.right+1, TAB_TOP);
			
				Win32.MoveToEx(hdc, rect.right, TAB_TOP, out dummy);
				Win32.LineTo(hdc, rect.right, this.Height-1);
				Win32.LineTo(hdc, this.Width, this.Height-1);

				DrawTitleString(hdc);
			}
			finally {
				g.ReleaseHdc(hdc);
			}
		}
		private void DrawTitleString(IntPtr hdc) {
			Win32.SelectObject(hdc, Env.Preference.HeaderHFont);
			Win32.SetBkColor(hdc, Util.ToCOLORREF(SystemColors.ControlLight));
			Win32.SetTextColor(hdc, Util.ToCOLORREF(Color.Black));
			AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
			ChartUtil.DrawText(hdc, 10, (this.Height-(int)Env.Preference.DefaultCharPitch.Height)/2+2, br.Name);
			Win32.SelectObject(hdc, Env.Preference.DefaultHFont);
		}

        public void InitUI() {
			_backButton.Enabled = Env.BrandHistory.HasBack;
			_frontButton.Enabled = Env.BrandHistory.HasFront;
			Preference pref = Env.Preference;
            _lockScaleButton.Checked = pref.ScaleLock;
            _splitButton.Checked = pref.AdjustSplit;
			_priceButton.Checked = pref.ShowPrice;
			_volumeButton.Checked = pref.ShowVolume!=HeightConfig.None;
			_accumulativeVolumeButton.Checked = pref.ShowAccumulativeVolume;
			_inverseButton.Checked = pref.InverseChart;
			_logButton.Checked = pref.LogScale;
			for(int i=0; i<OscillatorPreference.LENGTH; i++) {
				_oscillatorButtons[i].Checked = Env.Preference.OscillatorPreferences[i].Config!=HeightConfig.None;
				_oscillatorButtons[i].Invalidate();
			}
			this.Invalidate(true);
		}

		private void OnBackButtonClicked(object sender, EventArgs args) {
			Env.Command.Exec(CID.Back);
		}
		private void OnFrontButtonClicked(object sender, EventArgs args) {
			Env.Command.Exec(CID.Front);
		}
		private void OnBackButtonPopupMenu(object sender, EventArgs args) {
			ShowHistoryMenu(Env.BrandHistory.BackBrands, true);
		}
		private void OnFrontButtonPopupMenu(object sender, EventArgs args) {
			ShowHistoryMenu(Env.BrandHistory.FrontBrands, false);
		}
#if USE_UPDATE_BUTTON
		private void OnUpdateButtonClicked(object sender, EventArgs args) {
			UpdateCurrentBrand();
		}
#endif
        private void OnToggleScaleLock(object sender, EventArgs args) {
            Env.Command.Exec(CID.ToggleScaleLock);
        }
        private void OnToggleSplitAdjuster(object sender, EventArgs args) {
            Env.Command.Exec(CID.ToggleSplitAdjuster);
        }
		private void OnVolumeButtonClicked(object sender, EventArgs args) {
			_volumeMenu.Show(this, new Point(_volumeButton.Left, _volumeButton.Bottom));
		}
		private void OnTogglePrice(object sender, EventArgs args) {
			Env.Command.Exec(CID.TogglePriceEnabled);
		}
		private void OnToggleOscillator(object sender_, EventArgs args) {
			ToggleButtonEx sender = sender_ as ToggleButtonEx;
			int index = 0;
			for(int i=0; i<_oscillatorButtons.Length; i++) {
				if(sender==_oscillatorButtons[i]) {
					index = i;
					break;
				}
			}
			AdjustOscillatorMenu(index);
			_oscillatorMenu.Show(this, new Point(sender.Left, sender.Bottom));
		}
		private void OnToggleAccumulativeVolume(object sender, EventArgs args) {
			Env.Command.Exec(CID.ToggleAccumulativeVolumeEnabled);
		}
		private void OnToggleInverseStyle(object sender, EventArgs args) {
			Env.Command.Exec(CID.ToggleInverseStyle);
		}
		private void OnToggleLogStyle(object sender, EventArgs args) {
			Env.Command.Exec(CID.ToggleLogarithmStyle);
		}
		private void OnChartFormatButtonClicked(object sender, EventArgs args) {
			_chartFormatMenu.Show(this, new Point(_chartFormatButton.Left, _chartFormatButton.Bottom));
		}
		private void OnCandleWidthButtonClicked(object sender, EventArgs args) {
			_candleWidthMenu.Show(this, new Point(_candleWidthButton.Left, _candleWidthButton.Bottom));
		}

#if USE_UPDATE_BUTTON
		//!!ボタンのEnabled設定がからむのでここに配置しているが、本来はコマンドの体系で処理すべきもの
		public void UpdateCurrentBrand() {
			_updateButton.Enabled = false;
			DownloadOrder order = new DownloadOrder();
			order.Stock = DownloadOrder.StockDownload.SingleBrand;
			order.TargetBrand = Env.Frame.ChartCanvas.GetBrand();
			_dataSource = order.ConvertToDataSource();
			_dataSource.AsyncProcess(this.Handle);
		}

		private void ProgressStatus(int code, int brand) {
			try {
				if(code==AsyncConst.LPARAM_FINISHED) {
					//最新日付へ
					AbstractBrand b = Env.Frame.ChartCanvas.GetBrand();
					b.ReserveFarm().LoadFor(b);
					Env.Frame.ChartCanvas.LoadBrand(b, false);
					Env.Frame.ChartCanvas.MoveToLatest();
				}
				else if(code==AsyncConst.LPARAM_ERROR) {
					Util.Warning(this, _dataSource.ErrorMessage);
				}
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				Util.Warning(this, ex.Message);
			}
			_updateButton.Enabled = true;
		}
#endif
		private void AdjustOscillatorMenu(int index) {
			_currentOscillatorPreferenceIndex = index;

			//毎回メニューを作るのが早そう
			OscillatorPreference op = Env.Preference.OscillatorPreferences[index];
			EventHandler handler = new EventHandler(OnOscillatorMenu);
			_oscillatorMenu = new ContextMenu();
			ZMenuItem sizes = CreateMenu("サイズ(&S)", null, false);
			AddHeightConfigMenu(sizes, handler, (int)op.Config);
			_oscillatorMenu.MenuItems.Add(sizes);
			_oscillatorMenu.MenuItems.Add(CreateMenu("-", handler, false));
			foreach(OscillatorGroup g in Env.CurrentIndicators.OscillatorGroups)
				_oscillatorMenu.MenuItems.Add(CreateMenu(g.Title, handler, op.Config!=HeightConfig.None && g==op.OscillatorGroup));
		}
		private void ShowHistoryMenu(IEnumerable e, bool is_back) {
			ContextMenu m = new ContextMenu();
			EventHandler h = is_back? new EventHandler(OnBackBrandMenu) : new EventHandler(OnFrontBrandMenu);
			foreach(AbstractBrand br in e) {
				ZMenuItem mi = CreateMenu(String.Format("{0} {1}", br.CodeAsString, br.Name), h, false);
				m.MenuItems.Add(mi);
			}
			ButtonEx b = is_back? _backButton : _frontButton;
			b.Reset();
			m.Show(this, new Point(b.Left, b.Bottom));
		}
		private void OnBackBrandMenu(object sender, EventArgs args) {
			int index = (sender as MenuItem).Index;
			CommandExec.Back(index+1);
		}
		private void OnFrontBrandMenu(object sender, EventArgs args) {
			int index = (sender as MenuItem).Index;
			CommandExec.Front(index+1);
		}

		protected override void WndProc(ref Message m) {
			base.WndProc (ref m);
#if USE_UPDATE_BUTTON
			if(m.Msg==AsyncConst.WM_ASYNCPROCESS) {
				ProgressStatus(m.LParam.ToInt32(), m.WParam.ToInt32());
			}
#endif
		}


		private void InitMenu() {
			_chartFormatMenu = new ContextMenu();
			_chartFormatMenu.Popup += new EventHandler(OnPopupChartFormatMenu);
#if DOJIMA
			_chartFormatMenu.MenuItems.Add(_halfDailyMenu= CreateMenu("半日足(&H)", CID.SetStyleHalfDaily));
#endif
			_chartFormatMenu.MenuItems.Add(_dailyMenu  = CreateMenu("日足(&D)", CID.SetStyleDaily));
			_chartFormatMenu.MenuItems.Add(_weeklyMenu = CreateMenu("週足(&W)", CID.SetStyleWeekly));
			_chartFormatMenu.MenuItems.Add(_monthlyMenu= CreateMenu("月足(&M)", CID.SetStyleMonthly));

			_candleWidthMenu = new ContextMenu();
			_candleWidthMenu.Popup += new EventHandler(OnPopupCandleWidthMenu);
			for(int width = Env.Constants.MIN_CANDLE_WIDTH; width <=Env.Constants.MAX_CANDLE_WIDTH; width+=2) { //奇数でないとだめ
				ZCandleMenuItem mi = new ZCandleMenuItem(width);
				mi.Click += new EventHandler(OnCandleWidthMenuClicked);
				_candleWidthMenu.MenuItems.Add(mi);
			}

			_volumeMenu = new ContextMenu();
			_volumeMenu.Popup += new EventHandler(OnPopupVolumeMenu);
			AddHeightConfigMenu(_volumeMenu, new EventHandler(OnMenuSetVolume), -1);
		}

		private ZMenuItem CreateMenu(string text, CID cid) {
			ZMenuItem mi = new ZMenuItem();
			mi.Text = text;
			mi.CID = cid;
			mi.ShortcutKey = cid==CID.None? Keys.None : Env.Command.Get(cid).Shortcut;
			mi.Click += new EventHandler(OnMenuClicked);
			return mi;
		}
		private ZMenuItem CreateMenu(string text, EventHandler h, bool check) {
			ZMenuItem mi = new ZMenuItem();
			mi.Text = text;
			mi.CID = CID.None;
			if(h!=null) mi.Click += h;
			mi.Checked = check;
			return mi;
		}
		private void OnMenuClicked(object sender, EventArgs args) {
			Env.Command.Exec(((ZMenuItem)sender).CID);
		}
		private void OnCandleWidthMenuClicked(object sender, EventArgs args) {
			CommandExec.SetCandleWidth(((ZCandleMenuItem)sender).Width);
		}
		private void OnPopupChartFormatMenu(object sender, EventArgs args) {
			ChartFormat fmt = Env.CurrentIndicators.Format;
#if DOJIMA
			_halfDailyMenu.Checked = fmt==ChartFormat.HalfDaily;
#endif
			_dailyMenu.Checked = fmt==ChartFormat.Daily;
			_weeklyMenu.Checked = fmt==ChartFormat.Weekly;
			_monthlyMenu.Checked = fmt==ChartFormat.Monthly;
		}
		private void OnPopupCandleWidthMenu(object sender, EventArgs args) {
			foreach(ZCandleMenuItem mi in _candleWidthMenu.MenuItems)
				mi.Checked = (mi.Width==Env.Preference.CandleWidth);
		}
		private void OnPopupVolumeMenu(object sender, EventArgs args) {
			HeightConfig v = Env.Preference.ShowVolume;
			int t = (int)v;
			for(int i=0; i<_volumeMenu.MenuItems.Count; i++)
				_volumeMenu.MenuItems[i].Checked = i==t;
		}
		private void OnOscillatorMenu(object sender, EventArgs args) {
			OscillatorPreference op = Env.Preference.OscillatorPreferences[_currentOscillatorPreferenceIndex];
			MenuItem src = sender as MenuItem;
			if(src.Parent is MenuItem) { //コンテナであることだけでは判定が微妙だが
				int height_index = src.Index;
				op.Config = (HeightConfig)height_index;
			}
			else {
				int index = src.Index;
				op.OscillatorGroup = Env.CurrentIndicators.GetOscillatorGroupAt(index-2); //なし　のメニューとバー
				if(op.Config==HeightConfig.None) op.Config = HeightConfig.Middle; //表示しない設定においてグループが選択されたら中サイズで初期化
			}
			_oscillatorButtons[_currentOscillatorPreferenceIndex].Checked = op.Config!=HeightConfig.None;
			_oscillatorButtons[_currentOscillatorPreferenceIndex].Invalidate();
			CommandExec.ResetLayout();
		}
		private void OnMenuSetVolume(object sender, EventArgs args) {
			MenuItem src = sender as MenuItem;
			Env.Preference.ShowVolume = (HeightConfig)src.Index;
			_volumeButton.Checked = src.Index!=0;
			_volumeButton.Invalidate();
			CommandExec.ResetLayout();
		}

		protected override bool ProcessDialogKey(Keys keyData) {
			if(Env.Command.Exec(keyData)!=CommandResult.Ignored)
				return true;
			else
				return false;
		}
		/*
		 * MainFrameが処理できるのでここでは何もしない。ChartTitleに将来ホイールに反応するコントロールを置いたときが問題
		protected override void OnMouseWheel(MouseEventArgs e) {
			Env.Command.ExecMouseWheel(e.Delta);
		}
		*/

		private void AddHeightConfigMenu(Menu item, EventHandler handler, int height_config) {
			item.MenuItems.Add(CreateMenu("表示しない(&N)", handler, 0==height_config));
			item.MenuItems.Add(CreateMenu("大(&1)", handler, 1==height_config));
			item.MenuItems.Add(CreateMenu("中(&2)", handler, 2==height_config));
			item.MenuItems.Add(CreateMenu("小(&3)", handler, 3==height_config));
		}

	}
}
