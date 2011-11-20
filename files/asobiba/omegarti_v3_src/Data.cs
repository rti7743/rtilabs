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

using Zanetti.Arithmetic;
using Zanetti.Arithmetic.Series;
using Zanetti.Indicators;
//hacked by rti
using Zanetti.CodeExtendData;   //コード拡張
//----------

namespace Zanetti.Data
{

	//primitive indicatorをサポートするためのもの
	internal class DataFarmPrimitiveAccess {
		//delegateの引数になるためにこの形でないとだめ
		internal static double GetDate(TradeData tr) {
			return (double)tr.Farm.GetInt(tr.Offset);
		}
		internal static double GetOpen(TradeData tr) {
			return AdjustPrice((double)tr.Farm.GetInt(tr.Offset+DataFarm.OPEN_OFFSET), tr);
		}
		internal static double GetHigh(TradeData tr) {
			return AdjustPrice((double)tr.Farm.GetInt(tr.Offset+DataFarm.HIGH_OFFSET), tr);
		}
		internal static double GetLow(TradeData tr) {
			return AdjustPrice((double)tr.Farm.GetInt(tr.Offset+DataFarm.LOW_OFFSET), tr);
		}
		internal static double GetClose(TradeData tr) {
			return AdjustPrice((double)tr.Farm.GetInt(tr.Offset+DataFarm.CLOSE_OFFSET), tr);
		}
		internal static double GetVolume(TradeData tr) {
			//オーバーフロー対策の一時しのぎ
			return AdjustVolume((double)(uint)tr.Farm.GetInt(tr.Offset+DataFarm.VOLUME_OFFSET), tr);
		}
		internal static double GetCreditLong(TradeData tr) {
			return AdjustVolume((double)tr.Farm.GetInt(tr.Offset+DataFarm.CREDITLONG_OFFSET), tr);
		}
		internal static double GetCreditShort(TradeData tr) {
			return AdjustVolume((double)tr.Farm.GetInt(tr.Offset+DataFarm.CREDITSHORT_OFFSET), tr);
		}

		private static double AdjustPrice(double value, TradeData tr) {
			double split = Env.Preference.AdjustSplit? tr.Farm.CalcSplitRatio(tr.Date) : 1;
			if(value==0 && GetVolume(tr)==0) { //出来高がない日は価格が０と記入されているので前日の終値で代用
				TradeData pr = tr.Prev;
				return pr==null? 0 : GetClose(tr.Prev);
			}
			else
				return tr.Farm.Brand.PriceScale * value / split;
		}
		private static double AdjustVolume(double value, TradeData tr) {
            double split = Env.Preference.AdjustSplit? tr.Farm.CalcSplitRatio(tr.Date) : 1;
            return value * split;
		}
	}

	internal class NewDailyData {
		public int open;
		public int high;
		public int low;
		public int close;
		public int volume;
	}

    //hacked by rti
    internal abstract class DataFarm : IDisposable ,ICloneable
    {
//	internal abstract class DataFarm : IDisposable {
    //------
		public const int RECORD_LENGTH = 32;

		public const int OPEN_OFFSET        = 4;
		public const int HIGH_OFFSET        = 8;
		public const int LOW_OFFSET         = 12;
		public const int CLOSE_OFFSET       = 16;
		public const int VOLUME_OFFSET      = 20;
		public const int CREDITSHORT_OFFSET = 24;
		public const int CREDITLONG_OFFSET  = 28;

		protected bool _isEmpty; //エラーなどで利用不能なことを示すフラグ
		protected AbstractBrand _brand;

		protected byte[] _farm;      //一次データ。同一のDataFarmオブジェクトを他の銘柄に使いまわすときもあるので、必要以上の長さが確保されることもある
		protected int _byteLength;   //_farmの論理的な長さ
		protected TradeData[] _data; //必要に応じて生成されるTradeDataの列。一目など未来の日付のデータがあると配列の長さは_farmに対応する分より大きいこともある
		protected int _filledLength;   //最新日付までの長さ

        //hacked by rti-----------
        protected CodeExtendData.Data _CodeExtend = new CodeExtendData.Data();    //コード拡張

