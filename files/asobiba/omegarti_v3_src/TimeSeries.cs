/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Diagnostics;

using Zanetti.Indicators;

namespace Zanetti.Arithmetic.Series
{
	/// <summary>
	/// TimeSeries の概要の説明です。
	/// </summary>
	internal abstract class TimeSeries {
		public TimeSeries() {
			InitStats();
		}
		public abstract int Count {
			get;
		}

		public abstract class Cursor {
			public abstract bool HasNext {
				get;
			}
			public abstract double Next {
				get;
			}
		}

		public abstract Cursor CreateCursor();
		public abstract double LastValue { get; }

		private double _min;
		private double _max;
		private double _avg;
		private void InitStats() {
			_min = Double.NaN;
			_max = Double.NaN;
			_avg = Double.NaN;
		}

		public double Min {
			get {
				if(Double.IsNaN(_min)) {
					double t = Double.MaxValue;
					Cursor c = CreateCursor();
					while(c.HasNext) {
						double v = c.Next;
						if(t > v) t = v;
					}
					_min = t;
				}
				return _min;
			}
		}
		public double Max {
			get {
				if(Double.IsNaN(_max)) {
					double t = Double.MinValue;
					Cursor c = CreateCursor();
					while(c.HasNext) {
						double v = c.Next;
						if(t < v) t = v;
					}
					_max = t;
				}
				return _max;
			}
		}
		public double Average {
			get {
				if(Double.IsNaN(_avg)) {
					double t = 0;
					Cursor c = CreateCursor();
					while(c.HasNext) {
						double v = c.Next;
						t += v;
					}
					_avg = t / this.Count;
				}
				return _avg;
			}
		}

        //hacked by rti
        public double[] ToArray()
        {
            double [] arr = new double[ this.Count ];
            Cursor c = this.CreateCursor();
            for (int i = 0; c.HasNext ;  i ++)
            {
                arr[i] = c.Next;
            }
            return arr;
        }
        //----------------------
		//public double Deviation
	}

	internal class BinOpTimeSeries : TimeSeries {
		public delegate double OP(double v1, double v2);
		public static readonly OP ADD = new OP(Add);
		public static readonly OP SUB = new OP(Sub);
		public static readonly OP MUL = new OP(Mul);
		public static readonly OP DIV = new OP(Div);
		public static readonly OP POW = new OP(Pow);
		private static double Add(double v1, double v2) {
			return v1 + v2;
		}
		private static double Sub(double v1, double v2) {
			return v1 - v2;
		}
		private static double Mul(double v1, double v2) {
			return v1 * v2;
		}
		private static double Div(double v1, double v2) {
			return v1 / v2;
		}
		private static double Pow(double v1, double v2) {
			return Math.Pow(v1, v2);
		}

		protected TimeSeries _left;
		protected TimeSeries _right;
		protected OP _operator;

		public BinOpTimeSeries(TimeSeries left, TimeSeries right, OP op) {
			if(left.Count!=right.Count)
				throw new Exception("長さの違うTimeSeriesを演算しようとしました");
			_left = left;
			_right = right;
			_operator = op;
		}
		public override int Count {
			get {
				return _left.Count;
			}
		}

		protected class BinOpCursor : Cursor {
			private Cursor _left;
			private Cursor _right;
			private BinOpTimeSeries _parent;

			public BinOpCursor(BinOpTimeSeries ser, Cursor left, Cursor right) {
				_parent = ser;
				_left = left;
				_right = right;
			}
			public override bool HasNext {
				get {
					return _left.HasNext;
				}
			}
			public override double Next {
				get {
					return _parent._operator(_left.Next, _right.Next);
				}
			}

		}
		public override Cursor CreateCursor() {
			return new BinOpCursor(this, _left.CreateCursor(), _right.CreateCursor());
		}
		public override double LastValue {
			get {
				return _operator(_left.LastValue, _right.LastValue);
			}
		}
	}

