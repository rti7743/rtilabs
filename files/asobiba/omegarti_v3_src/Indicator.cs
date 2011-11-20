/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Drawing;

using Travis.Util;
using Zanetti.Data;
using Zanetti.Arithmetic;
using Zanetti.UI;
using Zanetti.Parser;

namespace Zanetti.Indicators
{
	internal enum IndicatorTarget {
		Price,
		Volume,
		Oscillator,
        //hacked by rti
        None,
        //------------
		NA
	}
	[Flags]
	internal enum IndicatorDisplay {
		None        = 0,
		Chart       = 1,
		Value       = 2,
		Explanation = 4,
		Normal      = Chart|Value|Explanation
	}

	internal abstract class Indicator {
		protected int _laneID;
		protected string _name;
		protected string _asciiName;
		protected string _groupName;
		protected TargetBrandCondition _targetBrand;
		protected RelativiseParam _relativiseParam;
		protected IndicatorTarget _target;
		protected IndicatorAppearance _appearance; //表示しないやつはnull
		protected IndicatorDisplay _display;

		protected string _formatString;
		protected FormatModifier _formatModifier;
		
		//このIndicatorが計算可能な範囲を対象日付からの相対位置で示す。これはIndicator毎に不変だし、
		//計算するごとに例外が投げられるとあまりにも遅くなるため
		protected DateRange _dateRange;