        //コード拡張の取得
        public CodeExtendData.Data CodeExtend
        {
            get
            {
                return this._CodeExtend;
            }
        }
        public abstract object Clone();
        public object Clone(DataFarm newObject)
        {
            newObject._isEmpty = this._isEmpty;
            newObject._brand = this._brand;
            if (this._farm == null)
            {
                newObject._farm = null;
            }
            else
            {
                newObject._farm = (byte[])this._farm.Clone();
            }
            newObject._byteLength = this._byteLength;
            if (this._data == null)
            {
                newObject._data = null;
            }
            else
            {
                newObject._data = (TradeData[])this._data.Clone();
            }
            newObject._filledLength = this._filledLength;
            return newObject;
        }
        //-----

		public DataFarm() {
		}
		public abstract void LoadFor(AbstractBrand br);

		public AbstractBrand Brand {
			get {
				return _brand;
			}
		}

		public int TotalLength {
			get {
				return _data.Length;
			}
		}
		public int FilledLength {
			get {
				return _filledLength;
			}
		}
		public bool IsEmpty {
			get {
				return _isEmpty;
			}
		}

		public byte[] RawDataImage {
			get {
				return _farm;
			}
		}

		internal int GetInt(int offset) {
			if(offset>=_byteLength)
				throw new IndexOutOfRangeException();
			unsafe {
				fixed(byte* p = &_farm[0]) {
					return *(int*)(p+offset);
				}
			}
		}
		
		public TradeData GetByIndex(int index) {
			Debug.Assert(_data!=null);
			if(index<0 || index>=_data.Length)
				throw new TradeDataOverflowException(index.ToString() + " is out of range");
			TradeData td = _data[index];
			if(td!=null) return td; //cache hit

			td = new TradeData(this, index, index*RECORD_LENGTH);
			_data[index] = td;
			return td;
		}

		public abstract int LastDate { get; }
		public abstract int FirstDate { get; }

		public int DateToIndex(int date) {
			return DateToIndex(0, _filledLength, date);
		}
		private int DateToIndex(int begin, int end, int date) {
			//binary search
			if(end-begin <= 1) 
				return begin;
			else {
				int h = (begin+end)/2;
				int t = GetByIndex(h).Date;
				if(date < t)
					return DateToIndex(begin, h, date);
				else
					return DateToIndex(h, end, date);
			}
		}

		//分割比率の取得
		public double CalcSplitRatio(int date) {
			return _brand.CalcSplitRatio(date, this.LastDate);

		}

		public void Dispose() {
			_farm = null;
			_data = null;
		}

		internal static int GetInt(byte[] rawdata, int offset) {
			Debug.Assert(rawdata.Length>0);
			unsafe {
				fixed(byte* p = &rawdata[0]) {
					return *(int*)(p+offset);
				}
			}
		}
		internal static void SetInt(byte[] rawdata, int offset, int value) {
			unsafe {
				fixed(byte* p = &rawdata[0]) {
					*(int*)(p+offset) = value;
				}
			}
		}

		protected static int AdjustPrice(int raw, double ratio) {
			return (int)((double)raw / ratio);
		}
		protected static int AdjustVolume(int raw, double ratio) {
			return (int)((double)raw * ratio);
		}
	}

	internal class DailyDataFarm : DataFarm {
		
		protected int _extraDataOffset; //１日単位でデータの追加をしたときのために

        //hacked by rti
        public override object Clone()
        {
            DailyDataFarm p = new DailyDataFarm();
            return this.Clone(p);
        }
        protected object Clone(DailyDataFarm newObject)
        {
            newObject._extraDataOffset = this._extraDataOffset;
            ((DataFarm)this).Clone((DataFarm)newObject);
            return newObject;
        }
        //------------

