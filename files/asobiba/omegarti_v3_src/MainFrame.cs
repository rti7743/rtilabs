/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using Zanetti.Forms;
using Zanetti.Data;
using Zanetti.DataSource;
using Zanetti.Indicators;
using Zanetti.Commands;
using Zanetti.UI;
//hacked by rti
using Zanetti.RealTimeUpdate;
//--------------

using Crownwood.DotNetMagic.Common;
using Crownwood.DotNetMagic.Controls;
using Crownwood.DotNetMagic.Docking;

using Zanetti.DDE;

namespace Zanetti.Forms
{
	/// <summary>
	/// MainFrame の概要の説明です。
	/// </summary>
	internal class MainFrame : System.Windows.Forms.Form
	{
		private DockingManager _manager;
		private Content _bookmarkContent;
		private InitialAction _initialAction;
		private IZModelessForm _modelessDialog;
		private AsyncSchemaLoader _asyncLoader;
		private BrandListPane _currentBrandListPane;

		private System.Windows.Forms.MainMenu _mainMenu;
		private System.Windows.Forms.StatusBar _statusBar;
		private ChartCanvas _chartCanvas;
		private ZMainMenuItem _menuFile;
		private ZMenuItem _menuDownload;
		private ZMenuItem _menuMRUDownload;
		private ZMenuItem _menuBarFile1;
		private ZMenuItem _menuInitialize;		
		private ZMenuItem _menuBarFile2;
		private ZMenuItem _menuPrint;		
		private ZMenuItem _menuBarFile3;
		private ZMenuItem _menuExport;
		private ZMenuItem _menuExportHalfDaily;
		private ZMenuItem _menuDownloadIndexFile;
		private ZMenuItem _menuBarFile4;
		private ZMenuItem _menuQuit;
		private ZMainMenuItem _menuView;
		private ZMenuItem _menuHalfDaily;
		private ZMenuItem _menuDaily;
		private ZMenuItem _menuWeekly;
		private ZMenuItem _menuMonthly;		
		private ZMenuItem _menuBarView1;
		private ZMenuItem _menuJump;
		private ZMenuItem _menuJumpHome;
		private ZMenuItem _menuJumpEnd;
		private ZMenuItem _menuBarJump;
		private ZMenuItem _menuJumpPrev;
		private ZMenuItem _menuJumpNext;
		private ZMenuItem _menuBarView2;
		private ZMenuItem _menuBookmark;
		private ZMenuItem _menuAddBookmark;
		private ZMenuItem _menuBarView3;
		private ZMenuItem _menuClearFreeLines;
		private ZMenuItem _menuBarView4;
		private ZMenuItem _menuLinkToInfo;
		private ZMenuItem _menuLinkToYahoo;
		private ZMenuItem _menuLinkToNikkei;
		private ZMenuItem _menuLinkToInfoseek;
		private ZMenuItem _menuLinkToLivedoor;
        //hacked by rti
        private ZMenuItem _menuBarView5;
        private ZMenuItem _menuRealTimeUpdate;
        //----------------
        private ZMainMenuItem _menuMove;
		private ZMenuItem _menuBack;
		private ZMenuItem _menuFront;
		private ZMenuItem _menuBarMove1;
		private ZMenuItem _menuInputCode;
		private ZMenuItem _menuInputName;
		private ZMenuItem _menuIndices;
		private ZMenuItem _menuCommodities;
		private ZMenuItem _menuDerivedBrand;
		private ZMainMenuItem _menuTool;
		private ZMenuItem _menuStartScreening;
		private ZMenuItem _menuStartAutoTrading;
		private ZMenuItem _menuStartVerification;
		private ZMenuItem _menuBarTool1;
		private ZMenuItem _menuCustomize;
		private ZMenuItem _menuKeyConfig;
        private ZMenuItem _menuEnvironment;
        private ZMenuItem _menuBarTool2;
        private ZMenuItem _menuReloadExtensionKit;
		private ZMenuItem _menuListExtensionKit;
		private ZMenuItem _menuTestExtensionKit;
		private ZMainMenuItem _menuHelp;
		private ZMenuItem _menuOpenWeb;
		private ZMenuItem _menuRegisterCode;
		private ZMenuItem _menuBarHelp;
		private ZMenuItem _menuAboutBox;
#if ENABLE_SUPPORT_COMMAND
		private ZMainMenuItem _menuSupport;
		private ZMenuItem _menuRebuildIndex;
		private ZMenuItem _menuCreateKey;
		//private ZMenuItem _menuSignKit;
		private ZMenuItem _menuReload;
		private ZMenuItem _menuCreateCert;
		private ZMenuItem _menuStatistics;
		private ZMenuItem _menuShrinkData;
#endif
        //hacked by rti
        public RealTimeUpdate.Main RealTimeUpdater;
        //hacked by rti
        //株価に更新があったら飛んでくるメッセージメッセージ
        const int WM_REAL_TIME_UPDATER_CHANGE = Win32.WM_USER + 10;
        //--------------

		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		//Layout.Initの時点ではFrameにアクセスできないといけないので初期化を分離
		public MainFrame() {
		}
		public void Init() {

			Env.Layout.Init();
			_chartCanvas = new Zanetti.UI.ChartCanvas();
			_chartCanvas.Dock = System.Windows.Forms.DockStyle.Fill;

			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();
			
			InitStatusBar();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			AdjustShortcut();
		}

		public void WarmUpMagicLibrary() {
			Type m = typeof(DockingManager);
		}

