/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Drawing;
using System.Text;

using Zanetti.Data;
using Travis.Storage;

namespace Zanetti.UI
{
	/// <summary>
	/// FreeLine の概要の説明です。
	/// </summary>
	internal class FreeLine {
		private Point _pivot;
		private Point _destination;
		private int _id; //SolidFreeLineからの作成時のみセット、それ以外は-1
		private LineDrawMode _mode;

		public enum LineDrawMode {
			Normal,
			Hilight
		}

		public FreeLine(Point p) {
			_pivot = p;
			_destination = p;
			_id = -1;
		}
		public FreeLine(Point p1, Point p2) {
			_pivot = p1;
			_destination = p2;
			_id = -1;
		}
		//リサイズしたときなどの再計算
		public FreeLine(DataFarm farm, int firstdateindex, SolidFreeLine fl, Trans value_to_y) {
			int p = Env.Layout.DatePitch;
			_pivot = new Point((farm.DateToIndex(fl._date1)-firstdateindex)*p + p/2, (int)value_to_y.TransValue(fl._value1));
			_destination = new Point((farm.DateToIndex(fl._date2)-firstdateindex)*p + p/2, (int)value_to_y.TransValue(fl._value2));
			_id = fl._id;
		}

		public Point Pivot {
			get {
				return _pivot;
			}
		}
		public Point Destination {
			get {
				return _destination;
			}
			set {
				_destination = value;
			}
		}
		public LineDrawMode DrawMode {
			get {
				return _mode;
			}
			set {
				_mode = value;
			}
		}
		public int ID {
			get {
				return _id;
			}
		}


		public Rectangle GetInclusion(Rectangle rect) {
			if(_pivot==_destination)
				return new Rectangle(_pivot, new Size(0,0));
			Point[] es = GetEdge(rect, _pivot, _destination);
			return new Rectangle(Math.Min(es[0].X, es[1].X), Math.Min(es[0].Y, es[1].Y), Math.Abs(es[0].X-es[1].X), Math.Abs(es[0].Y-es[1].Y));
		}

		//十分に離した位置でないと線を確定させないようにする
		public bool PivotHasEnoughDistanceTo(Point pt) {
			return Math.Abs(_pivot.X-pt.X)>10 || Math.Abs(_pivot.Y-pt.Y)>10;
		}

		public void Draw(Rectangle rect, IntPtr hdc) {
			Win32.POINT pt = new Win32.POINT();
			Win32.SelectObject(hdc, _mode==LineDrawMode.Normal? Env.Preference.FreeLinePen.Handle : Env.Preference.FreeLineBoldPen.Handle);
			Point p1 = _pivot;
			Point p2 = _destination;
			if(!rect.Contains(p1) && !rect.Contains(p2)) { //どっちも含まれていないときは
				ArrayList f = GetAllEdges(rect, p1, p2);
				Win32.SelectObject(hdc, _mode==LineDrawMode.Normal? Env.Preference.FreeLineDottedPen.Handle : Env.Preference.FreeLineBoldPen.Handle);
				Win32.MoveToEx(hdc, ((Point)f[1]).X, ((Point)f[1]).Y, out pt);
				Win32.LineTo(hdc, ((Point)f[f.Count==2? 0 : 2]).X, ((Point)f[f.Count==2? 0 : 2]).Y); //通常2,3番目の点に注目すればよい
			}
			else {
				if(!rect.Contains(p1)) p1 = GetEdge(rect, p2, p1)[0];
				if(!rect.Contains(p2)) p2 = GetEdge(rect, p1, p2)[0];
				Win32.MoveToEx(hdc, p1.X, p1.Y, out pt);
				Win32.LineTo(hdc, p2.X, p2.Y);
			
				Point[] es = GetEdge(rect, p1, p2);
				Win32.SelectObject(hdc, _mode==LineDrawMode.Normal? Env.Preference.FreeLineDottedPen.Handle : Env.Preference.FreeLineBoldPen.Handle);
				Win32.MoveToEx(hdc, p2.X, p2.Y, out pt);
				Win32.LineTo(hdc, es[0].X, es[0].Y);
				Win32.MoveToEx(hdc, p1.X, p1.Y, out pt);
				Win32.LineTo(hdc, es[1].X, es[1].Y);
			}
		}

