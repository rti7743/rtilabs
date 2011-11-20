/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Text;
using System.Reflection;

using Travis.Util;

using Zanetti.Arithmetic;
using Zanetti.Parser;
 
namespace Zanetti.Data
{
	[EnumDesc(typeof(MarketType))]
	internal enum MarketType {
		[EnumValue(Description="指数")] B, //指数、為替など
		[EnumValue(Description="東証１部")] T1,
		[EnumValue(Description="東証２部")] T2,
		[EnumValue(Description="大証１部")] O1,
		[EnumValue(Description="大証２部")] O2,
		[EnumValue(Description="JASDAQ")] J,
		[EnumValue(Description="マザーズ")] M,
		[EnumValue(Description="ヘラクレス")] H,
        [EnumValue(Description="派生銘柄")] Custom,
        [EnumValue(Description="商品")] C //当面堂島バージョンでのみ必要
    }

	[EnumDesc(typeof(BuiltInIndex))]
	internal enum BuiltInIndex {
		//国内指数
		[EnumValue(Description="日経平均")] Nikkei225 = 101,
		[EnumValue(Description="TOPIX")]    TOPIX = 102,
		[EnumValue(Description="長期金利")] LONGTERM_INTEREST = 0, //これはDreamVisorからしか取得できないのでヤメ。値をレンジ外にしている
		[EnumValue(Description="JASDAQ")]       JASDAQ = 106,
		//株価指数先物
		[EnumValue(Description="日経平均先物")] Nikkei225_F = 151,
		[EnumValue(Description="TOPIX先物")]    TOPIX_F = 152,
		//為替
		[EnumValue(Description="円／ドル")] JPYUSD = 201,
		[EnumValue(Description="円／ユーロ")] JPYEUR = 202,
		//外国株式指数
		[EnumValue(Description="NYダウ")] Dow = 301,
		[EnumValue(Description="NASDAQ")] Nasdaq = 302,
		[EnumValue(Description="SP500")] SP500 = 303,
		//[EnumValue(Description="FT100")] FT100 = 304, //ケンミレ事件のためこの２つはあきらめる
		//[EnumValue(Description="DAX30")] DAX30 = 305,
		//業種別
		[EnumValue(Description="水産")]      SI_FISHING = 501, //派生銘柄は400番台にしてしまったので
		[EnumValue(Description="鉱業")]      SI_MINING, 
		[EnumValue(Description="建設")]      SI_CONSTRUCTION,
		[EnumValue(Description="食品")]      SI_FOOD,

		[EnumValue(Description="繊維")]      SI_FABRIC,
		[EnumValue(Description="パルプ")]    SI_PULP ,
		[EnumValue(Description="化学")]      SI_CHEMICAL,
		[EnumValue(Description="医薬")]      SI_MEDICAL,

		[EnumValue(Description="石油")]      SI_OIL,
		[EnumValue(Description="ゴム")]      SI_RUBBER,
		[EnumValue(Description="ガラス")]    SI_CERAMIC,
		[EnumValue(Description="鉄鋼")]      SI_IRON,
		
		[EnumValue(Description="非鉄")]      SI_NONEMETAL,
		[EnumValue(Description="金属")]      SI_METAL,
		[EnumValue(Description="機械")]      SI_MACHINE ,
		[EnumValue(Description="電気")]      SI_ELECTRIC,
		
		[EnumValue(Description="輸送")]      SI_FREIGHTER_MACHINE,
		[EnumValue(Description="精密")]      SI_PRECISIONS,
		[EnumValue(Description="製造")]      SI_MANIFUCTURING,
		[EnumValue(Description="ガス")]      SI_GAS,
		
		[EnumValue(Description="陸運")]      SI_LANDTRANSPORT,
		[EnumValue(Description="海運")]      SI_MARINETRANSPORT,
		[EnumValue(Description="空運")]      SI_AIRTRANSPORT,
		[EnumValue(Description="倉庫")]      SI_STORAGE,

		[EnumValue(Description="情報")]      SI_COMMUNICATION,
		[EnumValue(Description="卸売")]      SI_TRADEFARM,
		[EnumValue(Description="小売")]      SI_RETAIL,
		[EnumValue(Description="銀行")]      SI_BANK,

