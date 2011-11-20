/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Diagnostics;

using Travis.Util;
using Zanetti.Arithmetic;
using Zanetti.Indicators;
using Zanetti.Data;
//hacked by rti
using Zanetti.SystemTrading.MoneyPlanning;
//-----------

namespace Zanetti.SystemTrading.AutoTrading
{
    //hacked by rti
    //アナリストさんたち、、、、なぜにデータの読み込み IndicatorSchema.cs と データ AutoTrading.cs と、別のクラスでの実装と、別にするのかは謎(w
    internal class AutoTradingAnalystItem
    {
        private AnalystType _Type;   //アナリストがいつ動きだすのか定義する
        private string _Name;        //アナリストの名前 カラム名になる
        private Expression _Expression;  //評価する式

        public AutoTradingAnalystItem(AnalystType type, string name, Expression expression)
        {
            _Type = type;
            _Name = name;
            _Expression = expression;
        }
        public AnalystType Type
        {
            get
            {
                return _Type;
            }
        }
        public string Name
        {
            get
            {
                return _Name;
            }
        }
        public Expression Expression
        {
            get
            {
                return _Expression;
            }
        }
    }
    //------

	internal class AutoTradingItem {
		private TradingType _type;
		private EntryType _entryType;
		private string _title;
		private string _header;
		private Expression _signal;
		private Expression _gyakusashine; //逆指値の式。使わないときはnull
		private Expression _exit;
		private Expression _losscut;
		private EvalResult[] _args;
        //hacked by rti
        private IntervalType _intervalType;
        protected ArrayList _AnalystArray;   //アナリストさんたち
        //-----

        public AutoTradingItem(string title, string header, TradingType type, EvalResult[] args, Expression s, EntryType et, Expression g, Expression e, Expression l, IntervalType intervalType, ArrayList analystArray)
        {
			_title = title;
			_header = header;
			_type = type;
			_entryType = et;
			_args = args;
			_signal = s;
			_gyakusashine = g;
			_exit = e;
			_losscut = l;
            //hacked by rti
            _intervalType = intervalType;
            _AnalystArray = analystArray;
            //--------------
			Debug.Assert(_entryType==EntryType.Gyakusashine ^ g==null); //どちらか１つだけが成立していること
		}


		public string Title {
			get {
				return _title;
			}
		}
		public string Header {
			get {
				return _header;
			}
		}
		public TradingType TradingType {
			get {
				return _type;
			}
		}
		public EntryType EntryType {
			get {
				return _entryType;
			}
		}
        public IntervalType IntervalType
        {
            get
            {
                return _intervalType;
            }
        }

		public Expression Signal {
			get {
				return _signal;
			}
		}
		public Expression Gyakusashine {
			get {
				return _gyakusashine;
			}
		}

		public Expression Exit {
			get {
				return _exit;
			}
		}
		public Expression Losscut {
			get {
				return _losscut;
			}
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
		}
        //hacked by rti
        public int GetAnalystCount()
        {
            return _AnalystArray.Count;
        }
        public AutoTradingAnalystItem GetAnalyst(int index)
        {
            return (AutoTradingAnalystItem)_AnalystArray[index];
        }
        //--------
		
	}

	internal enum SignalResultType {
		Unknown,
		Exit,
		Losscut,
		Draw,
		Skip //逆指値で約定せず
	}
	internal class SignalResult : SystemTradingResultEntry {
		private SignalResultType _result;
		private int _startDate;
		private double _startPrice;
		private int _endDate;
		private double _endPrice;
		private int _length;

        //hacked by rti
        //この結果について、アナリストさんたちの意見 
        //アナリストが何人いるかわからないので、面倒なので連想配列にしてしまえ...
        private Hashtable _AnalystComment = new Hashtable();
        //-------

		private double _gyalisahineEntryValue;


        //hacked by rti
        //index番目のアナリストさんにご意見をいただく
        public void SetAnalystComment(int index , string comment)
        {
            _AnalystComment[index] = comment;
        }
        //inedx番目のアナリストさんのご意見は?
        public string GetAnalystComment(int index)
        {
            return (string)_AnalystComment[index];
        }
        //--------