		public InitialAction InitialAction {
			get {
				return _initialAction;
			}
			set {
				_initialAction = value;
			}
		}
		public DockingManager DockingManager {
			get {
				return _manager;
			}
		}
		public Content BookmarkPaneContent {
			get {
				return _bookmarkContent;
			}
			set {
				_bookmarkContent = value;
			}
		}
		public IZModelessForm CurrentModelessDialog {
			get {
				return _modelessDialog;
			}
			set {
				_modelessDialog = value;
			}
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
            //hacked by rti
            this.RealTimeUpdater.Stop();
            //-------------
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainFrame));
			this._mainMenu = new System.Windows.Forms.MainMenu();
			this._menuFile = new ZMainMenuItem();
			this._menuDownload = new ZMenuItem();
			this._menuBarFile1 = new ZMenuItem();
			this._menuInitialize = new ZMenuItem();
			this._menuMRUDownload = new ZMenuItem();
			this._menuBarFile2 = new ZMenuItem();
			this._menuPrint = new ZMenuItem();
			this._menuBarFile3 = new ZMenuItem();
			this._menuExport = new ZMenuItem();
			this._menuExportHalfDaily = new ZMenuItem();
			this._menuDownloadIndexFile = new ZMenuItem();
			this._menuBarFile4 = new ZMenuItem();
			this._menuQuit = new ZMenuItem();
			this._menuView = new ZMainMenuItem();
			this._menuHalfDaily = new ZMenuItem();
			this._menuDaily = new ZMenuItem();
			this._menuWeekly = new ZMenuItem();
			this._menuMonthly = new ZMenuItem();
			this._menuBarView1 = new ZMenuItem();
			this._menuJump = new ZMenuItem();
			this._menuJumpHome = new ZMenuItem();
			this._menuJumpEnd = new ZMenuItem();
			this._menuBarJump = new ZMenuItem();
			this._menuJumpPrev = new ZMenuItem();
			this._menuJumpNext = new ZMenuItem();
			this._menuBarView2 = new ZMenuItem();
			this._menuBookmark = new ZMenuItem();
			this._menuAddBookmark = new ZMenuItem();
			this._menuBarView3 = new ZMenuItem();
			this._menuClearFreeLines = new ZMenuItem();
			this._menuBarView4 = new ZMenuItem();
			this._menuLinkToInfo = new ZMenuItem();
			this._menuLinkToYahoo = new ZMenuItem();
			this._menuLinkToInfoseek = new ZMenuItem();
			this._menuLinkToNikkei = new ZMenuItem();
			this._menuLinkToLivedoor = new ZMenuItem();
            //hacked by rti
            this._menuBarView5 =  new ZMenuItem();
            this._menuRealTimeUpdate =  new ZMenuItem();
            //-------------
			this._menuMove = new ZMainMenuItem();
			this._menuBack = new ZMenuItem();
			this._menuFront = new ZMenuItem();
			this._menuBarMove1 = new ZMenuItem();
			this._menuInputCode = new ZMenuItem();
			this._menuInputName = new ZMenuItem();
			this._menuIndices = new ZMenuItem();
			this._menuCommodities = new ZMenuItem();
			this._menuDerivedBrand = new ZMenuItem();
			this._menuTool = new ZMainMenuItem();
			this._menuStartScreening = new ZMenuItem();
			this._menuStartAutoTrading = new ZMenuItem();
			this._menuStartVerification = new ZMenuItem();
			this._menuBarTool1 = new ZMenuItem();
			this._menuCustomize = new ZMenuItem();
			this._menuKeyConfig = new ZMenuItem();
            this._menuEnvironment = new ZMenuItem();
            this._menuBarTool2 = new ZMenuItem();
            this._menuReloadExtensionKit = new ZMenuItem();
			this._menuListExtensionKit = new ZMenuItem();
			this._menuTestExtensionKit = new ZMenuItem();
			this._menuHelp = new ZMainMenuItem();
			this._menuOpenWeb = new ZMenuItem();
			this._menuRegisterCode = new ZMenuItem();
			this._menuBarHelp = new ZMenuItem();
			this._menuAboutBox = new ZMenuItem();
#if ENABLE_SUPPORT_COMMAND
			this._menuSupport = new ZMainMenuItem();
			this._menuRebuildIndex = new ZMenuItem();
			this._menuCreateKey = new ZMenuItem();
			this._menuReload = new ZMenuItem();
			this._menuStatistics = new ZMenuItem();
			this._menuCreateCert = new ZMenuItem();
			this._menuShrinkData = new ZMenuItem();
#endif
			this._statusBar = new System.Windows.Forms.StatusBar();
			this.SuspendLayout();
			// 
			// _mainMenu
			// 
			this._mainMenu.MenuItems.AddRange(new ZMainMenuItem[] {
																					  this._menuFile,
																					  this._menuView,
																					  this._menuMove,
																	                  this._menuTool,
																					  this._menuHelp,
#if ENABLE_SUPPORT_COMMAND																					
																					  this._menuSupport
#endif
																					});
			// 
			// _menuFile
			// 
			this._menuFile.Index = 0;
			this._menuFile.MenuItems.AddRange(new ZMenuItem[] {
																					  this._menuDownload,
																					this._menuMRUDownload,
																					this._menuBarFile1,
																					this._menuInitialize,
																					this._menuBarFile2,
																					  this._menuPrint,
																					  this._menuBarFile3,
																					  this._menuExport,
																					this._menuExportHalfDaily,
																					this._menuDownloadIndexFile,
																					  this._menuBarFile4,
																					  this._menuQuit});
			this._menuFile.Text = "ファイル(&F)";
			this._menuFile.Popup += new System.EventHandler(this.OnAdjustFileMenu);
			// 
			// _menuDownload
			// 
			this._menuDownload.Index = 0;
			this._menuDownload.Text = "データのダウンロード(&D)...";
			this._menuDownload.Click += new System.EventHandler(this.OnMenu);
			this._menuDownload.CID = CID.ShowDownloadDialog;
			// 
			// _menuMRUDownload
			// 
			this._menuMRUDownload.Index = 1;
			this._menuMRUDownload.Text = "保存した設定でダウンロード(&S)";
			// 
			// _menuBarFile1
			// 
			this._menuBarFile1.Index = 2;
			this._menuBarFile1.Text = "-";
			// 
			// _menuInitialize
			// 
			this._menuInitialize.Index = 3;
			this._menuInitialize.Text = "データの初期化(&I)...";
			this._menuInitialize.Click += new System.EventHandler(this.OnMenu);
			this._menuInitialize.CID = CID.ShowInitializeDialog;
			// 
			// _menuBarFile2
			// 
			this._menuBarFile2.Index = 4;
			this._menuBarFile2.Text = "-";
			// 
			// _menuPrint
			// 
			this._menuPrint.Index = 5;
			this._menuPrint.Text = "印刷(&P)...";
			this._menuPrint.Click += new System.EventHandler(this.OnMenu);	
			this._menuPrint.CID = CID.ShowPrintDialog;
			// 
			// _menuBarFile3
			// 
			this._menuBarFile3.Index = 6;
			this._menuBarFile3.Text = "-";
			// 
			// _menuExport
			// 
			this._menuExport.Index = 7;
			this._menuExport.Text = "データのエクスポート(&E)...";
			this._menuExport.Click += new System.EventHandler(this.OnMenu);
			this._menuExport.CID = CID.ExportBrand;
			// 
			// _menuExportHalfDaily
			// 
			this._menuExportHalfDaily.Index = 8;
			this._menuExportHalfDaily.Text = "半日足のエクスポート(&H)...";
			this._menuExportHalfDaily.Click += new System.EventHandler(this.OnMenu);
			this._menuExportHalfDaily.CID = CID.ExportHalfDaily;
			this._menuExportHalfDaily.Visible = false;
			// 
			// _menuDownloadIndexFile
			// 
			this._menuDownloadIndexFile.Index = 9;
			this._menuDownloadIndexFile.Text = "インデックスファイルのダウンロード(&I)";
			this._menuDownloadIndexFile.Click += new System.EventHandler(this.OnMenu);
			this._menuDownloadIndexFile.CID = CID.DownloadIndexFile;
			// 
			// _menuBarFile4
			// 
			this._menuBarFile4.Index = 10;
			this._menuBarFile4.Text = "-";
			// 
			// _menuQuit
			// 
			this._menuQuit.Index = 11;
			this._menuQuit.Text = "終了(&X)";
			this._menuQuit.Click += new System.EventHandler(this.OnMenu);
			this._menuQuit.CID = CID.Quit;
			// 
			// _menuView
			// 
			this._menuView.Index = 1;
			this._menuView.MenuItems.AddRange(new ZMenuItem[] {
																					this._menuHalfDaily,
																					this._menuDaily,
																					  this._menuWeekly,
																					  this._menuMonthly,
																					  this._menuBarView1,
																					  this._menuBookmark,
																					  this._menuAddBookmark,
																					  this._menuBarView2,
																					  this._menuClearFreeLines,
																					  this._menuBarView3,
																					  this._menuLinkToInfo
                                                                                      //hacked by rti
                                                                                      ,this._menuBarView5
                                                                                      ,this._menuRealTimeUpdate
                                                                                      //-------------
                                                                                      });
			this._menuView.Text = "表示(&V)";
			this._menuView.Popup += new System.EventHandler(this.OnAdjustViewMenu);
			// 
			// _menuHalfDaily
			// 
			this._menuHalfDaily.Index = 0;
			this._menuHalfDaily.Visible = false;
			this._menuHalfDaily.Text = "半日足(&H)";
			this._menuHalfDaily.Click += new System.EventHandler(this.OnMenu);
			this._menuHalfDaily.CID = CID.SetStyleHalfDaily;
			// 
			// _menuDaily
			// 
			this._menuDaily.Index = 1;
			this._menuDaily.Text = "日足(&D)";
			this._menuDaily.Click += new System.EventHandler(this.OnMenu);
			this._menuDaily.CID = CID.SetStyleDaily;
			// 
			// _menuWeekly
			// 
			this._menuWeekly.Index = 2;
			this._menuWeekly.Text = "週足(&W)";
			this._menuWeekly.Click += new System.EventHandler(this.OnMenu);
			this._menuWeekly.CID = CID.SetStyleWeekly;
			// 
			// _menuMonthly
			// 
			this._menuMonthly.Index = 3;
			this._menuMonthly.Text = "月足(&M)";
			this._menuMonthly.Click += new System.EventHandler(this.OnMenu);
			this._menuMonthly.CID = CID.SetStyleMonthly;
			// 
			// _menuBarView1
			// 
			this._menuBarView1.Index = 4;
			this._menuBarView1.Text = "-";
			// 
			// _menuBookmark
			// 
			this._menuBookmark.Index = 5;
			this._menuBookmark.Text = "お気に入りの表示(&B)";
			this._menuBookmark.Click += new System.EventHandler(this.OnMenu);
			this._menuBookmark.CID = CID.ToggleBookmarkPane;
			// 
			// _menuAddBookmark
			// 
			this._menuAddBookmark.Index = 6;
			this._menuAddBookmark.Text = "お気に入りへ追加(&A)...";
			this._menuAddBookmark.Click += new System.EventHandler(this.OnMenu);
			this._menuAddBookmark.CID = CID.ShowAddBookmarkDialog;
			// 
			// _menuBarView2
			// 
			this._menuBarView2.Index = 7;
			this._menuBarView2.Text = "-";
			// 
			// _menuClearFreeLines
			// 
			this._menuClearFreeLines.Index = 8;
			this._menuClearFreeLines.Text = "自由直線のクリア(&C)";
			this._menuClearFreeLines.Click += new System.EventHandler(this.OnMenu);
			this._menuClearFreeLines.CID = CID.ClearFreeLines;
			// 
			// _menuBarView5
			// 
			this._menuBarView3.Index = 9;
			this._menuBarView3.Text = "-";
			// 
			// _menuLinkToInfo
			// 
			this._menuLinkToInfo.Index = 10;
			this._menuLinkToInfo.MenuItems.AddRange(new ZMenuItem[] {
																		this._menuLinkToYahoo,
																		this._menuLinkToInfoseek,
																		this._menuLinkToNikkei,
																		this._menuLinkToLivedoor});
			this._menuLinkToInfo.Text = "企業情報へのリンク(&L)";
            //hacked by rti
            // 
            // _menuBarView6
            // 
            this._menuBarView5.Index = 11;
            this._menuBarView5.Text = "-";
            // 
            // _menuClearFreeLines
            // 
            this._menuRealTimeUpdate.Index = 12;
            this._menuRealTimeUpdate.Text = "リアルタイム更新を行う";
            this._menuRealTimeUpdate.Click += new System.EventHandler(this.OnMenu);
            this._menuRealTimeUpdate.CID = CID.ToggleRealTimeUpdate;
            //-----------------------------------