		public DailyDataFarm() : base() {
		}
		public override void LoadFor(AbstractBrand br) {
			_brand = br;
			Construct(Util.GetDailyDataFileName(br.Code), 0);
		}
		public void LoadFor(AbstractBrand br, int extra_dates) {
			_brand = br;
            Construct(Util.GetDailyDataFileName(br.Code), extra_dates);
		}
		private void Construct(string filename, int extra_dates) {
			_isEmpty = true;
#if DOJIMA
			Dojima.DojimaUtil.HalfDailyDataFarmCache.Clear(_brand);
#endif
			if(File.Exists(filename)) {
				int length = (int)new FileInfo(filename).Length;
				if(length==0) return;

				if(_farm==null || _farm.Length<length + extra_dates*RECORD_LENGTH)
					_farm = new byte[length + extra_dates*RECORD_LENGTH];
				int future_length = Env.CurrentIndicators.GetAddedFutureLength(ChartFormat.Daily);
				_filledLength = length/RECORD_LENGTH;
				_data = new TradeData[_filledLength + future_length];
				_byteLength = length;
				_extraDataOffset = 0;
				_isEmpty = false;

				FileStream s = null;
				try {
					s = new FileStream(filename, FileMode.Open);
					s.Read(_farm, 0, length);
				}
				finally {
					if(s!=null) s.Close();
				}
			}

            //hacked by rti
            try
            {
                this._CodeExtend.Read(_brand.Code);
            }
            catch(Exception ex) 
            {
                Debug.WriteLine(String.Format("拡張データ読み込み中にエラー {1}:{0}", _brand.Code, ex.Message));
                throw;
            }
            //----------------
		}
		public void Save(string filename) {
			if(_farm!=null) { //エラーハンドリングできていない
				FileStream s = new FileStream(filename, FileMode.Create);
				s.Write(_farm, 0, _byteLength + _extraDataOffset);
				s.Close();
			}
		}
		internal void WriteExtraData(int record_offset, int value) {
			unsafe {
				fixed(byte* p = &_farm[0]) {
					*(int*)(p + _byteLength + _extraDataOffset + record_offset) = value;
				}
			}
		}
		internal void ProgressExtraDataAddress() {
			_extraDataOffset += RECORD_LENGTH;
			Debug.Assert(_extraDataOffset<=_farm.Length);
		}

		//連続的に複数の日付を更新することもできるが、増加方向であることが必須
		internal void UpdateDataFarm(int date, NewDailyData td) {
			int ld;
			if(this.IsEmpty) {
				//とりあえず１日書き込める分だけ初期化
				if(_farm==null) {
					_farm = new byte[RECORD_LENGTH * 200]; //この上限はどこかで取得すべきだが
					_filledLength = 0;
					_data = null;
					_byteLength = 0;
					_extraDataOffset = 0;
				}
				ld = 0;
			}
			else
				ld = this.LastDate;


			int offset;
			if(ld < date) {
				offset = _byteLength + _extraDataOffset; //emptyのときは常にこれ
				_extraDataOffset += RECORD_LENGTH;
			}
			else {
				offset = _byteLength - RECORD_LENGTH;
				do {
					int t = GetInt(offset);
					if(t==date)
						break;
					else if(t < date) {
						offset += RECORD_LENGTH;
						break;
					}
					else
						offset -= RECORD_LENGTH;
				} while(true);
			}

			unsafe {
				fixed(byte* p = &_farm[0]) {
					byte* a = p + offset;
					*(int*)(a +  0) = date;
					*(int*)(a +  4) = td.open;
					*(int*)(a +  8) = td.high;
					*(int*)(a + 12) = td.low;
					*(int*)(a + 16) = td.close;
					*(int*)(a + 20) = td.volume;
				}
			}

		}

		//次の２つはTradeDataを作らないようにしている、注意
		public override int LastDate {
			get {
				return GetInt(_byteLength - RECORD_LENGTH);
			}
		}
		public override int FirstDate {
			get {
				return GetInt(0);
			}
		}

	}

	internal class WeeklyDataFarm : DataFarm {
		private int _firstDate;
		private int _lastDate;

        //hacked by rti
        public override object Clone()
        {
            WeeklyDataFarm p = new WeeklyDataFarm();
            return this.Clone(p);
        }
        public object Clone(WeeklyDataFarm newObject)
        {
            newObject._firstDate = this._firstDate;
            newObject._lastDate = this._lastDate;
            ((DataFarm)this).Clone((DataFarm)newObject);
            return newObject;
        }
        //------------

		public WeeklyDataFarm() : base() {
		}
		public override void LoadFor(AbstractBrand br) {
			_brand = br;
            Construct(Util.GetDailyDataFileName(br.Code));
		}

