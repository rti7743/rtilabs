/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

using Travis.PKI;
using Travis.Storage;
using Zanetti.Parser;
using Zanetti.SystemTrading;

namespace Zanetti.Indicators.Schema
{
	internal abstract class SchemaItemBase {
		protected string _name;

		public abstract string UniqueID { get; }
		public string Name {
			get {
				return _name;
			}
		}

		public abstract void VerifyExpression();

		protected static void VerifyExpression(string name, string expression) {
			try {
				ExpressionConstructor ep = new ExpressionConstructor();
				new ZPredicationParser(new StringReader(expression), ep).Parse();
			}
			catch(Exception ex) {
				throw new FormatException(String.Format("{0} 内のexpression\n{1}\nは誤っています。\n{2}", name, expression, ex.Message));
			}
		}

	}

	//パラメータ
	internal class SchemaParameter {
		private SchemaItem _owner;
		private int _index;
		private string _name;
		private double[] _dailyValues;
		private string _dailyUnit;
		private double[] _weeklyValues;
		private string _weeklyUnit;
		private double[] _monthlyValues;
		private string _monthlyUnit;

		public SchemaParameter(SchemaItem owner, StorageNode definition, StorageNode config) {
			_owner = owner;
			_index = Int32.Parse(Util.LoadMandatoryAttr(definition, "index"));
			_dailyValues = new double[owner.MaxInstanceCount];
			_weeklyValues = new double[owner.MaxInstanceCount];
			_monthlyValues = new double[owner.MaxInstanceCount];

			if(definition["default"]!=null) { //日足・週足・月足関係ないパラメータ
				string unit = Util.LoadMandatoryAttr(definition, "unit");
				_dailyUnit = unit;
				_weeklyUnit = unit;
				_monthlyUnit = unit;
				string def  = Util.LoadMandatoryAttr(definition, "default");
				FillParams(_dailyValues, config, "d", def);
				FillParams(_weeklyValues, config, "w", def);
				FillParams(_monthlyValues, config, "m", def);
			}
			else {
				foreach(StorageNode ch in definition.Children) {
					if(ch.Name=="name")
						_name = ch.TextValue;
					else if(ch.Name=="daily") {
						_dailyUnit = ch["unit"];
						FillParams(_dailyValues, config, "d", ch["default"]);
					}
					else if(ch.Name=="weekly") {
						_weeklyUnit = ch["unit"];
						FillParams(_weeklyValues, config, "w", ch["default"]);
					}
					else if(ch.Name=="monthly") {
						_monthlyUnit = ch["unit"];
						FillParams(_monthlyValues, config, "m", ch["default"]);
					}
				}
			}
		}
		private void FillParams(double[] dest, StorageNode config, string type, string defaults) {
			string[] ds = null;
			for(int i=0; i<dest.Length; i++) {
				string v = config==null? "" : config.GetValue(String.Format("param_{0}{1}{2}", _index, type, i), "");
				if(v.Length==0) {
					if(ds==null) ds = defaults.Split(','); //Splitは遅延
					if(i<ds.Length) v = ds[i];
				}
				dest[i] = v.Length==0? 0 : Double.Parse(v);
			}
		}
		public void SaveTo(StorageNode node) {
			for(int i=0; i<_dailyValues.Length; i++)
				node[String.Format("param_{0}d{1}", _index, i)] = _dailyValues[i].ToString();
			for(int i=0; i<_weeklyValues.Length; i++)
				node[String.Format("param_{0}w{1}", _index, i)] = _weeklyValues[i].ToString();
			for(int i=0; i<_monthlyValues.Length; i++)
				node[String.Format("param_{0}m{1}", _index, i)] = _monthlyValues[i].ToString();			
		}

		public SchemaItem Owner {
			get {
				return _owner;
			}
		}
		public string Name {
			get {
				return _name;
			}
		}

		public double[] DailyValues {
			get {
				return _dailyValues;
			}
			set {
				_dailyValues = value;
			}
		}
		public string DailyUnit {
			get {
				return _dailyUnit;
			}
		}
		public double[] WeeklyValues {
			get {
				return _weeklyValues;
			}
			set {
				_weeklyValues = value;
			}
		}
		public string WeeklyUnit {
			get {
				return _weeklyUnit;
			}
		}
		public double[] MonthlyValues {
			get {
				return _monthlyValues;
			}
			set {
				_monthlyValues = value;
			}
		}
		public string MonthlyUnit {
			get {
				return _monthlyUnit;
			}
		}
		public int Index {
			get {
				return _index;
			}
		}

