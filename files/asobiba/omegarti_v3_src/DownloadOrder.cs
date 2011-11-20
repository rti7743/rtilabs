/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using Travis.Storage;

using Zanetti.Data;

namespace Zanetti.DataSource
{
	//何をダウンロードするかの設定
	internal class DownloadOrder
	{
		public enum StockDownload {
			None,       //ダウンロードしない
			All,        //全部
			Bookmark,   //ブックマーク内の銘柄のみ
			Recent,     //最近の日付のみ
			SingleBrand //１銘柄
		}

		private string _name; //名前をつけていなければnull
		private StockDownload _stock;
		private bool _domesticIndices;
		private bool _foreignIndices;
		private bool _sectors;
		private bool _currencies;
		private bool _commodities;
        private bool _fundamental;
        private bool _isIgnoreObsolete;
		private int[] _dateArray; //_stock==Recentのみで有効
		private AbstractBrand _targetBrand; //_stock==SingleBrandでのみ有効 //!!このあたりはクラス階層を分離すべきかも

		public DownloadOrder() {
		}

		public string Name {
			get {
				return _name;
			}
			set {
				_name = value;
			}
		}
		public StockDownload Stock {
			get {
				return _stock;
			}
			set {
				_stock = value;
			}
		}
		public bool DomesticIndices {
			get {
				return _domesticIndices;
			}
			set {
				_domesticIndices = value;
			}
		}
		public bool ForeignIndices {
			get {
				return _foreignIndices;
			}
			set {
				_foreignIndices = value;
			}
		}
		public bool Sectors {
			get {
				return _sectors;
			}
			set {
				_sectors = value;
			}
		}
		public bool Currencies {
			get {
				return _currencies;
			}
			set {
				_currencies = value;
			}
		}
		public bool Commodities {
			get {
				return _commodities;
			}
			set {
				_commodities = value;
			}
		}
        //hacked by rti
        public bool Fundamental
        {
            get
            {
                return _fundamental;
            }
            set
            {
                _fundamental = value;
            }
        }
        public bool IsIgnoreObsolete
        {
            get
            {
                return _isIgnoreObsolete;
            }
            set
            {
                _isIgnoreObsolete = value;
            }
        }
        //--------------------
        public int[] DateArray
        {
			get {
				return _dateArray;
			}
			set {
				_dateArray = value;
			}
		}
		public AbstractBrand TargetBrand {
			get {
				return _targetBrand;
			}
			set {
				_targetBrand = value;
			}
		}
		public CombinedDataSource ConvertToDataSource() {
			//これらでオプションからダウンロード元を適当に選定してもよい。
			ArrayList col = new ArrayList();
#if BUILD_INITIAL_DATA
			//チェックボックスとは関係なくダウンロード
			AddDreamVisorDomesticIndicesDataSources(col);
			AddDreamVisorForeignIndicesDataSources(col);
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.SectorsEnumerator()));
			AddDreamVisorCurrenciesDataSources(col);
			col.Add(new Specialized.KenMilleDataSource(new CodeEnumerator.AllStockEnumerator()));
#else //通常版
            if (_domesticIndices) AddDomesticIndicesDataSources(col);
			if(_foreignIndices) AddForeignIndicesDataSources(col);

			if(_sectors)    col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.SectorsEnumerator()));
			if(_currencies) AddCurrenciesDataSources(col);
#if DOJIMA
			if(_commodities) col.Add(new Specialized.YutakaDataSource(new CodeEnumerator.CommodityEnumerator()));
#endif
			//ダウンロードの残り時間を自然に見せるため、国内株式は最後にする
			SingleDataSource stock_ds = null;
			switch(_stock) {
				case StockDownload.SingleBrand:
#if DOJIMA
					if(_targetBrand.IsCommodity)
						stock_ds = new Specialized.YutakaDataSource(new CodeEnumerator.Single(_targetBrand));
					else
#endif
						throw new NotSupportedException("単一銘柄のダウンロードはサポートされていません。");
				case StockDownload.All:
					throw new NotSupportedException("全銘柄一括ダウンロードはサポートされていません。");
				case StockDownload.Bookmark:
					throw new NotSupportedException("ブックマーク内のダウンロードはサポートされていません。");
				case StockDownload.Recent: {
                    /*
                    Specialized.MujinzouDataSource m = new Specialized.MujinzouDataSource(_dateArray); 
					m.IncludesDomesticIndices = true; //最近の銘柄で国内指数はカバーする
                    //hacked by rti
                    //業績情報のダウンロードの是非
                    m.IsFundamentalDownload = this._fundamental;
                    //-------------------------
                    */
                    //Yahoo!からダウンロードする
                    Specialized.YahooDataSource m = new Specialized.YahooDataSource(_dateArray);
                    m.IsIgnoreObsolete = this._isIgnoreObsolete;

					stock_ds = m;
					break;
					}
			}
			if(stock_ds!=null) col.Add(stock_ds);
