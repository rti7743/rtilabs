/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Collections;

namespace Zanetti.Data
{
	//–ˆTŒ—j‚©‚ç‚ÌŒ©o‚µ€–Ú
	internal class WeeklyIndex
	{
		private int _index;
		private int _firstDate;
		private bool _startOfQuarter;

		public int FirstDate {
			get {
				return _firstDate;
			}
		}
		public int Index {
			get {
				return _index;
			}
		}
		public bool StartOfQuarter {
			get {
				return _startOfQuarter;
			}
		}

		public WeeklyIndex(int index, int firstdate, bool quarter) {
			_index = index;
			_firstDate = firstdate;
			_startOfQuarter = quarter;
		}
	}

	//–ˆTŒ—j‚ÌƒRƒŒƒNƒVƒ‡ƒ“
	internal class WeekFormat {
		private WeeklyIndex[] _data;

		private void Construct() {
			DateTime dt = new DateTime(1987,12,27); //‚±‚¢‚Â‚Í“ú—j“ú
			ArrayList t = new ArrayList(500);
			DateTime now = DateTime.Now;
			int i = 0;
			while(dt < now) {
				int dti = Util.DateToInt(dt);
				int m = (dti % 10000) / 100;
				int d = dti % 100;
				t.Add(new WeeklyIndex(i++, dti, d<=7 && (m==1 || m==4 || m==7 || m==10)));
				dt = dt.AddDays(7);
			}

			_data = (WeeklyIndex[])t.ToArray(typeof(WeeklyIndex));
		}

		public WeeklyIndex FindByDate(int date) {
			if(_data==null) Construct();
			if(date<_data[1].FirstDate)
				return _data[0]; //—¼’[‚Í‚æ‚­‚ ‚éƒP[ƒX‚È‚Ì‚Å
			else if(date>=_data[_data.Length-1].FirstDate)
				return _data[_data.Length-1];
			else
				return FindByDate(date, 0, _data.Length); 
		}
		private WeeklyIndex FindByDate(int date, int start, int end) {
			int m = (start+end)/2;
			if(_data[m].FirstDate > date)
				return FindByDate(date, start, m);
			else if(_data[m+1].FirstDate <= date)
				return FindByDate(date, m+1, end);
			else
				return _data[m];
		}

		public WeeklyIndex GetAt(int index) {
			if(_data==null) Construct();
			Debug.Assert(index>=0 && index<_data.Length);
			return _data[index];
		}
		public int Length {
			get {
				if(_data==null) Construct();
				return _data.Length;
			}
		}
	}


}