		/*
		public override string UniqueID {
			get {
				return _owner.UniqueID + "/P" + _index.ToString();
			}
		}
		*/

	}

	//見え方の定義
	internal class SchemaAppearance {
		private IndicatorAppearance[] _appearances;
		private SchemaItem _owner;

		public SchemaAppearance(SchemaItem owner, StorageNode definition, StorageNode config) {
			_owner = owner;
			int c = owner.MaxInstanceCount;
			string[] styles = Util.LoadMandatoryAttr(definition, "style").Split(',');
			string[] colors = Util.LoadMandatoryAttr(definition, "color").Split(',');
			_appearances = new IndicatorAppearance[c];
			for(int i=0; i<_appearances.Length; i++) {
				string style = config==null? "" : config.GetValue(String.Format("style{0}", i), "");
				if(style.Length==0 && styles.Length>i) style = styles[i];
				string color = config==null? "" : config.GetValue(String.Format("color{0}", i), "");
				if(color.Length==0 && colors.Length>i) color = colors[i];
				_appearances[i] = new IndicatorAppearance(IndicatorAppearance.ParseStyle(style), color.Length==0? Color.Empty : Util.ParseColor(color, Color.White));
			}
		}
		public void SaveTo(StorageNode node) {
			int i = 0;
			foreach(IndicatorAppearance a in _appearances) {
				node[String.Format("style{0}", i)] = a.Style.ToString();
				node[String.Format("color{0}", i++)] = Util.FormatColor(a.Color);
			}
		}


		public SchemaItem Owner {
			get {
				return _owner;
			}
		}
		public IndicatorAppearance[] Appearances {
			get {
				return _appearances;
			}
		}	  

		/*
		public override string UniqueID {
			get {
				return _owner.UniqueID + "/A";
			}
		}
		*/
	}

	//Indicator/Screening/Validationの項目
	internal abstract class SchemaItem : SchemaItemBase {
		protected ExtensionKit _owner;
		protected string _title;
		protected string _headerString;
		protected string _description;
		protected string _formatString;
		protected string _targetBrand;
		protected string _relativiseParam;

		protected FormatModifier _formatModifier;
		protected SchemaAppearance _appearance;
		protected ArrayList _parameters;
		//protected SchemaItem _paramDependency;
		//protected SchemaItem _appearanceDependency;
		protected int _maxInstanceCount;

		public SchemaItem(ExtensionKit owner, StorageNode definition, StorageNode parent_config) {
			_owner = owner;
			_parameters = new ArrayList();
			_name = Util.LoadMandatoryAttr(definition, "name");
			_headerString = "";
			_description = "";
			_targetBrand = definition["brand"];
			_relativiseParam = definition["relativise"];
			_maxInstanceCount = Util.ParseInt(definition["count"], 1);
			StorageNode config = parent_config==null? null : parent_config.FindChildNode(_name);

			ParseFormat(definition);

			string d = definition["depends"];
			SchemaItem dep = null;
			if(d!=null) {
				dep = _owner.FindSchemaItemByName(d);
				if(dep==null)
					throw new FormatException(d + " is not found");
				_maxInstanceCount = dep.MaxInstanceCount;
			}

			foreach(StorageNode e in definition.Children) {
				if(e.Name=="param") {
					SchemaParameter p = CreateOrFindSchemaParameter(e, config);
					while(_parameters.Count<=p.Index) _parameters.Add(null);
					_parameters[p.Index] = p;
				}
				else if(e.Name=="defaultappearance")
					_appearance = CreateOrFindSchemaAppearance(e, config);
				else if(e.Name=="title")
					_title = e.TextValue;
				else if(e.Name=="description")
					_description = e.TextValue;
				else if(e.Name=="header")
					_headerString = e.TextValue;
			}
			//if(_title==null) throw new FormatException(String.Format("titleがないか空の{0}があります。", definition.Name));
			if(_headerString==null) throw new FormatException(String.Format("headerがないか空の{0}があります。", definition.Name));

			//依存先からの補完
			if(dep!=null) {
				if(_appearance==null) _appearance = dep.Appearance;
				for(int i=0; i<_parameters.Count; i++)
					if(_parameters[i]==null) _parameters[i] = dep.GetParameter(i);
				while(_parameters.Count<dep.ParameterCount) _parameters.Add(dep.GetParameter(_parameters.Count));
			}
		}
		public void SaveTo(StorageNode node) {
			//if(_paramDependency!=null && _appearanceDependency!=null) return;
			if(_appearance==null && _parameters.Count==0) return;

			StorageNode ch = new StorageNode();
			ch.Name = _name;
			foreach(SchemaParameter p in _parameters) {
				if(p.Owner==this) p.SaveTo(ch);
			}
			if(_appearance!=null && _appearance.Owner==this) _appearance.SaveTo(ch);
			node.AddChild(ch);
		}