		[EnumValue(Description="証券")]      SI_SECURITY,
		[EnumValue(Description="保険")]      SI_INSUARANCE,
		[EnumValue(Description="金融")]      SI_FINANCE,
		[EnumValue(Description="不動産")]    SI_REALESTATES,

		[EnumValue(Description="サービス")]  SI_SERVICE,

        //600台は商品
        //COMMOITY_START = 600
	}
	internal enum BuiltInIndexGroup {
		DomesticIndexStart = BuiltInIndex.Nikkei225,
		DomesticIndexEnd   = BuiltInIndex.TOPIX_F,
		ForeignIndexStart  = BuiltInIndex.JPYUSD, //属性としてはほぼ一緒
		ForeignIndexEnd    = BuiltInIndex.SP500,
		SectorIndexStart   = BuiltInIndex.SI_FISHING,
		SectorIndexEnd     = BuiltInIndex.SI_SERVICE
	}




	//分割情報
	internal class SplitInfo {
		private int _date;
		private double _ratio;

		public int Date {
			get {
				return _date;
			}
		}
		public double Ratio {
			get {
				return _ratio;
			}
		}
		public SplitInfo(int date, double ratio) {
			_date = date;
			_ratio = ratio;
		}
		public SplitInfo(string txt) {
			//S:ddd:valueからの構築
			string[] t = txt.Split(':');
			_date = Int32.Parse(t[1]);
			_ratio = Double.Parse(t[2]);
		}
	}

	//銘柄の基本情報
	internal abstract class AbstractBrand {
		protected string _name;
		protected int _code;
		protected MarketType _market;
		protected int _referenceValue;
		protected SplitInfo[] _splitInfo;


		public AbstractBrand(string name, int code, MarketType m) {
			_name = name;
			_code = code;
			_market = m;
		}
		public string Name {
			get {
				return _name;
			}
		}
		public int Code {
			get {
				return _code;
			}
		}
		public string CodeAsString {
			get {
				string t = _code.ToString();
				Debug.Assert(t.Length<=4);
				if(t.Length<4) {
					StringBuilder bld = new StringBuilder(4);
					for(int i=0; i<4-t.Length; i++) bld.Append('0');
					bld.Append(t);
					return bld.ToString();
				}
				else
					return t;
			}
		}
		public SplitInfo[] SplitInfo {
			get {
				return _splitInfo;
			}
			set {
				_splitInfo = value;
			}
		}
		public MarketType Market {
			get {
				return _market;
			}
		}

		public bool IsBuiltIn { 
			get {
				return _code<1000;
			}
		}
		public bool IsSectorIndex {
			get {
				return (int)BuiltInIndexGroup.SectorIndexStart<=_code && _code<=(int)BuiltInIndexGroup.SectorIndexEnd;
			}
		}
		public bool IsDomesticIndex {
			get {
				return (int)BuiltInIndexGroup.DomesticIndexStart<=_code && _code<=(int)BuiltInIndexGroup.DomesticIndexEnd;
			}
		}
		public bool IsForeignIndex {
			get {
				return (int)BuiltInIndexGroup.ForeignIndexStart<=_code && _code<=(int)BuiltInIndexGroup.ForeignIndexEnd;
			}
		}
		public bool IsCommodity {
			get {
				//他との統一性では_codeで分類すべきだが、定数がDOJIMAディレクトリにあって面倒くさい
				return _market==MarketType.C;
			}
		}
		public bool IsIndexFuture {
			get {
				return (int)BuiltInIndex.Nikkei225_F==_code || (int)BuiltInIndex.TOPIX_F==_code;
			}
		}

		public virtual bool IsVolumeAvailable {
			get {
				if(!IsBuiltIn) //多くはこれだ
					return true;
				else if(IsDomesticIndex)
					return _code==(int)BuiltInIndex.Nikkei225 || _code==(int)BuiltInIndex.TOPIX || _code==(int)BuiltInIndex.JASDAQ ||
						_code==(int)BuiltInIndex.Nikkei225_F || _code==(int)BuiltInIndex.TOPIX_F;
				else if(this.IsForeignIndex)
					return _code!=(int)BuiltInIndex.JPYUSD && _code!=(int)BuiltInIndex.JPYEUR;
				else if(this.IsSectorIndex)
					return false;
				else
					return true;

			}
		}