		public SignalResultType Result {
			get {
				return _result;
			}
			set {
				_result = value;
			}
		}
		public int StartDate {
			get {
				return _startDate;
			}
			set {
				_startDate = value;
			}
		}
		public double StartPrice {
			get {
				return _startPrice;
			}
			set {
				_startPrice = value;
			}
		}
		public int EndDate {
			get {
				return _endDate;
			}
			set {
				_endDate = value;
			}
		}
		public double EndPrice {
			get {
				return _endPrice;
			}
			set {
				_endPrice = value;
			}
		}
		public int Length {
			get {
				return _length;
			}
			set {
				_length = value;
			}
		}

		public double GyalisahineEntryValue {
			get {
				return _gyalisahineEntryValue;
			}
			set {
				_gyalisahineEntryValue = value;
			}
		}
				


		public double Performance(TradingType tt) {
			double p = (_endPrice - _startPrice) / _startPrice;
			if(tt==TradingType.Short) p = -p;
			return p;
		}

		public SignalResult(AbstractBrand br) : base(br) {
		}

		//hacked by rti
		//Close() を読んでしまうと、 _endPrice と _endDate が上書きされてしまう、、、
		//ここでの _endPrice は _endDate  利確(exit)の情報らしい。
		//損切りは、ここではないどっかで先に損切りの情報が _endPrice と _endDate  に書き込まれているらしい。
		//なんで、ここではドローってことだけを記録することにする、、、ってこれでいいのか?
        public void DrawClose(SignalResultType result)
        {
            _result = result;
        }
        //----
        
        public void Close(SignalResultType result, double endprice, int enddate) {
			_result = result;
			_endPrice = endprice;
			_endDate = enddate;
		}
		public override int CompareTo(object obj) {
			//銘柄、日付の順
			int n = _brand.Code - ((SignalResult)obj)._brand.Code;
			if(n!=0)
				return n;
			else
				return _startDate - ((SignalResult)obj)._startDate;
		}

	}

	internal class AutoTradingResult : SystemTradingResult {
		private AutoTradingItem _item;
        private MoneyPlanningItem _MoneyPlanningItem;

		private int _startDate;
		private int _endDate;
		private int _totalCheckCount;
		
		public const int MAX_SIGNAL_COUNT = 1000;

        //hacked by rti
//        public AutoTradingResult(AutoTradingItem item,  int sd, int ed) : base(MAX_SIGNAL_COUNT)
//        {
//			_item = item;
//			_startDate = sd;
//			_endDate = ed;
//		}
        public AutoTradingResult(AutoTradingItem item, MoneyPlanningItem moneyPlanningItem, int sd, int ed)
            : base(MAX_SIGNAL_COUNT)
        {
            _item = item;
            _MoneyPlanningItem = moneyPlanningItem;
            _startDate = sd;
            _endDate = ed;
        }
        //----------------------------

        public void AddSignal(SignalResult t)
        {
			_sortRequired = true;
			_data.Add(t);
		}
		public AutoTradingItem Item {
			get {
				return _item;
			}
		}
        //hacked by rti
        public MoneyPlanningItem MoneyPlanningItem
        {
            get
            {
                return _MoneyPlanningItem;
            }
        }
        //---------------------
		public int TotalCheckCount {
			get {
				return _totalCheckCount;
			}
		}
		public int StartDate {
			get {
				return _startDate;
			}
		}
		public int EndDate {
			get {
				return _endDate;
			}
		}
		public void AddCheckCount(bool signal) {
			_totalCheckCount++;
		}

		public SignalResult GetAtS(int index) {
			return base.GetAt(index) as SignalResult;
		}