            // 
			// _menuLinkToYahoo
			// 
			this._menuLinkToYahoo.Index = 0;
			this._menuLinkToYahoo.Text = "Yahoo(&Y)";
			this._menuLinkToYahoo.Click += new System.EventHandler(this.OnMenu);
			this._menuLinkToYahoo.CID = CID.OpenCompanyInfoSite_Yahoo;
			// 
			// _menuLinkToInfoseek
			// 
			this._menuLinkToInfoseek.Index = 1;
			this._menuLinkToInfoseek.Text = "Infoseek(&I)";
			this._menuLinkToInfoseek.Click += new System.EventHandler(this.OnMenu);
			this._menuLinkToInfoseek.CID = CID.OpenCompanyInfoSite_Infoseek;
			// 
			// _menuLinkToNikkei
			// 
			this._menuLinkToNikkei.Index = 2;
			this._menuLinkToNikkei.Text = "日経新聞(&N)";
			this._menuLinkToNikkei.Click += new System.EventHandler(this.OnMenu);
			this._menuLinkToNikkei.CID = CID.OpenCompanyInfoSite_Nikkei;
			// 
			// _menuLinkToLivedoor
			// 
			this._menuLinkToLivedoor.Index = 3;
			this._menuLinkToLivedoor.Text = "livedoor(&L)";
			this._menuLinkToLivedoor.Click += new System.EventHandler(this.OnMenu);
			this._menuLinkToLivedoor.CID = CID.OpenCompanyInfoSite_Livedoor;
			
