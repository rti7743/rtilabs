/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Diagnostics;
using System.Collections;
using Zanetti.Data;

using Zanetti.Parser;
using Zanetti.Indicators;
using Zanetti.Arithmetic.Series;

namespace Zanetti.Arithmetic
{
	internal delegate EvalResult EvalDelegate(Evaluator evaluator, Expression[] args);
	internal delegate DateRange  DateRangeDelegate(DateRangeChecker ch, Evaluator evaluator, Expression[] args);


	/// 組み込み関数のコレクション
	internal class ArithmeticLibrary {

		private class Entry {
			public string name;
			public int index;
			public EvalDelegate function;
			public DateRangeDelegate daterange;

			public Entry(string n, int i, EvalDelegate d, DateRangeDelegate dr) {
				name = n;
				index = i;
				function = d;
				daterange= dr;
			}
		}

		private ArrayList _delegates; //本体の配列
		private Hashtable _index;     //名前文字列からindexを引く

		public ArithmeticLibrary() {
			_index = new Hashtable();
			_delegates = new ArrayList();
		}

		public void InitBuiltins() {
			_index.Clear();
			_delegates.Clear();

			DateRangeDelegate default_dr = new DateRangeDelegate(DateRangeDefault);
			Add("avg",      new EvalDelegate(CalcAverage),           default_dr);
			Add("sqrt",     new EvalDelegate(CalcSqrt),              default_dr);
			Add("pow",      new EvalDelegate(CalcPow),               default_dr);
			Add("log",      new EvalDelegate(CalcLog),               default_dr);
			Add("sdev",     new EvalDelegate(CalcStandardDeviation), default_dr);
			Add("first",    new EvalDelegate(CalcFirst),             default_dr);
			Add("last",     new EvalDelegate(CalcLast),              default_dr);
			Add("value_at", new EvalDelegate(CalcValueAt),             new DateRangeDelegate(DateRangeForValueAt));
			Add("each",     new EvalDelegate(CreateTimeSeriesForEach), new DateRangeDelegate(DateRangeForEach));
			Add("diff",     new EvalDelegate(CalcDiff),              default_dr);
			Add("sum",      new EvalDelegate(CalcSum),               default_dr);
			Add("min",      new EvalDelegate(CalcMin),               default_dr);
			Add("max",      new EvalDelegate(CalcMax),               default_dr);
			Add("suma",     new EvalDelegate(CalcSumAbsolute),       default_dr);
			Add("sump",     new EvalDelegate(CalcSumPositive),       default_dr);
			Add("sumn",     new EvalDelegate(CalcSumNegative),       default_dr);
			Add("series",   new EvalDelegate(CreateTimeSeries),      new DateRangeDelegate(DateRangeForSeries));
			Add("rankcorrelation", new EvalDelegate(CalcRankCorrelation), default_dr);
			Add("if",       new EvalDelegate(CalcIf),                new DateRangeDelegate(DateRangeForIf));
			Add("abs",      new EvalDelegate(CalcAbs),               default_dr);
			Add("pdm",      new EvalDelegate(CalcPDM),               default_dr);
			Add("mdm",      new EvalDelegate(CalcMDM),               default_dr);
			Add("sq_length",new EvalDelegate(CalcSQLength),          default_dr);
			Add("refbrand", new EvalDelegate(RefBrand),              new DateRangeDelegate(DateRangeForRefBrand));
			Add("unit_trading",new EvalDelegate(CalcUnitTrading),    default_dr);
			//hacked by rti
            Add("code", new EvalDelegate(CalcCode), default_dr);
            Add("uriage", new EvalDelegate(CalcUriage), default_dr);
            Add("keijyourieki", new EvalDelegate(CalcKeijyourieki), default_dr);
            Add("rieki", new EvalDelegate(CalcRieki), default_dr);
            Add("haitoukin", new EvalDelegate(CalcHaitoukin), default_dr);
            Add("hitoikabueki", new EvalDelegate(CalcHitoikabueki), default_dr);
            Add("hitokabujyunsisan", new EvalDelegate(CalcHitokabujyunsisan), default_dr);
            Add("rsq", new EvalDelegate(CalcRsq), default_dr);
            Add("corref", new EvalDelegate(CalcCorref), default_dr);
            //--

            Add("yobine", new EvalDelegate(CalcYobine), default_dr);
			Add("datalength",new EvalDelegate(CalcDataLength),       default_dr);

			//以下は非公開
			Add("sq_forecast", new EvalDelegate(CalcSQForecast), default_dr);
			Add("sqma_forecast", new EvalDelegate(CalcSQMAForecast), default_dr);
		}
		public int FindLaneID(string name) {
			object t = _index[name];
			if(t==null)
				return -1;
			else
				return (Int32)t;
		}
		public EvalResult Calc(int lane, Evaluator ev, Expression[] args) {
			return ((Entry)_delegates[lane]).function(ev, args);
		}
		public DateRange CalcDateRange(int lane, DateRangeChecker ch, Evaluator ev, Expression[] args) {
			Entry e = (Entry)_delegates[lane];
			return e.daterange==null? DateRange.Empty : e.daterange(ch, ev, args);
		}

