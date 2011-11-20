/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Xml;
using System.IO;
using System.Drawing;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Security.Cryptography;

using Zanetti.Arithmetic;
using Zanetti.Data;
using Zanetti.DataSource;
using Zanetti.Forms;
using Zanetti.Config;
using Zanetti.SystemTrading.Screening;
using Zanetti.SystemTrading.AutoTrading;
using Zanetti.Prediction;
using Zanetti.Indicators;
using Zanetti.UI;
using Zanetti.Parser;

using Travis.Storage;
using Travis.Util;
using Travis.PKI;

using Crownwood.DotNetMagic.Common;
using Crownwood.DotNetMagic.Controls;
using Crownwood.DotNetMagic.Docking;
//using Crownwood.DotNetMagic.Menus;


namespace Zanetti.Commands {
	/* 新しいコマンドを追加するときは次のことをすること
	 * 
	 * 1) Command.CIDのEnum値に追加
	 * 2) Command#Init, Execを修正
	 * 3) コマンドがメニューやツールバーを持つ場合は、MenuItemとCommandの関連付けなどを追加
	 */ 
	internal enum CommandResult {
		Succeeded,
		Ignored,
		Failed
	}
	//キーカスタマイズダイアログではこの順番で表示される
	internal enum CID {
		None = 0 ,
		//ダイアログ表示
		ShowDownloadDialog,
		ShowInitializeDialog,
		ShowPrintDialog,
		ShowCustomizeDialog,
		ShowScreeningDialog,
		ShowPredictionDialog,
		ShowAutoTradingDialog,
		ShowAddBookmarkDialog,
		ShowAboutBox,
		ShowExtensionKitDialog,
		ShowTestKitDialog,
		ShowSearchBrandDialog,
		ShowKeyConfigDialog,
        ShowEnvironmentDialog,
		RegisterUserCode,
		//スタイル変更
		SetStyleDaily,
		SetStyleWeekly,
		SetStyleMonthly,
		SetStyleHalfDaily, //堂島用
		ShrinkCandleWidth,
		ExpandCandleWidth,
		ToggleLogarithmStyle,
		ToggleInverseStyle,
		ClearFreeLines,
		ToggleBookmarkPane,
        ToggleSplitAdjuster,
        ToggleScaleLock,
		TogglePriceEnabled,
		ToggleAccumulativeVolumeEnabled,
        //hacked by rti
        ToggleRealTimeUpdate,
        //-------------
		//移動
		MoveToFirstDate,
		MoveToLastDate,
		MoveToPrevPage,
		MoveToNextPage,
		MoveToPrevDate, //ロウソク１本分の移動
		MoveToNextDate, 
		ShowPrevBrand, //現在見えているペインでの銘柄移動
		ShowNextBrand,  
		ShowPrevCode,  //コード順での移動
		ShowNextCode,   
		Back,
		Front,
		//データ関係
		UpdateCurrentData,
		DownloadIndexFile,
		ExportBrand,
		ExportHalfDaily,
		//Web
		OpenWeb,
		OpenCompanyInfoSite_Yahoo,
		OpenCompanyInfoSite_Infoseek,
		OpenCompanyInfoSite_Nikkei,
		OpenCompanyInfoSite_Livedoor,
		//その他
		Quit,
		ReloadKit,

		//これより前にあるコマンドはキーカスタマイズ等の一覧で出てくる。後のコマンドは出てこない。
		VisibleCount,
		ShowInputCode,
		ShowSpecifiedBrand,

		//サポート用コマンド
#if ENABLE_SUPPORT_COMMAND
		SupRebuildIndex,
		SupCreateKey,
		SupReload,
		SupCreateCert,
		SupStatistics,
		SupShrinkData,
#endif
		//終端
		Count
	}

	internal class Command : ICloneable {
		private CID _id;
		private string _description;
		private Keys _shortcut;

		public CID ID {
			get {
				return _id;
			}
		}
		public string Description {
			get {
				return _description;
			}
		}
		public Keys Shortcut {
			get {
				return _shortcut;
			}
		}
		public object Clone() {
			return new Command(_id, _description, _shortcut);
		}

		/// これはCommandCollectionからしか呼ばない。
		public void SetChortcut(Keys key) {
			_shortcut = key;
		}

		public bool Visible {
			get {
				return _id < CID.VisibleCount;
			}
		}



		public Command(CID id, string description, Keys shortcut) {
			_id = id;
			_description = description;
			_shortcut = shortcut;
		}