		public ExtensionKit Owner {
			get {
				return _owner;
			}
		}

		public string Title {
			get {
				return _title;
			}
		}
		public string HeaderString {
			get {
				return _headerString;
			}
		}
		public string Description {
			get {
				return _description;
			}
		}
		public string FormatString {
			get {
				return _formatString;
			}
		}
		public FormatModifier FormatModifier {
			get {
				return _formatModifier;
			}
		}
		public string TargetBrand {
			get {
				return _targetBrand;
			}
		}
		public string RelativiseParam {
			get {
				return _relativiseParam;
			}
		}

		public int ParameterCount {
			get {
				//if(_dependsTo!=null)
				//	return _dependsTo.ParameterCount;
				//else
				return _parameters.Count;
			}
		}
		public SchemaParameter GetParameter(int index) {
			//if(_dependsTo!=null)
			//	return _dependsTo.GetParameter(index);
			//else
			return (SchemaParameter)_parameters[index];
		}
		public IEnumerable Parameters {
			get {
				//if(_dependsTo!=null)
				//	return _dependsTo.Parameters;
				//else
				return _parameters;
			}
		}

		public SchemaAppearance Appearance {
			get {
				//if(_dependsTo!=null)
				//	return _dependsTo.Appearance;
				//else
				return _appearance;
			}
			set {
				_appearance = value;
			}
		}


		public override string UniqueID {
			get {
				return _owner.ID + ":" + _name;
			}
		}
		public virtual int MaxInstanceCount {
			get {
				//if(_dependsTo!=null)
				//	return _dependsTo.MaxInstanceCount;
				//else
				return _maxInstanceCount;
			}
		}
		public bool OwnsConfig {
			get {
				bool a = _appearance!=null && _appearance.Owner==this;
				if(a) return true;
				foreach(SchemaParameter p in _parameters)
					if(p.Owner==this) return true;
				return false;
			}
		}
		public SchemaParameter[] OwnedParameters {
			get {
				ArrayList temp = new ArrayList();
				foreach(SchemaParameter p in _parameters)
					if(p.Owner==this) temp.Add(p);
				return (SchemaParameter[])temp.ToArray(typeof(SchemaParameter));
			}
		}


		private void ParseFormat(StorageNode definition) {
			string f = definition["format"];
			if(f==null) {
				_formatString = "F2";
				_formatModifier = FormatModifier.Nop;
			}
			else {
				string[] fs = f.Split(',');
				if(fs.Length>0) {
					_formatString = fs[0];
					if(fs.Length>1) _formatModifier = ParseFormatModifier(fs[1]);
				}
				else
					_formatString = "F2";
			}
		}

		private SchemaParameter CreateOrFindSchemaParameter(StorageNode definition, StorageNode config) {
			string d = definition["depends"];
			if(d==null)
				return new SchemaParameter(this, definition, config);
			else {
				SchemaItem si = _owner.FindSchemaItemByName(d);
				if(si==null) throw new FormatException(d + " is not found");
				return si.GetParameter(Int32.Parse(definition["index"]));
			}
		}

		public void CollectParameter(ArrayList col) {
			foreach(SchemaParameter p in _parameters)
				if(p.Owner==this) col.Add(p);
		}
		public void CollectAppearance(ArrayList col) {
			if(_appearance!=null && _appearance.Owner==this) col.Add(_appearance);
		}

		private SchemaAppearance CreateOrFindSchemaAppearance(StorageNode definition, StorageNode config) {
			string d = definition["depends"];
			if(d==null)
				return new SchemaAppearance(this, definition, config);
			else {
				SchemaItem si = _owner.FindSchemaItemByName(d);
				if(si==null) throw new FormatException(d + " is not found");
				return si.Appearance;
			}
		}