			// 
			// _menuMove
			// 
			this._menuMove.Index = 2;
			this._menuMove.MenuItems.AddRange(new ZMenuItem[] {
																  this._menuBack,
																  this._menuFront,
																  this._menuBarMove1,
																	this._menuInputCode,
																	this._menuInputName,
																	this._menuIndices,
																	this._menuCommodities,
																	this._menuDerivedBrand
															  });

			this._menuMove.Text = "移動(&M)";
			this._menuMove.Popup += new System.EventHandler(this.OnAdjustMoveMenu);
			// 
			// _menuBack
			// 
			this._menuBack.Index = 0;
			this._menuBack.Text = "戻る(&B)";
			this._menuBack.Click += new System.EventHandler(this.OnMenu);
			this._menuBack.CID = CID.Back;
			// 
			// _menuFront
			// 
			this._menuFront.Index = 1;
			this._menuFront.Text = "進む(&F)";
			this._menuFront.Click += new System.EventHandler(this.OnMenu);
			this._menuFront.CID = CID.Front;
			// 
			// _menuBarTool1
			// 
			this._menuBarMove1.Index = 2;
			this._menuBarMove1.Text = "-";
			// 
			// _menuInputCode
			// 
			this._menuInputCode.Index = 3;
			this._menuInputCode.Text = "銘柄コードを指定(&C)...";
			this._menuInputCode.Click += new System.EventHandler(this.OnMenu);
			this._menuInputCode.CID = CID.ShowInputCode;
			// 
			// _menuInputName
			// 
			this._menuInputName.Index = 4;
			this._menuInputName.Text = "名前から検索(&S)...";
			this._menuInputName.Click += new System.EventHandler(this.OnMenu);
			this._menuInputName.CID = CID.ShowSearchBrandDialog;
			// 
			// _menuIndices
			// 
			this._menuIndices.Index = 5;
			this._menuIndices.Text = "指数・為替(&I)";
			// 
			// _menuCommodities
			// 
			this._menuCommodities.Index = 6;
			this._menuCommodities.Visible = false;
			this._menuCommodities.Text = "商品先物(&O)";
			// 
			// _menuDerivedBrand
			// 
			this._menuDerivedBrand.Index = 7;
			this._menuDerivedBrand.Text = "派生銘柄(&U)";
			// 
			// _menuTool
			// 
			this._menuTool.Index = 3;
			this._menuTool.MenuItems.AddRange(new ZMenuItem[] {
																					  this._menuStartScreening,
																					  this._menuStartAutoTrading,
																					  this._menuStartVerification,
																					  this._menuBarTool1,
																					  this._menuCustomize,
																					  this._menuKeyConfig,
																					  this._menuEnvironment,
																					  this._menuBarTool2,
																					  this._menuReloadExtensionKit,
																					  this._menuListExtensionKit,
																					  this._menuTestExtensionKit});
			this._menuTool.Text = "ツール(&T)";
			// 
			// _menuStartScreening
			// 
			this._menuStartScreening.Index = 0;
			this._menuStartScreening.Text = "スクリーニング(&S)...";
			this._menuStartScreening.Click += new System.EventHandler(this.OnMenu);
			this._menuStartScreening.CID = CID.ShowScreeningDialog;
			// 
			// _menuStartAutoTrading
			// 
			this._menuStartAutoTrading.Index = 1;
			this._menuStartAutoTrading.Text = "自動売買検証(&U)...";
			this._menuStartAutoTrading.Click += new System.EventHandler(this.OnMenu);
			this._menuStartAutoTrading.CID = CID.ShowAutoTradingDialog;
			// 
			// _menuStartVerification
			// 
			this._menuStartVerification.Index = 2;
			this._menuStartVerification.Text = "セオリー検証(&V)...";
			this._menuStartVerification.Click += new System.EventHandler(this.OnMenu);
			this._menuStartVerification.CID = CID.ShowPredictionDialog;
			// 
			// _menuBarTool1
			// 
			this._menuBarTool1.Index = 3;
			this._menuBarTool1.Text = "-";
			// 
			// _menuCustomize
			// 
			this._menuCustomize.Index = 4;
			this._menuCustomize.Text = "カスタマイズ(&C)...";
			this._menuCustomize.Click += new System.EventHandler(this.OnMenu);
			this._menuCustomize.CID = CID.ShowCustomizeDialog;
			// 
			// _menuKeyConfig
			// 
			this._menuKeyConfig.Index = 5;
			this._menuKeyConfig.Text = "キー割り当て(&A)...";
			this._menuKeyConfig.Click += new System.EventHandler(this.OnMenu);
			this._menuKeyConfig.CID = CID.ShowKeyConfigDialog;
            // 
            // _menuEnvironment
            // 
            this._menuEnvironment.Index = 6;
            this._menuEnvironment.Text = "環境設定(&E)...";
            this._menuEnvironment.Click += new System.EventHandler(this.OnMenu);
            this._menuEnvironment.CID = CID.ShowEnvironmentDialog;
            // 
            // _menuBarTool2
			// 
			this._menuBarTool2.Index = 7;
			this._menuBarTool2.Text = "-";
			// 
			// _menuReloadExtensionKit
			// 
			this._menuReloadExtensionKit.Index = 8;
			this._menuReloadExtensionKit.Text = "拡張キットのリロード(&R)";
			this._menuReloadExtensionKit.Click += new System.EventHandler(this.OnMenu);
			this._menuReloadExtensionKit.CID = CID.ReloadKit;
			// 
			// _menuListExtensionKit
			// 
			this._menuListExtensionKit.Index = 9;
			this._menuListExtensionKit.Text = "拡張キットの一覧(&K)...";
			this._menuListExtensionKit.Click += new System.EventHandler(this.OnMenu);
			this._menuListExtensionKit.CID = CID.ShowExtensionKitDialog;
			// 
			// _menuTestExtensionKit
			// 
			this._menuTestExtensionKit.Index = 10;
			this._menuTestExtensionKit.Text = "拡張キットのテスト(&T)...";
			this._menuTestExtensionKit.Click += new System.EventHandler(this.OnMenu);
			this._menuTestExtensionKit.CID = CID.ShowTestKitDialog;
			// 
			// _menuHelp
			// 
			this._menuHelp.Index = 4;
			this._menuHelp.MenuItems.AddRange(new ZMenuItem[] {
																					  this._menuOpenWeb,
																					  this._menuRegisterCode,
																					  this._menuBarHelp,
																					  this._menuAboutBox});
			this._menuHelp.Text = "ヘルプ(&H)";
			// 
			// _menuOpenWeb
			// 
			this._menuOpenWeb.Index = 0;
			this._menuOpenWeb.Text = "OmegaChartのWebを開く(&W)";
			this._menuOpenWeb.Click += new System.EventHandler(this.OnMenu);
			this._menuOpenWeb.CID = CID.OpenWeb;
			// 
			// _menuRegistCode
			// 
			this._menuRegisterCode.Index = 1;
			this._menuRegisterCode.Text = "Contribution Certificate登録(&R)...";
			this._menuRegisterCode.Click += new System.EventHandler(this.OnMenu);
			this._menuRegisterCode.CID = CID.RegisterUserCode;
			// 
			// _menuBarHelp
			// 
			this._menuBarHelp.Index = 2;
			this._menuBarHelp.Text = "-";
			// 
			// _menuAboutBox
			// 
			this._menuAboutBox.Index = 3;
			this._menuAboutBox.Text = "バージョン情報(&A)...";
			this._menuAboutBox.Click += new System.EventHandler(this.OnMenu);
			this._menuAboutBox.CID = CID.ShowAboutBox;
#if ENABLE_SUPPORT_COMMAND																							 
			// 
			// _menuSupport
			// 
			this._menuSupport.Index = 5;
			this._menuSupport.MenuItems.AddRange(new ZMenuItem[] {
																							  this._menuRebuildIndex,
																							  this._menuCreateKey,
																							  this._menuReload,
																							  this._menuCreateCert,
																							  this._menuStatistics,
																							  this._menuShrinkData
																 });
			this._menuSupport.Text = "サポート";
			// 
			// _menuRebuildIndex
			// 
			this._menuRebuildIndex.Index = 0;
			this._menuRebuildIndex.Text = "インデックス再構築";
			this._menuRebuildIndex.Click += new System.EventHandler(this.OnMenu);
			this._menuRebuildIndex.CID = CID.SupRebuildIndex;
			// 
			// _menuCreateKey
			// 
			this._menuCreateKey.Index = 1;
			this._menuCreateKey.Text = "鍵の作成（既存の鍵は消える）";
			this._menuCreateKey.Click += new System.EventHandler(this.OnMenu);
			this._menuCreateKey.CID = CID.SupCreateKey;
			// 
			// _menuReload
			// 
			this._menuReload.Index = 2;
			this._menuReload.Text = "Reload";
			this._menuReload.Click += new System.EventHandler(this.OnMenu);
			this._menuReload.CID = CID.SupReload;
			// 
			// _menuCreateCert
			// 
			this._menuCreateCert.Index = 3;
			this._menuCreateCert.Text = "Certificate生成";
			this._menuCreateCert.Click += new System.EventHandler(this.OnMenu);
			this._menuCreateCert.CID = CID.SupCreateCert;
			// 
			// _menuStatistics
			// 
			this._menuStatistics.Index = 4;
			this._menuStatistics.Text = "Statistics";
			this._menuStatistics.Click += new System.EventHandler(this.OnMenu);
			this._menuStatistics.CID = CID.SupStatistics;
			// 
			// _menuShrinkData
			// 
			this._menuShrinkData.Index = 4;
			this._menuShrinkData.Text = "データ縮小";
			this._menuShrinkData.Click += new System.EventHandler(this.OnMenu);
			this._menuShrinkData.CID = CID.SupShrinkData;
#endif
			// 
			// _statusBar
			// 
			this._statusBar.Location = new System.Drawing.Point(0, 213);
			this._statusBar.Name = "_statusBar";
			this._statusBar.Size = new System.Drawing.Size(292, 22);
			this._statusBar.TabIndex = 0;
			this._statusBar.DoubleClick += new System.EventHandler(this.OnStatusBarDoubleClicked);
			// 
			// MainFrame
			// 
			this.Controls.Add(this._chartCanvas);
			this.Controls.Add(this._statusBar);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Menu = this._mainMenu;
			this.Name = "MainFrame";
			this.Text = Env.Constants.AppTitle;
			this.ImeMode = ImeMode.Disable;
			this.ResumeLayout(false);