		private void Construct(string filename) {
			_isEmpty = true;
			if(File.Exists(filename)) {
				int length = (int)new FileInfo(filename).Length;
				if(length > 0) {
					//まずは日足を読む
					byte[] daily = new byte[length];
					FileStream s = null;
					try {
						s = new FileStream(filename, FileMode.Open);
						s.Read(daily, 0, length);
					}
					finally {
						if(s!=null) s.Close();
					}

					_isEmpty = false;
					_firstDate= GetInt(daily, 0);
					_lastDate = GetInt(daily, daily.Length-RECORD_LENGTH);
					WeeklyIndex weekly_begin = Env.WeekFormat.FindByDate(GetInt(daily, 0));
					WeeklyIndex weekly_end   = Env.WeekFormat.FindByDate(GetInt(daily, daily.Length-RECORD_LENGTH));
					_filledLength = weekly_end.Index+1 - weekly_begin.Index;
					_data = new TradeData[_filledLength + Env.CurrentIndicators.GetAddedFutureLength(ChartFormat.Weekly)];

					//byte[]部分のデータ読み
					_farm = new byte[_data.Length * RECORD_LENGTH];
					_byteLength = _farm.Length;
					int offset = 0;
					for(int i=weekly_begin.Index; i<=weekly_end.Index; i++) {
						WeeklyIndex wi = Env.WeekFormat.GetAt(i);
						offset = FillWeeklyData((i-weekly_begin.Index)*RECORD_LENGTH, daily, offset, wi);
						if(offset>=daily.Length) break;
					}
				}
			}
		}
		private int FillWeeklyData(int farmoffset, byte[] daily, int offset, WeeklyIndex wi) {

			int enddate = Env.WeekFormat.Length>wi.Index+1? Env.WeekFormat.GetAt(wi.Index+1).FirstDate : Util.DateToInt(Util.IntToDate(wi.FirstDate).AddDays(7));

			int vol = 0, high = Int32.MinValue, low = Int32.MaxValue;
			int open = 0, close = 0, cre_long = 0, cre_short = 0;

			int today = GetInt(daily, offset);
			bool is_index = _brand.IsBuiltIn;
			// base_splitを得るのに最初の取引日である 'today' を使うのは誤り。
			// 下の、SetInt(_farm, farmoffset, wi.FirstDate);
			// で、後に式を評価する際に用いられる基準日として日曜基準で 'wi.FirstDate' を使っているのだから、
			// ここでもこの値を使うべき。　2005/3/15 T. SARUKI
			//
			double base_split = this.CalcSplitRatio(wi.FirstDate); //分割を考慮する場合は期間内の調整が要る
			while(offset<=daily.Length-RECORD_LENGTH && today<enddate) {
				//if(!is_index && today>20031201) Debugger.Break();
                double split = Env.Preference.AdjustSplit? this.CalcSplitRatio(today) / base_split : 1;
                int v = AdjustVolume(GetInt(daily, offset+VOLUME_OFFSET), split);
				if(is_index || v!=0) { //非indexで出来高０の日は集計しない
					if(open==0) open = AdjustPrice(GetInt(daily, offset+OPEN_OFFSET), split);
					close = AdjustPrice(GetInt(daily, offset+CLOSE_OFFSET), split);
					high = Math.Max(high, AdjustPrice(GetInt(daily, offset+HIGH_OFFSET), split));
					low  = Math.Min(low,  AdjustPrice(GetInt(daily, offset+LOW_OFFSET), split));
					cre_long = AdjustVolume(GetInt(daily, offset+CREDITLONG_OFFSET), split);
					cre_short = AdjustVolume(GetInt(daily, offset+CREDITSHORT_OFFSET), split);
					vol += v;
				}

				offset += RECORD_LENGTH;
				if(offset<daily.Length) today = GetInt(daily, offset);
			}

			SetInt(_farm, farmoffset, wi.FirstDate);
			SetInt(_farm, farmoffset+OPEN_OFFSET,  open);
			SetInt(_farm, farmoffset+HIGH_OFFSET,  high);
			SetInt(_farm, farmoffset+LOW_OFFSET,   low);
			SetInt(_farm, farmoffset+CLOSE_OFFSET, close);
			SetInt(_farm, farmoffset+VOLUME_OFFSET, vol);
			SetInt(_farm, farmoffset+CREDITLONG_OFFSET, cre_long);
			SetInt(_farm, farmoffset+CREDITSHORT_OFFSET, cre_short);

			return offset;
		}