		private static FormatModifier ParseFormatModifier(string t) {
			if(t=="%")
				return FormatModifier.Percent;
			else if(t=="100")
				return FormatModifier.Mul100;
			else
				return FormatModifier.Nop;
		}
	}

	//Indicatorを示すスキーマ
	internal class SchemaIndicatorItem : SchemaItem {
		protected IndicatorTarget _target;
		protected IndicatorDisplay _display;
		protected string _rawExpression;
		protected string _groupName;

		public SchemaIndicatorItem(ExtensionKit owner, StorageNode definition, StorageNode parameters) : base(owner, definition, parameters) {
			_target = ParseTarget(definition["target"]);
			_groupName = definition["group"];
            //hacked by rti
			//_display = _target==IndicatorTarget.Oscillator? IndicatorDisplay.Normal : ParseDisplay(definition["display"]);
            if (_target == IndicatorTarget.Oscillator || _target == IndicatorTarget.None)
            {
                _display = IndicatorDisplay.Normal;
            }
            else
            {
                _display = ParseDisplay(definition["display"]);
            }
            //------------
			
			_rawExpression = Util.LoadMandatoryText(definition, "expression");
			if(_rawExpression.Length==0)
				throw new FormatException("expression is empty");

			if(_appearance==null)
				throw new FormatException(String.Format("{0}にdefaultappearanceがありません。", _name));
		}

		public IndicatorTarget Target {
			get {
				return _target;
			}
		}
		public IndicatorDisplay Display {
			get {
				return _display;
			}
		}
		public string RawExpression {
			get {
				return _rawExpression;
			}
		}	  
		public string GroupName {
			get {
				return _groupName;
			}
		}
		public override void VerifyExpression() {
			VerifyExpression(_name, _rawExpression);
		}

		private static IndicatorTarget ParseTarget(string value) {
			if(value=="price")
				return IndicatorTarget.Price;
			else if(value=="volume")
				return IndicatorTarget.Volume;
			else if(value=="oscillator")
				return IndicatorTarget.Oscillator;
            //hacked by rti
            else if (value == "none")
                return IndicatorTarget.None;
            //^^^^^^^^^^^^^
            else
				throw new FormatException(String.Format("{0}はtargetの値として正しくありません。", value));
		}
		private static IndicatorDisplay ParseDisplay(string value) {
			if(value=="normal")
				return IndicatorDisplay.Normal;
			else {
				IndicatorDisplay d = IndicatorDisplay.None;
				foreach(string t in value.Split('+')) {
					if(t=="value")
						d |= IndicatorDisplay.Value;
					else if(t=="explanation")
						d |= IndicatorDisplay.Explanation;
					else if(t=="chart")
						d |= IndicatorDisplay.Chart;
					else 
						throw new FormatException(String.Format("{0}はdisplayの値として正しくありません。", value));
				}
				return d;
			}
		}
	}

	internal class SchemaScreeningItem : SchemaItem {
		protected string _rawExpression;
		protected string _rawFilter;

		public SchemaScreeningItem(ExtensionKit owner, StorageNode definition, StorageNode parameters) : base(owner, definition, parameters) {
			_rawExpression = Util.LoadMandatoryText(definition, "expression");
			StorageNode fl = definition.FindChildNode("filter");
			_rawFilter = fl==null? "" : fl.TextValue;
			if(_rawExpression.Length==0)
				throw new FormatException("expression is empty");
		}
		public string RawExpression {
			get {
				return _rawExpression;
			}
		}	  
		public string RawFilter {
			get {
				return _rawFilter;
			}
		}
		public override void VerifyExpression() {
			VerifyExpression(_name, _rawExpression);
			if(_rawFilter.Length>0)
				VerifyExpression(_name, _rawFilter);
		}
	}

