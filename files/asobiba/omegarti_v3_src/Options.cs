/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Zanetti.SystemTrading.Screening;
using Zanetti.DataSource;
using Travis.Storage;

namespace Zanetti.Config
{

    internal enum DataFeedAction {
		None,
		FullDownload,
		DailyDownload
	}


	//各種オプション設定（プロテクトの必要ないもの）を書き出す
	internal class Options {
		private Rectangle _frameLocation;
		private FormWindowState _windowState;
		private bool _showsBookmark;
		private ChartFormat _chartFormat;
		private DataFeedAction _dataFeedAction;
		private DownloadOrderCollection _downloadOrders;
		private KeyConfig _keyConfig;
        private ProxyConfig _proxyConfig;
        private string _rawCertString;
		private int _launchCount;
		//以下はserializeの必要はない
		private ScreeningOrder _prevScreeningOrder;
		private SimpleUserKey _certificate;

		//ケンミレからのダウンロードキー
		private int _kenmilleKey;

        //無尽蔵で使用するサイトが過去データかどうか
        private bool _mujinzou_uses_kakodata;

        //hacked by rti
        private int _CacheCapacity;
        //---------

        public Options() {
			_showsBookmark = false;
			_downloadOrders = new DownloadOrderCollection();
			_keyConfig = new KeyConfig();
            _proxyConfig = new ProxyConfig();
        }
		public void Init() {
			_windowState = FormWindowState.Normal;
			_chartFormat = ChartFormat.Daily;
			Rectangle r = Screen.PrimaryScreen.Bounds;
			_frameLocation = new Rectangle(r.Width/6, r.Height/6, r.Width*2/3, r.Height*2/3);
			_dataFeedAction = DataFeedAction.FullDownload;
			_keyConfig.Reset();
		}
		public DownloadOrderCollection DownloadOrders {
			get {
				return _downloadOrders;
			}
		}
		public KeyConfig KeyConfig {
			get {
				return _keyConfig;
			}
			set {
				_keyConfig = value;
			}
		}
        public ProxyConfig ProxyConfig {
            get {
                return _proxyConfig;
            }
            set {
                _proxyConfig = value;
            }
        }

        public int LauchCount {
			get {
				return _launchCount;
			}
			set {
				_launchCount = value;
			}
		}

		public Rectangle FrameLocation {
			get {
				return _frameLocation;
			}
			set {
				_frameLocation = value;
			}
		}
		public FormWindowState WindowState {
			get {
				return _windowState;
			}
			set {
				_windowState = value;
			}
		}
		public DataFeedAction DataFeedAction {
			get {
				return _dataFeedAction;
			}
			set {
				_dataFeedAction = value;
			}
		}
		public bool ShowsBookmark {
			get {
				return _showsBookmark;
			}
			set {
				_showsBookmark = value;
			}
		}
		public ChartFormat ChartFormat {
			get {
				return _chartFormat;
			}
			set {
				_chartFormat = value;
			}
		}
		public ScreeningOrder PrevScreeningOrder {
			get {
				return _prevScreeningOrder;
			}
			set {
				_prevScreeningOrder = value;
			}
		}
		public string RawCertString {
			get {
				return _rawCertString;
			}
			set {
				_rawCertString = value;
			}
		}
		public SimpleUserKey Certificate {
			get {
				if(_certificate==null) _certificate = SimpleUserKey.Load(_rawCertString);
				return _certificate;
			}
			set {
				_certificate = value;
			}
		}
		public int KenmilleKey {
			get {
				return _kenmilleKey;
			}
		}
        public bool MujinzouUsesKakoData
        {
            get
            {
                return _mujinzou_uses_kakodata;
            }
        }
        //hacked by rti
        //銘柄キャッシュの数
        public int CacheCapacity
        {
            get
            {
                return _CacheCapacity;
            }
        }
        //----


        public void SaveTo(StorageNode parent) {
			StorageNode node = new StorageNode();
			node.Name = "options";
			node["window-state"] = _windowState.ToString();
			if(_windowState==FormWindowState.Normal) {
				node["frameX"] = _frameLocation.X.ToString();
				node["frameY"] = _frameLocation.Y.ToString();
				node["frameW"] = _frameLocation.Width.ToString();
				node["frameH"] = _frameLocation.Height.ToString();
			}
			node["chart-format"] = _chartFormat.ToString();
			node["data-feed-action"] = _dataFeedAction.ToString();
			if(_rawCertString!=null) node["user-code"] = _rawCertString;
			node["launch-count"] = _launchCount.ToString();
			node["kenmille-key"] = _kenmilleKey.ToString("X");
            node["mujinzou-uses-kakodata"] = _mujinzou_uses_kakodata.ToString();
            //hacked by rti
            node["cache-capacity"] = _CacheCapacity.ToString();
            //-------
#if DOJIMA
			node["dojima-password-verified"] = _dojimaPasswordVerified.ToString();
#endif

			_downloadOrders.SaveTo(node);
			_keyConfig.SaveTo(node);
            _proxyConfig.SaveTo(node);
            parent.AddChild(node);
		}