	internal class SingleOpTimeSeries : TimeSeries {
		public delegate double OP(double v);
		public static readonly OP ABS = new OP(Math.Abs);
		public static readonly OP LOG = new OP(Math.Log);
		public static readonly OP SQRT= new OP(Math.Sqrt);
		public static readonly OP NEG = new OP(Neg);
		private static double Neg(double v) {
			return -v;
		}

		//定数倍
		public static OP CreateScalar(double v) {
			return new OP(new Scalar(v).Calc);
		}
		//定数加算
		public static OP CreateSlided(double v) {
			return new OP(new Slided(v).Calc);
		}
		//定数乗
		public static OP CreatePow(double v) {
			return new OP(new Pow(v).Calc);
		}
		private class Scalar {
			private double _v;
			public Scalar(double v) {
				_v = v;
			}
			public double Calc(double t) {
				return t * _v;
			}
		}
		private class Slided {
			private double _v;
			public Slided(double v) {
				_v = v;
			}
			public double Calc(double t) {
				return t + _v;
			}
		}
		private class Pow {
			private double _v;
			public Pow(double v) {
				_v = v;
			}
			public double Calc(double t) {
				return Math.Pow(t, _v);
			}
		}

		protected TimeSeries _ts;
		protected OP _operator;

		public SingleOpTimeSeries(TimeSeries ts, OP op) {
			_ts = ts;
			_operator = op;
		}
		public override int Count {
			get {
				return _ts.Count;
			}
		}
		public override Cursor CreateCursor() {
			return new SingleOpCursor(_ts.CreateCursor(), this);
		}
		public override double LastValue {
			get {
				return _operator(_ts.LastValue);
			}
		}


		private class SingleOpCursor : Cursor {
			private SingleOpTimeSeries _parent;
			private Cursor _cursor;

			public SingleOpCursor(Cursor c, SingleOpTimeSeries p) {
				_cursor = c;
				_parent = p;
			}
			public override bool HasNext {
				get {
					return _cursor.HasNext;
				}
			}
			public override double Next {
				get {
					return _parent._operator(_cursor.Next);
				}
			}
		}
	}


	//差分　サイズは１へる
	internal class DiffTimeSeries : TimeSeries {
		protected TimeSeries _ts;

		public DiffTimeSeries(TimeSeries ts) {
			_ts = ts;
		}
		public override int Count {
			get {
				return _ts.Count-1;
			}
		}
		public override Cursor CreateCursor() {
			return new DiffCursor(_ts.CreateCursor());
		}
		public override double LastValue {
			get {
				//Diffのときは仕方があるまい
				Cursor c = CreateCursor();
				double t = c.Next;
				double p = 0;
				while(c.HasNext) {
					p = t;
					t = c.Next;
				}
				return p;
			}
		}


		private class DiffCursor : Cursor {
			private Cursor _cursor;
			private double _prevval;

			public DiffCursor(Cursor c) {
				_cursor = c;
				Debug.Assert(c.HasNext);
				_prevval = c.Next;
			}
			public override bool HasNext {
				get {
					return _cursor.HasNext;
				}
			}
			public override double Next {
				get {
					double v = _cursor.Next;
					double ret = v - _prevval;
					_prevval = v;
					return ret;
				}
			}
		}
	}

	//生データによるTimeSeries
	internal class RawTimeSeries : TimeSeries {
		protected double[] _data;

		public RawTimeSeries(double[] data) {
			_data = data;
		}
		public override int Count {
			get {
				return _data.Length;
			}
		}
		public override Cursor CreateCursor() {
			return new RawCursor(_data);
		}
		public override double LastValue {
			get {
				return _data[_data.Length-1];
			}
		}


		private class RawCursor : Cursor {
			private double[] _data;
			private int _index;

			public RawCursor(double[] t) {
				_data = t;
				_index = 0;
			}
			public override bool HasNext {
				get {
					return _index<_data.Length;
				}
			}
			public override double Next {
				get {
					return _data[_index++];
				}
			}
		}
	}
}
