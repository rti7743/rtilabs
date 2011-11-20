/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

using Travis.Storage;

using Zanetti.UI;
using Zanetti.Forms;
using Zanetti.Commands;
using Zanetti.Data;
using Zanetti.DataSource;
using Zanetti.Parser;
using Zanetti.Config;
using Zanetti.Arithmetic;
using Zanetti.Indicators;
using Zanetti.Indicators.Schema;


namespace Zanetti
{
	/// <summary>
	/// Env の概要の説明です。
	/// </summary>
	internal class Env {
		private static Preference _preference;
		private static Options _options;
		private static LayoutInfo _layoutInfo;
		private static Bookmark _bookmark;
		private static FreeLineCollection _freeLines;
		private static BrandCollection _brandCollection;
		private static ArithmeticLibrary _arithmeticLibrary;
		private static ZanettiSchema _schema;
		private static IndicatorSet _currentIndicatorSet;
		private static MainFrame _mainFrame;
		private static KitTestDialog _kitTestDialog;
		private static CommandCollection _command;
		private static BrandHistory _history;
		private static StorageNode _rootStorageNode;
		private static WeekFormat _weekFormat;
		private static ImageList _imageList16;

		public static Preference Preference {
			get {
				return _preference;
			}
		}
		public static Options Options {
			get {
				return _options;
			}
		}
		public static LayoutInfo Layout {
			get {
				return _layoutInfo;
			}
		}	 
		
		public static BrandCollection BrandCollection {
			get {
				return _brandCollection;
			}
		}
		public static ZanettiSchema Schema {
			get {
				return _schema;
			}
		}
		public static IndicatorSet CurrentIndicators {
			get {
				return _currentIndicatorSet;
			}
			set {
				_currentIndicatorSet = value;
			}
		}
		public static ArithmeticLibrary ArithmeticLibrary {
			get {
				return _arithmeticLibrary;
			}
		}
		public static MainFrame Frame {
			get {
				return _mainFrame;
			}
		}
		public static KitTestDialog KitTestDialog {
			get {
				return _kitTestDialog;
			}
			set {
				_kitTestDialog = value;
			}
		}
		public static CommandCollection Command {
			get {
				return _command;
			}
		}
		public static BrandHistory BrandHistory {
			get {
				return _history;
			}
		}
		public static Bookmark Bookmark {
			get {
				return _bookmark;
			}
		}
		public static FreeLineCollection FreeLines {
			get {
				return _freeLines;
			}
		}
		public static WeekFormat WeekFormat {
			get {
				return _weekFormat;
			}
		}

		//パース済み設定ファイルのルート　ネーミングはちとおかしいが
		public static StorageNode RootStorageNode {
			get {
				return _rootStorageNode;
			}
		}
		public static ImageList ImageList16 {
			get {
				if(_imageList16==null) {
					ImageListForm frm = new ImageListForm();
					_imageList16 = frm.ImageList16;
				}
				return _imageList16;
			}
		}

