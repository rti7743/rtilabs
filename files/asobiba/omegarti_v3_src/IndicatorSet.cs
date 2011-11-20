/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Collections;
using System.Windows.Forms;
using System.Threading;

using Travis.Storage;

using Zanetti.Data;
using Zanetti.Config;
using Zanetti.Arithmetic;
using Zanetti.Parser;
using Zanetti.Prediction;
using Zanetti.SystemTrading;
using Zanetti.SystemTrading.Screening;
using Zanetti.SystemTrading.AutoTrading;
//hacked by rti
using Zanetti.SystemTrading.MoneyPlanning;
//--------
using Zanetti.Indicators.Schema;

namespace Zanetti.Indicators
{
	internal class OscillatorGroup : IEnumerable {
		private string _name;
		private string _title;
		private int _index;
		private ArrayList _indicators;
		private ValueRange _type;

		private OscillatorGroup() {
			_indicators = new ArrayList();
			_type = ValueRange.Origin0;
			_name = "";
			_title = "";
		}
		public OscillatorGroup(SchemaOscillatorGroup gr) {
			_indicators = new ArrayList();
			_type = gr.Type;
			_name = gr.Name;
			_title = gr.Title;
		}
		public OscillatorGroup(string name, ValueRange vr) {
			_indicators = new ArrayList();
			_name = name;
			_title = name; //共有
			_type = vr;
		}

		public string Name {
			get {
				return _name;
			}
		}
		public ValueRange Type {
			get {
				return _type;
			}
		}
		public string Title {
			get {
				return _title;
			}
		}
		public int Index {
			get {
				return _index;
			}
			set {
				_index = value;
			}
		}
		public int Count {
			get {
				return _indicators.Count;
			}
		}

		public void AddIndicator(Indicator ind) {
			_indicators.Add(ind);
		}
		public IEnumerator GetEnumerator() {
			return _indicators.GetEnumerator();
		}
		public bool Contains(Indicator ind) {
			return _indicators.Contains(ind);
		}

		public static OscillatorGroup CreateDummy() {
			return new OscillatorGroup();
		}

	}

	internal enum ValueRange {
		Default,
		Percent0_1, //0-1のタイプ(RSIなど
		Percent1_1, //-1 - 1のタイプ(RCIなど
		Origin0,    //0を軸にしたタイプ(乖離率
	}



	internal class IndicatorSet {
		private ChartFormat _format;
		private ArrayList _indicators;
		private Indicator[] _chartIndicators;
		private Indicator[] _valueWindowIndicators;
		private Indicator[] _explanationWindowIndicators;

		private ArrayList _oscillatorGroups;
		private ArrayList _predictionItems;
		private ArrayList _autoTradingItems;
        //hacked by rti
        private ArrayList _moneyPlanningItems;
        //--------------------
        
		private ArrayList _screeningItems;

		private ArrayList _ichimokuKumo; //一目均衡表の雲は特別扱い
		private int _ichimokuSenkoDays;
		
		private bool _valid;

		public IndicatorSet(ChartFormat format) {
			_format = format;
			_indicators = new ArrayList();
			_oscillatorGroups = new ArrayList();
			_screeningItems = new ArrayList();
			_predictionItems = new ArrayList();
			_autoTradingItems = new ArrayList();
            //hacked by rti
            _moneyPlanningItems = new ArrayList();
            //---------------
			_ichimokuKumo = new ArrayList();
			_valid = true;
			InitPrimitives();
		}
		public ChartFormat Format {
			get {
				return _format;
			}
		}

		//描画中のエラーが起きるとこのフラグをいじって無効化
		public bool Valid {
			get {
				return _valid;
			}
			set {
				_valid = value;
			}
		}
		