	/* バージョン１形式
	 *   <prediction name="uehige_p">
    <title>上ひげ陰線の売り場検証</title>
    <description>上ひげ（チャート上のひげの長さが終値の1%以上で判定しています）かつ陰線を出し、
さらにその上ひげで高値を更新したときに空売りをしたと想定して勝率を測定します。</description>
    <header>上ひげ陰線かつ{0}高値更新なら空売りで{1}以内に{2}以上下降</header>
    <condition>ue_hige() and insen() and (high()==max(high({0})))</condition>
    <claim><![CDATA[min(low(0,0-{1})) < close()*(1-{2}/100)]]></claim>
    <param index="0" type="positiveInteger" unit="日" default="5"/>
    <param index="1" type="positiveInteger" unit="日" default="5"/>
    <param index="2" type="positiveInteger" unit="%" default="2"/>
  </prediction>
    
	*/
	internal class SchemaPredictionItem : SchemaItem {
		protected string _rawCondition;
		protected string _rawClaim;

		public SchemaPredictionItem(ExtensionKit owner, StorageNode definition, StorageNode parameters) : base(owner, definition, parameters) {
			_rawCondition = Util.LoadMandatoryText(definition, "condition");
			_rawClaim = Util.LoadMandatoryText(definition, "claim");
		}
		public string RawCondition {
			get {
				return _rawCondition;
			}
		}
		public string RawClaim {
			get {
				return _rawClaim;
			}
		}
		public override void VerifyExpression() {
			VerifyExpression(_name, _rawCondition);
			VerifyExpression(_name, _rawClaim);
		}
	}

	internal class SchemaAutoTradingItem : SchemaItem {

		protected TradingType _tradingType;
		protected string _rawSignal;
		protected string _rawExit;
		protected string _rawLosscut;
		protected EntryType _rawEntryType;
		protected string _rawGyakusashine;
        //hacked by rti
        protected IntervalType _rawIntervalType;
        protected ArrayList _rawAnalystArray;   //アナリストさんたち
        //-----


		public SchemaAutoTradingItem(ExtensionKit owner, StorageNode definition, StorageNode parameters) : base(owner, definition, parameters) {
			_tradingType = ParseTradingType(Util.LoadOptionalText(definition, "type", "long"));
			_rawSignal = Util.LoadMandatoryText(definition, "signal");
			_rawExit   = Util.LoadMandatoryText(definition, "exit");
			_rawLosscut= Util.LoadMandatoryText(definition, "losscut");
			StorageNode entry = definition.FindChildNode("entry");
			if(entry==null) {
				_rawEntryType = EntryType.TodayClose;
				_rawGyakusashine = "";
			}
			else {
				_rawEntryType = ParseEntryType(entry.GetValue("type", ""));
				_rawGyakusashine = entry.TextValue;
			}
            //hacked by rti
            StorageNode interval = definition.FindChildNode("interval");
            if (interval == null)
            {
                _rawIntervalType = IntervalType.ClosingTrade;
            }
            else
            {
                _rawIntervalType = ParseIntervalType(interval.GetValue("type", ""));
            }
            //---------------------

            //hacked by rti
            //アナリストの読み込み
            _rawAnalystArray = new ArrayList();
            foreach (StorageNode ch in definition.Children)
            {
                if (ch.Name == "analyst")
                {
                    SchemaAnalystItem analist = new SchemaAnalystItem(ch);
                    _rawAnalystArray.Add(analist);
                }
            }
            //-----
		}
		public string RawSignal {
			get {
				return _rawSignal;
			}
		}
		public string RawExit {
			get {
				return _rawExit;
			}
		}
		public string RawLosscut {
			get {
				return _rawLosscut;
			}
		}
		public EntryType RawEntryType {
			get {
				return _rawEntryType;
			}
		}
		public string RawGyakusashine {
			get {
				return _rawGyakusashine;
			}
		}
        //hacked by rti
        public IntervalType IntervalType
        {
            get
            {
                return _rawIntervalType;
            }
        }
        public int GetAnalystCount()
        {
            return _rawAnalystArray.Count;
        }
        public SchemaAnalystItem GetAnalyst(int index)
        {
            return (SchemaAnalystItem)_rawAnalystArray[index];
        }
        //-----
        public TradingType TradingType
        {
			get {
				return _tradingType;
			}
		}
		public override void VerifyExpression() {
			VerifyExpression(_name, _rawSignal);
			VerifyExpression(_name, _rawExit);
			VerifyExpression(_name, _rawLosscut);
		}

