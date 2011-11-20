/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;

using Zanetti.Arithmetic;
using Zanetti.Indicators;
using Zanetti.Data;

namespace Zanetti.Prediction
{
	internal class PredictionItem {
		private string _title;
		private string _header;
		private Expression _condition;
		private Expression _prediction;
		private EvalResult[] _args;

		public PredictionItem(string title, string header, EvalResult[] args, Expression c, Expression p) {
			_title = title;
			_header = header;
			_args = args;
			_condition = c;
			_prediction = p;
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

		public Expression Condition {
			get {
				return _condition;
			}
		}
		public Expression Prediction {
			get {
				return _prediction;
			}
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
		}
		
	}

	internal enum PredictionResult {
		ConditionNotMet,
		False,
		True
	}

	internal class VerificationResultEntry : IComparable {
		private int _date;
		private PredictionResult _result;

		public int Date {
			get {
				return _date;
			}
		}
		public PredictionResult Result {
			get {
				return _result;
			}
		}

		public VerificationResultEntry(int dt, PredictionResult res) {
			_date = dt;
			_result = res;
		}

		public int CompareTo(object obj) {
			// 日付降順 (セオリー検証結果過去互換)
			return ((VerificationResultEntry)obj)._date - _date;
		}
	}

	internal class VerificationResult {
		private ArrayList _results;
		private PredictionItem _item;
		private AbstractBrand _brand;
		private Evaluator _evaluator;
		private int _maxcount;
		private int _hitcount;
		private int _truecount;

		public VerificationResult(PredictionItem item) {
			_results = new ArrayList();
			_item = item;
			_maxcount = 100;
		}

		public IEnumerable Entries {
			get {
				return _results;
			}
		}
		public PredictionItem Item {
			get {
				return _item;
			}
		}
		public int MaxCount {
			get {
				return _maxcount;
			}
			set {
				_maxcount = value;
			}
		}
		public int HitCount {
			get {
				return _hitcount;
			}
		}
		public int TrueCount {
			get {
				return _truecount;
			}
		}
		public AbstractBrand Brand {
			get {
				return _brand;
			}
		}
		
		public void Verify(DataFarm farm) {
			_brand = farm.Brand;

			//まず日付範囲チェック
			DateRangeChecker ch = new DateRangeChecker(_item.Title, _item.Args);
			DateRange rc = (DateRange)_item.Condition.Apply(ch);
			DateRange pc = (DateRange)_item.Prediction.Apply(ch);
			if(rc!=null)
				rc.Merge(pc);
			else if(pc!=null)
				rc = pc;
			else
				rc = DateRange.Empty;

			_hitcount = 0;
			_truecount = 0;

			_evaluator = new Evaluator(_item.Title);
			_evaluator.Farm = farm; //rcの中身ただしいかチェックしてくれ
			_evaluator.Args = _item.Args;
			//!!ここは非同期でやったほうがいいだろうか
			for(int i = farm.FilledLength-1; i>=0; i--) {
				if(i+rc.begin>=0 && i+rc.end<=farm.FilledLength) {
					_evaluator.BaseIndex = i;
					Check(farm.GetByIndex(i));
					if(_results.Count>=_maxcount) break;
				}
			}
		}

		private void Check(TradeData td) {
			_evaluator.BaseIndex = td.Index;
			_evaluator.Args = _item.Args;
			EvalResult cond = (EvalResult)_item.Condition.Apply(_evaluator);
			if(!cond.IsBool) throw new ZArithmeticException("セオリー検証の条件式は bool 型の値を返すものでなくてはなりません。");
			if(cond.BoolVal) {
				_hitcount++;
				EvalResult pred = (EvalResult)_item.Prediction.Apply(_evaluator);
				if(!pred.IsBool) throw new ZArithmeticException("セオリー検証の条件式は bool 型の値を返すものでなくてはなりません。");
				_results.Add(new VerificationResultEntry(td.Date, pred.BoolVal? PredictionResult.True : PredictionResult.False));
				if(pred.BoolVal) _truecount++;
			}
		}
	}


}