            //hacked by rti
            this.RealTimeUpdater = new RealTimeUpdate.Main(this.Handle, WM_REAL_TIME_UPDATER_CHANGE);
            //-------------
		}
		#endregion

		public ChartCanvas ChartCanvas {
			get {
				return _chartCanvas;
			}
		}
		public BrandListPane CurrentBrandListPane {
			get {
				return _currentBrandListPane;
			}
			set {
				_currentBrandListPane = value;
			}
		}

		private void InitMenus() {
			//MRU
			InitMRUDownloadMenu();

			//指標を追加
			AddIndexMenu(BuiltInIndex.Nikkei225);
			AddIndexMenu(BuiltInIndex.TOPIX);
			AddIndexMenu(BuiltInIndex.JASDAQ);
			//AddIndexMenu(BuiltInIndex.LONGTERM_INTEREST); DreamVisor
			AddIndexMenu(BuiltInIndex.Nikkei225_F);
			AddIndexMenu(BuiltInIndex.TOPIX_F);
			Util.AddMenuBar(_menuIndices);
			AddIndexMenu(BuiltInIndex.JPYUSD);
			AddIndexMenu(BuiltInIndex.JPYEUR);
			Util.AddMenuBar(_menuIndices);		
			AddIndexMenu(BuiltInIndex.Dow);
			AddIndexMenu(BuiltInIndex.Nasdaq);
			AddIndexMenu(BuiltInIndex.SP500);
			Util.AddMenuBar(_menuIndices);
			MenuItem sectors = new ZMenuItem();
			sectors.Text = "業種別(&S)";
			_menuIndices.MenuItems.Add(sectors);
			for(int i=(int)BuiltInIndexGroup.SectorIndexStart; i<=(int)BuiltInIndexGroup.SectorIndexEnd; i++)
				AddBrandMenu(sectors, Env.BrandCollection.FindBrand(i));
#if DOJIMA
			for(int i=CommodityUtil.INDEX_START; i<=CommodityUtil.INDEX_END; i++)
				AddBrandMenu(_menuCommodities, Env.BrandCollection.FindBrand(i));
			_menuCommodities.Visible = true;
			_menuHalfDaily.Visible = true;
			_menuExportHalfDaily.Visible = true;
			_menuOpenWeb.Visible = false;
			_menuRegisterCode.Visible = false;
			_menuBarHelp.Visible = false;
#endif

			ArrayList derived = Env.BrandCollection.DerivedBrands;
			if(derived.Count>0) {
				foreach(DerivedBrand br in derived)
					AddBrandMenu(_menuDerivedBrand, br);
			}
		}
		private void AddIndexMenu(BuiltInIndex mi) {
			AbstractBrand br = Env.BrandCollection.FindBrand((int)mi);
			AddBrandMenu(_menuIndices, br);
		}
		private void AddBrandMenu(Menu parent, AbstractBrand br) {
			ZMenuItem menu = new ZMenuItem();
			menu.Text = String.Format("{0}:{1}", br.CodeAsString, br.Name.Replace("&", "&&"));
			menu.Index = _menuIndices.MenuItems.Count;
			menu.Click += new EventHandler(OnMenu);
			menu.CID = CID.ShowSpecifiedBrand;
			parent.MenuItems.Add(menu);
		}
		public void InitMRUDownloadMenu() {
			_menuMRUDownload.MenuItems.Clear();
			bool mru_found = false;
			for(int i=0; i<Env.Options.DownloadOrders.Count; i++) {
				DownloadOrder o = Env.Options.DownloadOrders[i];
				ZMenuItem mi = new ZMenuItem();
				mi.Text = String.Format("&{0} {1}", i+1, Env.Options.DownloadOrders.GetDescription(i));
				mi.Enabled = o!=null;
				mi.Click += new EventHandler(OnMRUDownloadOrder);
				_menuMRUDownload.MenuItems.Add(mi);
				mru_found = true;
			}
			_menuMRUDownload.Enabled = mru_found;
		}
		private void OnMRUDownloadOrder(object sender, EventArgs args) {
			DownloadOrder o = Env.Options.DownloadOrders[((MenuItem)sender).Index];
			Debug.Assert(o!=null);
			CommandExec.ShowDownloadDialog(o);
		}

		private void InitStatusBar() {
			StatusBarPanel mp = new StatusBarPanel();
			mp.Style = StatusBarPanelStyle.Text;
			mp.AutoSize = StatusBarPanelAutoSize.Spring;
			_statusBar.ShowPanels = true;
			_statusBar.Panels.Add(mp);
		}
		public void SetStatusBarText(string text, string tooltip) {
			_statusBar.Panels[0].Text = text;
			_statusBar.Panels[0].ToolTipText = tooltip;
		}
		private void OnStatusBarDoubleClicked(object sender, EventArgs args) {
			if(_modelessDialog!=null) {
				_modelessDialog.ZShow();
				SetStatusBarText("", "");
			}
		}

		public void AdjustShortcut() {
			foreach(MenuItem mi in this._mainMenu.MenuItems)
				AdjustShortcut(mi);
		}
		private void AdjustShortcut(MenuItem mi) {
			ZMenuItem zmi = mi as ZMenuItem;
			if(zmi!=null && zmi.CID!=CID.None)
				zmi.ShortcutKey = Env.Command.Get(zmi.CID).Shortcut;
			foreach(MenuItem ch in mi.MenuItems)
				AdjustShortcut(ch);
		}

		public bool IsBookmarkPaneVisible {
			get {
				return _bookmarkContent!=null && _bookmarkContent.Visible;
			}
		}

		private void OnMenu(object sender, EventArgs args) {
			try {
				if(_asyncLoader!=null) return; //ロード中はメニュー起動しない
				
				ZMenuItem mi = (ZMenuItem)sender;

				if(mi.CID==CID.ShowInputCode)
					CommandExec.PromptInputCode(true, '\0');
				else if(mi.CID==CID.ShowSpecifiedBrand)
					CommandExec.ShowBrand(Env.BrandCollection.FindBrand(MenuItemToCode(mi)));
				else if(mi.CID!=CID.None)
					Env.Command.Exec(mi.CID);
				else
					Util.Warning(this, "NOT IMPLEMENTED YET!");
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
			}
		}
		private void OnAdjustViewMenu(object sender, EventArgs args) {
			AdjustViewMenu(_menuView);
		}
		private void OnAdjustMoveMenu(object sender, EventArgs args) {
			_menuBack.Enabled = Env.BrandHistory.HasBack;
			_menuFront.Enabled = Env.BrandHistory.HasFront;
		}
		

		//本体とコンテキストメニューの両方があるので
		private void AdjustViewMenu(Menu parent) {
			parent.MenuItems[_menuBookmark.Index].Checked = (_bookmarkContent!=null && _bookmarkContent.Visible);
#if DOJIMA
			parent.MenuItems[_menuHalfDaily.Index].Checked = Env.CurrentIndicators.Format==ChartFormat.HalfDaily;
#endif
			parent.MenuItems[_menuDaily.Index].Checked = Env.CurrentIndicators.Format==ChartFormat.Daily;
			parent.MenuItems[_menuWeekly.Index].Checked = Env.CurrentIndicators.Format==ChartFormat.Weekly;
			parent.MenuItems[_menuMonthly.Index].Checked = Env.CurrentIndicators.Format==ChartFormat.Monthly;
			parent.MenuItems[_menuLinkToInfo.Index].Enabled = !_chartCanvas.GetBrand().IsBuiltIn;
            //hacked by rti
            parent.MenuItems[_menuRealTimeUpdate.Index].Checked = this.RealTimeUpdater.IsAlive();
            //--------------
		}
		private void OnAdjustFileMenu(object sender, EventArgs args) {
			_menuDownload.Enabled = _modelessDialog==null;
			_menuMRUDownload.Enabled = _modelessDialog==null;
		}

		public ContextMenu CreateContextMenu() {
			ContextMenu cm = new ContextMenu();
			foreach(ZMenuItem mi in _menuView.MenuItems) {
				MenuItem cl = mi.CloneMenu();
				cm.MenuItems.Add(cl);
			}
			AdjustViewMenu(cm);
			return cm;
		}

		private void FinishAsyncLoad(IntPtr lparam) {
			Debug.Assert(_asyncLoader!=null);

			InitMenus();
			//失敗の場合でもDockingManagerは必要
			_manager = new DockingManager(this, VisualStyle.IDE2005);
			_manager.ContentHidden += new Crownwood.DotNetMagic.Docking.DockingManager.ContentHandler(_manager_ContentHidden);
			_manager.ContentShown  += new Crownwood.DotNetMagic.Docking.DockingManager.ContentHandler(_manager_ContentShown);
			_manager.OuterControl = _statusBar;
			_manager.InnerControl = _chartCanvas;

			if(lparam.ToInt32()==AsyncConst.LPARAM_FINISHED) {
				Env.CurrentIndicators = _asyncLoader.IndicatorSet;
				Env.BrandCollection.ClearAllFarms();
				AbstractBrand br = _chartCanvas.GetBrand();
				_chartCanvas.ReloadFromPreference();
				_chartCanvas.LoadBrand(br, false);
				Invalidate(true);
			}
			else {
				Util.Warning(this, "スキーマのロード中にエラーが発生しました。\n"+_asyncLoader.ErrorMessage);
			}

			_asyncLoader = null;
			foreach(MenuItem mi in _mainMenu.MenuItems) mi.Enabled = true;
			this.Cursor = Cursors.Default;

			CommandExec.ResetLayout();

#if DOJIMA
			if(!Env.Options.DojimaPasswordVerified)
				new Dojima.PasswordDialog().ShowDialog(this);

#endif
		}


		protected override void OnActivated(EventArgs e) {
			base.OnActivated (e);
			if(_initialAction==null || _initialAction.Performed) {
				//if(!_chartCanvas.Focused) _chartCanvas.Focus();
				return;
			}

			_initialAction.Performed = true;

			//!!こういった処理がOnActivatedにあるのも変だが...
			foreach(MenuItem mi in _mainMenu.MenuItems) mi.Enabled = false;
			CommandExec.ShowBrand(Env.BrandCollection.FindBrand(_initialAction.BrandCode));
			this.Cursor = Cursors.AppStarting;
			_asyncLoader = new AsyncSchemaLoader(this);
			_asyncLoader.AsyncLoad();
		}
		protected override void OnClosing(CancelEventArgs e) {
			base.OnClosing (e);
			Env.Options.FrameLocation = new Rectangle(this.Location, this.Size);
			Env.Options.WindowState = this.WindowState;
			if(_modelessDialog!=null) _modelessDialog.ZAbort();
		}

		protected override bool ProcessDialogKey(Keys keyData) {
			//Debug.WriteLine("ProcessDialogKey(Frame)");
			return ProcessShortcut(keyData);
		}
		public bool IsPriorShortcutKey(Keys keyData) {
			return (Keys.D0<=keyData && keyData<=Keys.D9) || (Keys.NumPad0<=keyData && keyData<=Keys.NumPad9);
		}

		public bool ProcessShortcut(Keys keyData) {
			if(Env.Command.Exec(keyData)!=CommandResult.Ignored)
				return true;
			else
				return false;
		}

		protected override void OnMouseWheel(MouseEventArgs e) {
			Env.Command.ExecMouseWheel(e.Delta);
		}


		private int MenuItemToCode(object sender) {
			string t = ((MenuItem)sender).Text;
			return Int32.Parse(t.Substring(0, t.IndexOf(':')));
		}

		private void _manager_ContentHidden(Content c, EventArgs cea) {
			_chartCanvas.Focus();
			if(c.Control is BrandListPane)
				_currentBrandListPane = null;
		}
		private void _manager_ContentShown(Content c, EventArgs cea) {
			if(c.Control is BrandListPane)
				_currentBrandListPane = (BrandListPane)c.Control;
		}

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == AsyncConst.WM_ASYNCPROCESS)
                FinishAsyncLoad(m.LParam);
            if (m.Msg == WM_REAL_TIME_UPDATER_CHANGE)
                OnRealTimeUpdateEvent(m.LParam);
            //hacked by rti   秘密のルーチン ディフォではコンパイルされない。