		public AutoTradingResultSummary CreateSummary() {
			return new AutoTradingResultSummary(this);
		}
	}
	internal class AutoTradingResultSummary {
		//いちいちプロパティにするまでもないのでpublic
		public double _avgTradeLength; //平均何日間トレードしたか
		public int _exitCount;  //判定不能があるのでwinCount,loseCountを足してもResultCountには一致しない
		public int _losscutCount;
		public int _undecidableCount;
		public int _totalCount;
		public double _avgExit;
		public double _sdExit;
		public double _avgExitTradeLength;
		public double _avgLosscut;
		public double _sdLosscut;
		public double _avgLosscutTradeLength;
		public double _avgPerformance;
		public double _totalPerformance;
		public double _sdTotal;
		public SignalResult _maxExit;
		public SignalResult _maxLosscut;

		private void Construct(AutoTradingResult r) {
			Phase1(r); //平均値など
			Phase2(r); //標準偏差
		}
		private void Phase1(AutoTradingResult r) {
			int total_length = 0, total_exit_length = 0, total_losscut_length = 0;
			double sum_exit = 0, sum_losscut = 0;
			double max_exit = Double.MinValue, max_losscut = Double.MaxValue;
			for(int i=0; i<r.ResultCount; i++) {
				SignalResult sr = r.GetAtS(i);
				double p = sr.Performance(r.Item.TradingType);
				
				//hacked by rti.
                /*
				if(sr.Result==SignalResultType.Exit) {
					total_length += sr.Length;
					total_exit_length += sr.Length;
					_totalCount++;
					_exitCount++;
					sum_exit += p;
					_totalPerformance += p;
					if(p > max_exit) {
						max_exit = p;
						_maxExit = sr;
					}
				}
				else if(sr.Result==SignalResultType.Losscut) {
					total_length += sr.Length;
					total_losscut_length += sr.Length;
					_totalCount++;
					_losscutCount++;
					sum_losscut += p;
					_totalPerformance += p;
					if(p < max_losscut) {
						max_losscut = p;
						_maxLosscut = sr;
					}
				}
				else
					_undecidableCount++;
                */
                //結果が出たやつは集計する
                if ( sr.Result==SignalResultType.Exit ||
                    sr.Result==SignalResultType.Losscut || 
                    sr.Result==SignalResultType.Draw  ) 
                {
                    if (p > 0)
                    {
                        total_length += sr.Length;
                        total_exit_length += sr.Length;
                        _totalCount++;
                        _exitCount++;
                        sum_exit += p;
                        _totalPerformance += p;
                        if (p > max_exit)
                        {
                            max_exit = p;
                            _maxExit = sr;
                        }
                    }
                    else
                    {
                        total_length += sr.Length;
                        total_losscut_length += sr.Length;
                        _totalCount++;
                        _losscutCount++;
                        sum_losscut += p;
                        _totalPerformance += p;
                        if (p < max_losscut)
                        {
                            max_losscut = p;
                            _maxLosscut = sr;
                        }
                    }
                }
                //結果が不明(約定しなかった、売る暇がなかった)
                else
                {
                    _undecidableCount++;
                }
                //----

			}

			if(_totalCount   !=0) _avgTradeLength        = (double)total_length / _totalCount;
			if(_exitCount    !=0) _avgExit               = sum_exit / _exitCount;
			if(_losscutCount !=0) _avgLosscut            = sum_losscut / _losscutCount;
			if(_exitCount    !=0) _avgExitTradeLength    = total_exit_length / _exitCount;
			if(_losscutCount !=0) _avgLosscutTradeLength = total_losscut_length / _losscutCount;
			if(_totalCount   !=0) _avgPerformance        = _totalPerformance / _totalCount;
		}
		private void Phase2(AutoTradingResult r) {
			double exit_sd_sum = 0, losscut_sd_sum = 0, total_sd_sum = 0;
			for(int i=0; i<r.ResultCount; i++) {
				SignalResult sr = r.GetAtS(i);
				double p = sr.Performance(r.Item.TradingType);
				if(sr.Result==SignalResultType.Exit) {
					double d = p - _avgExit;
					exit_sd_sum += d*d;
					d = p - _avgPerformance;
					total_sd_sum += d*d;
				}
				else if(sr.Result==SignalResultType.Losscut) {
					double d = p - _avgLosscut;
					losscut_sd_sum += d*d;
					d = p - _avgPerformance;
					total_sd_sum += d*d;
				}
			}

			if(_exitCount!=0)    _sdExit    = Math.Sqrt(exit_sd_sum / _exitCount);
			if(_losscutCount!=0) _sdLosscut = Math.Sqrt(losscut_sd_sum / _losscutCount);
			if(_totalCount!=0)   _sdTotal   = Math.Sqrt(total_sd_sum / _totalCount);
		}