#endif //BUILD_INITIAL_DATA

			return new CombinedDataSource((SingleDataSource[])col.ToArray(typeof(SingleDataSource)));
		}

		//株価指数
		private void AddDomesticIndicesDataSources(ArrayList col) {
			//国内指数は、日経平均の出来高の無尽蔵との整合性を優先してDREAMVISORを使う。
			//DREAMVISORが指定されていないときは日々の更新のみ
#if DREAMVISOR
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.DomesticIndicesEnumerator()));
#endif
		}
		private void AddForeignIndicesDataSources(ArrayList col) {
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.ForeignIndicesEnumerator()));
		}
		private void AddCurrenciesDataSources(ArrayList col) {
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.CurrenciesEnumerator()));
		}
		private void AddDreamVisorDomesticIndicesDataSources(ArrayList col) {
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.DomesticIndicesEnumerator()));
		}
		private void AddDreamVisorForeignIndicesDataSources(ArrayList col) {
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.ForeignIndicesEnumerator()));
		}
		private void AddDreamVisorCurrenciesDataSources(ArrayList col) {
			col.Add(new Specialized.DreamVisorDataSource(new CodeEnumerator.CurrenciesEnumerator()));
		}
		public void Load(StorageNode node) {
			_name = node["name"];
			_stock = (StockDownload)Enum.Parse(typeof(StockDownload), node.GetValue("stock", "All"));
			_domesticIndices = Util.ParseBool(node["domesticIndices"], true);
			_foreignIndices = Util.ParseBool(node["foreignIndices"], true);
			_sectors = Util.ParseBool(node["sectors"], false);
			_currencies = Util.ParseBool(node["currencies"], false);
			_commodities = Util.ParseBool(node["commodities"], false);
		}
		public StorageNode SaveAsNewNode() {
			StorageNode node = new StorageNode();
			if(_name!=null) node["name"] = _name;
			node["stock"] = _stock.ToString();
			node["domesticIndices"] = _domesticIndices.ToString();
			node["foreignIndices"] = _foreignIndices.ToString();
			node["sectors"] = _sectors.ToString();
			node["currencies"] = _currencies.ToString();
			node["commodities"] = _commodities.ToString();
			return node;
		}

		public static DownloadOrder CreateDefault() {
			DownloadOrder r = new DownloadOrder();
			r.Sectors = false;
			r.DomesticIndices = true;
			r.ForeignIndices = true;
			r.Currencies = true;
			r.Commodities = false;
            //hacked by rti 上場廃止銘柄は無視
            r.IsIgnoreObsolete = true;
            //--------
			r.Stock = StockDownload.All;
			return r;
		}
	}

	//所定の個数は保存できるようにする
	internal class DownloadOrderCollection {
		private DownloadOrder[] _data;

		public DownloadOrderCollection() {
			_data = new DownloadOrder[3];
		}
		public DownloadOrder this[int i] {
			get {
				return _data[i];
			}
			set {
				_data[i] = value;
			}
		}
		public int Count {
			get {
				return _data.Length;
			}
		}
		public string GetDescription(int i) {
			DownloadOrder o = _data[i];
			return o==null? "未定義" : o.Name==null? "名前なし" : o.Name;
		}

		public void SaveTo(StorageNode parent) {
			for(int i=0; i<_data.Length; i++) {
				DownloadOrder o = _data[i];
				if(o!=null) {
					StorageNode ch = o.SaveAsNewNode();
					ch.Name = "download-order";
					parent.AddChild(ch);
				}
			}
		}
		public void Load(StorageNode node) {
			int i = 0;
			foreach(StorageNode ch in node.Children) {
				if(ch.Name=="download-order") {
					DownloadOrder o = new DownloadOrder();
					o.Load(ch);
					_data[i++] = o;
				}
			}
		}
	}
}