		[STAThread]
		public static void Main(string[] args) {
			try {
				InitEnv();
				Application.Run(_mainFrame);
				SaveEnv();
			}
			catch(Exception ex) {
				Util.ReportCriticalError(ex);
			}
		}
        //hacked by rti
        private static void InitEnv()
        {
            Application.EnableVisualStyles();

            string dir = GetAppDir() + "data";
            if (!Directory.Exists(dir))
                Directory.CreateDirectory(dir);

            ThemeUtil.Init();
            //こいつらは後で初期化する
            //_brandCollection = new BrandCollection();
            //_arithmeticLibrary = new ArithmeticLibrary();
            //_arithmeticLibrary.InitBuiltins();
            _schema = new ZanettiSchema();
            _options = new Options();
            _bookmark = new Bookmark();
            _freeLines = new FreeLineCollection();
            _weekFormat = new WeekFormat();
            _history = new BrandHistory();
            InitialAction act = new InitialAction();

            //_brandCollection.Load(GetAppDir() + "index.txt");

            _rootStorageNode = null;
            string option_file = GetAppDir() + "options.conf";
            if (!File.Exists(option_file))
            {
                _options.Init();
                _bookmark.Clear();
            }
            else
            {
                StreamReader reader = null;
                try
                {
                    reader = new StreamReader(option_file, Encoding.Default);
                    _rootStorageNode = new TextNodeReader(reader).Read().GetChildAt(0);
                    StorageNode options = _rootStorageNode.FindChildNode("options");
                    if (options == null)
                        _options.Init();
                    else
                        _options.Load(options);

                    StorageNode bookmark = _rootStorageNode.FindChildNode("bookmark-group");
                    if (bookmark == null)
                        _bookmark.Clear();
                    else
                        _bookmark.Load(bookmark);

                    _freeLines.Load(_rootStorageNode);
                }
                catch (Exception ex)
                {
                    act.AddErrorMessage("オプションファイルの読み込みに失敗しました。" + ex.Message);
                }
                finally
                {
                    if (reader != null) reader.Close();
                }
            }
            _brandCollection = new BrandCollection();
            _arithmeticLibrary = new ArithmeticLibrary();
            _arithmeticLibrary.InitBuiltins();

            _brandCollection.Load(GetAppDir() + "index.txt");


            _preference = new Preference(_rootStorageNode == null ? null : _rootStorageNode.FindChildNode("preference"));
            _command = new CommandCollection(_rootStorageNode == null ? null : _rootStorageNode.FindChildNode("command"));
            _currentIndicatorSet = new IndicatorSet(_options.ChartFormat); //最低限の内容で初期化
            _layoutInfo = new LayoutInfo();

            act.BrandCode = (int)BuiltInIndex.Nikkei225;

            _mainFrame = new MainFrame();
            _mainFrame.StartPosition = FormStartPosition.Manual;
            _mainFrame.InitialAction = act;
            _mainFrame.Size = Env.Options.FrameLocation.Size;
            _mainFrame.Location = Env.Options.FrameLocation.Location;
            _mainFrame.WindowState = _options.WindowState;
            _mainFrame.Init();

            //ここまできたら起動回数を１ふやす
            _options.LauchCount++;
        }
        /*
        private static void InitEnv() {
            Application.EnableVisualStyles();
           
            string dir = GetAppDir() + "data";
			if(!Directory.Exists(dir))
				Directory.CreateDirectory(dir);
			
			ThemeUtil.Init();
			_brandCollection = new BrandCollection();
			_arithmeticLibrary = new ArithmeticLibrary();
			_arithmeticLibrary.InitBuiltins();
			_schema = new ZanettiSchema();
			_options = new Options();
			_bookmark = new Bookmark();
			_freeLines = new FreeLineCollection();
			_weekFormat = new WeekFormat();
			_history = new BrandHistory();
			InitialAction act = new InitialAction();
			
			_brandCollection.Load(GetAppDir() + "index.txt");

			_rootStorageNode = null;
			string option_file = GetAppDir() + "options.conf";
			if(!File.Exists(option_file)) {
				_options.Init();
				_bookmark.Clear();
			}
			else {
				StreamReader reader = null;
				try {
					reader = new StreamReader(option_file, Encoding.Default);
					_rootStorageNode = new TextNodeReader(reader).Read().GetChildAt(0);
					StorageNode options = _rootStorageNode.FindChildNode("options");
					if(options==null)
						_options.Init();
					else
						_options.Load(options);

					StorageNode bookmark = _rootStorageNode.FindChildNode("bookmark-group");
					if(bookmark==null)
						_bookmark.Clear();
					else
						_bookmark.Load(bookmark);

					_freeLines.Load(_rootStorageNode);
				}
				catch(Exception ex) {
					act.AddErrorMessage("オプションファイルの読み込みに失敗しました。" + ex.Message);
				}
				finally {
					if(reader!=null) reader.Close();
				}
			}

			_preference = new Preference(_rootStorageNode==null? null : _rootStorageNode.FindChildNode("preference"));
			_command    = new CommandCollection(_rootStorageNode==null? null : _rootStorageNode.FindChildNode("command"));
			_currentIndicatorSet = new IndicatorSet(_options.ChartFormat); //最低限の内容で初期化
			_layoutInfo = new LayoutInfo();

			act.BrandCode = (int)BuiltInIndex.Nikkei225;

			_mainFrame = new MainFrame();
			_mainFrame.StartPosition = FormStartPosition.Manual;
			_mainFrame.InitialAction = act;
			_mainFrame.Size = Env.Options.FrameLocation.Size;
			_mainFrame.Location = Env.Options.FrameLocation.Location;
			_mainFrame.WindowState = _options.WindowState;
			_mainFrame.Init();
	
			//ここまできたら起動回数を１ふやす
			_options.LauchCount++;
		}
        */
        //------

		//Commandから呼ぶためのスキーマ再構成
		public static void ReloadSchema() {
			_schema = new ZanettiSchema();
			_schema.Load(GetAppDir() + "extension", _rootStorageNode==null? null : _rootStorageNode.FindChildNode("params"));
		}
		public static void ResetWithoutConfig() {
			_schema = new ZanettiSchema();
			_schema.Load(GetAppDir() + "extension", null);
			_preference = new Preference(null);
		}

		public static void SaveEnv() {
			string option_file = GetAppDir() + "options.conf";
			StreamWriter writer = null;
			try {
				writer = new StreamWriter(option_file, false, Encoding.Default);
				StorageNode root = new StorageNode();
				root.Name = "omega-chart-options";
				_options.SaveTo(root);
				_bookmark.SaveTo(root);
				_preference.SaveTo(root);
				_schema.SaveTo(root);
				_freeLines.SaveTo(root);
				new TextNodeWriter(writer).Write(root);
				writer.Close();
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				Util.Warning("オプションの保存に失敗しました。"+ex.Message);
			}
			finally {
				if(writer!=null) writer.Close();
			}
		}

		private static string _appDir;
		public static string GetAppDir() {
			if(_appDir==null)
				_appDir = AppDomain.CurrentDomain.BaseDirectory;
			return _appDir;
		}

		internal class Constants {
#if DOJIMA
			public const string AppTitle = Zanetti.Dojima.DojimaUtil.AppTitle;
#else
			public const string AppTitle = "Omega Chart";
#endif
			public const int LaunchCountForPrompt = 30;

			public const int MIN_CANDLE_WIDTH = 3;
			public const int MAX_CANDLE_WIDTH = 17;
		}
	}

	internal class InitialAction {
		private bool _indexConstructionRequired;
		private bool _performed;
		private string _message;
		private int _brandCode;
		private ArrayList _errorMessages = new ArrayList();

		public bool IndexConstructionRequired {
			get {
				return _indexConstructionRequired;
			}
		}
		public string Message {
			get {
				return _message;
			}
		}
		public IEnumerable ErrorMessages {
			get {
				return _errorMessages;
			}
		}


		public bool Performed {
			get {
				return _performed;
			}
			set {
				_performed = value;
			}
		}
		public int BrandCode {
			get {
				return  _brandCode;
			}
			set {
				_brandCode = value;
			}
		}
		
		public void SetIndexConstructionRequired(string msg) {
			_indexConstructionRequired = true;
			_message = msg;
		}
		public void AddErrorMessage(string value) {
			_errorMessages.Add(value);
		}
	}
}