		//保存されているデータから実数への変換に何をかければよいか
		public virtual double PriceScale {
			get {
				if(!IsBuiltIn) //多くはこれだ
					return 1;
				else if(IsDomesticIndex) {
					if(_code==(int)BuiltInIndex.Nikkei225_F)
						return 1;
					else if(_code==(int)BuiltInIndex.LONGTERM_INTEREST)
						return 0.001;
					else if(_code==(int)BuiltInIndex.TOPIX_F)
						return 0.1;
					else
						return 0.01;
				}
				else if(this.IsForeignIndex || this.IsSectorIndex) {
					return 0.01;
				}
				else if(IsCommodity)
					return 1;
				else
					return 1;
			}
		}
		public virtual string PriceFormatString {
			get {
				if(!IsBuiltIn) //多くはこれだ
					return "F0";
				else if(IsDomesticIndex) {
					if(_code==(int)BuiltInIndex.Nikkei225_F)
						return "F0";
					else if(_code==(int)BuiltInIndex.LONGTERM_INTEREST)
						return "F3";
					else
						return "F2";
				}
				else if(this.IsForeignIndex || this.IsSectorIndex)
					return "F2";
				else if(IsCommodity)
					return "F0";
				else
					return "F0";
			}
		}
		public int ReferenceValue {
			get {
				return _referenceValue;
			}
			set {
				_referenceValue = value;
			}
		}
		public bool HasFarm {
			get {
				return Env.BrandCollection.HasFarm(this);
			}
		}

		//dateの日付の１株はbasisの日に何倍になっているかを分割データから計算
		public double CalcSplitRatio(int date, int basis) {
			if(_splitInfo==null) return 1;
			double r = 1;
			for(int i=_splitInfo.Length-1; i>=0; i--) {
				SplitInfo si = _splitInfo[i];
				if(si.Date>basis) continue; 
				if(date < si.Date) r *= si.Ratio;
				else break;
			}
			return r;
		}

		public abstract DataFarm ReserveFarm();
        //hacked by rti
        public abstract DataFarm ReserveFarm(ChartFormat format);
        //--------
		public abstract DataFarm CreateDailyFarm(int extra_dates);
        //!!以下２つって両方必要？
        //hacked by rti
		//protected abstract DataFarm CreateFarm(ChartFormat format);
        //		public abstract DataFarm CreateFarm(ChartFormat format);

        public abstract DataFarm CloneFarm(ChartFormat format);
        //-----

	}

	//個別の１銘柄
	internal class BasicBrand : AbstractBrand {
		private int  _unit; //単位株数 これが当てはまらない銘柄では０
		private bool _nikkei225;
		private bool _active500;
		private bool _obsolete; //上場廃止

		public BasicBrand(string name, int code, MarketType m) : base(name, code, m) {
		}

		public bool Nikkei225 {
			get {
				return _nikkei225;
			}
			set {
				_nikkei225 = value;
			}
		}
		public bool Active500 {
			get {
				return _active500;
			}
			set {
				_active500 = value;
			}
		}
		public bool Obsolete {
			get {
				return _obsolete;
			}
			set {
				_obsolete = value;
			}
		}
		public int Unit {
			get {
				return _unit;
			}
			set {
				_unit = value;
			}
		}

		public override DataFarm ReserveFarm() {
			return Env.BrandCollection.ReserveFarm(this);
		}
        //hacked by rti
        public override DataFarm ReserveFarm(ChartFormat format)
        {
            return Env.BrandCollection.ReserveFarm(this , format);
        }
        //--------

		public override DataFarm CreateDailyFarm(int extra_dates) {
			return Env.BrandCollection.CreateDailyFarm(this, extra_dates);
		}
        //hacked by rti
//        protected override DataFarm CreateFarm(ChartFormat format) {
//        	return Env.BrandCollection.CreateFarm(this, format);
//        }
        //public override DataFarm CreateFarm(ChartFormat format) {
        //	return Env.BrandCollection.CreateFarm(this, format);
        //}
        public override DataFarm CloneFarm(ChartFormat format)
        {
            return Env.BrandCollection.CloneFarm(this, format);
        }
        //

	}