		public AutoTradingResultSummary(AutoTradingResult tr) {
			Construct(tr);
		}
	}

	internal class AutoTradingExecutor : SystemTradingExecutor {
		private AutoTradingItem _item;
        //hacked by rti
        private MoneyPlanningItem _MoneyPlanningItem;
        //--------------
		private int _startDate;
		private int _endDate;
		private int _signalCountLimit;
	
		private AutoTradingResult _result;
		private AutoTradingEvaluator _evaluator;

        //hacked by rti
//		public AutoTradingExecutor(AutoTradingItem item, int startDate, int endDate, int signalCountLimit) : base(item.Title) {
//			_item = item;
//			_startDate = startDate;
//			_endDate = endDate;
//			_signalCountLimit = signalCountLimit;
//		}
        public AutoTradingExecutor(AutoTradingItem item, MoneyPlanningItem moneyPlanningItem, int startDate, int endDate, int signalCountLimit)
            : base(item.Title)
        {
            _item = item;
            _MoneyPlanningItem = moneyPlanningItem;
            _startDate = startDate;
            _endDate = endDate;
            _signalCountLimit = signalCountLimit;
        }
        //------------------------------
        public AutoTradingItem Item
        {
			get {
				return _item;
			}
		}
        //hacked by rti
        public MoneyPlanningItem MoneyPlanningItem
        {
            get
            {
                return _MoneyPlanningItem;
            }
        }
        //---------------------
        public int StartDate
        {
			get {
				return _startDate;
			}
		}
		public int EndDate {
			get {
				return _endDate;
			}
		}
		public int SignalCountLimit {
			get {
				return _signalCountLimit;
			}
		}

        protected override void BeforeExecute() {
            //hacked by rti
			//_result = new AutoTradingResult(_item, _startDate, _endDate);
            _result = new AutoTradingResult(_item, _MoneyPlanningItem , _startDate, _endDate);
            //------------------------
		}

		public override SystemTradingResult Result {
			get {
				return _result;
			}
		}