		public override int LastDate {
			get {
				return _lastDate;
			}
		}
		public override int FirstDate {
			get {
				return _firstDate;
			}
		}

	}
	internal class MonthlyDataFarm : DataFarm {
		private int _firstDate;
		private int _lastDate;

        //hacked by rti
        public override object Clone()
        {
            MonthlyDataFarm p = new MonthlyDataFarm();
            return this.Clone(p);
        }
        public object Clone(MonthlyDataFarm newObject)
        {
            newObject._firstDate = this._firstDate;
            newObject._lastDate = this._lastDate;
            ((DataFarm)this).Clone((DataFarm)newObject);
            return newObject;
        }
        //------------

		public MonthlyDataFarm() : base() {
		}
		public override void LoadFor(AbstractBrand br) {
			_brand = br;
			Construct(Util.GetDailyDataFileName(br.Code));
		}

		private void Construct(string filename) {
			_isEmpty = true;
			if(File.Exists(filename)) {
				int length = (int)new FileInfo(filename).Length;
				if(length > 0) {
					//まずは日足を読む
					byte[] daily = new byte[length];
					_isEmpty = false;
					FileStream s = null;
					try {
						s = new FileStream(filename, FileMode.Open);
						s.Read(daily, 0, length);
					}
					finally {
						if(s!=null) s.Close();
					}

					_firstDate= GetInt(daily, 0);
					_lastDate = GetInt(daily, daily.Length-RECORD_LENGTH);
					DateTime monthly_begin = new DateTime(_firstDate / 10000, (_firstDate % 10000) / 100, (_firstDate % 100));
					DateTime monthly_end = new DateTime(_lastDate / 10000, (_lastDate % 10000) / 100, (_lastDate % 100));
					_filledLength = (monthly_end.Year - monthly_begin.Year) * 12 + monthly_end.Month+1 - monthly_begin.Month;
				
					_data = new TradeData[_filledLength + Env.CurrentIndicators.GetAddedFutureLength(ChartFormat.Monthly)];
				
					// 以下WeeklyIndexとかぶって冗長

					//byte[]部分のデータ読み
					_farm = new byte[_data.Length * RECORD_LENGTH];
					_byteLength = _farm.Length;
					DateTime yearmonth = monthly_begin;
					int offset = 0;
					for(int i=0; i<_filledLength; i++) {
						offset = FillMonthlyData(i*RECORD_LENGTH, daily, offset, yearmonth);
						if(offset>=daily.Length) break;
						yearmonth = yearmonth.AddMonths(1);
					}
				}
			}
		}
		// このメソッドもWeeklyIndexのFillWeeklyDataとかぶってかなり冗長
		private int FillMonthlyData(int farmoffset, byte[] daily, int offset, DateTime yearmonth) {

			DateTime endmonth = yearmonth.AddMonths(1);
			int enddate = endmonth.Year * 10000 + endmonth.Month * 100 + 1;			

			int vol = 0, high = Int32.MinValue, low = Int32.MaxValue;
			int open = 0, close = 0, cre_long = 0, cre_short = 0;

			int today = GetInt(daily, offset);
			bool is_index = _brand.IsBuiltIn;
            // base_splitを得るのに最初の取引日である 'today' を使うのは誤り。
			// 下の、SetInt(_farm, farmoffset, yearmonth.Year * 10000 + yearmonth.Month * 100 + 1);
			// で、後に式を評価する際に用いられる基準日として月の初日である 'yearmonth.Year * 10000 + yearmonth.Month * 100 + 1' を使っているのだから、
			// ここでもこの値を使うべき。　2005/3/15 T. SARUKI
			//
			double base_split = this.CalcSplitRatio(Util.DateToInt(yearmonth.Year, yearmonth.Month, 1));
			while(offset <= daily.Length - RECORD_LENGTH && today < enddate) {
                double split = Env.Preference.AdjustSplit? this.CalcSplitRatio(today) / base_split : 1;
                int v = AdjustVolume(GetInt(daily, offset+VOLUME_OFFSET), split);
				if(is_index || v!=0) { //非indexで出来高０の日は集計しない
					if(open==0) open = AdjustPrice(GetInt(daily, offset+OPEN_OFFSET), split);
					close = AdjustPrice(GetInt(daily, offset+CLOSE_OFFSET), split);
					high = Math.Max(high, AdjustPrice(GetInt(daily, offset+HIGH_OFFSET), split));
					low  = Math.Min(low,  AdjustPrice(GetInt(daily, offset+LOW_OFFSET), split));
					cre_long = AdjustVolume(GetInt(daily, offset+CREDITLONG_OFFSET), split);
					cre_short = AdjustVolume(GetInt(daily, offset+CREDITSHORT_OFFSET), split);
					vol += v;
				}

				offset += RECORD_LENGTH;
				if(offset<daily.Length) today = GetInt(daily, offset);
			}

			SetInt(_farm, farmoffset, yearmonth.Year * 10000 + yearmonth.Month * 100 + 1);
			SetInt(_farm, farmoffset+OPEN_OFFSET,  open);
			SetInt(_farm, farmoffset+HIGH_OFFSET,  high);
			SetInt(_farm, farmoffset+LOW_OFFSET,   low);
			SetInt(_farm, farmoffset+CLOSE_OFFSET, close);
			SetInt(_farm, farmoffset+VOLUME_OFFSET, vol);
			SetInt(_farm, farmoffset+CREDITLONG_OFFSET, cre_long);
			SetInt(_farm, farmoffset+CREDITSHORT_OFFSET, cre_short);

			return offset;
		}