		public Indicator(int laneID, string name, string ascii) {
			_laneID = laneID;
			_name = name;
			_asciiName = ascii;
			_display = IndicatorDisplay.Normal;
			_dateRange = null;
		}
		public int LaneID {
			get {
				return _laneID;
			}
		}
		public string Name {
			get {
				return _name;
			}
		}
		public string AsciiName {
			get {
				return _asciiName;
			}
		}
		public IndicatorTarget Target {
			get {
				return _target;
			}
			set {
				_target = value;
			}
		}
		public string GroupName {
			get {
				return _groupName;
			}
			set {
				_groupName = value;
			}
		}
		public IndicatorDisplay Display {
			get {
				return _display;
			}
			set {
				_display = value;
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
		public TargetBrandCondition TargetBrand {
			get {
				return _targetBrand;
			}
			set {
				_targetBrand = value;
			}
		}
		public RelativiseParam RelativiseParam {
			get {
				return _relativiseParam;
			}
			set {
				_relativiseParam = value;
			}
		}


		public IndicatorAppearance Appearance {
			get {
				return _appearance;
			}
			set {
				_appearance = value;
			}
		}

		public virtual DateRange DateRange {
			get {
				return _dateRange;
			}
		}

		public abstract double Calculate(TradeData td);
		public abstract bool   CheckRange(TradeData td);
		public abstract string GetFormatString(AbstractBrand br);
	}
	internal class DelegateIndicator : Indicator {
		public delegate double Calculator(TradeData td);
		protected Calculator _func;

		public DelegateIndicator(int laneID, string name, string ascii, Calculator func) : base(laneID, name, ascii) {
			_func = func;
		}

		public override double Calculate(TradeData td) {
			return _func(td);
		}
		public override  bool  CheckRange(TradeData td) {  //今はPrimitiveなやつしかないので
			int t = td.Index;
			return t>=0 && t<td.Farm.FilledLength;
		}
		public override string GetFormatString(AbstractBrand br) {
			return _formatString==null? br.PriceFormatString : _formatString;
		}

	}
	internal class ExpressionIndicator : Indicator {
		protected Expression _expr;
		protected EvalResult[] _args;
		protected Evaluator _eval;

		public ExpressionIndicator(int laneID, string name, string ascii, Expression expr, EvalResult[] args) : base(laneID, name, ascii) {
			_expr = expr;
			_args = args;
			_eval = new Evaluator(name);
			_formatString = "F2";
		}
		public Expression Expression {
			get {
				return _expr;
			}
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
		}
		public int ArgCount {
			get {
				return _args==null? 0 : _args.Length;
			}
		}


		public EvalResult Calculate(DataFarm farm, int index) {
			_eval.Farm = farm;
			_eval.BaseIndex = index;
			_eval.Args = _args;
			return (EvalResult)_expr.Apply(_eval);
		}
		public override double Calculate(TradeData td) {
			EvalResult r = Calculate(td.Farm, td.Index);
			Debug.Assert(r.IsDouble);
			return r.DoubleVal;
		}
		public override DateRange DateRange {
			get {
				if(_dateRange==null) {
					DateRangeChecker ch = new DateRangeChecker(_name, _args);
					_dateRange = (DateRange)_expr.Apply(ch);
					if(_dateRange==null) _dateRange = DateRange.Empty;
				}
				return _dateRange;
			}
		}

		public override bool CheckRange(TradeData td) {
			DateRange r = this.DateRange;
			int t = td.Index;
			return t+r.begin>=0 && t+r.end<=td.Farm.FilledLength;
		}
		public override string GetFormatString(AbstractBrand br) {
			return _formatString;
		}


	}


	//表示関係
	[EnumDesc(typeof(IndicatorStyle))]
	internal enum IndicatorStyle {
		[EnumValue(Description="表示しない")] None,      
		[EnumValue(Description="実線")] Line,    
		[EnumValue(Description="太線")] BoldLine,
		[EnumValue(Description="点線")] Dotted
	}

	internal class IndicatorAppearance {
		private IndicatorStyle _style;
		private ZPen _pen;

		public IndicatorAppearance(IndicatorStyle style, Color color) {
			_style = style;
			_pen = new ZPen(color, ToPenStyle(style));
		}

		public IndicatorStyle Style {
			get {
				return _style;
			}
			set {
				_style = value;
				_pen.Update(_pen.Color, ToPenStyle(_style));
			}
		}
		public Color Color {
			get {
				return _pen.Color;
			}
			set {
				_pen.Update(value, ToPenStyle(_style));
			}
		}
		public ZPen Pen {
			get {
				return _pen;
			}
		}

		private static ZPen.PenStyle ToPenStyle(IndicatorStyle style) {
			switch(style) {
				case IndicatorStyle.Dotted:
					return ZPen.PenStyle.Dotted;
				case IndicatorStyle.BoldLine:
					return ZPen.PenStyle.Bold;
				default:
					return ZPen.PenStyle.Normal;
			}
		}

		public static IndicatorStyle ParseStyle(string value) {
			if(value=="Line")
				return IndicatorStyle.Line;
			else if(value=="BoldLine")
				return IndicatorStyle.BoldLine;
			else if(value=="Dotted")
				return IndicatorStyle.Dotted;
			else if(value=="None" || value.Length==0)
				return IndicatorStyle.None;
			else
				throw new FormatException(value + " is unsupported style");
		}
		public static IndicatorStyle ParseStyle(string value, IndicatorStyle def) {
			if(value=="Line")
				return IndicatorStyle.Line;
			else if(value=="BoldLine")
				return IndicatorStyle.BoldLine;
			else if(value=="Dotted")
				return IndicatorStyle.Dotted;
			else if(value=="None")
				return IndicatorStyle.None;
			else
				return def;
		}
	}

	//特定の銘柄にのみ適用できる指標を書く　今は非常に単純なもののみ
	internal class TargetBrandCondition {
		private int _code;
		private bool _neg;
		public TargetBrandCondition(int code, bool neg) {
			_code = code;
			_neg = neg;
		}
		public bool Applicable(int code) {
			bool t = _code==0 || _code==code; //0は必ず適用できることを示す
			if(_neg) t = !t;
			return t;
		}

		public static TargetBrandCondition Parse(string t) {
			if(t==null)
				return null; //便宜的にnullを返しておく
			else {
				if(t.StartsWith("!"))
					return new TargetBrandCondition(Int32.Parse(t.Substring(1)), true);
				else
					return new TargetBrandCondition(Int32.Parse(t), false);
			}
		}
	}

	internal class RelativiseParam {
		private int _date;

		public RelativiseParam(int date) {
			_date = date;
		}
		public int Date {
			get {
				return _date;
			}
		}
		public double CalcMultiplier(Indicator ind, DataFarm current) {
			TradeData td = current.GetByIndex(current.DateToIndex(_date));
			if(ind.Target==IndicatorTarget.Price || ind.Target==IndicatorTarget.Volume) {
				double realvalue  = ind.Calculate(td);
				double normalized = ind.Target==IndicatorTarget.Volume? td.Volume : td.Close;
				return normalized / realvalue;
			}
			else
				throw new Exception("relativise機能はTargetがprice,volumeでのみ使えます");
		}

		public static RelativiseParam Parse(string t) {
			if(t==null)
				return null;
			else {
				int d = Util.ParseInt(t, 0);
				if(d<20000000 || d>21000000)
					throw new Exception(t + "はrelativiseの値として不適切です。");
				return new RelativiseParam(d);
			}
		}
	} 


}