		public CommandResult Exec() {
			switch(_id) {
				//ダイアログ表示
				case CID.ShowDownloadDialog:
					return CommandExec.ShowDownloadDialog();
				case CID.ShowInitializeDialog:
					return CommandExec.ShowInitializeDialog();
				case CID.ShowPrintDialog:
					return CommandExec.ShowPrintDialog();
				case CID.ShowCustomizeDialog:
					return CommandExec.ShowCustomizeDialog();
				case CID.ShowScreeningDialog:
					return CommandExec.ShowScreeningDialog();
				case CID.ShowPredictionDialog:
					return CommandExec.ShowPredictionDialog();
				case CID.ShowAutoTradingDialog:
					return CommandExec.ShowAutoTradingDialog();
				case CID.ShowAddBookmarkDialog:
					return CommandExec.ShowAddBookmarkDialog();
				case CID.ShowAboutBox:
					return CommandExec.AboutBox();
				case CID.ShowExtensionKitDialog:
					return CommandExec.ShowExtensionKitDialog();
				case CID.ShowTestKitDialog:
					return CommandExec.ShowKitTestDialog();
				case CID.ShowSearchBrandDialog:
					return CommandExec.ShowSearchBrandDialog();
				case CID.ShowKeyConfigDialog:
					return CommandExec.ShowKeyConfigDialog();
                case CID.ShowEnvironmentDialog:
                    return CommandExec.ShowEnvironmentDialog();
                case CID.RegisterUserCode:
                    return CommandExec.RegisterUserCode();
				//スタイル変更
				case CID.SetStyleDaily:
					return CommandExec.SetChartFormat(ChartFormat.Daily);
				case CID.SetStyleWeekly:
					return CommandExec.SetChartFormat(ChartFormat.Weekly);
				case CID.SetStyleMonthly:
					return CommandExec.SetChartFormat(ChartFormat.Monthly);
#if DOJIMA
				case CID.SetStyleHalfDaily:
					return CommandExec.SetChartFormat(ChartFormat.HalfDaily);
				case CID.ExportHalfDaily:
					return CommandExec.ExportHalfDailyData();
#endif
				case CID.ShrinkCandleWidth:
					return CommandExec.ShrinkCandleWidth();
				case CID.ExpandCandleWidth:
					return CommandExec.ExpandCandleWidth();
				case CID.ToggleLogarithmStyle:
					return CommandExec.SetLogarithmStyle(!Env.Preference.LogScale);
				case CID.ToggleInverseStyle:
					return CommandExec.SetInverseStyle(!Env.Preference.InverseChart);
				case CID.ClearFreeLines:
					return CommandExec.ClearFreeLines();
                //hacked by rti
                case CID.ToggleRealTimeUpdate:
                    return CommandExec.RealTimeUpdater();
                //------------
				case CID.ToggleBookmarkPane:
					return CommandExec.ShowBookmarkPane(!Env.Frame.IsBookmarkPaneVisible);
                case CID.ToggleSplitAdjuster:
                    return CommandExec.SetSplitAdjuster(!Env.Preference.AdjustSplit);
                case CID.ToggleScaleLock:
                    return CommandExec.SetScaleLock(!Env.Preference.ScaleLock);
				case CID.TogglePriceEnabled:
					return CommandExec.SetPriceEnabled(!Env.Preference.ShowPrice);
				case CID.ToggleAccumulativeVolumeEnabled:
					return CommandExec.SetAccumulativeVolumeEnabled(!Env.Preference.ShowAccumulativeVolume);
				//移動
                case CID.MoveToFirstDate:
					return CommandExec.JumpDate(Keys.Home);
				case CID.MoveToLastDate:
					return CommandExec.JumpDate(Keys.End);
				case CID.MoveToNextPage:
					return CommandExec.JumpDate(Keys.PageDown);
				case CID.MoveToPrevPage:
					return CommandExec.JumpDate(Keys.PageUp);
				case CID.MoveToNextDate:
					return CommandExec.JumpDate(Keys.Right);
				case CID.MoveToPrevDate:
					return CommandExec.JumpDate(Keys.Left);
				case CID.ShowNextBrand:
					return CommandExec.ShowNextBrand();
				case CID.ShowPrevBrand:
					return CommandExec.ShowPrevBrand();
				case CID.ShowNextCode:
					return CommandExec.ShowNextCode();
				case CID.ShowPrevCode:
					return CommandExec.ShowPrevCode();
				case CID.Back:
					return CommandExec.Back(1);
				case CID.Front:
					return CommandExec.Front(1);
					//データ関係
				case CID.DownloadIndexFile:
					return CommandExec.DownloadIndexFile();
				case CID.UpdateCurrentData:
					return CommandExec.UpdateCurrentData();
				case CID.ExportBrand:
					return CommandExec.ExportBrand();
					//Web
				case CID.OpenWeb:
					return CommandExec.OpenWeb();
				case CID.OpenCompanyInfoSite_Yahoo:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Yahoo);
				case CID.OpenCompanyInfoSite_Infoseek:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Infoseek);
				case CID.OpenCompanyInfoSite_Nikkei:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Nikkei);
				case CID.OpenCompanyInfoSite_Livedoor:
					return CommandExec.OpenCompanyInfoPage(CompanyInfoSite.Livedoor);
					//その他
				case CID.Quit:
					return CommandExec.Quit();
				case CID.ReloadKit:
					return CommandExec.Reload(null);
#if ENABLE_SUPPORT_COMMAND
				case CID.SupRebuildIndex: {
					Data.StaticGrouping.Run();
					return CommandResult.Succeeded;
				}
				case CID.SupReload:
					return CommandExec.Reload(null);
				case CID.SupCreateKey:
					return CommandExec.CreateKey();
				case CID.SupCreateCert:
					return CommandExec.GenerateUserCode();
				case CID.SupStatistics:
					return CommandExec.StatisticsTest();
				case CID.SupShrinkData:
					return CommandExec.ShrinkData();
#endif
				default:
					Debug.WriteLine("Unknown command " + _id);
					return CommandResult.Ignored;
			}
		}
	}
	internal class CommandCollection : ICloneable {
		
		//Collection
		private Hashtable _keyToCommand;
		private Command[] _idArray;

		public CommandCollection(StorageNode keys) {
			Init(keys);
		}
		public void Reset() {
			Init(null);
		}

		private CommandCollection() {
			_keyToCommand = new Hashtable();
			_idArray = new Command[(int)CID.Count];
		}
		public object Clone() {
			CommandCollection r = new CommandCollection();
			foreach(Command cmd in _idArray) {
				Command nc = (Command)cmd.Clone();
				r._idArray[(int)nc.ID] = nc;
				if(nc.Shortcut!=Keys.None) r._keyToCommand[nc.Shortcut] = nc;
			}
			return r;
		}

		private void Init(StorageNode keys) {
			_keyToCommand = new Hashtable();
			_idArray = new Command[(int)CID.Count];

			//ダイアログ表示
			AddCommand(CID.ShowDownloadDialog,      "データのダウンロード",     keys, Keys.Control|Keys.O);
			AddCommand(CID.ShowInitializeDialog,    "データの初期化",           keys, Keys.None);
			AddCommand(CID.ShowPrintDialog,         "印刷",                     keys, Keys.Control|Keys.P);
			AddCommand(CID.ShowCustomizeDialog,     "カスタマイズ",             keys, Keys.Control|Keys.T);
			AddCommand(CID.ShowScreeningDialog,     "スクリーニング",           keys, Keys.Control|Keys.S);
			AddCommand(CID.ShowAutoTradingDialog,   "自動売買検証",             keys, Keys.Control|Keys.V);
			AddCommand(CID.ShowPredictionDialog,    "セオリー検証",             keys, Keys.None);
			AddCommand(CID.ShowAddBookmarkDialog,   "ブックマークへの追加",     keys, Keys.None);
			AddCommand(CID.ShowAboutBox,            "バージョン情報の表示",     keys, Keys.None);
			AddCommand(CID.ShowKeyConfigDialog,     "キー割り当てのカスタマイズ", keys, Keys.None);
            AddCommand(CID.ShowEnvironmentDialog,   "環境設定",                 keys, Keys.None);
            AddCommand(CID.ShowExtensionKitDialog, "拡張キット一覧", keys, Keys.None);
            AddCommand(CID.ShowTestKitDialog,       "拡張キットテスト",         keys, Keys.None);
			AddCommand(CID.ShowSearchBrandDialog,   "銘柄の検索",               keys, Keys.Control|Keys.F);
			AddCommand(CID.RegisterUserCode,        "Contribution Certificateの登録", keys, Keys.None);
			//スタイル変更
			AddCommand(CID.SetStyleDaily,           "日足表示",                 keys, Keys.Control|Keys.D);
			AddCommand(CID.SetStyleWeekly,          "週足表示",                 keys, Keys.Control|Keys.W);
			AddCommand(CID.SetStyleMonthly,         "月足表示",                 keys, Keys.Control|Keys.M);
			AddCommand(CID.SetStyleHalfDaily,       "半日足表示",               keys, Keys.Control|Keys.H);
			AddCommand(CID.ShrinkCandleWidth,       "ロウソク幅の縮小",         keys, Keys.Control|Keys.OemMinus);
			AddCommand(CID.ExpandCandleWidth,       "ロウソク幅の拡大",         keys, Keys.Control|Keys.Oemplus);
			AddCommand(CID.ToggleLogarithmStyle,    "対数表示切替",             keys, Keys.Control|Keys.L);
			AddCommand(CID.ToggleInverseStyle,      "上下反転表示切替",         keys, Keys.Control|Keys.R);
			AddCommand(CID.ClearFreeLines,          "自由直線のクリア",         keys, Keys.None);
			AddCommand(CID.ToggleBookmarkPane,      "ブックマーク表示の切替",   keys, Keys.Control|Keys.B);
            AddCommand(CID.ToggleSplitAdjuster,     "分割の考慮切替",           keys, Keys.None);
            AddCommand(CID.ToggleScaleLock,         "縮尺の固定",               keys, Keys.None);
			AddCommand(CID.TogglePriceEnabled,      "価格と凡例の表示",         keys, Keys.None);
			AddCommand(CID.ToggleAccumulativeVolumeEnabled, "価格帯出来高の表示",keys,Keys.None);
            //hacked by rti
            AddCommand(CID.ToggleRealTimeUpdate, "リアルタイム更新を行う", keys, Keys.None);
            //-------------------
			//移動
            AddCommand(CID.MoveToFirstDate,         "最初の日付へ移動",         keys, Keys.Home);
			AddCommand(CID.MoveToLastDate,          "最後の日付へ移動",         keys, Keys.End);
				//このブロックはKeyConfigDialogでカスタマイズできる。１コマンドずつの独立ではないことに注意
			AddCommand(CID.MoveToNextPage,          "１画面先の日付へ移動",     keys, Keys.None);
			AddCommand(CID.MoveToPrevPage,          "１画面前の日付へ移動",     keys, Keys.None);
			AddCommand(CID.MoveToNextDate,          "１日次へ",                 keys, Keys.None);
			AddCommand(CID.MoveToPrevDate,          "１日前へ",                 keys, Keys.None);
			AddCommand(CID.ShowNextBrand,           "次の表示順銘柄",           keys, Keys.None);
			AddCommand(CID.ShowPrevBrand,           "前の表示順銘柄",           keys, Keys.None);
			AddCommand(CID.ShowNextCode,            "次のコード順銘柄",         keys, Keys.None);
			AddCommand(CID.ShowPrevCode,            "前のコード順銘柄",         keys, Keys.None);
			AddCommand(CID.Back,                    "戻る",                     keys, Keys.Back);
			AddCommand(CID.Front,                   "進む",                     keys, Keys.Shift|Keys.Back);
			
			AddCommand(CID.ShowInputCode,           "コード入力",               keys, Keys.None);
			//データ関係
			AddCommand(CID.DownloadIndexFile,       "インデックスファイルのダウンロード", keys, Keys.None);
			AddCommand(CID.UpdateCurrentData,       "現在の銘柄を更新",         keys, Keys.F5);
			AddCommand(CID.ExportBrand,             "データのエクスポート", keys, Keys.None);
			AddCommand(CID.ExportHalfDaily,         "半日足データのエクスポート", keys, Keys.None);
			//Web
			AddCommand(CID.OpenWeb,                  "OmegaChartのWebを開く",   keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Yahoo,    "Yahooの企業情報ページを開く",    keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Infoseek, "Infoseekの企業情報ページを開く", keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Nikkei,   "日経の企業情報ページを開く",     keys, Keys.None);
			AddCommand(CID.OpenCompanyInfoSite_Livedoor, "livedoorの企業情報ページを開く", keys, Keys.None);
			//その他
			AddCommand(CID.Quit,                     "OmegaChartの終了", keys, Keys.Control|Keys.Shift|Keys.X);
			AddCommand(CID.ReloadKit,                "拡張キットのリロード", keys, Keys.None);
			//サポート用コマンド
#if ENABLE_SUPPORT_COMMAND
			AddCommand(CID.SupRebuildIndex,          "Active500などのindex再作成", keys, Keys.None);
			AddCommand(CID.SupReload,                "リロード",                   keys, Keys.None);
			AddCommand(CID.SupCreateKey,             "キーの再作成",               keys, Keys.None);
			AddCommand(CID.SupCreateCert,            "Contribution Cert作成",      keys, Keys.None);
			AddCommand(CID.SupStatistics,            "Statistics",                 keys, Keys.None);
			AddCommand(CID.SupShrinkData,            "データ縮小",                 keys, Keys.None);
#endif
		}
		
		private void AddCommand(CID cid, string desc, StorageNode keys, Keys def) {
			Keys k = keys==null? def : Util.ParseKey(keys[cid.ToString()]);
			Command cmd = new Command(cid, desc, k);
			_idArray[(int)cid] = cmd;
			if(k!=Keys.None) _keyToCommand[k] = cmd;
		}
		public Command Get(CID cid) {
			Command r = _idArray[(int)cid];
			Debug.Assert(r!=null);
			return r;
		}
		public CommandResult Exec(CID cid) {
			Command cmd = Get(cid);
			Debug.Assert(cmd!=null);
			return cmd.Exec();
		}
		public CommandResult Exec(Keys key) {
			Command cmd = _keyToCommand[key] as Command;
			CID cid;
			if(cmd!=null)
				return cmd.Exec();
			else if((cid = Env.Options.KeyConfig.Translate(key))!=CID.None) {
				return Exec(cid);														
			}
			else if(Keys.D0 <= key && key <= Keys.D9) {
				CommandExec.PromptInputCode(false, (char)('0' + (key-Keys.D0)));
				return CommandResult.Succeeded;
			}
			else if(Keys.NumPad0 <= key && key <= Keys.NumPad9) {
				CommandExec.PromptInputCode(false, (char)('0' + (key-Keys.NumPad0)));
				return CommandResult.Succeeded;
			}
			else
				return CommandResult.Ignored;
		}
		public CommandResult Exec(ref Message m) {
			if(m.Msg==Win32.WM_KEYDOWN) {
				int k = m.WParam.ToInt32();
				return Exec((Keys)k | Control.ModifierKeys); //VKシリーズ定数と同じと仮定
			}
			else if(m.Msg==Win32.WM_MOUSEWHEEL) {
				return ExecMouseWheel(m.WParam.ToInt32() >> 16);
			}
			else
				return CommandResult.Ignored;
		}

		public CommandResult ExecMouseWheel(int delta) {
			CID cid;
			if((cid = Env.Options.KeyConfig.TranslateWheel(delta))!=CID.None) {
				return Exec(cid);			
			}
			else
				return CommandResult.Ignored;
		}

		public IEnumerator Enum() {
			return _idArray.GetEnumerator();
		}

		public void SetKeyAssign(Command cmd, Keys newkey) {
			Debug.Assert(_keyToCommand[newkey]==null);
			_keyToCommand[cmd.Shortcut] = null;
			_keyToCommand[newkey] = cmd;
			cmd.SetChortcut(newkey);
		}

		public void SaveTo(StorageNode node) {
			foreach(Command cmd in _idArray) {
				if(cmd.Shortcut!=Keys.None)
					node[cmd.ID.ToString()] = Util.FormatShortcut(cmd.Shortcut);
			}
		}
	}

	internal class CommandExec {

		public static CommandResult ShowBrand(AbstractBrand b) {
			AbstractBrand old = Env.Frame.ChartCanvas.GetBrand();
			if(old!=null) Env.BrandHistory.Update(old);
			CommandResult r = ShowBrandInternal(b);
			return r;
		}
		private static CommandResult ShowBrandInternal(AbstractBrand b) {
			Env.Preference.ScaleLock = false;
			RefreshChart();			
			Env.Frame.ChartCanvas.LoadBrand(b, true);

            //hacked by rti
            //表示する銘柄を切り替えたので、リアルタイムに監視する銘柄も切り替える
            if (Env.Frame.RealTimeUpdater.IsAlive())
            {
                Env.Frame.RealTimeUpdater.SetNextCode(b.Code, b.Market);
            }
            //-

			KitTestDialog kittest = Env.Frame.CurrentModelessDialog as KitTestDialog;
			if(kittest!=null) kittest.UpdateBrandName();
			return CommandResult.Succeeded;
		}
		public static CommandResult Back(int step) {
			if(!Env.BrandHistory.HasBack) return CommandResult.Ignored;
			AbstractBrand b = Env.BrandHistory.Back(Env.Frame.ChartCanvas.GetBrand(), step);
			return ShowBrandInternal(b);
		}
		public static CommandResult Front(int step) {
			if(!Env.BrandHistory.HasFront) return CommandResult.Ignored;
			AbstractBrand b = Env.BrandHistory.Redo(Env.Frame.ChartCanvas.GetBrand(), step);
			return ShowBrandInternal(b);
		}

		public static CommandResult ClearFreeLines() {
			Env.FreeLines.Clear(Env.Frame.ChartCanvas.GetBrand(), Env.CurrentIndicators.Format, Env.Preference.LogScale);
			Env.Frame.ChartCanvas.DrawingEngine.ClearScale();
			Env.Frame.ChartCanvas.Invalidate();
			return CommandResult.Succeeded;
		}
        //hacked by rti
        public static CommandResult RealTimeUpdater()
        {
            if (Env.Frame.RealTimeUpdater.IsAlive())
            {
                Env.Frame.RealTimeUpdater.Stop();
            }
            else
            {
                AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
                Env.Frame.RealTimeUpdater.SetNextCode(br.Code , br.Market);
                Env.Frame.RealTimeUpdater.Start();
            }
            return CommandResult.Succeeded;
        }
        //-----

		public static CommandResult ShowDownloadDialog() {
			return ShowDownloadDialog(null);
		}
		public static CommandResult ShowDownloadDialog(DownloadOrder order) {
			MainFrame f = Env.Frame;
			//!!ModelessDialog機構はまだ未完成

			DownloadWizard dlg = new DownloadWizard(order);

#if PRIVATE_FEATURE //自分専用：ダウンロードしたら即hottestでスクリーニング
			if(Control.ModifierKeys==Keys.Control) {
				dlg.RunScreeningNow = true;
				dlg.Text = "自分専用ダウンロード 即スクリーニング";
			}
#endif
			dlg.Owner = f;
			Rectangle rc = f.DesktopBounds;
			dlg.Left = (rc.Left + rc.Right)/2 - dlg.Width/2;
			dlg.Top  = (rc.Top  + rc.Bottom)/2 - dlg.Height/2;
			//f.CurrentModelessDialog = dlg;
			dlg.Show();
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowInitializeDialog() {
			InitializeData dlg = new InitializeData();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				return RefreshChart();
			}
			else
				return CommandResult.Failed;
		}

        private static CodeInput _codeInput;

		public static void PromptInputCode(bool fromMenu, char ch) {
            if(_codeInput==null) _codeInput = new CodeInput(); //２回目から再利用
			if(!fromMenu) _codeInput.StockCode = (int)(ch - '0');
			if(_codeInput.ShowDialog(Env.Frame)==DialogResult.OK) {
                AbstractBrand br = Env.BrandCollection.FindBrand(_codeInput.StockCode);
				if(br!=null)
					ShowBrand(br);

				if(fromMenu)
					Util.Information(Env.Frame, "このメニューを使うかわりに、通常の画面で４桁の数値を打ち込めば銘柄を指定することができます。");
			}
		}

		public static CommandResult ShowPrintDialog() {
			new PrintSupport().ShowPrintDialog();
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowCustomizeDialog() {
			CustomizeDialog dlg = new CustomizeDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				RefreshChart(null);
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ExportBrand() {
            ExportDialog dlg = new ExportDialog();
            dlg.ShowDialog(Env.Frame);
            return CommandResult.Succeeded;
        }


		public static CommandResult ShowScreeningDialog() {
			if(Env.Frame.CurrentModelessDialog!=null) {
				Util.Warning(Env.Frame, "現在ダウンロードを実行中のため、スクリーニングはできません。");
				return CommandResult.Succeeded;
			}

			ScreeningDialog dlg = new ScreeningDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.ScreeningOrder.Result!=null) {
					if(dlg.ScreeningOrder.Result.ResultCount > 0) {
						ScreeningResultPane pane = new ScreeningResultPane(dlg.ScreeningOrder);
						AddDockingPane(pane, dlg.ScreeningOrder.Name, pane.RequiredWidth, IconConst.SEARCH);

					}
				}
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowPredictionDialog() {
			PredictionDialog dlg = new PredictionDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.Result!=null) {
					if(dlg.Result.HitCount==0) {
						Util.Warning(Env.Frame, "条件に該当するデータはありませんでした。");
					}
					else {
						PredictionResultPane pane = new PredictionResultPane(dlg.Result);
						AddDockingPane(pane, dlg.Result.Item.Title, pane.RequiredWidth, IconConst.COPY);
					}
				}
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowAutoTradingDialog() {
			AutoTradingDialog dlg = new AutoTradingDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				if(dlg.Result!=null) {
					AutoTradingResultPane pane = new AutoTradingResultPane(dlg.Result);
					AddDockingPane(pane, dlg.Result.Item.Title, pane.RequiredWidth, IconConst.COPY);
				}
			}
			return CommandResult.Succeeded;
		}
		private static Content AddDockingPane(Control content, string title, int width, int imgindex) {
			DockingManager m = Env.Frame.DockingManager;
			WindowContentTabbed wc = FindTabControl(m, State.DockLeft);
			Content c;
			if(wc==null) {
				c = m.Contents.Add(content, title);
				c.DisplaySize = new Size(width+16,500);
				c.FloatingSize = new Size(width+16,500); 
				m.AddContentWithState(c, State.DockLeft);
			}
			else {
				c = m.Contents.Add(content, title);
				//通常のタブがあればそこに追加
				if(wc.TabControl.HideTabsMode==HideTabsModes.HideUsingLogic)
					m.AddContentToWindowContent(c, wc);
				c.BringToFront();
			}
			c.ImageList = Env.ImageList16;
			c.ImageIndex = imgindex;
			return c;

		}
		private static WindowContentTabbed FindTabControl(DockingManager m, State s) {
			foreach(Content c in m.Contents) {
				WindowContentTabbed t = c.ParentWindowContent as WindowContentTabbed;
				if(t!=null && t.State==s) return t;
			}
			return null;
		}

		public static CommandResult ShowAddBookmarkDialog() {
			SelectBookmarkFolder dlg = new SelectBookmarkFolder();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
				if(dlg.ResultFolder.ContainsCode(br.Code,false))
					Util.Warning(Env.Frame, "既に同じ銘柄が登録されています。");
				else{
					Content cnt = Env.Frame.BookmarkPaneContent;
					if(cnt!=null && !cnt.Visible)
						ShowBookmarkPane(true);				
					dlg.ResultFolder.AddChild(new BrandBookmark(dlg.ResultFolder,br.Code),null);
				}
			}
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowBookmarkPane(bool show) {
			if(show) {
				foreach(Content content in Env.Frame.DockingManager.Contents) {
					if(content.Control is BookmarkPane) {
						content.BringToFront();
						return CommandResult.Failed;
					}
				}
				BookmarkPane pane = new BookmarkPane();
				Env.Frame.BookmarkPaneContent = AddDockingPane(pane, "お気に入り", 150, IconConst.STAR);				
			}
			else {
				foreach(Content content in Env.Frame.DockingManager.Contents) {
					if(content.Control is BookmarkPane) {
						Env.Frame.DockingManager.Contents.Remove(content);
						return CommandResult.Failed;
					}
				}
				//Env.Frame.DockingManager.Contents.Remove(Env.Frame.BookmarkPaneContent);
				//Env.Frame.BookmarkPaneContent = null;
				//Env.Frame.DockingManager.HideContent(Env.Frame.BookmarkPaneContent);
			}
			return CommandResult.Succeeded;
		}
        public static CommandResult SetSplitAdjuster(bool adjust) {
            Env.BrandCollection.ClearAllFarms();
            Env.Preference.AdjustSplit = adjust;
            return ResetLayout();
        }
        public static CommandResult SetScaleLock(bool lock_) {
            Env.BrandCollection.ClearAllFarms();
            Env.Preference.ScaleLock = lock_;
			return ResetLayout();
		}
		public static CommandResult SetPriceEnabled(bool value) {
			Env.Preference.ShowPrice = value;
			return ResetLayout();
		}
		public static CommandResult SetAccumulativeVolumeEnabled(bool value) {
			Env.Preference.ShowAccumulativeVolume = value;
			return ResetLayout();
		}


        //!!Keysを使うのはやめたいところ
		public static CommandResult JumpDate(Keys key) {
			DataFarm farm = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
			if(farm.IsEmpty) return CommandResult.Failed;
			int limit = farm.TotalLength;

			int w = Env.Layout.DisplayColumnCount;
			int n = Env.Frame.ChartCanvas.FirstDateIndex;
			int cursor = n;
			switch(key) {
				case Keys.Home:
					n = 0;
					cursor = n;
					break;
				case Keys.End:
					n = limit - w;
					if(n<0) n = 0;
					cursor = limit-1;
					break;
				case Keys.PageUp:
					n -= w;
					if(n<0) n = 0;
					cursor = n;
					break;
				case Keys.PageDown:
					n += w;
					if(n > limit-w) n = limit-w;
					if(n < 0) n = 0;
					cursor = n + w - 1;
					break;
				case Keys.Left:
					n -= 5;
					if(n < 0) n=0;
					cursor = n;
					break;
				case Keys.Right:
					n += 5;
					if(n > limit-w) n = limit-w;
					if(n < 0) n = 0;
					cursor = n + w - 1;
					break;
			}

			Env.Frame.ChartCanvas.SetDateIndex(n, cursor);
			return CommandResult.Succeeded;
		}
		public static CommandResult OpenWeb() {
			try {
				Process.Start("http://www.omegachart.org/");
			}
			catch(Exception) {
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult AboutBox() {
			AboutBox dlg = new AboutBox();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowExtensionKitDialog() {
			ExtensionKitListDialog dlg = new ExtensionKitListDialog();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				Env.SaveEnv();
				Reload(null);
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowKeyConfigDialog() {
			KeyConfigDialog dlg = new KeyConfigDialog();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}
        public static CommandResult ShowEnvironmentDialog() {
            EnvironmentDialog dlg = new EnvironmentDialog();
            dlg.ShowDialog(Env.Frame);
            return CommandResult.Succeeded;
        }


        public static CommandResult ShowKitTestDialog() {
			KitTestDialog dlg = Env.KitTestDialog;
			if(dlg==null) {
				dlg = new KitTestDialog();
				Rectangle rc = Env.Frame.DesktopBounds;
				dlg.Left = (rc.Left + rc.Right)/2 - dlg.Width/2;
				dlg.Top  = (rc.Top  + rc.Bottom)/2 - dlg.Height/2;
			}
			if(dlg.Visible) return CommandResult.Ignored;

			Env.Frame.CurrentModelessDialog = dlg;
			dlg.Owner = Env.Frame;
			dlg.Show();
			return CommandResult.Succeeded;
		}
		public static CommandResult ShowSearchBrandDialog() {
			SearchBrandDialog dlg = new SearchBrandDialog();
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		//IndicatorSetの再構築と再表示
		public static CommandResult RefreshChart() {
			RefreshChart(null);
			return CommandResult.Succeeded;
		}
		public static CommandResult RefreshChart(IIndicatorCustomizer ic) {
			Env.Frame.Cursor = Cursors.WaitCursor;
			ChartCanvas cnv = Env.Frame.ChartCanvas;
			AbstractBrand br = cnv.GetBrand();
			ChartFormat format= Env.Options.ChartFormat;
#if DOJIMA
			//派生銘柄時の半日足は許可しない　原理的にはできるはずだが
			if(br is DerivedBrand && format==ChartFormat.HalfDaily)
				format = ChartFormat.Daily;
#endif
			Env.BrandCollection.ClearAllFarms();
			Env.Preference.Refresh();

			IndicatorSetBuilder bld = new IndicatorSetBuilder();
			bld.Customizer = ic;
			bld.Construct(Env.Options.ChartFormat);
			Env.CurrentIndicators = bld.Result;

			cnv.ReloadFromPreference();
			cnv.LoadBrand(br, false);
			ResetLayout();
			Env.Frame.Cursor = Cursors.Default;
			return CommandResult.Succeeded;
		}
		public static CommandResult ResetLayout() {
			Env.Layout.Init();
			Env.Frame.ChartCanvas.ResetLayout();
			Env.Frame.Invalidate(true);
			return CommandResult.Succeeded;
		}


		public static CommandResult Reload(IIndicatorCustomizer ic) {
			Env.ReloadSchema();
			RefreshChart(ic);
			//Util.Information(Env.Frame, "リロードしました");
			return CommandResult.Succeeded;
		}

			
		public static CommandResult Quit() {
			Env.Frame.Close();
			return CommandResult.Succeeded;
		}

		//署名関係
		public static CommandResult CreateKey() {
			RSAKeyPair kp = RSAKeyPair.GenerateNew(256, new Random());
			//PrivateKey保存
			StreamWriter wr = new StreamWriter(Env.GetAppDir()+"privatekey.txt");
			wr.WriteLine(kp.D.ToHexString());
			wr.WriteLine(kp.P.ToHexString());
			wr.WriteLine(kp.Q.ToHexString());
			wr.WriteLine(kp.U.ToHexString());
			wr.Close();
			//PublicKeyダンプ
			RSAPublicKey pub = (RSAPublicKey)kp.PublicKey;
			Debug.WriteLine("Pubkey-Mod="+pub.Modulus.ToHexString());
			Debug.WriteLine("Pubkey-Exp="+pub.Exponent.ToHexString());
			return CommandResult.Succeeded;
		}

		public static CommandResult SignKit() {

			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "XMLフォーマットの拡張キット選択";
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				string fn = dlg.FileName;
				StorageNode node = new DOMNodeReader(XmlUtil.LoadDOM(fn)).Read();
				Stream strm = new FileStream(fn+".bin", FileMode.Create, FileAccess.Write);
				new BinaryNodeWriter(strm).Write(node);
				strm.Close();
				SignFile(fn+".bin", fn+".signed");
			}
			return CommandResult.Succeeded;
		}
		public static CommandResult SignFile(string fn, string destfile) {
			StreamReader re = new StreamReader(Env.GetAppDir()+"privatekey.txt");
			BigInteger d = new BigInteger(re.ReadLine(), 16);
			BigInteger p = new BigInteger(re.ReadLine(), 16);
			BigInteger q = new BigInteger(re.ReadLine(), 16);
			BigInteger u = new BigInteger(re.ReadLine(), 16);

			RSAPublicKey pub = ZPublicKey.PubKeyForExtensionKit;
			RSAKeyPair kp = new RSAKeyPair(pub.Exponent, d, pub.Modulus, u, p, q);

			byte[] data = new byte[(int)new FileInfo(fn).Length];
			FileStream s = new FileStream(fn, FileMode.Open, FileAccess.Read);
			s.Read(data, 0, data.Length);
			s.Close();
			Debug.WriteLine("Signed length="+data.Length);
			byte[] hash = new SHA1CryptoServiceProvider().ComputeHash(data, 0, data.Length);
			byte[] signature = kp.Sign(hash);
			kp.Verify(signature, hash);

			Stream strm = new FileStream(destfile, FileMode.Create, FileAccess.Write);
			strm.Write(data, 0, data.Length);
			strm.Write(signature, 0, signature.Length);
			strm.Close();
			return CommandResult.Succeeded;
		}

		public static CommandResult GenerateUserCode() {
			RegistrationDialog dlg = new RegistrationDialog();
			dlg.GeneratingCode = true;
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}
		public static CommandResult RegisterUserCode() {
			RegistrationDialog dlg = new RegistrationDialog();
			dlg.GeneratingCode = false;
			dlg.ShowDialog(Env.Frame);
			return CommandResult.Succeeded;
		}

		public static CommandResult OpenCompanyInfoPage(CompanyInfoSite type) {
			try {
				string url = null;
				int code = Env.Frame.ChartCanvas.GetBrand().Code;
				switch(type) {
					case CompanyInfoSite.Yahoo:
						url = String.Format("http://profile.yahoo.co.jp/biz/fundamental/{0}.html", code);
						break;
					case CompanyInfoSite.Infoseek:
						url = String.Format("http://money.www.infoseek.co.jp/MnStock?qt={0}&sv=MN&pg=mn_creport.html", code);
						break;
					case CompanyInfoSite.Nikkei:
						url = String.Format("http://company.nikkei.co.jp/index.cfm?scode={0}", code);
						break;
					case CompanyInfoSite.Livedoor:
						url = String.Format("http://finance.livedoor.com/quote/profile?c={0}", code);
						break;
				}
				Process.Start(url);
				return CommandResult.Succeeded;
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				return CommandResult.Failed;
			}
		}
		//!!このあたりのペアはdelegateを使うなどしてまとめたい
		public static CommandResult ShowNextCode(){
			AbstractBrand br = Env.BrandCollection.FindNextBrand(
				Env.Frame.ChartCanvas.GetBrand().Code);
			ShowBrand(br);
			return CommandResult.Succeeded;
		}	
		public static CommandResult ShowPrevCode() {
			AbstractBrand br = Env.BrandCollection.FindPrevBrand(
				Env.Frame.ChartCanvas.GetBrand().Code);
			ShowBrand(br);
			return CommandResult.Succeeded;
		}

		public static CommandResult ShowNextBrand(){
			BrandListPane pane = Env.Frame.CurrentBrandListPane;
			if(pane!=null) {
				AbstractBrand br = pane.NextBrand;
				if(br!=null)
					return ShowBrand(br);
				else
					return CommandResult.Failed;
			}
			return CommandResult.Ignored;
		}	
		public static CommandResult ShowPrevBrand() {
			BrandListPane pane = Env.Frame.CurrentBrandListPane;
			if(pane!=null) {
				AbstractBrand br = pane.PrevBrand;
				if(br!=null)
					return ShowBrand(br);
				else
					return CommandResult.Failed;
			}
			return CommandResult.Ignored;
		}
		public static CommandResult ShrinkCandleWidth() {
			int hcw = Env.Preference.HalfCandleWidth;
			if(--hcw < 1) {
				hcw = 1;
			}
			Env.Preference.CandleWidth = hcw * 2;
			ResetLayout();		
			return CommandResult.Succeeded;
		}
		public static CommandResult ExpandCandleWidth(){
			int hcw = Env.Preference.HalfCandleWidth;
			if(++hcw > 20) { // 特に意味なし
				hcw = 20;
			}
			Env.Preference.CandleWidth = hcw * 2;		
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetCandleWidth(int width) {
			Env.Preference.CandleWidth = width;		
			ResetLayout();
			return CommandResult.Succeeded;
		}

		public static CommandResult SetLogarithmStyle(bool value){
			Env.Preference.LogScale = value;
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetInverseStyle(bool value){
			Env.Preference.InverseChart = value;
			ResetLayout();
			return CommandResult.Succeeded;
		}
		public static CommandResult SetChartFormat(ChartFormat fmt) {
			if(Env.Options.ChartFormat==fmt) return CommandResult.Ignored;
			Env.Options.ChartFormat = fmt;
			RefreshChart();
			return CommandResult.Succeeded;
		}
		public static CommandResult UpdateCurrentData() {
#if KENMILLE
			Env.Frame.ChartCanvas.ChartTitle.UpdateCurrentBrand();
			return CommandResult.Succeeded;
#else
			return CommandResult.Failed;
#endif
		}
		public static CommandResult DownloadIndexFile() {
			MemoryStream ns = null;
			try {
				ns = Util.HttpDownload("http://www.omegachart.org/download/index.txt");
				int dt = BrandCollection.GuessDate(ns);
				ns.Position = 0;
				if(dt > Env.BrandCollection.LastUpdatedDate) {
					Util.Information(Env.Frame, "新しいインデックスファイルが見つかりました。反映させるにはOmegaChartの再起動が必要です。");
					Util.StreamToFile(ns, Env.GetAppDir() + "index.txt");
					return CommandResult.Succeeded;
				}
				else {
					Util.Information(Env.Frame, "新しいインデックスファイルはありません。");
					return CommandResult.Ignored;
				}
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
				return CommandResult.Failed;
			}
			finally {
				if(ns!=null) ns.Close();
			}
		}

#if DOJIMA
		public static CommandResult ExportHalfDailyData() {
			if(Env.CurrentIndicators.Format!=ChartFormat.HalfDaily) {
				Util.Warning("半日足を表示した状態でのみエクスポートできます");
				return CommandResult.Failed;
			}

			SaveFileDialog dlg = new SaveFileDialog();
			dlg.Title = "半日足データのエクスポート";
			dlg.Filter = "CSV Files(*.csv)|*.csv";
			dlg.DefaultExt = "csv";
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				string filename = dlg.FileName;
				DailyDataFarm f = (DailyDataFarm)Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
				Dojima.DojimaUtil.HalfDailyDataFarmCache.Get(f).ExportInCSV(filename);
				return CommandResult.Succeeded;
			}
			else
				return CommandResult.Ignored;
		}
#endif

		//初期データの縮小
		public static CommandResult ShrinkData() {
			Env.Frame.Text = "データ縮小中";
			int date = 20050104; //ここ以降の日付のみ切り出す
			string dir = "shrinked";
			if(!Directory.Exists(dir))
				Directory.CreateDirectory("shrinked");

			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				BasicBrand br = ie.Value as BasicBrand;
				if(br!=null) {
					DailyDataFarm f = new DailyDataFarm();
					f.LoadFor(br);
					if(!f.IsEmpty) {
						int index = f.DateToIndex(date);

						FileStream fs = new FileStream(dir + "\\" + br.CodeAsString, FileMode.Create, FileAccess.Write);
						fs.Write(f.RawDataImage, index*DataFarm.RECORD_LENGTH, (f.TotalLength-index)*DataFarm.RECORD_LENGTH);
						fs.Close();
					}
				}
			}
			Util.Information(Env.Frame, "終了");
			return CommandResult.Succeeded;
		}

		//試作：統計情報
		public static CommandResult StatisticsTest() {
			//RunPrivateScreening();
			//MyStatistics.Do();
			return CommandResult.Succeeded;

		}
#if PRIVATE_FEATURE 
		public static void RunPrivateScreening() {
			ScreeningItem item = FindScreeningItem("hottest");
			ScreeningOrder so = new ScreeningOrder(item.Header, item);
			so.Execute();
			ScreeningResult sr = so.Result as ScreeningResult; //!!asがいやらしい
			Util.Information(Env.Frame, "完了");
			if(sr.ResultCount > 0) {
				ScreeningResultPane pane = new ScreeningResultPane(so);
				AddDockingPane(pane, item.Header, pane.RequiredWidth, IconConst.SEARCH);
			}
		}
		private static ScreeningItem FindScreeningItem(string name) {
			foreach(ScreeningItem item in Env.CurrentIndicators.GetScreeningItems()) {
				if(item.Header=="要注意銘柄") return item;
			}
			return null;
		}
#endif
	}
}
