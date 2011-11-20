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
using Zanetti.Indicators;
using Zanetti.Arithmetic;
using Zanetti.Parser;

namespace Zanetti.Prediction
{
	/// <summary>
	/// Statistics の概要の説明です。
	/// </summary>
	internal class Statistics
	{
		public class Entry {
			public int date;
			public double value;
		}

		private class ItemComparer : IComparer {

			public int Compare(object x, object y) {
				return ((Entry)x).value.CompareTo(((Entry)y).value);
			}


		}


		public class HistgramEntry {
			public double startRange;
			public int count;
		}


		private ArrayList _data;
		private double _average;
		private double _absAverage;
		private double _standardDev;
		private double _absStandardDev;
		private double _min;
		private int _minDate;
		private double _max;
		private int _maxDate;

		private ArrayList _histgram;

		public Statistics() {
			_data = new ArrayList();
		}

		public double Average {
			get {
				return _average;
			}
		}
		public double AbsAverage {
			get {
				return _absAverage;
			}
		}
		public double StandardDev {
			get {
				return _standardDev;
			}
		}
		public double AbsStandardDev {
			get {
				return _absStandardDev;
			}
		}
		public double Min {
			get {
				return _min;
			}
		}
		public double Max {
			get {
				return _max;
			}
		}
		public double ValueAt(int index) {
			return ((Entry)_data[index]).value;
		}
		public int MinDate {
			get {
				return _minDate;
			}
		}
		public int MaxDate {
			get {
				return _maxDate;
			}
		}
		public int Count {
			get {
				return _data.Count;
			}
		}
		public IEnumerable HistgramEntries {
			get {
				return _histgram;
			}
		}
		public void AddData(Entry e) {
			_data.Add(e);
		}

		public void CalcStats() {
			FirstLoop();
			SecondLoop();

			_data.Sort(new ItemComparer());
		}

		private void FirstLoop() {
			//Phase2 １回目の嘗め
			_average = 0;
			_absAverage = 0;
			_min = Double.MaxValue;
			_max = Double.MinValue;
			foreach(Entry e in _data) {
				_average += e.value;
				_absAverage += Math.Abs(e.value);
				if(_min > e.value) {
					_min = e.value;
					_minDate = e.date;
				}
				if(_max < e.value) {
					_max = e.value;
					_maxDate = e.date;
				}
			}
			_average /= (double)_data.Count;
			_absAverage /= (double)_data.Count;

			//histgram準備
			_histgram = new ArrayList();
			double pitch = 0.01;
			for(int i=0; i<18; i++) {
				HistgramEntry e  = new HistgramEntry();
				e.startRange = -0.1 + pitch*i;
				_histgram.Add(e);
			}


		}
		private void SecondLoop() {
			//Phase3 ２回目。
			_standardDev = 0;
			_absStandardDev = 0;
			double hb = ((HistgramEntry)_histgram[0]).startRange;
			double pitch = ((HistgramEntry)_histgram[1]).startRange - hb;

			foreach(Entry e in _data) {
				double t = e.value - _average;
				_standardDev += t*t;
				t = Math.Abs(e.value) - _absAverage;
				_absStandardDev += t*t;

				int i = (int)Math.Floor((e.value-hb)/pitch);
				if(i<0) i = 0;
				if(i>=_histgram.Count) i = _histgram.Count-1;
				((HistgramEntry)_histgram[i]).count++;
			}
			_standardDev = Math.Sqrt(_standardDev / (double)_data.Count);
			_absStandardDev = Math.Sqrt(_absStandardDev / (double)_data.Count);
		}
	}