		public override int LastDate {
			get {
				return _lastDate;
			}
		}
		public override int FirstDate {
			get {
				return _firstDate;
			}
		}

	}

	//他の銘柄から導出される銘柄
	internal class DerivedDataFarm : DataFarm {
		private int _firstDate;
		private int _lastDate;
		private DerivedBrand _derivedBrand;
		private ChartFormat _chartFormat;

        //hacked by rti
        public override object Clone()
        {
            DerivedDataFarm p = new DerivedDataFarm(this._derivedBrand, this._chartFormat);
            return this.Clone(p);
        }
        public object Clone(DerivedDataFarm newObject)
        {
            newObject._firstDate = this._firstDate;
            newObject._lastDate = this._lastDate;
            newObject._derivedBrand = this._derivedBrand;
            newObject._chartFormat = this._chartFormat;
            ((DataFarm)newObject).Clone(newObject);
            return newObject;
        }
        //------------

		public DerivedDataFarm(DerivedBrand br, ChartFormat fmt) : base() {
			_derivedBrand = br;
			_chartFormat = fmt;
		}
		public override void LoadFor(AbstractBrand br) {
			Debug.Assert(br is DerivedBrand);
			_brand = br;
			_derivedBrand = (DerivedBrand)br;
			Construct(_derivedBrand);
		}

		private void Construct(DerivedBrand br) {
			DataFarm[] fs = new DataFarm[br.Dependencies.Length];
			int len = Int32.MaxValue;
			int shortest_farm_index = 0;
			for(int i=0; i<fs.Length; i++) {
				DataFarm f = Env.BrandCollection.ReserveFarm(br.Dependencies[i], _chartFormat);
				if(f.IsEmpty) {
					_isEmpty = true;
					return; //一つでも利用不可があればダメ
				}
				fs[i] = f;
				if(f.FilledLength < len) {
					shortest_farm_index = i;
					len = f.FilledLength;
				}
			}

			DataFarm shortest_farm = fs[shortest_farm_index];
			if(_farm==null || _farm.Length<len*RECORD_LENGTH) _farm = new byte[len*RECORD_LENGTH];
			_byteLength = len*RECORD_LENGTH;

			_data = new TradeData[len + Env.CurrentIndicators.GetAddedFutureLength(_chartFormat)];
			_filledLength = len;
			_isEmpty = false;
	
			_firstDate = shortest_farm.GetByIndex(0).Date;
			_lastDate  = shortest_farm.GetByIndex(shortest_farm.FilledLength-1).Date;
			//データの構築　本当はここも遅延評価すると効率的だが
			FillData(len, shortest_farm_index, br, fs);
		}