		//p1,p2がrectに入っているとき、p1からp2に伸ばした線とp2からp1に伸ばした線がそれぞれrectと交差するところを返す
		private static Point[] GetEdge(Rectangle rect, Point p1, Point p2) {
			Point[] result = new Point[2];
			ArrayList ar = GetAllEdges(rect, p1, p2);
			if(ar.Count==2) return (Point[])ar.ToArray(typeof(Point));

			result[0] = (Point)(p1.X>p2.X? ar[1] : ar[2]);
			result[1] = (Point)(p1.X>p2.X? ar[2] : ar[1]);
			return result;
		}
		//p1とp2を結ぶ線がrectを構成する４直線と交差する点を返す。水平・垂直のときは２個になる
		private static ArrayList GetAllEdges(Rectangle rect, Point p1, Point p2) {
			ArrayList ar = new ArrayList();
			if(p1.X==p2.X) {
				ar.Add(new Point(p1.X, p1.Y<p2.Y? rect.Bottom : rect.Top));
				ar.Add(new Point(p1.X, p1.Y>p2.Y? rect.Bottom : rect.Top));
				return ar;
			}
			else if(p1.Y==p2.Y) {
				ar.Add(new Point(p1.X<p2.X? rect.Right : rect.Left, p1.Y));
				ar.Add(new Point(p1.X>p2.X? rect.Right : rect.Left, p1.Y));
				return ar;
			}

			LinearTrans lt = LinearTrans.Solve(p1.X, p1.Y, p2.X, p2.Y);
			ar.Add(new Point(rect.Left, (int)lt.TransValue(rect.Left)));
			ar.Add(new Point(rect.Right, (int)lt.TransValue(rect.Right)));
			ar.Add(new Point((int)lt.Inverse(rect.Top), rect.Top));
			ar.Add(new Point((int)lt.Inverse(rect.Bottom), rect.Bottom));
			//この４点をX座標の順に並べ、２番目と３番目が答え
			ar.Sort(new PointComparer());
			return ar;
		}
		private class PointComparer : IComparer {

			public int Compare(object x, object y) {
				return ((Point)x).X - ((Point)y).X;
			}
		}

		public SolidFreeLine ToSolid(DataFarm farm, int firstdateindex, Trans value_to_y) {
			SolidFreeLine fl = new SolidFreeLine();
			int ind1 = firstdateindex + (int)Math.Floor((double)_pivot.X/Env.Layout.DatePitch);
            int ind2 = firstdateindex + (int)Math.Floor((double)_destination.X / Env.Layout.DatePitch);

            LinearTrans tr = LinearTrans.Solve(ind1, _pivot.Y, ind2, _destination.Y);
			double y1 = _pivot.Y;
			if(ind1>=farm.FilledLength) {
				ind1 = farm.FilledLength-1;
				y1 = tr.TransValue(ind1);
			}
			else if(ind1<0) {
				ind1 = 0;
				y1 = tr.TransValue(ind1);
			}
			fl._date1  = farm.GetByIndex(ind1).Date;
			fl._value1 = value_to_y.Inverse(y1);

			double y2 = _destination.Y;
			if(ind2>=farm.FilledLength) {
				ind2 = farm.FilledLength-1;
				y2 = tr.TransValue(ind2);
			}
			else if(ind2<0) {
				ind2 = 0;
				y2 = tr.TransValue(0);
			}
			fl._date2  = farm.GetByIndex(ind2).Date;
			fl._value2 = value_to_y.Inverse(y2);
			if(_id==-1) {
				fl._id = SolidFreeLine.NextID++;
				_id = fl._id;
			}
			else {
				fl._id = _id;
			}
			return fl;
		}

		//p1,p2を結ぶ直線とpの距離を返す
		public double GetDistance(Point p) {
			double[] par = GetNormalizedParam();
			return Math.Abs(par[0] * p.X + par[1] * p.Y + par[2]);
		}

		//両端を通る直線を ax+by+c=0, a^2+b^2=1 となる形式でa,b,cの配列で返す
		private double[] GetNormalizedParam() {
			double a, b, c;
			if(_pivot.X==_destination.X) {
				a = 1;
				b = 0;
			}
			else {
				double d = -(_pivot.Y - _destination.Y) / (double)(_pivot.X - _destination.X);
				b = Math.Sqrt(1 / (1 + d*d));
				a = b * d;
			}
			c = -(a * _pivot.X + b * _pivot.Y);
			return new double[] { a, b, c };
		}
	}