	/*
	internal class MyStatistics {

		public static void Do() {
			ExpressionConstructor cp = new ExpressionConstructor();
			new ZPredicationParser(new StringReader("close()/HMA(25) - 1"), cp).Parse();
			ExpressionConstructor vp = new ExpressionConstructor();
			new ZPredicationParser(new StringReader("close_at(0-{0})/close() - 1"), vp).Parse();

			DataFarm farm = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
			foreach(int days in new int[] {5, 8, 10, 13, 15, 20}) {
				MyStatistics ms = new MyStatistics(farm, cp.Result, vp.Result, days);
			}

		}

		private ArrayList _stats;
		private int _days;

		public MyStatistics(DataFarm farm, Expression cond, Expression value, int days) {
			_days = days;
			_stats = new ArrayList();
			BuildData(farm, cond, value);
			Print();
		}

		private void BuildData(DataFarm farm, Expression cond, Expression value) {
			//Phase1 データ取得
			DateRangeChecker ch = new DateRangeChecker(null);
			DateRange r = (DateRange)cond.Apply(ch);
			DateRange rv = (DateRange)value.Apply(ch);
			if(r!=null)
				r.Merge(rv);
			else if(rv!=null)
				r = rv;
			else
				r = DateRange.Default;
			//r.begin = -25;
			//r.end = 25;
			Evaluator ev = new Evaluator();
			ev.Args = new EvalResult[] { new EvalResult((double)_days) };
			ev.Farm = farm;

			for(int i=0; i<farm.RealLength; i++) {
				if(i+r.begin>=0 && i+r.end<=farm.RealLength) {
					ev.BaseIndex = i;

					TradeData td = farm.GetByIndex(i);
					double kairi = ((EvalResult)cond.Apply(ev)).DoubleVal;
					Statistics s = GetTarget(kairi);
					Statistics.Entry e = new Statistics.Entry();
					e.date = td.Date;
					e.value = ((EvalResult)value.Apply(ev)).DoubleVal;
					s.AddData(e);
				}
			}

			foreach(Statistics st in _stats)
				st.CalcStats();
		}
		private Statistics GetTarget(double value) {
			if(_stats.Count==0) {
				for(int j=0; j<18; j++)
					_stats.Add(new Statistics());
			}

			int i;
			if(value<-0.08)
				i = 0;
			else if(value>0.08)
				i = 17;
			else
				i = (int)Math.Floor(value/0.01) + 8;

			return (Statistics)_stats[i];
		}

		private void Print() {
			StreamWriter wr = new StreamWriter("C:\\Zanetti\\privatekit\\optionstrategy\\day"+_days+".csv", false, System.Text.Encoding.Default);
			wr.Write("分布,最小日付,最小値,");
			for(int i=0; i<18; i++)
				wr.Write(String.Format("～{0}%,", i-9));
			wr.WriteLine("合計,最大日付,最大,平均,偏差");
			int val = -8;
			foreach(Statistics st in _stats) {
				wr.Write(String.Format("～{0}%,", val++));
				if(st.Count>0) {
					wr.Write(st.MinDate);
					wr.Write(String.Format(",{0:F2},",st.Min*100));
					foreach(Statistics.HistgramEntry ent in st.HistgramEntries) {
						wr.Write(ent.count);
						wr.Write(",");
					}
					wr.Write(st.Count);
					wr.Write(",");
					wr.Write(st.MaxDate);
					wr.Write(String.Format(",{0:F2},",st.Max*100));
					wr.Write(String.Format("{0:F2}", st.Average*100));
					wr.Write(",");
					wr.Write(String.Format("{0:F2}", st.StandardDev*100));

				}
				wr.WriteLine();
			}
			wr.Close();
		}
	}
	*/
	internal class ChangeStatistics {
		private static Statistics Calc(DataFarm farm, int days) {
			Statistics st = new Statistics();
			for(int i=0; i<farm.FilledLength-days; i++) {
				double change = farm.GetByIndex(i+days).Close / farm.GetByIndex(i).Close - 1;
				Statistics.Entry e = new Statistics.Entry();
				e.date = farm.GetByIndex(i).Date;
				e.value = change;
				if(days==1 && change>0.018)
					Debug.WriteLine(String.Format("{0} {1:F2}%", Util.FormatShortDate(e.date), change*100));
				st.AddData(e);
			}
			st.CalcStats();
			return st;
		}
		private static void Print(StreamWriter wr, Statistics st, int days) {
			wr.Write(days.ToString());
			wr.Write(",");
			wr.Write(st.MinDate);
			wr.Write(String.Format(",{0:F2},",st.Min*100));
			foreach(Statistics.HistgramEntry ent in st.HistgramEntries) {
				wr.Write(ent.count);
				wr.Write(",");
			}
			wr.Write(st.Count);
			wr.Write(",");
			wr.Write(st.MaxDate);
			wr.Write(String.Format(",{0:F2},",st.Max*100));
			wr.Write(String.Format("{0:F2}", st.Average*100));
			wr.Write(",");
			wr.Write(String.Format("{0:F2}", st.StandardDev*100));
			wr.Write(",");
			wr.Write(String.Format("{0:F2}", st.ValueAt((int)(st.Count*0.1))*100));
			wr.Write(",");
			wr.Write(String.Format("{0:F2}", st.ValueAt((int)(st.Count*0.9))*100));
			wr.WriteLine();
		}

		public static void Exec() {
			DataFarm farm = Env.Frame.ChartCanvas.GetBrand().ReserveFarm();
			StreamWriter wr = new StreamWriter("C:\\Zanetti\\privatekit\\optionstrategy\\changes.csv", false, System.Text.Encoding.Default);
			for(int days=1; days<=15; days++) {
				Statistics st = Calc(farm, days);
				Print(wr, st, days);
			}
			wr.Close();
		}
	}
}