		public void Load(StorageNode sec) {
			_windowState = ParseWindowState(sec["window-state"]);
			if(_windowState==FormWindowState.Normal) {
				_frameLocation.X = ParseInt(sec["frameX"], 0);
				_frameLocation.Y = ParseInt(sec["frameY"], 0);
				_frameLocation.Width = ParseInt(sec["frameW"], 0);
				_frameLocation.Height = ParseInt(sec["frameH"], 0);
			}
			else {
				if(_windowState==FormWindowState.Minimized) _windowState = FormWindowState.Normal;
				Rectangle r = Screen.PrimaryScreen.Bounds;
				_frameLocation = new Rectangle(r.Width/6, r.Height/6, r.Width*2/3, r.Height*2/3);
			}
			_chartFormat = ParseChartFormat(sec["chart-format"]);
			_dataFeedAction = ParseDataFeedAction(sec["data-feed-action"]);
			_rawCertString = sec["user-code"];
			_launchCount = ParseInt(sec["launch-count"], 0);
			_downloadOrders.Load(sec);
			_keyConfig.Load(sec);
            _proxyConfig.Load(sec);
			//このキーがケンミレからのダウンロードでは必要になる。
			//別途調べたキーの値をoptions.confファイルに書き、OmegaChartを起動するとここで読み込まれる。
			//2005年11月時点ではこの方法でOKであるが、いつまで有効かはわからない。
			//キーを調べるためのヒント:
			// * まず、何でもよいのでJavaのデコンパイラを用意する。
			// * ケンミレのチャート表示JavaAppletを構成するファイルのうち、ChForbidden.classの中をデコンパイラで表示する
			// * その中にキーが書いてある。どれがキーかはKenmille.csファイルのコードを見れば分かるであろう。
			//
			//ついでにケンミレへのコメント：
			//　俺が最も恐れていたのは、チャートを見るにはユーザ登録を必須にして、ちゃんと認証するようにWebサイトを変えられるか、
			//  Flashなどコード解析がより困難なプラットフォームにチャートのアプリが変更されることだった。
			//  今回はそのどちらでもなかったので、正直安心した。
			_kenmilleKey = Util.ParseHexInt(sec["kenmille-key"], 0);

            _mujinzou_uses_kakodata = Util.ParseBool(sec["mujinzou-uses-kakodata"], false);
            _CacheCapacity = Util.ParseInt(sec["cache-capacity"], 100);
#if DOJIMA
			_dojimaPasswordVerified = Util.ParseBool(sec["dojima-password-verified"], false);
#endif
		}

		private static FormWindowState ParseWindowState(string t) {
			if(t=="Minimized")
				return FormWindowState.Minimized;
			else if(t=="Maximized")
				return FormWindowState.Maximized;
			else
				return FormWindowState.Normal;
		}

		private static int ParseInt(string value, int def) {
			try {
				if(value.Length==0) return def;
				return Int32.Parse(value);
			}
			catch(Exception) {
				return def;
			}
		}
		private static float ParseFloat(string value, float def) {
			try {
				if(value.Length==0) return def;
				return Single.Parse(value);
			}
			catch(Exception) {
				return def;
			}
		}
		private static ChartFormat ParseChartFormat(string value) {
			if(value=="Monthly")
				return ChartFormat.Monthly;
			else if(value=="Weekly")
				return ChartFormat.Weekly;
			else // daily
				return ChartFormat.Daily;
		}
		private static DataFeedAction ParseDataFeedAction(string value) {
			if(value=="None")
				return DataFeedAction.None;
			else if(value=="DailyDownload")
				return DataFeedAction.DailyDownload;
			else
				return DataFeedAction.FullDownload;
		}

#if DOJIMA
		private bool _dojimaPasswordVerified;
		public bool DojimaPasswordVerified {
			get {
				return _dojimaPasswordVerified;
			}
			set {
				_dojimaPasswordVerified = value;
			}
		}
#endif
	}

    internal class ProxyConfig {
        private bool _useIESetting;
        private string _address;
        private int _port;
        public ProxyConfig() {
            _useIESetting = true;
        }

        public bool UseIESetting {
            get {
                return _useIESetting;
            }
            set {
                _useIESetting = value;
            }
        }
        public string Address {
            get {
                return _address;
            }
            set {
                _address = value;
            }
        }
        public int Port {
            get {
                return _port;
            }
            set {
                _port = value;
            }
        }

        public void SaveTo(StorageNode node) {
            node["use-ie-proxy"] = _useIESetting.ToString();
            node["proxy-address"] = _address;
            node["proxy-port"] = _port.ToString();
        }
        public void Load(StorageNode node) {
            _useIESetting = Util.ParseBool(node["use-ie-proxy"], true);
            _address = node["proxy-address"];
            _port = Util.ParseInt(node["proxy-port"], 80);
        }
    }


}
