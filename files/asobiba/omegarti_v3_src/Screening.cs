/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Diagnostics;
using System.Threading;

using Travis.Util;
using Zanetti.Arithmetic;
using Zanetti.Data;
using Zanetti.Indicators;
using Zanetti.Config;		// for BookmarkFolder

namespace Zanetti.SystemTrading.Screening
{
	[EnumDesc(typeof(ResultCountType))]
	internal enum ResultCountType {
		[EnumValue(Description="上位・下位50銘柄ずつ")]
		Both50,
		[EnumValue(Description="上位100銘柄")]
		Top100,
		[EnumValue(Description="下位100銘柄")]
		Bottom100,
        [EnumValue(Description = "上位1000銘柄")]
		Top1000,
        [EnumValue(Description = "下位1000銘柄")]
		Bottom1000,
    }

    internal class ScreeningItem {
		private string _header;
		private Expression _expression;
		private Expression _filter;
		private EvalResult[] _args;
		private string _formatString;
		private FormatModifier _formatModifier;
		private Evaluator _eval;

		public ScreeningItem(string header, EvalResult[] args, Expression e, Expression f) {
			_header = header;
			_expression = e;
			_args = args;
			_filter = f;
			_eval = new Evaluator(header);
		}

		public string Header {
			get {
				return _header;
			}
		}

		public Expression Expression {
			get {
				return _expression;
			}
		}
		public Expression Filter {
			get {
				return _filter;
			}
		}
		public string FormatString {
			get {
				return _formatString;
			}
			set {
				_formatString = value;
			}
		}
		public FormatModifier FormatModifier {
			get {
				return _formatModifier;
			}
			set {
				_formatModifier = value;
			}
		}
		
		public EvalResult Calculate(DataFarm farm, int index) {
			_eval.Farm = farm;
			_eval.BaseIndex = index;
			_eval.Args = _args;
			EvalResult r = (EvalResult)_expression.Apply(_eval);
			if(!r.IsDouble) throw new ZArithmeticException("スクリーニングの式は数値を返すものでなくてはなりません。");
			return r;
		}

		public bool CheckRange(TradeData td) {
			int t = td.Index;

			DateRangeChecker ch = new DateRangeChecker(_header, _args);
			DateRange dr = (DateRange)_expression.Apply(ch);
			if(dr==null) return false;
			bool result = t+dr.begin>=0 && t+dr.end<=td.Farm.TotalLength;
			if(!result) return false; //ここでひっかかればアウト

			if(_filter==null) return true;
			ch = new DateRangeChecker(_header, _args);
			dr = (DateRange)_filter.Apply(ch);
			result = t+dr.begin>=0 && t+dr.end<=td.Farm.TotalLength;

			return result;
		}
		public bool CheckFilter(TradeData td) {
			if(_filter==null) return true; //フィルタなし

			_eval.Farm = td.Farm;
			_eval.BaseIndex = td.Index;
			_eval.Args = _args;
			EvalResult r = (EvalResult)_filter.Apply(_eval);
			//TODO: ここで式の内容を表示してエラーを通知するのがよい
			if(!r.IsBool) throw new ZArithmeticException("スクリーニングのフィルタの式は bool 型の値を返すものでなくてはなりません。");
			return r.BoolVal;
		}
	}

	internal class ScreeningOrder : SystemTradingExecutor {
		private ResultCountType _preferredResultCount;
		private int _originDate;

		private ScreeningItem _primaryItem; //ソート対象
		private ArrayList _secondaryItems; //補助的に表示
		private ScreeningResult _result;
        //hacked by rti
        //メンバにしなくてもいいんぢゃねぇ?
//		private DataFarm _tempFarm;
        //

		public ScreeningOrder(string name, ScreeningItem pr) : base(name) {
			_primaryItem = pr;
			_secondaryItems = new ArrayList();
		}
		public int OriginDate {
			get {
				return _originDate;
			}
			set {
				_originDate = value;
			}
		}
		public override SystemTradingResult Result {
			get {
				return _result;
			}
		}

		public void AddSecondaryItem(ScreeningItem ind) {
			_secondaryItems.Add(ind);
		}

		public ResultCountType PreferredResultCountType {
			get {
				return _preferredResultCount;
			}
			set {
				_preferredResultCount = value;
			}

		}