	internal class DerivedBrand : AbstractBrand {
		protected Expression _expression;
		protected string _rawExpression;
		protected AbstractBrand[] _dependencies;
		protected double _priceScale;
		protected string _formatString;

		public DerivedBrand(AbstractBrand[] dep, string expr, string name, int code, MarketType m, double pricescale, string formatstring) : base(name, code, m) {
			_rawExpression = expr;
			_dependencies = dep;
			_priceScale = pricescale;
			_formatString = formatstring;
		}
		public override double PriceScale {
			get {
				return _priceScale;
			}
		}
		public override string PriceFormatString {
			get {
				return _formatString;
			}
		}
		public Expression Expression {
			get {
				if(_expression==null) {
					ExpressionConstructor ee = new ExpressionConstructor();
					new ZPredicationParser(new StringReader(_rawExpression), ee).Parse();
					_expression = ee.Result;
				}
				return _expression;
			}
		}
		public AbstractBrand[] Dependencies {
			get {
				return _dependencies;
			}
		}

		public override DataFarm ReserveFarm() {
			return Env.BrandCollection.ReserveFarm(this);
		}
        //hacked by rti
        public override DataFarm ReserveFarm(ChartFormat format)
        {
            return Env.BrandCollection.ReserveFarm(this, format);
        }
        //------
        public override DataFarm CreateDailyFarm(int extra_dates)
        {
			return Env.BrandCollection.CreateDailyFarm(this, extra_dates);
		}
        //hacked by rti
//        protected override DataFarm CreateFarm(ChartFormat format) {
//        	return Env.BrandCollection.CreateFarm(this, format);
//        }
        //public override DataFarm CreateFarm(ChartFormat format) {
		//	return Env.BrandCollection.CreateFarm(this, format);
		//}
        public override DataFarm CloneFarm(ChartFormat format)
        {
            return Env.BrandCollection.CloneFarm(this, format);
        }

        //-------
		public override bool IsVolumeAvailable {
			get {
				return false;
			}
		}

	}
//hacked by rti
//キャッシュがいかされていないので、スクラップ・アンド・<S>スクラップ</S>ビルドする
    internal class BrandCollection
    {
        private Hashtable _data; //codeの値からAbstractBrandへ
        private Hashtable _farms; //AbstractBrandからFarmへ　Farmの個数を制限する機構はまだ

        private int _lastUpdatedDate;

        private int _stockBrandCount;
        private ArrayList _derivedBrands;

        //生きているDataFarmの数がfarmCapacityを超えないようにする
        private int _farmCapacity;
        private int _nextRefCount;

        public BrandCollection()
        {
            _data = new Hashtable();
            _farms = new Hashtable();
            _derivedBrands = new ArrayList();

            _farmCapacity = Env.Options.CacheCapacity;
            _nextRefCount = 1;
        }
        public int TotalCount
        {
            get
            {
                return _data.Count;
            }
        }
        //個別銘柄の数
        public int StockBrandCount
        {
            get
            {
                return _stockBrandCount;
            }
        }
        public int DerivedBrandCount
        {
            get
            {
                return _derivedBrands.Count;
            }
        }
        public int LastUpdatedDate
        {
            get
            {
                return _lastUpdatedDate;
            }
        }