		private void InitPrimitives() {
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.Date,        "日付",     "pdate", new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetDate)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.Open,        "始値",     "popen", new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetOpen)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.High,        "高値",     "phigh", new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetHigh)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.Low,         "安値",     "plow",  new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetLow)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.Close,       "終値",     "pclose",new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetClose)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.Volume,      "出来高",   "pvol",  new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetVolume)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.CreditLong,  "信用買残", "pcl",   new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetCreditLong)));
			_indicators.Add(new DelegateIndicator((int)PrimitiveIndicator.CreditShort, "信用売残", "pcs",   new DelegateIndicator.Calculator(DataFarmPrimitiveAccess.GetCreditShort)));

			GetPrimitive2(PrimitiveIndicator.Volume).FormatString = "";
			GetPrimitive2(PrimitiveIndicator.Date).Display = IndicatorDisplay.None;
			//GetPrimitive2(PrimitiveIndicator.CreditLong).Display = IndicatorDisplay.None;
			//GetPrimitive2(PrimitiveIndicator.CreditShort).Display = IndicatorDisplay.None;
			
			DelegateIndicator ind = GetPrimitive2(PrimitiveIndicator.CreditLong);
			ind.FormatString = "";
			ind.Target = IndicatorTarget.Volume;
			ind.Appearance = Env.Preference.CreditLongAppearance;

			ind = GetPrimitive2(PrimitiveIndicator.CreditShort);
			ind.FormatString = "";
			ind.Target = IndicatorTarget.Volume;
			ind.Appearance = Env.Preference.CreditShortAppearance;
		}

		public Indicator GetPrimitive(PrimitiveIndicator ind) {
			return (Indicator)_indicators[(int)ind];
		}
		private DelegateIndicator GetPrimitive2(PrimitiveIndicator ind) {
			return (DelegateIndicator)_indicators[(int)ind];
		}
		public Indicator GetIndicator(int index) {
			return (Indicator)_indicators[index];
		}
		public Indicator FindFromAscii(string name) {
			foreach(Indicator ind in _indicators)
				if(ind.AsciiName==name) return ind;
			return null;
		}
		public int IndicatorCount {
			get {
				return _indicators.Count;
			}
		}
		public int OscillatorGroupCount {
			get {
				return _oscillatorGroups.Count;
			}
		}
		public OscillatorGroup GetOscillatorGroupAt(int i) {
			if(_oscillatorGroups.Count==0)
				return OscillatorGroup.CreateDummy();
			else
				return (OscillatorGroup)_oscillatorGroups[i];
		}
		public OscillatorGroup FindOscillatorGroup(string name) {
			foreach(OscillatorGroup g in _oscillatorGroups) {
				if(g.Name==name) return g;
			}
			return null;
		}

		//一目均衡表の特例パラメータ
		public IList IchimokuKumo {
			get {
				Debug.Assert(_ichimokuKumo.Count<=2);
				return _ichimokuKumo;
			}
		}

		//!!今は異なるChartFormatに同時にIndicatorSetが対応することはないのでfmtは無視
		public int GetAddedFutureLength(ChartFormat fmt) {
#if DOJIMA
			//無双線表示時は少なくとも１日追加
			if(Dojima.DojimaChart.IsMusousenAvailable(fmt)) return Math.Max(1, _ichimokuSenkoDays);
#endif
			return _ichimokuSenkoDays;
		}

		//非プリミティブで表示可能なやつのリストを返す
		public Indicator[] GetIndicatorsForChart() {
			if(_chartIndicators==null) {
				ArrayList ar = new ArrayList();
				foreach(Indicator ind in _indicators)
					if(ind.LaneID>=(int)PrimitiveIndicator.CreditLong &&
					   (ind.Display & IndicatorDisplay.Chart)!=IndicatorDisplay.None &&
					   ind.Target!=IndicatorTarget.Oscillator) ar.Add(ind);

				_chartIndicators = (Indicator[])ar.ToArray(typeof(Indicator));
			}
			return _chartIndicators;
		}
		public Indicator[] GetIndicatorsForValueWindow() {
			if(_valueWindowIndicators==null) {
				ArrayList ar = new ArrayList();
				foreach(Indicator ind in _indicators)
					if((ind.Display & IndicatorDisplay.Value)!=IndicatorDisplay.None) ar.Add(ind);

				_valueWindowIndicators = (Indicator[])ar.ToArray(typeof(Indicator));
			}
			return _valueWindowIndicators;
		}
		public Indicator[] GetIndicatorsForExplanationWindow() {
			if(_explanationWindowIndicators==null) {
				ArrayList ar = new ArrayList();
				foreach(Indicator ind in _indicators)
					if(ind.LaneID>=(int)PrimitiveIndicator.CreditLong &&
						(ind.Display & IndicatorDisplay.Explanation)!=IndicatorDisplay.None) ar.Add(ind);

				_explanationWindowIndicators = (Indicator[])ar.ToArray(typeof(Indicator));
			}
			return _explanationWindowIndicators;
		}

		private void ClearIndicatorCollectionCache() {
			_chartIndicators = null;
			_valueWindowIndicators = null;
			_explanationWindowIndicators = null;
		}

		public ScreeningItem[] GetScreeningItems() {
			return (ScreeningItem[])_screeningItems.ToArray(typeof(ScreeningItem));
		}

	
		public ExpressionIndicator AddExpressionIndicator(string expr_image, EvalResult[] args, string name, SchemaItemBase bi) {
			ExpressionConstructor ec = new ExpressionConstructor();
			new ZPredicationParser(new StringReader(expr_image), ec).Parse();
            Expression expr = ec.Result;

			int laneID = _indicators.Count;
			string asciiname = bi==null? name : bi.Name; //!!asciinameというネーミングはまずかった
			ExpressionIndicator ind = new ExpressionIndicator(laneID, name, asciiname, expr, args);
			_indicators.Add(ind);

			ClearIndicatorCollectionCache();

			//特別な扱いが必要なブツ
			if(asciiname=="ichimoku_senkouA" || asciiname=="ichimoku_senkouB") {
				_ichimokuKumo.Add(ind);
				if(((SchemaIndicatorItem)bi).Appearance.Appearances[0].Style!=IndicatorStyle.None) //未来領域を表示するやつがいれば値をセット
					_ichimokuSenkoDays = (int)args[1].DoubleVal;
			}
			
			return ind;
		}


		public void AddOscillatorGroup(OscillatorGroup gr) {
			_oscillatorGroups.Add(gr);
			gr.Index = _oscillatorGroups.Count-1;
		}

		public void AddScreeningItem(SchemaScreeningItem si, EvalResult[] args, string title) {
			ExpressionConstructor ee = new ExpressionConstructor();
            new ZPredicationParser(new StringReader(si.RawExpression), ee).Parse();

			Expression filter = null;
			if(si.RawFilter.Length>0) { //これはないこともある
				ExpressionConstructor ef = new ExpressionConstructor();
                new ZPredicationParser(new StringReader(si.RawFilter), ef).Parse();
				filter = ef.Result;
			}
			ScreeningItem it = new ScreeningItem(title, args, ee.Result, filter);
			it.FormatString = si.FormatString;
			it.FormatModifier = si.FormatModifier;
			_screeningItems.Add(it);
		}

		public void AddPredictionItem(SchemaPredictionItem it, EvalResult[] args, string title, string header) {
			ExpressionConstructor ec = new ExpressionConstructor();
			new ZPredicationParser(new StringReader(it.RawCondition), ec).Parse();
            ExpressionConstructor ep = new ExpressionConstructor();
            new ZPredicationParser(new StringReader(it.RawClaim), ep).Parse();

			_predictionItems.Add(new PredictionItem(title, header, args, ec.Result, ep.Result));
		}
		public void AddAutoTradingItem(SchemaAutoTradingItem it, EvalResult[] args, string title, string header) {
			ExpressionConstructor es = new ExpressionConstructor();
			new ZPredicationParser(new StringReader(it.RawSignal), es).Parse();
            ExpressionConstructor ee = new ExpressionConstructor();
            new ZPredicationParser(new StringReader(it.RawExit), ee).Parse();
            ExpressionConstructor el = new ExpressionConstructor();
            new ZPredicationParser(new StringReader(it.RawLosscut), el).Parse();
			
			Expression gyakusashine = null;
			if(it.RawEntryType==EntryType.Gyakusashine) {
				ExpressionConstructor eg = new ExpressionConstructor();
                new ZPredicationParser(new StringReader(it.RawGyakusashine), eg).Parse();
                gyakusashine = eg.Result;
			}

            //hacked by rti
            //アナリストさんたちの読み込み
            ArrayList autoTradingItemsArray = new ArrayList();
            for (int i = 0; i < it.GetAnalystCount(); i++)
            {
                SchemaAutoTradingItem.SchemaAnalystItem anaS = it.GetAnalyst(i);
                //アナリストさんの評価式のパース
                ExpressionConstructor e = new ExpressionConstructor();
                new ZPredicationParser(new StringReader(anaS.RawExpression), e).Parse();

                autoTradingItemsArray.Add(new AutoTradingAnalystItem(anaS.RawType, anaS.RawName, e.Result));
            }

            //_autoTradingItems.Add(new AutoTradingItem(title, header, it.TradingType, args, es.Result, it.RawEntryType, gyakusashine, ee.Result, el.Result));
            _autoTradingItems.Add(new AutoTradingItem(title, header, it.TradingType, args, es.Result, it.RawEntryType, gyakusashine, ee.Result, el.Result, it.IntervalType, autoTradingItemsArray));
            //----------------
		}
        //hacked by rti
        public void AddMoneyPlanningItem(SchemaMoneyPlanningItem it, EvalResult[] args, string title, string header)
        {
            ExpressionConstructor es = new ExpressionConstructor();
            new ZPredicationParser(new StringReader(it.RawSignal), es).Parse();

            _moneyPlanningItems.Add(new MoneyPlanningItem(title, header,  args, es.Result));
        }
        //----------------------------------

		public IEnumerable OscillatorGroups {
			get {
				return _oscillatorGroups;
			}
		}
		
		//!!このあたりの一貫性のなさひどい
		public IEnumerable PredictionItems {
			get {
				return _predictionItems;
			}
		}
		public AutoTradingItem[] AutoTradingItems {
			get {
				return (AutoTradingItem[])_autoTradingItems.ToArray(typeof(AutoTradingItem));
			}
		}
        //hacked by rti
        public MoneyPlanningItem[] MoneyPlanningItems
        {
            get
            {
                return (MoneyPlanningItem[])_moneyPlanningItems.ToArray(typeof(MoneyPlanningItem));
            }
        }
        //----------------
	}

	internal interface IIndicatorCustomizer {
		void ModifyIndicatorSet(IndicatorSet indicators);
	}

	internal class IndicatorSetBuilder {
		private IndicatorSet _result;
		private IIndicatorCustomizer _customizer;

		private class FixedParameters {
			public string title;
			public string header;
			public EvalResult[] args;
		}
		private FixedParameters FillParameter(SchemaItem it, ChartFormat format, int index) {
			FixedParameters result = new FixedParameters();
			ArrayList pl_value = new ArrayList();
			ArrayList pl_string = new ArrayList();
			foreach(SchemaParameter p in it.Parameters) {
				if(format==ChartFormat.Daily
#if DOJIMA
					|| format==ChartFormat.HalfDaily //パラメータを読み出すときは半日足のときも日足と同じ
#endif
					) {
					pl_value.Add(new EvalResult(p.DailyValues[index])); 
					pl_string.Add(p.DailyValues[index] + p.DailyUnit);
				}
				else if(format==ChartFormat.Weekly){
					pl_value.Add(new EvalResult(p.WeeklyValues[index]));
					pl_string.Add(p.WeeklyValues[index] + p.WeeklyUnit);
				}
				else{	// monthly
					pl_value.Add(new EvalResult(p.MonthlyValues[index]));
					pl_string.Add(p.MonthlyValues[index] + p.MonthlyUnit);
				}
			}
			Debug.Assert(it.HeaderString!=null);
			result.title = it.Title;
			result.header = String.Format(it.HeaderString, pl_string.ToArray());
			result.args = (EvalResult[])pl_value.ToArray(typeof(EvalResult));
			return result;
		}

		public IndicatorSet Result {
			get {
				return _result;
			}
		}
		public IIndicatorCustomizer Customizer {
			get {
				return _customizer;
			}
			set {
				_customizer = value;
			}
		}


		public void Construct(ChartFormat format) {
			_result = new IndicatorSet(format);
			ArrayList col = new ArrayList();

			Hashtable nameToGroup = new Hashtable();
			Env.Schema.CollectOscillatorGroup(col);
			int index = 0;
			foreach(SchemaOscillatorGroup gr in col) {
				OscillatorGroup og = new OscillatorGroup(gr);
				og.Index = index++;
				_result.AddOscillatorGroup(og);
				nameToGroup[gr.Name] = og;
			}

			col.Clear();
			Env.Schema.CollectIndicatorSchema(col);
			foreach(SchemaIndicatorItem si in col) {
				int n = si.MaxInstanceCount; 
				for(int i=0; i<n; i++) {
					SchemaAppearance sa = si.Appearance;
					if(sa!=null && (sa.Appearances.Length<=i || sa.Appearances[i].Style==IndicatorStyle.None)) continue;

					FixedParameters param = FillParameter(si, format, i);
					ExpressionIndicator ind = _result.AddExpressionIndicator(si.RawExpression, param.args, param.header, si);
					ind.Appearance = sa==null? null : sa.Appearances[i]; 
					ind.Display = si.Display;
					ind.Target = si.Target;
					ind.GroupName = si.GroupName;
					ind.FormatString = si.FormatString;
					ind.FormatModifier = si.FormatModifier;
					ind.TargetBrand = TargetBrandCondition.Parse(si.TargetBrand);
					ind.RelativiseParam = RelativiseParam.Parse(si.RelativiseParam);
					if(ind.Target==IndicatorTarget.Oscillator)
						((OscillatorGroup)nameToGroup[ind.GroupName]).AddIndicator(ind); //!!エントリがなかったときのエラーハンドリング
				}
			}

			//!!この項目収集はいかがわしい
			col.Clear();
			Env.Schema.CollectScreeningSchema(col);
			foreach(SchemaScreeningItem si in col) {
				FixedParameters param = FillParameter(si, format, 0);
				_result.AddScreeningItem(si, param.args, param.header);
			}

			col.Clear();
			Env.Schema.CollectPredictionSchema(col);
			foreach(SchemaPredictionItem si in col) {
				FixedParameters param = FillParameter(si, format, 0);
				_result.AddPredictionItem(si, param.args, param.title, param.header);
			}

			col.Clear();
			Env.Schema.CollectAutoTradingSchema(col);
			foreach(SchemaAutoTradingItem si in col) {
				FixedParameters param = FillParameter(si, format, 0);
				_result.AddAutoTradingItem(si, param.args, param.title, param.header);
			}
            //hacked by rti
            col.Clear();
            Env.Schema.CollectMoneyPlanningSchema(col);
            foreach (SchemaMoneyPlanningItem si in col)
            {
                FixedParameters param = FillParameter(si, format, 0);
                _result.AddMoneyPlanningItem(si, param.args, param.title, param.header);
            }
            //--------------

			//library
			col.Clear();
			Env.Schema.CollectLibraryFunction(col);
			foreach(SchemaExtensionLibrary lib in col) {
				ExtendedLibraryFunction f = new ExtendedLibraryFunction(lib.Name, lib.Expression);
				Env.ArithmeticLibrary.Add(lib.Name, new EvalDelegate(f.Calc), new DateRangeDelegate(f.CalcDateRange));
			}

			//customizer
			if(_customizer!=null) _customizer.ModifyIndicatorSet(_result);
		}
	}

	internal class AsyncSchemaLoader {
		private IntPtr _parent;

		private IndicatorSet  _indicators;
		private string _errorMessage;

		public IndicatorSet IndicatorSet {
			get {
				return _indicators;
			}
		}
		public string ErrorMessage {
			get {
				return _errorMessage;
			}
		}
		public AsyncSchemaLoader(IWin32Window p) {
			_parent = p.Handle;
		}

		public void AsyncLoad() {
			Thread th = new Thread(new ThreadStart(Run));
			th.Priority = ThreadPriority.Normal;
			th.Start();
		}

		private void Run() {
			try {
				//long t = DateTime.Now.Ticks;
				LoadSchema();
				BuildIndicators();
				Env.Frame.WarmUpMagicLibrary();
				//Debug.WriteLine("Load Time " + (DateTime.Now.Ticks-t)/10000);
				Win32.SendMessage(_parent, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_FINISHED));
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				_errorMessage = ex.Message;
				Win32.SendMessage(_parent, AsyncConst.WM_ASYNCPROCESS, IntPtr.Zero, new IntPtr(AsyncConst.LPARAM_ERROR));
			}
		}

		private void LoadSchema() {
			StorageNode rs = Env.RootStorageNode;
			Env.Schema.Load(Env.GetAppDir() + "extension", rs==null? null : rs.FindChildNode("params"));
		}
		private void BuildIndicators() {
			IndicatorSetBuilder bld = new IndicatorSetBuilder();
			bld.Construct(Env.Options.ChartFormat);
			_indicators = bld.Result;
		}
	}

}