	internal class SolidFreeLine {
		public int _id;
		public int _code;
		public ChartFormat _targetFormat;
		public bool _logScale;
		public int _date1;
		public double _value1;
		public int _date2;
		public double _value2;


		private static int _nextID;
		public static int NextID {
			get {
				return _nextID;
			}
			set {
				_nextID = value;
			}
		}
	}

	internal class FreeLineCollection {
		private ArrayList _data;
		public FreeLineCollection() {
			_data = new ArrayList();
		}
		public int Count {
			get {
				return _data.Count;
			}
		}

		public void Add(AbstractBrand br, ChartFormat format, bool logScale, SolidFreeLine fl) {
			fl._code = br.Code;
			fl._targetFormat = format;
			fl._logScale = logScale;
			_data.Add(fl);
		}

		public SolidFreeLine[] Find(AbstractBrand br, ChartFormat format, bool logScale) {
			ArrayList t = new ArrayList();
			foreach(SolidFreeLine fl in _data) {
				if(fl._code==br.Code && fl._targetFormat==format && fl._logScale==logScale)
					t.Add(fl);
			}
			return (SolidFreeLine[])t.ToArray(typeof(SolidFreeLine));
		}

		public void ClearAll() {
			_data.Clear();
		}
		public void Clear(AbstractBrand br, ChartFormat format, bool logScale) {
			ArrayList temp = new ArrayList();
			IEnumerator ie = _data.GetEnumerator();
			while(ie.MoveNext()) {
				SolidFreeLine fl = (SolidFreeLine)ie.Current;
				if(!(fl._code==br.Code && fl._targetFormat==format && fl._logScale==logScale))
					temp.Add(fl);
			}
			_data = temp;
		}
		public void Remove(int id) {
			for(int i=0; i<_data.Count; i++) {
				SolidFreeLine l = (SolidFreeLine)_data[i];
				if(l._id==id) {
					_data.RemoveAt(i);
					break;
				}
			}
		}


		public void Load(StorageNode parent) {
			string t = parent["free-lines"];
			if(t==null) return;
			foreach(string ee in t.Split(',')) {
				SolidFreeLine fl = new SolidFreeLine();
				string[] e = ee.Split(':');
                //hacked by rti
                if (e.Length <= 0 || e[0] == "")
                {
                    continue;
                }
                //--------------
				fl._code = Int32.Parse(e[0]);
				switch(e[1]){
					case "D":
						fl._targetFormat = ChartFormat.Daily;
						fl._logScale = false;
						break;
					case "W":
						fl._targetFormat = ChartFormat.Weekly;
						fl._logScale = false;
						break;
					case "M":
						fl._targetFormat = ChartFormat.Monthly;
						fl._logScale = false;
						break;
					case "DL":
						fl._targetFormat = ChartFormat.Daily;
						fl._logScale = true;
						break;
					case "WL":
						fl._targetFormat = ChartFormat.Weekly;
						fl._logScale = true;
						break;
					case "ML":
						fl._targetFormat = ChartFormat.Monthly;
						fl._logScale = true;
						break;
				}
				fl._date1 = Int32.Parse(e[2]);
				fl._value1 = Double.Parse(e[3]);
				fl._date2 = Int32.Parse(e[4]);
				fl._value2 = Double.Parse(e[5]);
				fl._id = SolidFreeLine.NextID++;
				_data.Add(fl);
			}
		}
		public void SaveTo(StorageNode parent) {
			StringBuilder bld = new StringBuilder();
			foreach(SolidFreeLine sl in _data) {
				if(bld.Length>0) bld.Append(",");
				String format;
				switch(sl._targetFormat){
					case ChartFormat.Daily:
					default:
						format = "D";
						break;
					case ChartFormat.Weekly:
						format = "W";
						break;
					case ChartFormat.Monthly:
						format = "M";
						break;
				}
				if(sl._logScale){
					format += "L";
				}
				bld.Append(String.Format("{0}:{1}:{2}:{3:F2}:{4}:{5:F2}", sl._code, format, sl._date1, sl._value1, sl._date2, sl._value2));
			}
			parent["free-lines"] = bld.ToString();
		}
	}
}