        public void Load(string filename)
        {
            //indexファイルから読んで構築
            //!!バージョンチェック必要
            StreamReader sr = new StreamReader(filename, Encoding.Default);
            string line = sr.ReadLine();
            ReadHeader(line);
            line = sr.ReadLine();

            int n = 1;
            ArrayList splits = new ArrayList();
            _stockBrandCount = 0;

            _data.Clear();
            _farms.Clear();

            LoadBuiltIns();

            while (line != null)
            {
                string[] d = line.Split(',');
                if (d.Length < 4) throw new IOException("index file format error: line" + n);
                try
                {
                    int code = Int32.Parse(d[0]);
                    BasicBrand br = new BasicBrand(d[1], code, (MarketType)Enum.Parse(typeof(MarketType), d[2], true));
                    br.Unit = Int32.Parse(d[3]);
                    _data[code] = br;
                    _stockBrandCount++;

                    for (int i = 4; i < d.Length; i++)
                    {
                        string col = d[i];
                        if (col == "N225") br.Nikkei225 = true;
                        if (col == "A500") br.Active500 = true;
                        if (col == "OBS") br.Obsolete = true;
                        Debug.Assert(col.Length > 0);
                        if (col.Length > 0 && col[0] == 'S') splits.Add(new SplitInfo(d[i]));
                    }
                    if (splits.Count > 0)
                    {
                        br.SplitInfo = (SplitInfo[])splits.ToArray(typeof(SplitInfo));
                        splits.Clear();
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(String.Format("{0} 行{1} {2}", ex.Message, n, line));
                }
                line = sr.ReadLine();
                n++;
                //Debug.WriteLine(n);
            }
            sr.Close();
        }

        public AbstractBrand FindBrand(int code)
        {
            return (AbstractBrand)_data[code];
        }
        public AbstractBrand FindNextBrand(int code)
        {
            // TODO:10000は定数に出す
            for (int i = code + 1; i < 10000; i++)
            {
                if (_data.ContainsKey(i))
                {
                    return (AbstractBrand)_data[i];
                }
            }
            return FindNextBrand(0);
        }
        public AbstractBrand FindPrevBrand(int code)
        {
            for (int i = code - 1; i > 0; i--)
            {
                if (_data.ContainsKey(i))
                {
                    return (AbstractBrand)_data[i];
                }
            }
            return FindPrevBrand(10000);
        }

        public void Append(AbstractBrand br)
        {
            _data[br.Code] = br;
            if (br is DerivedBrand) _derivedBrands.Add(br);
        }
        public ArrayList DerivedBrands
        {
            get
            {
                return _derivedBrands;
            }
        }

        public IDictionaryEnumerator GetEnumerator()
        {
            return _data.GetEnumerator();
        }
        public DataFarm ReserveFarm(AbstractBrand br)
        {
            return ReserveFarm(br, Env.CurrentIndicators.Format);
        }

        public DataFarm ReserveFarm(AbstractBrand br, ChartFormat fmt)
        {
            DataFarm f = (DataFarm)_farms[br.Code];
            bool ok = false;
            if (f == null)
            {
                //キャッシュにないぢゃん
            }
            else if (Util.IsDailyBased(fmt) && f is DailyDataFarm)
            {
                ok = true;
            }
            else if (fmt == ChartFormat.Weekly &&  f is WeeklyDataFarm)
            {
                ok = true;
            }
            else if (fmt == ChartFormat.Monthly && f is MonthlyDataFarm)
            {
                ok = true;
            }
            if (ok)
            {//キャッシュにヒット
                if (br != _data[br.Code])
                {
                    Debug.Assert(false);
                }


                if (f.IsEmpty) f.LoadFor(br);
                if (br.ReferenceValue < _nextRefCount - 1)
                    br.ReferenceValue = _nextRefCount++;
                return f;
            }

            if (_farms.Count >= _farmCapacity) CleanFarms();

            if (br is BasicBrand)
                f = CreateFarm((BasicBrand)br, fmt);
            else
                f = CreateFarm((DerivedBrand)br, fmt);

            _farms[br.Code] = f;
            br.ReferenceValue = _nextRefCount++;
            return f;
        }
        public bool HasFarm(AbstractBrand br)
        {
            return _farms.Contains(br);
        }

        private void CleanFarms()
        {
            ArrayList t = new ArrayList(_farms.Keys);
            t.Sort(new BrandComparer(_data));

            Hashtable newfarms = new Hashtable();
            for (int n = t.Count / 2; n < t.Count; n++)
            {
                int code = (int)t[n];
                AbstractBrand br = (AbstractBrand)_data[code];
                br.ReferenceValue = n;
                newfarms[code] = _farms[code];
            }
            _nextRefCount = t.Count;
            _farms = newfarms;
        }

        //キャッシュとは無関係にDataFarmを作成
        public DataFarm CreateDailyFarm(AbstractBrand br, int extra_dates)
        {
            DailyDataFarm f = new DailyDataFarm();
            f.LoadFor(br, extra_dates);
            return f;
        }
        private DataFarm CreateFarm(BasicBrand br, ChartFormat format)
        {
            DataFarm f;
            if (Util.IsDailyBased(format))
                f = new DailyDataFarm();
            else if (format == ChartFormat.Weekly)
                f = new WeeklyDataFarm();
            else	// Monthly
                f = new MonthlyDataFarm();
            f.LoadFor(br);
            return f;
        }
        private DataFarm CreateFarm(DerivedBrand br, ChartFormat format)
        {
            DerivedDataFarm f = new DerivedDataFarm(br, format);
            f.LoadFor(br);
            return f;
        }

        //キャッシュからクローンを作って渡す.
        //本当はキャッシュのデータをそのまま渡した方がいいんだが、
        //この return 先がこのデータを書き換えるかもしれない。
        //C++ だったっら、 const DataFarm ってやるだけで判別できるのに C#はクソだからそれが出来ない。
        //諸君、こんな大規模開発に向かない言語は見捨てるしかない!!
        public DataFarm CloneFarm(AbstractBrand br, ChartFormat format)
        {
            DataFarm p = ReserveFarm(br, format);
            return (DataFarm)p.Clone();
        }


        private class BrandComparer : IComparer
        {
            Hashtable Data;

            public BrandComparer(Hashtable inData)
            {
                this.Data = inData;
            }
            public int Compare(object x, object y)
            {
                return ((AbstractBrand)this.Data[x]).ReferenceValue - ((AbstractBrand)this.Data[y]).ReferenceValue;
            }

        }

        public void ClearAllFarms()
        {
            _farms.Clear();

            _nextRefCount = 1;
#if DOJIMA
			Dojima.DojimaUtil.HalfDailyDataFarmCache.ClearAll();
#endif
        }

        public ICollection Values
        {
            get
            {
                return _data.Values;
            }
        }

        private void LoadBuiltIns()
        {
            AddBrandFromEnumType(typeof(BuiltInIndex).GetMembers(), MarketType.B);
#if DOJIMA
			AddBrandFromEnumType( typeof(CommodityIndex).GetMembers(), MarketType.C );
#endif
        }
        private void AddBrandFromEnumType(MemberInfo[] ms, MarketType mt)
        {
            foreach (MemberInfo mi in ms)
            {
                FieldInfo fi = mi as FieldInfo;
                //!!EnumDescAttributeは０から順に並んだ場合しか対応していなかったので急遽。後でまとめること
                if (fi != null && fi.IsStatic && fi.IsPublic)
                {
                    EnumValueAttribute a = (EnumValueAttribute)(fi.GetCustomAttributes(typeof(EnumValueAttribute), false)[0]);
                    int code = (int)fi.GetValue(null);
                    BasicBrand br = new BasicBrand(a.Description, code, mt);
                    br.Unit = 0;
                    _data[code] = br;
                }
            }
        }

        private void ReadHeader(string line)
        {
            _lastUpdatedDate = ReadDateFromHeader(line);
        }
        private static int ReadDateFromHeader(string line)
        {
            foreach (string e in line.Split(','))
            {
                int eq = e.IndexOf('=');
                if (eq == -1) throw new IOException("index.txtのヘッダフォーマットエラー");
                string name = e.Substring(0, eq);
                string value = e.Substring(eq + 1);
                if (name == "date")
                    return Int32.Parse(value); //いまのところこれしかデータなし
            }
            return -1;
        }

        public static int GuessDate(MemoryStream strm)
        {
            strm.Position = 0;
            TextReader s = new StreamReader(strm, Encoding.Default);
            string line = s.ReadLine();
            int dt = ReadDateFromHeader(line);
            return dt;
        }
    }
/*
        internal class BrandCollection {
            private Hashtable _data; //codeの値からAbstractBrandへ
            private Hashtable _farms; //AbstractBrandからFarmへ　Farmの個数を制限する機構はまだ
	
            private int _lastUpdatedDate;

            private int _stockBrandCount;
            private ArrayList _derivedBrands;

            //生きているDataFarmの数がfarmCapacityを超えないようにする
            private int _farmCapacity;
            private int _nextRefCount;

            public BrandCollection() {
                _data = new Hashtable();
                _farms = new Hashtable();
                _derivedBrands = new ArrayList();

                _farmCapacity = 100;
                _nextRefCount = 1;
            }
            public int TotalCount {
                get {
                    return _data.Count;
                }
            }
            //個別銘柄の数
            public int StockBrandCount {
                get {
                    return _stockBrandCount;
                }
            }
            public int DerivedBrandCount {
                get {
                    return _derivedBrands.Count;
                }
            }
            public int LastUpdatedDate {
                get {
                    return _lastUpdatedDate;
                }
            }

            public void Load(string filename) {
                //indexファイルから読んで構築
                //!!バージョンチェック必要
                StreamReader sr = new StreamReader(filename, Encoding.Default);
                string line = sr.ReadLine();
                ReadHeader(line);
                line = sr.ReadLine();

                int n = 1;
                ArrayList splits = new ArrayList();
                _stockBrandCount = 0;

                _data.Clear();
                _farms.Clear();

                LoadBuiltIns();

                while(line!=null) {
                    string[] d = line.Split(',');
                    if(d.Length<4) throw new IOException("index file format error: line"+ n);
                    try {
                        int code = Int32.Parse(d[0]);
                        BasicBrand br = new BasicBrand(d[1], code, (MarketType)Enum.Parse(typeof(MarketType), d[2], true));
                        br.Unit = Int32.Parse(d[3]);
                        _data[code] = br;
                        _stockBrandCount++;

                        for(int i=4; i<d.Length; i++) {
                            string col = d[i];
                            if(col=="N225") br.Nikkei225 = true;
                            if(col=="A500") br.Active500 = true;
                            if(col=="OBS") br.Obsolete = true;
                            Debug.Assert(col.Length>0);
                            if(col.Length>0 && col[0]=='S') splits.Add(new SplitInfo(d[i]));
                        }
                        if(splits.Count>0) {
                            br.SplitInfo = (SplitInfo[])splits.ToArray(typeof(SplitInfo));
                            splits.Clear();
                        }
                    }
                    catch(Exception ex) {
                        Debug.WriteLine(String.Format("{0} 行{1} {2}", ex.Message, n, line));
                    }
                    line = sr.ReadLine();
                    n++;
                    //Debug.WriteLine(n);
                }
                sr.Close();
            }

            public AbstractBrand FindBrand(int code) {
                return (AbstractBrand)_data[code];
            }
            public AbstractBrand FindNextBrand(int code){
                // TODO:10000は定数に出す
                for(int i=code+1;i<10000;i++){
                    if(_data.ContainsKey(i)){
                        return (AbstractBrand)_data[i];
                    }
                }
                return FindNextBrand(0);
            }
            public AbstractBrand FindPrevBrand(int code){
                for(int i=code-1;i>0;i--){
                    if(_data.ContainsKey(i)){
                        return (AbstractBrand)_data[i];
                    }	
                }
                return FindPrevBrand(10000);
            }
		
            public void Append(AbstractBrand br) {
                _data[br.Code] = br;
                if(br is DerivedBrand) _derivedBrands.Add(br);
            }
            public ArrayList DerivedBrands {
                get {
                    return _derivedBrands;
                }
            }

            public IDictionaryEnumerator GetEnumerator() {
                return _data.GetEnumerator();
            }
            public DataFarm ReserveFarm(AbstractBrand br) {
                return ReserveFarm(br, Env.CurrentIndicators.Format);
            }

            public DataFarm ReserveFarm(AbstractBrand br, ChartFormat fmt) {
                DataFarm f = (DataFarm)_farms[br];
                if(f!=null) { //キャッシュにヒット
                    if(f.IsEmpty) f.LoadFor(br);
                    if(br.ReferenceValue<_nextRefCount-1)
                        br.ReferenceValue = _nextRefCount++;
                    return f;
                }

                if(_farms.Count>=_farmCapacity) CleanFarms();

                if(br is BasicBrand)
                    f = CreateFarm((BasicBrand)br, fmt);
                else
                    f = CreateFarm((DerivedBrand)br, fmt);

                _farms[br] = f;
                br.ReferenceValue = _nextRefCount++;
                return f;
            }
            public bool HasFarm(AbstractBrand br) {
                return _farms.Contains(br);
            }

            private void CleanFarms() {
                ArrayList t = new ArrayList(_farms.Keys);
                t.Sort(new BrandComparer());

                Hashtable newfarms = new Hashtable();
                for(int n = t.Count/2; n<t.Count; n++) {
                    AbstractBrand br = (AbstractBrand)t[n];
                    br.ReferenceValue = n;
                    newfarms[br] = _farms[br];
                }
                _nextRefCount = t.Count;
                _farms = newfarms;
            }

            //キャッシュとは無関係にDataFarmを作成
            public DataFarm CreateDailyFarm(AbstractBrand br, int extra_dates) {
                DailyDataFarm f = new DailyDataFarm();
                f.LoadFor(br, extra_dates);
                return f;
            }
            public DataFarm CreateFarm(BasicBrand br, ChartFormat format) {
                DataFarm f;
                if(Util.IsDailyBased(format))
                    f = new DailyDataFarm();
                else if(format==ChartFormat.Weekly)
                    f = new WeeklyDataFarm();
                else	// Monthly
                    f = new MonthlyDataFarm();
                f.LoadFor(br);
                return f;
            }
            public DataFarm CreateFarm(DerivedBrand br, ChartFormat format) {
                DerivedDataFarm f = new DerivedDataFarm(br, format);
                f.LoadFor(br);
                return f;
            }

            private class BrandComparer : IComparer {
                public int Compare(object x, object y) {
                    return ((AbstractBrand)x).ReferenceValue - ((AbstractBrand)y).ReferenceValue;
                }

            }

            public void ClearAllFarms() {
                _farms.Clear();
                _nextRefCount = 1;
    #if DOJIMA
                Dojima.DojimaUtil.HalfDailyDataFarmCache.ClearAll();
    #endif
            }

            public ICollection Values {
                get {
                    return _data.Values;
                }
            }

            private void LoadBuiltIns() {
                AddBrandFromEnumType( typeof(BuiltInIndex).GetMembers(), MarketType.B );
    #if DOJIMA
                AddBrandFromEnumType( typeof(CommodityIndex).GetMembers(), MarketType.C );
    #endif
            }
            private void AddBrandFromEnumType(MemberInfo[] ms, MarketType mt) {
                foreach(MemberInfo mi in ms) {
                    FieldInfo fi = mi as FieldInfo;
                    //!!EnumDescAttributeは０から順に並んだ場合しか対応していなかったので急遽。後でまとめること
                    if(fi!=null && fi.IsStatic && fi.IsPublic) {
                        EnumValueAttribute a = (EnumValueAttribute)(fi.GetCustomAttributes(typeof(EnumValueAttribute), false)[0]);
                        int code = (int)fi.GetValue(null);
                        BasicBrand br = new BasicBrand(a.Description, code, mt);
                        br.Unit = 0;
                        _data[code] = br;
                    }
                }
            }

            private void ReadHeader(string line) {
                _lastUpdatedDate = ReadDateFromHeader(line);
            }
            private static int ReadDateFromHeader(string line) {
                foreach(string e in line.Split(',')) {
                    int eq = e.IndexOf('=');
                    if(eq==-1) throw new IOException("index.txtのヘッダフォーマットエラー");
                    string name = e.Substring(0, eq);
                    string value = e.Substring(eq+1);
                    if(name=="date")
                        return Int32.Parse(value); //いまのところこれしかデータなし
                }
                return -1;
            }

            public static int GuessDate(MemoryStream strm) {
                strm.Position = 0;
                TextReader s = new StreamReader(strm, Encoding.Default);
                string line = s.ReadLine();
                int dt = ReadDateFromHeader(line);
                return dt;
            }
        }
    */
}
