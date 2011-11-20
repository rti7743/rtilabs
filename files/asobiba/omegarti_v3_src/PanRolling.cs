/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Threading;
using System.Collections;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.DataSource.Specialized
{
	internal class PanRollingTradeData {
		public int open;
		public int high;
		public int low;
		public int close;
		public string volume;
		public double volumeratio;
	}

	internal class PanRollingDataSource : DailyDataSource {

		public PanRollingDataSource(params int[] dates) : base(dates) {
		}

		private Hashtable _codes_thousandth;

		public override void Run() {
			Hashtable[] newdata = new Hashtable[_dates.Length];
			InitThousandthCodes();
			//データをまずダウンロード
			for(int i=0; i<_dates.Length; i++)
				newdata[i] = FillData(_dates[i]);

			//各データの追加と保存
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = (AbstractBrand)ie.Value;
				using(DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(_dates.Length)) {
					if(!f.IsEmpty) { 
						for(int i=0; i<_dates.Length; i++) {
							if(f.LastDate<_dates[i]) {
								PanRollingTradeData td = (PanRollingTradeData)newdata[i][br.Code];
								if(td==null) {
									if(br.Market!=MarketType.J)
										Debug.WriteLine("Data not found : code="+br.Code+" market="+br.Market.ToString());
								}
								else
									ExtendDataFarm(f, _dates[i], td);
							}
						}
						f.Save(Util.GetDailyDataFileName(br.Code));
					}
				}
				SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}
		}

		private void ExtendDataFarm(DailyDataFarm f, int date, PanRollingTradeData td) {
			if(td.volumeratio==0)
				td.volumeratio = GuessVolumeRatio(f, (double)f.GetByIndex(f.FilledLength-1).Close, td.volume);

			unsafe {
				f.WriteExtraData(0 , date);
				f.WriteExtraData(4 , td.open);
				f.WriteExtraData(8 , td.high);
				f.WriteExtraData(12, td.low);
				f.WriteExtraData(16, td.close);
				f.WriteExtraData(20, (int)(Double.Parse(td.volume)*td.volumeratio));
				f.ProgressExtraDataAddress();
			}
		}

		//ある日付に対して、コードのint値からPanRollingTradeDataへのハッシュテーブルを構築して返す
		private Hashtable FillData(int date) {
			TextReader r = null;
			try {
				Hashtable result = new Hashtable();
				string url = String.Format("http://www.panrolling.com/data/daily/s{0}.txt", date);
				r = new StreamReader(Util.HttpDownload(url), Encoding.Default);
				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split('\t');
					if(t.Length==7) { //１行目には日付があるようだ
						int code = Int32.Parse(t[0]);
						double v = 1;
						bool skip = false;
						//コードの特例
						if(code==1001) { //日経平均
							v = 100;
							code = (int)BuiltInIndex.Nikkei225;
						}
						else if(code==1002) { //TOPIX
							v = 100;
							code = (int)BuiltInIndex.TOPIX;
						}
						else if(code==1003) { //ドル
							code = (int)BuiltInIndex.JPYUSD;
						}
						else if(code==1008) //ユーロ？
							skip = true;
						else if(_codes_thousandth.Contains(code))
							v = 1000;
						else if(code==9861 || code==7615 || code==9409 || code==9479)
							v = 100;

						if(!skip) {
							PanRollingTradeData td = new PanRollingTradeData();
							td.volumeratio = 0;
							td.volume = t[6];
							//Debug.WriteLine(line);
							td.open = (int)(Double.Parse(t[2]) * v);
							td.high = (int)(Double.Parse(t[3]) * v);
							td.low  = (int)(Double.Parse(t[4]) * v);
							td.close= (int)(Double.Parse(t[5]) * v);
							result[code] = td;
						}
					}
					line = r.ReadLine();
				}
				return result;
			}
			finally {
				if(r!=null) r.Close();
			}
		}

		//価格が実際の1/1000に書かれている銘柄をセット
		private void InitThousandthCodes() {
			_codes_thousandth = new Hashtable();
			int[] data = new int[] {
				4740, 4741, 4753, 4795, 6829, 7717, 9442, 8421, 4788, 4759,
				6786, 2656, 4763, 4771, 4777, 2665, 9444, 4765, 4697, 4764, 
				7589, 4792, 4797, 4800, 4744, 6790, 4776, 4751, 2652, 4815, 
				4824, 4747, 2667, 4787, 2681, 4784, 4814, 4742, 9445, 4794, 
				4779, 4821, 4818, 7641
									};
			foreach(int t in data)
				_codes_thousandth.Add(t, null);
		}
				

		//PanRollingのデータは出来高表記がまちまち。1/10/100/1000倍のいずれかをするのだが、これを自動判定する
		private double GuessVolumeRatio(DataFarm farm, double lastprice, string expr) {
			TradeData last = farm.GetByIndex(farm.FilledLength-1);
			double min = 1;
			int period = expr.IndexOf('.');
			if(period>0) {
				if(period==expr.Length-4) return 1000; //小数以下３位まであれば明らか
				
				if(period==expr.Length-3) min = 100;
				else if(period==expr.Length-2) min = 10;
			}

			double l = Math.Log10(Double.Parse(expr) / last.Volume);
			double cand;
			if(l<-2.5)
				cand = 1000;
			else if(l<-1.5)
				cand = 100;
			else if(l<-0.5)
				cand = 10;
			else
				cand = 1;

			cand = Math.Max(cand, min);

			if(0.9<=(lastprice/last.Close) && (lastprice/last.Close)<=1.1) {
				//値幅がごく普通の範囲に入っているなら、候補の値を信用する
				return cand;
			}
			else {
				//大きく動いて出来高も急増したときは、倍率を小さく判定するかもしれない。
				int t1 = Math.Min(GuessTanniKabu(last.Volume), GuessTanniKabu(last.Prev.Volume));
				do {
					int vol = (int)(Double.Parse(expr) * cand);
					int t2 = GuessTanniKabu(vol);
					if(t1==t2)
						return cand;
					else {
						Debug.WriteLine("GuessRatio rare case! code=" + farm.Brand.Code.ToString());
						cand *= 10;
					}
				} while(true);
			}

		}

		//出来高から単位株を推測
		private static int GuessTanniKabu(double vol_) {
			int vol = (int)vol_;
			if(vol % 10 != 0) return 1;
			else if(vol % 100 != 0) return 10;
			else if(vol % 1000 != 0) return 100;
			else return 1000;
		}

		//価格・出来高補正のためのテストプログラム
		public static void Test() {
			StreamReader pan = new StreamReader("C:\\zanetti\\doc\\pan.csv");
			StreamReader iticker = new StreamReader("C:\\zanetti\\doc\\iticker.csv");
			string pl = pan.ReadLine();
			while(pl!=null) {
				string[] rv = pl.Split(',');
				int code = Int32.Parse(rv[0]);
				double value1 = Double.Parse(rv[1]);
				double volume1 = Double.Parse(rv[2])*1000;

				string il;
				string[] sv;
				do {
					il = iticker.ReadLine();
					sv = il.Split(',');
				} while(code!=Int32.Parse(sv[0]));

				double value2 = Double.Parse(sv[1]);
				double volume2 = Double.Parse(sv[2]);
				if(value1!=value2 || volume1!=volume2) {
					if(value2==0 || volume2==0)
						Debug.WriteLine(String.Format("{0} ZERO", code));
					else
						Debug.WriteLine(String.Format("{0},{1},{2}", code, value1/value2, volume1/volume2));
				}

				pl = pan.ReadLine();
			}
		}

	}
}