		private void FillData(int len, int shortest_farm_index, DerivedBrand br, DataFarm[] deps) {
			int[] indexmap = new int[deps.Length];
			EvalResult[][] args = new EvalResult[4][];
			for(int i=0; i<4; i++) {
				args[i] = new EvalResult[deps.Length];
				for(int j=0; j<deps.Length; j++) args[i][j] = new EvalResult(0);
			}
			Indicator[] inds = new Indicator[] { 
												   Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Open), 
												   Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.High), 
												   Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Low), 
												   Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Close)};

			TradeData[] tds = new TradeData[deps.Length];

			Evaluator ev = new Evaluator(br.Name);

			for(int i=0; i<len; i++) {
				ev.BaseIndex = i;

				//日付の決定
				int date = deps[shortest_farm_index].GetByIndex(i).Date;

				int farmoffset = i * RECORD_LENGTH;
				for(int j=0; j<deps.Length; j++) {
					int candidate = indexmap[j]+1; //多くの場合日付とindexは一致しているので、DateToIndexの実行回数を減らすためindexmapを用意
					TradeData td = candidate<deps[j].TotalLength? deps[j].GetByIndex(candidate) : null;
					if(td==null || td.Date!=date) {
						candidate = deps[j].DateToIndex(date);
						td = deps[j].GetByIndex(candidate);
					}
					indexmap[j] = candidate;

					for(int k=0; k<inds.Length; k++)
						args[k][j].DoubleVal = td.GetValue(inds[k]);
				}

				//日付
				SetInt(_farm, farmoffset, date);

				//４本値の計算
				Expression expr = br.Expression;
				ev.Args = args[0];
				int open = (int)((EvalResult)expr.Apply(ev)).DoubleVal;
				SetInt(_farm, farmoffset+OPEN_OFFSET,  open);
				ev.Args = args[3];
				int close = (int)((EvalResult)expr.Apply(ev)).DoubleVal;
				SetInt(_farm, farmoffset+CLOSE_OFFSET, close);
				ev.Args = args[1];
				int v1 = (int)((EvalResult)expr.Apply(ev)).DoubleVal;
				ev.Args = args[2];
				int v2 = (int)((EvalResult)expr.Apply(ev)).DoubleVal;

				//計算式により、それぞれの高値・安値で計算したものが結果としてどうなるかは変わってしまう
				SetInt(_farm, farmoffset+HIGH_OFFSET,  Math.Max(Math.Max(open, close), Math.Max(v1, v2)));
				SetInt(_farm, farmoffset+LOW_OFFSET,   Math.Min(Math.Min(open, close), Math.Min(v1, v2)));
			}
		}

		public override int LastDate {
			get {
				return _lastDate;
			}
		}
		public override int FirstDate {
			get {
				return _firstDate;
			}
		}
	}



	//internal delegate double Calculate(Indicator indicator, TradeData data);

	//節の種類
	internal enum Fushi {
		Unknown,
		None,
		High,
		Low
	}

	/// 日足・週足・月足などの１件のレコード
	internal class TradeData {
		private DataFarm _farm;
		private int _index;
		private int _offset;
		private double[] _data;
		private Fushi _fushi;

		public TradeData(DataFarm farm, int index, int offset) {
			_farm = farm;
			_index = index;
			_offset = offset;
			_data = new double[Env.CurrentIndicators.IndicatorCount];
			_fushi = Fushi.Unknown;
			for(int i=0; i<_data.Length; i++)
				_data[i] = Double.NaN;
		}
		public DataFarm Farm {
			get {
				return _farm;
			}
		}
		public int Index {
			get {
				return _index;
			}
		}
		public int Offset {
			get {
				return _offset;
			}
		}
		public TradeData Prev {
			get {
				return _index>0? _farm.GetByIndex(_index-1) : null;
			}
		}
		public TradeData Next {
			get {
				return _index<_farm.TotalLength-1? _farm.GetByIndex(_index+1) : null;
			}
		}
		public bool IsFuture {
			get {
				return _index>=_farm.FilledLength;
			}
		}
		public bool CoversDate(int date) {
			if(date==this.Date)
				return true;
			else {
				int c = this.Date;
				if(c > date)
					return false;
				else {
					TradeData next = this.Next;
					return next!=null && date<next.Date;
				}
			}
		}

		public double GetValue(Indicator indicator) {
			double t = _data[indicator.LaneID];
			//overflowによる演算不可はPositiveInfinityであらわす
			if(Double.IsPositiveInfinity(t)) return Double.NaN;
			if(!Double.IsNaN(t)) return t; //キャッシュにヒット

			try {
				if(indicator.CheckRange(this)) {
					t = indicator.Calculate(this);
					_data[indicator.LaneID] = t;
				}
				else
					t = Double.NaN;
				return t;
			}
			catch(TradeDataOverflowException ) {
				//Debug.WriteLine("Out of range!");
				_data[indicator.LaneID] = Double.PositiveInfinity;
				return Double.NaN;
			}
		}
		public int Date {
			get {
				return (int)GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Date));
			}
		}
		public double Open {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Open));
			}
		}
		public double High {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.High));
			}
		}
		public double Low {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Low));
			}
		}
		public double Close {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Close));
			}
		}
		public double Volume {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.Volume));
			}
		}
		public double CreditLong {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.CreditLong));
			}
		}
		public double CreditShort {
			get {
				return GetValue(Env.CurrentIndicators.GetPrimitive(PrimitiveIndicator.CreditShort));
			}
		}

		//節の計算
		public Fushi Fushi {
			get {
				if(_fushi!=Fushi.Unknown) return _fushi;

				double h1 = Double.MinValue;
				double l1 = Double.MaxValue;
				double h2 = Double.MinValue;
				double l2 = Double.MaxValue;
				int fushi = Env.Preference.FushiRange;
				//あまり端に表示しても仕方ない
				if(_index<fushi || _index>_farm.FilledLength-fushi) {
					_fushi = Fushi.None;
					return _fushi;
				}
				for(int i=_index-fushi; i<_index; i++) {
					h1 = Math.Max(h1, _farm.GetByIndex(i).High);
					l1 = Math.Min(l1, _farm.GetByIndex(i).Low);
				}
				for(int i=_index+1; i<_index+fushi; i++) {
					h2 = Math.Max(h2, _farm.GetByIndex(i).High);
					l2 = Math.Min(l2, _farm.GetByIndex(i).Low);
				}

				//過去に同値があるときは無視、未来にあるときは節
				if(h1<this.High && h2<=this.High)
					_fushi = Fushi.High;
				else if(l1>this.Low && l2>=this.Low)
					_fushi = Fushi.Low;
				else
					_fushi = Fushi.None;

				return _fushi;
			}
		}

	}

	internal class TradeDataOverflowException : ApplicationException {
		public TradeDataOverflowException(string msg) : base(msg) {
		}
	}

	internal class IndicatorTimeSeries : TimeSeries {
		protected DataFarm _farm;
		protected int _begin;
		protected int _end;
		protected Indicator _indicator;

		public IndicatorTimeSeries(DataFarm farm, Indicator ind, int begin, int end) {
			_farm = farm;
			_begin = begin;
			_end = end;
			_indicator = ind;
		}

		public override int Count {
			get {
				return _end - _begin;
			}
		}
		public int BeginIndex {
			get {
				return _begin;
			}
		}
		public int EndIndex {
			get {
				return _end;
			}
		}
		public override double LastValue {
			get {
				return _farm.GetByIndex(_end-1).GetValue(_indicator);
			}
		}


		protected class IndicatorCursor : TimeSeries.Cursor {
			private int _index;
			private IndicatorTimeSeries _parent;

			public IndicatorCursor(IndicatorTimeSeries parent) {
				_parent = parent;
				_index = _parent._begin;
			}
			public override bool HasNext {
				get {
					return _index<_parent._end;
				}
			}
			public override double Next {
				get {
					return _parent._farm.GetByIndex(_index++).GetValue(_parent._indicator);
				}
			}
		}

		public override Cursor CreateCursor() {
			return new IndicatorCursor(this);
		}
	}

}