		public ScreeningItem PrimaryItem {
			get {
				return _primaryItem;
			}
		}
		public int SecondaryItemCount {
			get {
				return _secondaryItems.Count;
			}
		}
		public IEnumerable SecondaryItems {
			get {
				return _secondaryItems;
			}
		}
		public ScreeningItem GetSecondaryItem(int index) {
			return (ScreeningItem)_secondaryItems[index];
		}


		protected override void BeforeExecute() {
			_result = new ScreeningResult();
            //hacked by rti
            //メンバにはしません
			//_tempFarm = null;
            //------
		}

		protected override ExecuteBrandResult ExecuteBrand(AbstractBrand b) {
			BasicBrand br = b as BasicBrand;
			if(br==null || br.Obsolete) return ExecuteBrandResult.Ignored; //派生銘柄、上場廃止銘柄は無視

//hacked by rti
            DataFarm _tempFarm = b.CloneFarm(Env.CurrentIndicators.Format);
//			if(_tempFarm==null)
//                //hacked by rti キャッシュが効くようにする
//                //_tempFarm = b.CreateFarm(Env.CurrentIndicators.Format);
//				_tempFarm = b.CloneFarm(Env.CurrentIndicators.Format);
//                //-------------
//            else
//				_tempFarm.LoadFor(b);
//------

			if(_tempFarm.IsEmpty || _tempFarm.FirstDate>_originDate || _originDate>_tempFarm.LastDate) {
				_result.AddDataErrorBrand(b);
				return ExecuteBrandResult.DataError;
			}
			TradeData td = _tempFarm.GetByIndex(_tempFarm.DateToIndex(_originDate));
			if(!_primaryItem.CheckRange(td)) {
				_result.AddDataErrorBrand(b);
				return ExecuteBrandResult.DataError;
			}
			if(!_primaryItem.CheckFilter(td)) {
				_result.FilteredCount++;
				return ExecuteBrandResult.Filtered;
			}
		
			ScreeningResultEntry ent = new ScreeningResultEntry(b, _secondaryItems.Count);
			ent.PrimaryResult = _primaryItem.Calculate(td.Farm, td.Index);
            //hacked by rti 終値がほしい
            ent.EndPrice = td.Close;
            //---

			//secondary
			int i=0;
			for(i=0; i<_secondaryItems.Count; i++) {
				ScreeningItem item = (ScreeningItem)_secondaryItems[i];
				if(item.CheckRange(td))
					ent.SecondaryResults[i] = item.Calculate(td.Farm, td.Index);
				else {
					_result.AddDataErrorBrand(b);
					return ExecuteBrandResult.DataError;
				}
			}

			_result.Add(ent);
			return ExecuteBrandResult.Succeeded;
		}


	}

	internal class ScreeningResultEntry : SystemTradingResultEntry {
		private EvalResult _primaryResult;
		private EvalResult[] _secondaryResults;
        //hacked by rti
        private double _endPrice;    //終値
        //---

		public ScreeningResultEntry(AbstractBrand br, int secondary_length) : base(br) {
			_secondaryResults = new EvalResult[secondary_length];
		}

		public EvalResult PrimaryResult {
			get {
				return _primaryResult;
			}
			set {
				Debug.Assert(value.IsDouble);
				_primaryResult = value;
			}
		}
        //hacked by rti
        //終値だけどうしてもほしいんよ
        public double EndPrice
        {
            get
            {
                return _endPrice;
            }
            set
            {
                _endPrice = value;
            }
        }
        //----------
        public EvalResult[] SecondaryResults
        {
			get {
				return _secondaryResults;
			}
		}

		public override int CompareTo(object obj) {
			if(_primaryResult.IsDouble) {
				double t = ((ScreeningResultEntry)obj)._primaryResult.DoubleVal;
				if(_primaryResult.DoubleVal < t)
					return -1;
				else if(_primaryResult.DoubleVal > t)
					return 1;
				else
					return 0;
			}
			else {
				return _brand.Code - ((ScreeningResultEntry)obj)._brand.Code;
			}
		}
	}

	internal class ScreeningResult : SystemTradingResult {
		protected int _filteredCount;
		public ScreeningResult() : base(Env.BrandCollection.TotalCount) {
		}

		public void Add(ScreeningResultEntry ent) {
			_data.Add(ent);
			_sortRequired = true;
		}
		public ScreeningResultEntry GetAtS(int index) {
			return base.GetAt(index) as ScreeningResultEntry;
		}
		public int FilteredCount {
			get {
				return _filteredCount;
			}
			set {
				_filteredCount = value;
			}
		}

	}
}