#if  _RTI_ONLY
            if (m.Msg == Win32.WM_USER + 100)
                RTITEST((int)m.LParam);
#endif
            //--------------------------------------
        }
        //hacked by rti   秘密のルーチン ディフォではコンパイルされない。
#if  _RTI_ONLY
        private void RTITEST(int inParam)
        {
            Env.Command.Exec(CID.ReloadKit);
            //コマンドラインで指定されたことがあれば、ここで実行する
            CommandLine commandLine = new CommandLine();
            switch (inParam)
            {
                case 0:
                    commandLine.Run2();
                    break;
                case 1:
                    commandLine.Run3();
                    break;
                case 2:
                    commandLine.Run4();
                    break;
                default:
                    commandLine.Run2();
                    break;
            }
        }
#endif
        //株価に変更があった
        void OnRealTimeUpdateEvent(IntPtr lparam)
        {
            int code ;
            int date;
            NewDailyData td;
            this.RealTimeUpdater.GetKabuka(out code, out date, out td);

            //別のコードのやつを更新していないだろうな...
            AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
            if (br.Code != code)
            {
                return;
            }

            //渡された日のデータを更新

            DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(1);
            f.UpdateDataFarm(date, td);
            f.Save(Util.GetDailyDataFileName(br.Code));

            br.ReserveFarm().LoadFor(br);
            Env.Frame.ChartCanvas.LoadBrand(br, false);
            Env.Frame.ChartCanvas.MoveToLatest();
        }
        //---------------------------------------


	}

	internal interface IZModelessForm {
		void ZShow();
		void ZAbort();
	}
}