		private static TradingType ParseTradingType(string t) {
			if(t=="Short" || t=="short")
				return TradingType.Short;
			else if(t=="Long" || t=="long")
				return TradingType.Long;
			else
				throw new FormatException(t + " はTradingTypeの値として不正です。");
		}
		private static EntryType ParseEntryType(string t) {
			t = t.ToLower();
			if(t=="tomorrow-open")
				return EntryType.TomorrowOpen;
			else if(t=="signal-limitorder")
				return EntryType.Gyakusashine;
			else if(t=="todayclose" || t=="today-close" || t.Length==0) //タイプミスがあったようだ。互換性のためtodayclose, today-closeどちらもOKとする
				return EntryType.TodayClose;
			else
				throw new FormatException(t + " はEntryTypeの値として不正です。");
		}

        //hacked by rti

        //signalの間隔
        private static IntervalType ParseIntervalType(string t)
        {
			t = t.ToLower();
            if (t == "closing-trade")
                return IntervalType.ClosingTrade;
            else if (t == "every-day")
                return IntervalType.EveryDay;
			else
                throw new FormatException(t + " はIntervalTypeの値として不正です。");
		}
        //アナリスト <analyst>
        internal class SchemaAnalystItem
        {
            protected AnalystType _rawType;   //アナリストがいつ動きだすのか定義する
            protected string _rawName;        //アナリストの名前 カラム名になる
            protected string _rawExpression;  //評価する式

            public SchemaAnalystItem(StorageNode definition)
            {
                _rawType = ParseAnalystType(definition.GetValue("type", ""));
                _rawName = definition.GetValue("name", "");
                _rawExpression = definition.TextValue;
            }
            private static AnalystType ParseAnalystType(string t)
            {
                t = t.ToLower();
                if (t == "signal")
                    return AnalystType.Signal;
                else if (t == "exit")
                    return AnalystType.Exit;
                else if (t == "losscut")
                    return AnalystType.Losscut;
                else if (t == "gyakusashine")
                    return AnalystType.Gyakusashine;
                else
                    throw new FormatException(t + " はAnalystTypeの値として不正です。");
            }
            public AnalystType RawType
            {
                get
                {
                    return _rawType;
                }
            }
            public string RawName
            {
                get
                {
                    return _rawName;
                }
            }
            public string RawExpression
            {
                get
                {
                    return _rawExpression;
                }
            }
        }
        //------
    }

    //hacked by rti
    //お金の使い方
    internal class SchemaMoneyPlanningItem : SchemaItem
    {
        protected string _rawSignal;

        public SchemaMoneyPlanningItem(ExtensionKit owner, StorageNode definition, StorageNode parameters)
            : base(owner, definition, parameters)
        {
            _rawSignal = Util.LoadMandatoryText(definition, "signal");
        }
        public string RawSignal
        {
            get
            {
                return _rawSignal;
            }
        }
        public override void VerifyExpression()
        {
            VerifyExpression(_name, _rawSignal);
        }
    }
    //--------------
	internal class SchemaOscillatorGroup {
		private string _name;
		private string _title;
		private ValueRange _type;

		public SchemaOscillatorGroup(ExtensionKit parent, StorageNode definition) {
			_name = Util.LoadMandatoryAttr(definition, "name");
			_title = Util.LoadMandatoryAttr(definition, "title");
			_type = ParseValueRange(Util.LoadMandatoryAttr(definition, "type"));
		}
		public string Name {
			get {
				return _name;
			}
		}
		public string Title {
			get {
				return _title;
			}
		}
		public ValueRange Type {
			get {
				return _type;
			}
		}
		private static ValueRange ParseValueRange(string data) {
			if(data=="percent0_1")
				return ValueRange.Percent0_1;
			else if(data=="percent1_1")
				return ValueRange.Percent1_1;
			else if(data=="origin0")
				return ValueRange.Origin0;
			else
				return ValueRange.Default;
		}
	}

	internal class SchemaExtensionLibrary : SchemaItemBase {
		private ExtensionKit _owner;
		private string _expression;

		public SchemaExtensionLibrary(ExtensionKit owner, StorageNode definition) {
			_owner = owner;
			_name = Util.LoadMandatoryAttr(definition, "name");
			_expression = Util.LoadMandatoryText(definition, "expression");
		}
		public string Expression {
			get {
				return _expression;
			}
		}
		public override string UniqueID {
			get {
				return _owner.ID + ":"+ _name;
			}
		}
		public override void VerifyExpression() {
			VerifyExpression(_name, _expression);
		}
	}

}