		protected override ExecuteBrandResult ExecuteBrand(AbstractBrand br) {
			try {
                //hacked by rti キャッシュが効くようにする
				DataFarm farm = br.CloneFarm(ChartFormat.Daily);
                //DataFarm farm = br.CreateFarm(ChartFormat.Daily);
                //-----------

				//まず日付範囲チェック
				DateRangeChecker ch = new AutoTradingDateRangeChecker(_item.Title, _item.Args);
				DateRange sc = (DateRange)_item.Signal.Apply(ch);

				/* //!! value_at(day(),...)問題のためDateRangeチェックはstaticにできない
				DateRange exitDateRange    = (DateRange)_item.Exit.Apply(ch);
				DateRange losscutDateRange = (DateRange)_item.Losscut.Apply(ch);
				*/

				_evaluator = new AutoTradingEvaluator(_item.Title, _item);
				_evaluator.Farm = farm;
				_evaluator.Args = _item.Args;
				int start_index = farm.DateToIndex(_startDate);
				int end_index   = farm.DateToIndex(_endDate);

				SignalResult current = null;
				int index_on_signal = 0;
				for(int i = start_index; i<end_index; i++) {
					_result.AddCheckCount(current!=null);
					_evaluator.BaseIndex = i;

					if(current==null) { //シグナルチェック中
						if(i+sc.begin>=0 && i+sc.end<=farm.FilledLength) {
							current = CheckOneTradeData(farm.GetByIndex(i));
							_evaluator.CurrentSignal = current;
							if(current!=null) index_on_signal = i;
						}
					}
					else {
						TradeData td = farm.GetByIndex(i);
						if(_item.Gyakusashine!=null && td.Index==index_on_signal+1) { //逆指値使用時
							if(CheckGyakusashineEntry(current, td, index_on_signal)) {
								continue; //Lengthは加算せずに次の日へ
							}
							else
								current.Result = SignalResultType.Skip; //成立せず
						}

						current.Length++;
						if(CheckSignalFinish(current, td)) {
							_result.AddSignal(current);
							current = null;
                            //hacked by rti
                            //毎日エントリーするタイプだったら、エントリーした翌日に時間を戻す
                            if (_item.IntervalType == IntervalType.EveryDay)
                            {
                                i = index_on_signal;
                            }
                            //--------------------------
							if(_result.ResultCount>=_signalCountLimit) {
								_result.HasTooManuResults = true;
								return ExecuteBrandResult.TooManyResult;
							}
						}
					}

                    //hacked by rti
                    //次で終わりなら終了していないシグナルの処理
                    //なんでここで処理するかっていうと、毎日エントリーするタイプだと時間を戻さないといけないから。
                    if (current != null && i + 1 >= end_index)
                    {
                        current.Result = SignalResultType.Unknown;
                        _result.AddSignal(current);
                        current = null;
                        //毎日エントリーするタイプだったら、エントリーした翌日に時間を戻す
                        if (_item.IntervalType == IntervalType.EveryDay)
                        {
                            i = index_on_signal;
                        }
                    }
                    //-------------------------------------------
                }

                //hacked by rti
                /* //ループの中で処理する
				if(current!=null) { //もし終了していないシグナルが残留していたら
					current.Result = SignalResultType.Unknown;
					_result.AddSignal(current);
				}
                */
                //--------------
				return ExecuteBrandResult.Succeeded;
			}
			catch(ZArithmeticException) {
				throw;
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.Message);
				Debug.WriteLine(ex.StackTrace);
				return ExecuteBrandResult.DataError;
			}
		}

		//シグナルが成立したらSignalResultオブジェクトを作成して返す
		private SignalResult CheckOneTradeData(TradeData td) {
			EvalResult signal = (EvalResult)_item.Signal.Apply(_evaluator);
			if(!signal.IsBool) throw new ZArithmeticException("自動売買の条件式は bool 型の値を返すものでなくてはなりません。");
			if(signal.BoolVal) {
				SignalResult sr = new SignalResult(td.Farm.Brand);
				FillStartData(sr, td);
				return sr;
			}
			else
				return null;
		}

		//シグナルの終了チェック
		private bool CheckSignalFinish(SignalResult current, TradeData td) {
			if(current.Result==SignalResultType.Skip) return true; //すでに終了

			_evaluator.CheckingExit = true;
			EvalResult exit    = (EvalResult)_item.Exit.Apply(_evaluator);
			_evaluator.CheckingExit = false;
			EvalResult losscut = (EvalResult)_item.Losscut.Apply(_evaluator);
			if(!exit.IsBool) throw new ZArithmeticException("自動売買の利益確定式は bool 型の値を返すものでなくてはなりません。");
			if(!losscut.IsBool) throw new ZArithmeticException("自動売買のロスカット式は bool 型の値を返すものでなくてはなりません。");

			if(exit.BoolVal) {
				if(losscut.BoolVal)
					FillEndData(current, SignalResultType.Draw, td);
				else if(current.Result==SignalResultType.Unknown) //指値のときは上のApplyの過程で成立していることもある
					FillEndData(current, SignalResultType.Exit, td);
				return true;
			}
			else if(losscut.BoolVal) {
				if(current.Result==SignalResultType.Unknown)
					FillEndData(current, SignalResultType.Losscut, td);
				return true;
			}
			else {
				return false; //継続
			}
		}