		public void Add(string name, EvalDelegate fd, DateRangeDelegate dr) {
			Entry e = new Entry(name, _index.Count, fd, dr);
			if(_index.ContainsKey(name)) {
				int i = (int)_index[name];
				_delegates[i] = e;
			}
			else {
				_index.Add(name, _index.Count);
				_delegates.Add(e);
			}
		}

		private static EvalResult[] EvalExprs(Evaluator ev, Expression[] args) {
			EvalResult[] t = new EvalResult[args.Length];
			for(int i=0; i<t.Length; i++)
				t[i] = (EvalResult)args[i].Apply(ev);
			return t;
		}

		//個別の関数実装
		private static EvalResult CalcIf(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=3) throw new ArgumentException("if() must have 3 arguments");
			EvalResult t = (EvalResult)args[0].Apply(ev);
			if(!t.IsBool) throw new TypeMismatchException("first argument of if() must be a boolean value");
			if(t.BoolVal)
				return (EvalResult)args[1].Apply(ev);
			else
				return (EvalResult)args[2].Apply(ev);
		}
		private static EvalResult CalcAbs(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("abs() must have one argument");
			EvalResult t = (EvalResult)args[0].Apply(ev);
			if(t.IsDouble)
				return new EvalResult(Math.Abs(t.DoubleVal));
			else 
				return new EvalResult(new SingleOpTimeSeries(t.TimeSeriesVal, SingleOpTimeSeries.ABS));
		}
		private static EvalResult CalcSum(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("sum() must have one argument");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("sum() must be applied to TimeSeries");

			double sum = 0;
			TimeSeries.Cursor c = t.TimeSeriesVal.CreateCursor();
			while(c.HasNext)
				sum += c.Next;

			return new EvalResult(sum);
		}
		private static EvalResult CalcMin(Evaluator ev, Expression[] args) {
			if(args.Length==1) {
				EvalResult t = EvalExprs(ev, args)[0];
				if(!t.IsTimeSeries) throw new TypeMismatchException("min() must be applied to TimeSeries");
				return new EvalResult(t.TimeSeriesVal.Min);
			}
			else {
				double t = Double.MaxValue;
				foreach(Expression e in args) {
					EvalResult r = (EvalResult)e.Apply(ev);
					t = Math.Min(t, r.DoubleVal);
				}
				return new EvalResult(t);
			}
		}
		private static EvalResult CalcMax(Evaluator ev, Expression[] args) {
			if(args.Length==1) {
				EvalResult t = EvalExprs(ev, args)[0];
				if(!t.IsTimeSeries) throw new TypeMismatchException("max() must be applied to TimeSeries");

				return new EvalResult(t.TimeSeriesVal.Max);
			}
			else {
				double t = Double.MinValue;
				foreach(Expression e in args) {
					EvalResult r = (EvalResult)e.Apply(ev);
					t = Math.Max(t, r.DoubleVal);
				}
				return new EvalResult(t);
			}
		}
        private static EvalResult CalcAverage(Evaluator ev, Expression[] args)
        {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("avg() must have one argument");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("avg() must be applied to TimeSeries");

			return new EvalResult(t.TimeSeriesVal.Average);
		}
		private static EvalResult CalcSqrt(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("sqrt() must have one argument");
			EvalResult t = EvalExprs(ev, args)[0];
			if(t.IsDouble) {
				if(t.DoubleVal<0) throw new ArgumentException("sqrt() must be applied to positive value");
				return new EvalResult(Math.Sqrt(t.DoubleVal));
			}
			else if(t.IsTimeSeries) {
				return new EvalResult(new SingleOpTimeSeries(t.TimeSeriesVal, SingleOpTimeSeries.SQRT));
			}
			else
				throw new TypeMismatchException("sqrt() must be applied to value");
		}
		private static EvalResult CalcPow(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("sqrt() must have 2 arguments");
			EvalResult[] t = EvalExprs(ev, args);
			if(!t[1].IsDouble) throw new TypeMismatchException("the second arg of pow() must be a value");

			if(t[0].IsDouble)
				return new EvalResult(Math.Pow(t[0].DoubleVal, t[1].DoubleVal));
			else if(t[0].IsTimeSeries)
				return new EvalResult(new SingleOpTimeSeries(t[0].TimeSeriesVal, SingleOpTimeSeries.CreatePow(t[1].DoubleVal)));
			else
				throw new TypeMismatchException("pow() must be applied to a value or timeseries");
		}
		private static EvalResult CalcLog(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("log() must have 1 argument");
			EvalResult[] t = EvalExprs(ev, args);
			if(t[0].IsDouble)
				return new EvalResult(Math.Log(t[0].DoubleVal));
			else if(t[0].IsTimeSeries)
				return new EvalResult(new SingleOpTimeSeries(t[0].TimeSeriesVal, SingleOpTimeSeries.LOG));
			else
				throw new TypeMismatchException("log() must be applied to value");
		}
		private static EvalResult CalcUnitTrading(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=0) throw new ArgumentException("unit_trading() must not have arguments");

			BasicBrand br = ev.Farm.Brand as BasicBrand;
			return new EvalResult(br==null? 0 : br.Unit);
		}
		//hacked by rti
        //株価コードを返す
        private static EvalResult CalcCode(Evaluator ev, Expression[] args)
        {
			BasicBrand br = ev.Farm.Brand as BasicBrand;
			return new EvalResult(br==null? 0 : br.Code);
		}
        //ファンダメンタルを取得
        private static CodeExtendData.DataBasket GetFundamental(Evaluator ev, Expression[] args)
        {

            bool isFixed = false;
            int skip = 0;
            //オプション 
            if (Util.SafeArgLength(args) >= 1)
            {//一番目は、過去の決算 ディフォルトは0 (本年)
                //あんまり過去にさかのぼりすぎると情報がなくて空を返される.
                EvalResult t = EvalExprs(ev, args)[0];
                skip = (int)t.DoubleVal;
                if (Util.SafeArgLength(args) >= 2)
                {//二番目は 1 だったら、すでに確定した決算
                    //もし、連結を選択していて連結がなかった場合は、単体決算が返される
                    t = EvalExprs(ev, args)[1];
                    if ((int)t.DoubleVal == 1)
                    {//
                        isFixed = true;
                    }
                }
            }

            TradeData td = ev.Farm.GetByIndex(ev.BaseIndex);
            if (isFixed)
            {//時期決算予想と業績修正による修正を反映したデータ
                return ev.Farm.CodeExtend.FindFixedFundamental(td.Date, skip);
            }
            else
            {//既に確定したデータ
                return ev.Farm.CodeExtend.FindFundamental(td.Date, skip);
            }
        }
        //売上
        private static EvalResult CalcUriage(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);
            return new EvalResult(basket.Uriage);
        }
        //経常利益
        private static EvalResult CalcKeijyourieki(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);
            return new EvalResult(basket.Keijyourieki);
        }
        //利益(純利益?)
        private static EvalResult CalcRieki(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);
            return new EvalResult(basket.Rieki);
        }
        //配当
        private static EvalResult CalcHaitoukin(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);
            return new EvalResult(basket.Haitoukin);
        }
        //一株益
        private static EvalResult CalcHitoikabueki(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);

            //途中で株式分割をしているかもしれないので補正する.
            BasicBrand br = ev.Farm.Brand as BasicBrand;
            TradeData td = ev.Farm.GetByIndex(ev.BaseIndex);
            double split = br.CalcSplitRatio(basket.Kettusan, td.Date);
            
            return new EvalResult(basket.Hitoikabueki / split);
        }
        //一株純資産
        private static EvalResult CalcHitokabujyunsisan(Evaluator ev, Expression[] args)
        {
            CodeExtendData.DataBasket basket = GetFundamental(ev, args);

            //途中で株式分割をしているかもしれないので補正する.
            BasicBrand br = ev.Farm.Brand as BasicBrand;
            TradeData td = ev.Farm.GetByIndex(ev.BaseIndex);
            double split = br.CalcSplitRatio(basket.Kettusan, td.Date);

            return new EvalResult(basket.Hitokabujyunsisan / split);
        }
		private static EvalResult CalcRsq(Evaluator ev, Expression[] args) {
            if(Util.SafeArgLength(args)!=2) throw new ArgumentException("rsq() must have two argument");
			EvalResult t = EvalExprs(ev, args)[0];
            if (!t.IsTimeSeries) throw new TypeMismatchException("rsq() must be applied to TimeSeries");

            EvalResult t2 = EvalExprs(ev, args)[1];
            if (!t2.IsTimeSeries) throw new TypeMismatchException("rsq() must be applied to TimeSeries");

            double rsq = Statistics.RSQ(t.TimeSeriesVal.ToArray(), t2.TimeSeriesVal.ToArray());
            return new EvalResult(rsq);
		}
		private static EvalResult CalcCorref(Evaluator ev, Expression[] args) {
            if (Util.SafeArgLength(args) != 2) throw new ArgumentException("corref() must have two argument");

            EvalResult t = EvalExprs(ev, args)[0];
            if (!t.IsTimeSeries) throw new TypeMismatchException("corref() must be applied to TimeSeries");

            EvalResult t2 = EvalExprs(ev, args)[1];
            if (!t2.IsTimeSeries) throw new TypeMismatchException("corref() must be applied to TimeSeries");

            double corref = Statistics.CORREF(t.TimeSeriesVal.ToArray(), t2.TimeSeriesVal.ToArray());
            return new EvalResult(corref);
        }
        //--------------------------------------

		private static EvalResult CalcYobine(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("yobine() must have 1 argument");
			double v = EvalExprs(ev, args)[0].DoubleVal;
			return new EvalResult(Util.Yobine(ev.Farm.Brand.Market, v));
		}
		private static EvalResult CalcDataLength(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=0) throw new ArgumentException("datalength() must not have arguments");
			return new EvalResult(ev.Farm.TotalLength);
		}

		private static EvalResult CalcFirst(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("first() must be applied to TimeSeries");

			TimeSeries.Cursor c = t.TimeSeriesVal.CreateCursor();
			return new EvalResult(c.Next);
		}
		private static EvalResult CalcLast(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("last() must be applied to TimeSeries");

			return new EvalResult(t.TimeSeriesVal.LastValue);
		}
		private static EvalResult CalcValueAt(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsDouble) throw new TypeMismatchException("argument of value_at() must be a number");

			int saved = ev.BaseIndex;
			ev.BaseIndex = saved - (int)t.DoubleVal;
			EvalResult res = (EvalResult)args[1].Apply(ev);
			ev.BaseIndex = saved;
			return res;
		}

		private static EvalResult CalcDiff(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("diff must be applied to TimeSeries");

			return new EvalResult(new DiffTimeSeries(t.TimeSeriesVal));
		}

		private static EvalResult CalcSumAbsolute(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("suma must be applied to TimeSeries");

			double s = 0;
			TimeSeries.Cursor c = t.TimeSeriesVal.CreateCursor();
			while(c.HasNext)
				s += Math.Abs(c.Next);
			return new EvalResult(s);
		}

		private static EvalResult CalcSumPositive(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("sump must be applied to TimeSeries");

			double s = 0;
			TimeSeries.Cursor c = t.TimeSeriesVal.CreateCursor();
			while(c.HasNext) {
				double v = c.Next;
				if(v>0) s += v;
			}
			return new EvalResult(s);
		}

		private static EvalResult CalcSumNegative(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("sumn must be applied to TimeSeries");

			double s = 0;
			TimeSeries.Cursor c = t.TimeSeriesVal.CreateCursor();
			while(c.HasNext) {
				double v = c.Next;
				if(v<0) s += v;
			}
			return new EvalResult(s);
		}

		//０を基準とする標準偏差を求める
		private static EvalResult CalcStandardDeviation(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			EvalResult t = EvalExprs(ev, args)[0];
			if(!t.IsTimeSeries) throw new TypeMismatchException("sdev must be applied to TimeSeries");
			double result = 0;
			TimeSeries s = t.TimeSeriesVal;
			TimeSeries.Cursor c = s.CreateCursor();
			int i = 0;
			while(c.HasNext) {
				double v = c.Next;
				result += v*v;
				i++;
			}
			
			return new EvalResult(Math.Sqrt(result / s.Count));
		}

		//RCI用に、順位と距離の差の２乗の和を計算
		//もうちょっと分解したほうがいいかもしれない。また同値があったときの順位修正はしていない
		private static EvalResult CalcRankCorrelation(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			TimeSeries ts = ((EvalResult)args[0].Apply(ev)).TimeSeriesVal;

			int i = 0;
			ArrayList s = new ArrayList();
			TimeSeries.Cursor c = ts.CreateCursor();
			while(c.HasNext) {
				double v = c.Next;
				s.Add(new RCEntry(i++, v));
			}
			s.Sort();
			
			i = 0;
			int result = 0;
			foreach(RCEntry ent in s) {
				int t = (ent.index - i++);
				result += t*t;
			}
			
			return new EvalResult((double)result);
		}

		//(長さ、Expression）によってbaseindexをずらしながらTimeSeriesを作成
		private static EvalResult CreateTimeSeriesForEach(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("arg count is wrong");
			int baseindex = ev.BaseIndex;
			int length = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			
			if(args[1] is FunctionExpression) {
				Indicator ind = Env.CurrentIndicators.FindFromAscii(((FunctionExpression)args[1]).Name);
				if(ind!=null)
					return new EvalResult(new IndicatorTimeSeries(ev.Farm, ind, ev.BaseIndex-length+1, ev.BaseIndex+1));
			}

			//仕方がないのでここで計算
			double[] result = new double[length];
			int org = ev.BaseIndex;
			for(int i=0; i<result.Length; i++) {
				ev.BaseIndex = org - result.Length + i + 1;
				result[i] = ((EvalResult)args[1].Apply(ev)).DoubleVal;
			}
			return new EvalResult(new RawTimeSeries(result));
		}

		//(始点、終点、引数Index、Expression)によって整数を引数にしたTimeSeriesを作成
		//!!本体のExpressionの形によっては他に依存しない結果になり、FunctionExpressionの中に格納できる
		private static EvalResult CreateTimeSeries(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=4) throw new ArgumentException("arg count is wrong");
			int start = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			int end = (int)((EvalResult)args[1].Apply(ev)).DoubleVal;
			int ai = (int)((EvalResult)args[2].Apply(ev)).DoubleVal;
			if(ai<0 || ev.Args==null || ev.Args.Length<=ai)
				ev.EnsureArgIndex(ai);
			double[] result = new double[end-start];
			for(int i=0; i<result.Length; i++) {
				ev.Args[ai] = new EvalResult((double)(start+i));
				result[i] = ((EvalResult)args[3].Apply(ev)).DoubleVal;
			}
			return new EvalResult(new RawTimeSeries(result));

		}

		internal class RCEntry : IComparable {
			public int index;
			public double value;

			public RCEntry(int i, double v) {
				index = i;
				value = v;
			}


			public int CompareTo(object obj) {
				double t = value - ((RCEntry)obj).value;
				return t>0? 1 : t<0? -1 : 0;
			}

		}

		//DMI関係
		private static EvalResult CalcPDM(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			int dt = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			TradeData td1 = ev.Farm.GetByIndex(ev.BaseIndex+dt-1);
			TradeData td2 = ev.Farm.GetByIndex(ev.BaseIndex+dt);
			double pdm = td2.High - td1.High;
			double mdm = td1.Low - td2.Low;
			double r = 0;
			if(pdm > 0)
				r = pdm>mdm? pdm : 0;
			return new EvalResult(r);
		}
		private static EvalResult CalcMDM(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=1) throw new ArgumentException("arg count is wrong");
			int dt = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			TradeData td1 = ev.Farm.GetByIndex(ev.BaseIndex+dt-1);
			TradeData td2 = ev.Farm.GetByIndex(ev.BaseIndex+dt);
			double pdm = td2.High - td1.High;
			double mdm = td1.Low - td2.Low;
			double r = 0;
			if(mdm > 0)
				r = mdm>pdm? mdm : 0;
			return new EvalResult(r);
		}

		//他銘柄の参照
		private static EvalResult RefBrand(Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("arg count of refbrand() is wrong");
			int code = (int)((EvalResult)args[0].Apply(ev)).DoubleVal; //ちょっと強引
			AbstractBrand br = Env.BrandCollection.FindBrand(code);
			if(br==null) throw new ArgumentException(String.Format("参照されている銘柄 {0} がみつかりません。", code));

			DataFarm saved = ev.Farm;
			int saved_index= ev.BaseIndex;
			EvalResult result;
			ev.Farm = Env.BrandCollection.ReserveFarm(br);
			if(!ev.Farm.IsEmpty) {
				ev.BaseIndex = ev.Farm.DateToIndex(saved.GetByIndex(saved_index).Date); //!!高速化の余地ある
				result = (EvalResult)args[1].Apply(ev);
			}
			else { //0を返しておく
				result = new EvalResult(0);
			}
			ev.Farm = saved;
			ev.BaseIndex = saved_index;
			return result;
		}

		//DateRangeチェッカ
		private static DateRange DateRangeDefault(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			DateRange r = null;
			if(args!=null) {
				foreach(Expression ex in args) {
					if(r==null)
						r = (DateRange)ex.Apply(ch);
					else
						r.Merge((DateRange)ex.Apply(ch));
				}
			}
			return r;
		}
		private static DateRange DateRangeForValueAt(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("argument count of 'value_at' must be 2");
			DateRange r = (DateRange)args[1].Apply(ch);
			if(r==null) r = DateRange.Empty;
			int t = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			r.begin -= t;
			r.end -= t;
			return r;
		}
		private static DateRange DateRangeForEach(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=2) throw new ArgumentException("argument count of 'each' must be 2");
			int length = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			
			DateRange r = (DateRange)args[1].Apply(ch);
			r.begin -= length;
			return r;
		}
		private static DateRange DateRangeForSeries(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=4) throw new ArgumentException("argument count of 'series' must be 4");
			int start = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			int end = (int)((EvalResult)args[1].Apply(ev)).DoubleVal;
			int ai = (int)((EvalResult)args[2].Apply(ev)).DoubleVal;
			if(ai<0 || ev.Args==null || ev.Args.Length<=ai) { //ai<0が何なのかよくわからなくなってきた
				ch.EnsureArgIndex(ai);
			}
			DateRange r = DateRange.Empty;
			for(int i=start; i<end; i++) {
				ch.Args[ai] = new EvalResult((double)i);
				r.Merge((DateRange)args[3].Apply(ch));
			}
			return r;
		}
		private static DateRange DateRangeForIf(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			if(Util.SafeArgLength(args)!=3) throw new ArgumentException("argument count of 'if' must be 3");
			DateRange r0 = (DateRange)args[0].Apply(ch);
			DateRange r1 = (DateRange)args[1].Apply(ch);
			DateRange r2 = (DateRange)args[2].Apply(ch);
			
			r2 = r2==null? r0 : r2.Merge(r0);
			return r1==null? r2 : r1.Merge(r2);
		}
		private static DateRange DateRangeForRefBrand(DateRangeChecker ch, Evaluator ev, Expression[] args) {
			return DateRange.Empty; //!!これではうまくいかないこともあるかも
		}

		//SQまでの日にち
		private static EvalResult CalcSQLength(Evaluator ev, Expression[] args) {
			int today = ev.Farm.GetByIndex(ev.BaseIndex).Date;
			int l = 0;
			DateTime dt = Util.IntToDate(today);
			do {
				if(dt.DayOfWeek==DayOfWeek.Thursday && (dt.Day>=7 && dt.Day<=13)) break;
				if(Util.IsMarketOpenDate(dt)) l++;
				dt = dt.AddDays(1);
			} while(true);

			return new EvalResult(l);
		}

		//以下、非公開関数用

		private static double[] sqma_changes_p = { 1.82, 2.63, 3.29, 3.73, 4.27, 4.54, 4.86, 5.17, 5.53, 5.84, 6.28, 6.55, 6.85, 7.50, 7.03 };
		private static double[] sqma_changes_n = { -2.05, -2.90, -3.28, -4.10, -4.59, -4.69, -5.18, -5.21, -5.26, -5.60, -5.69, -5.64, -5.76, -6.09, -6.41 };
		//SQ日の移動平均を求める
		private static EvalResult CalcSQMAForecast(Evaluator ev, Expression[] args) {
			EvalResult l0 = CalcSQLength(ev, args);
			int l = (int)l0.DoubleVal;
			if(l>=15) throw new TradeDataOverflowException("sqma");

			int n = (int)((EvalResult)args[0].Apply(ev)).DoubleVal;
			double sum = 0;
			for(int i=ev.BaseIndex - n + 1 + l; i<=ev.BaseIndex; i++)
				sum += ev.Farm.GetByIndex(i).Close;

			bool plus = ((EvalResult)args[1].Apply(ev)).DoubleVal > 0;
			sum += ev.Farm.GetByIndex(ev.BaseIndex).Close * (l + (plus? sqma_changes_p[l] : sqma_changes_n[l])/100 * (l+1) / 2);
			return new EvalResult(sum / n);
		}
		private static EvalResult CalcSQForecast(Evaluator ev, Expression[] args) {
			EvalResult l0 = CalcSQLength(ev, args);
			int l = (int)l0.DoubleVal - 1;
			if(l<0 || l>=15) throw new TradeDataOverflowException("sqma");

			bool plus = ((EvalResult)args[0].Apply(ev)).DoubleVal > 0;
			double v = ev.Farm.GetByIndex(ev.BaseIndex).Close * (1 + (plus? sqma_changes_p[l] : sqma_changes_n[l])/100);
			return new EvalResult(v);
		}
	}

	//ユーザ定義の関数
	internal class ExtendedLibraryFunction {
		private Expression _body;
		private string _name;
		private int _numOfParams;

		public ExtendedLibraryFunction(string name, string body) {
			_name = name;
			ExpressionConstructor ec = new ExpressionConstructor();
			new ZPredicationParser(new StringReader(body), ec).Parse();
			_body = ec.Result;
			_numOfParams = ec.NumOfParams;
		}

		public EvalResult Calc(Evaluator ev, Expression[] args) {
			if(_numOfParams!=Util.SafeArgLength(args))
				throw new ArgumentException(String.Format("{0} は引数の数が {1} でなければなりませんが、実際には {2} 個です。", _name, _numOfParams, Util.SafeArgLength(args)));
			Evaluator e2 = new Evaluator(_name);
			e2.BaseIndex = ev.BaseIndex;
			e2.Farm = ev.Farm;
			e2.Args = ev.Eval(args);
			return (EvalResult)_body.Apply(e2);
		}

		public DateRange CalcDateRange(DateRangeChecker ch, Evaluator evaluator, Expression[] args) {
			if(_numOfParams!=Util.SafeArgLength(args))
				throw new ArgumentException(String.Format("{0} は引数の数が {1} でなければなりませんが、実際には {2} 個です。", _name, _numOfParams, Util.SafeArgLength(args)));
			//引数が日数範囲として使われる場合もあり、そのときはEvalが必要だが、引数で銘柄固有データが使われていることもあるので悩ましい。
			try {
				DateRangeChecker c2 = new DateRangeChecker(_name, evaluator.Eval(args));
				return (DateRange)_body.Apply(c2);
			}
			catch(Exception ) {
				//引数についてチェックするのみ
				DateRange r = DateRange.Empty;
				foreach(Expression e in args) {
					r = r.Merge((DateRange)e.Apply(ch));
				}
				return r;
			}
		}
	}
}