		//逆指値に入るかどうかのチェック
		private bool  CheckGyakusashineEntry(SignalResult current, TradeData td, int base_index) {
			Debug.Assert(_item.Gyakusashine!=null);
			_evaluator.BaseIndex = base_index;
			EvalResult gs = (EvalResult)_item.Gyakusashine.Apply(_evaluator);
			if(!gs.IsDouble) throw new ZArithmeticException("逆指値は数値型の値を返すものでなくてはなりません。");

			double v = Util.RoundToYobine(_evaluator.Farm.Brand.Market, gs.DoubleVal);
			if(v < td.Low || v > td.High)
				return false; //発動せず
			else {
				//Ｘ円以上でＸ円指値、というタイプなのでこれでよい。あまり複雑化させても仕方ない
				FillStartDataGyakusashine(current, td, v);
				return true;
			}
		}

		//hacked by rti
		/*
		private void FillEndData(SignalResult current, SignalResultType type, TradeData td) {
			if(_item.EntryType==EntryType.TodayClose || _item.EntryType==EntryType.Gyakusashine) {
				current.Close(type, td.Close, td.Date);
			}
			else if(_item.EntryType==EntryType.TomorrowOpen) {
				current.Close(type, td.Next.Open, td.Next.Date);
			}
		}
		*/
		private void FillEndData(SignalResult current, SignalResultType type, TradeData td) {
            //ここで値を td に上書きしてしまうと、DRAWの場合(ロストカットと利益確定が同時の場合)、
            //なぜか、利益確定の値になってしまうので、
            //正しくロストカットの値の方にするように修正する
            if (type == SignalResultType.Draw)
            {
                current.DrawClose(type);
            }
			else if(_item.EntryType==EntryType.TodayClose || _item.EntryType==EntryType.Gyakusashine) {
				current.Close(type, td.Close, td.Date);
			}
			else if(_item.EntryType==EntryType.TomorrowOpen) {
				current.Close(type, td.Next.Open, td.Next.Date);
			}


            //hacked by rti
            //結果について、アナリストさんに意見を求める
            if (type == SignalResultType.Draw)
            {
                InterviewAnalyst(current, AnalystType.Exit);
                InterviewAnalyst(current, AnalystType.Losscut);
            }
            else if (type == SignalResultType.Exit)
            {
                InterviewAnalyst(current, AnalystType.Exit);
            }
            else if (type == SignalResultType.Losscut)
            {
                InterviewAnalyst(current, AnalystType.Losscut);
            }
            //------
		}
		//-----
		
		
		private void FillStartData(SignalResult current, TradeData td) {
			if(_item.EntryType==EntryType.TodayClose || _item.EntryType==EntryType.Gyakusashine) {
				current.StartDate = td.Date;
				current.StartPrice = td.Close;
			}
			else if(_item.EntryType==EntryType.TomorrowOpen) {
				current.StartDate = td.Next.Date;
				current.StartPrice = td.Next.Open;
            }
            //hacked by rti
            //シグナルの成立について、アナリストさんに意見を求める
            InterviewAnalyst(current, AnalystType.Signal);
            //------
        }
		private void FillStartDataGyakusashine(SignalResult current, TradeData td, double value) {
			current.StartPrice = value;
            //hacked by rti
            //逆指値の成立について、アナリストさんに意見を求める
            InterviewAnalyst(current, AnalystType.Gyakusashine);
            //------
		}
		//hacked by rti
        //アナリストさんにコメントを求める
        private void InterviewAnalyst(SignalResult current, AnalystType type)
        {
            for(int i = 0 ; i < _item.GetAnalystCount() ; i++)
            {
                AutoTradingAnalystItem ana = _item.GetAnalyst(i);
                //この件について、コメントしてください、、、
                if (ana.Type == type)
                {
                    //ずはり言うわよ!!
                    EvalResult reslut = (EvalResult)ana.Expression.Apply(_evaluator);
                    current.SetAnalystComment( i , reslut.ToString("F0") );
                    reslut = null;
                }
            }
        }
        //------
	}

	internal class AutoTradingEvaluator : Evaluator {
		
		private AutoTradingItem _item;
		private SignalResult _currentSignal;
		private bool _checkingExit; //利益確定のチェックかロスカットのチェックか
		
		public SignalResult CurrentSignal {
			get {
				return _currentSignal;
			}
			set {
				_currentSignal = value;
			}
		}
		public bool CheckingExit {
			get {
				return _checkingExit;
			}
			set {
				_checkingExit = value;
			}
		}

		public AutoTradingEvaluator(string name, AutoTradingItem item) : base(name) {
			_item = item;
		}

		public override object Function(FunctionExpression expr) {
			if(expr.Category==FunctionExpression.FECategory.Unknown) { //名前の解決から。組み込みライブラリと定義済みIndicatorのどちらかでないといけない
				int i = FindAutoTradingFunctionID(expr.Name);
				if(i!=-1) {
					expr.Category = FunctionExpression.FECategory.Library;
					expr.LaneID = i;
				}
			}

			if(expr.Category==FunctionExpression.FECategory.Library && IsAutoTradingFunctionID(expr.LaneID)) 
				return CalcAutoTradingFunction(expr);

			return base.Function(expr);
		}

		private enum AutoTradingFunctionID {
			Start = 0x1000,
			Entry,
			Day,
			LimitOrder,
			End
		}

		private static int FindAutoTradingFunctionID(string name) {
			if(name=="entry")
				return (int)AutoTradingFunctionID.Entry;
			else if(name=="day")
				return (int)AutoTradingFunctionID.Day;
			else if(name=="limitorder")
				return (int)AutoTradingFunctionID.LimitOrder;
			else
				return -1;
		}
		private static bool IsAutoTradingFunctionID(int id) {
			return (int)AutoTradingFunctionID.Start < id && id < (int)AutoTradingFunctionID.End;
		}

		private EvalResult CalcAutoTradingFunction(FunctionExpression expr) {
			switch(expr.LaneID) {
				case (int)AutoTradingFunctionID.Entry:
					return new EvalResult(_currentSignal.StartPrice);
				case (int)AutoTradingFunctionID.Day:
					return new EvalResult(_currentSignal.Length);
				case (int)AutoTradingFunctionID.LimitOrder:
					if(Util.SafeArgLength(expr.Args)!=1) throw new ZArithmeticException("limitorderは引数が一つだけ許されます");
					return LimitOrder((EvalResult)expr.Args[0].Apply(this));
				default:
					Debug.Assert(false); //ここには来ない
					return null;
			}
		}
		private EvalResult LimitOrder(EvalResult value) {
			if(!value.IsDouble)
				throw new ZArithmeticException("limitorderの引数は数値でなければいけません");

			TradeData td = _farm.GetByIndex(_baseIndex);
			double price = value.DoubleVal;
			if(_item.TradingType==TradingType.Long ^ _checkingExit) { //買いのロスカット or 売りの利確
				if(price >= td.Low) {
					if(price > td.Open) price = td.Open; //急落時は始値の時点で条件を満たしている
					_currentSignal.Close(_item.TradingType==TradingType.Long? SignalResultType.Losscut : SignalResultType.Exit, Util.RoundToYobine(_farm.Brand.Market, price), td.Date);
					return new EvalResult(true);
				}
			}
			else { //売りのロスカット or 買いの利確
				if(price <= td.High) {
					if(price < td.Open) price = td.Open; //急騰時は始値の時点で条件を満たしている
					_currentSignal.Close(_item.TradingType==TradingType.Long? SignalResultType.Exit : SignalResultType.Losscut, Util.RoundToYobine(_farm.Brand.Market, price), td.Date);
					return new EvalResult(true);
				}
			}

			return new EvalResult(false);
		}

	}

	internal class AutoTradingDateRangeChecker : DateRangeChecker {
		public AutoTradingDateRangeChecker(string fn, EvalResult[] args) : base(fn, args) {
		}
		public override object Function(FunctionExpression expr) {
			if(expr.Name=="limitorder")
				return expr.Args[0].Apply(this);
			else if(expr.Name=="entry" || expr.Name=="day")
				return DateRange.Empty;
			else
				return base.Function(expr);
		}

		protected override Evaluator CreateEvaluator(string name) {
			//!!ここの構造で破綻。value_at(day(),...)のとき、DateRangeがstaticに決定できない！
			return new AutoTradingEvaluator(name, null);
		}

	}
}
