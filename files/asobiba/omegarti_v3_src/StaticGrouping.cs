/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Collections;

namespace Zanetti.Data
{
	//Brandオブジェクトにstaticにつける属性の計算など
	//実行はデバッグおよびメンテナンスに限り、publicではない
	internal class StaticGrouping
	{
		private class BrandComparer : IComparer {
			public int Compare(object x, object y) {
				return ((AbstractBrand)x).Code - ((AbstractBrand)y).Code;
			}

		}


		private class Entry : IComparable {
			public BasicBrand brand;
			public double activity;

			public int CompareTo(object obj) {
				double t = ((Entry)obj).activity;
				if(activity > t)
					return -1;
				else if(activity < t)
					return 1;
				else
					return 0;

			}
		}

		private static Hashtable _nikkei225;
		public static void Run() {
			_nikkei225 = new Hashtable();
			foreach(int code in NIKKEI225)
				_nikkei225.Add(code, null);

			CollectMajorBrands();
			Save();
		}


		private static void CollectMajorBrands() {
			ArrayList result = new ArrayList();
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			DataFarm f = null;
			while(ie.MoveNext()) {
				BasicBrand b = ie.Value as BasicBrand;
				if(b==null) continue;

//hacked by rti
                f = b.CloneFarm(ChartFormat.Daily);
//				if(f==null)
//                    //hacked by rti キャッシュが効くようにする
//                    //f = b.CreateFarm(ChartFormat.Daily);
//                    f = b.CloneFarm(ChartFormat.Daily);
//                    //-------------
//				else
//					f.LoadFor(b);
//------

				if(!f.IsEmpty) {
					TradeData td = f.GetByIndex(f.FilledLength-1); //最後のTradeDataを基準点にする
					Entry e = new Entry();
					e.brand = b;
					e.activity = 0;
					if(b.Market!=MarketType.B) {
						for(int i=0; i<5; i++) {
							e.activity += td.Close * td.Volume;
							td = td.Prev;
							if(td==null) break;
						}
					}
					result.Add(e);
				}
			}

			result.Sort();
			for(int i=0; i<result.Count; i++) {
				Entry e = (Entry)result[i];
				e.brand.Active500 = (i<500);
				e.brand.Nikkei225 = _nikkei225.Contains(e.brand.Code);
			}

		}

		private static void Save() {
			StreamWriter wr = new StreamWriter(Env.GetAppDir()+"newindex.txt", false, System.Text.Encoding.Default);
			ArrayList ar = new ArrayList(Env.BrandCollection.Values);
			ar.Sort(new BrandComparer());

			wr.WriteLine("date=20040912");
			foreach(AbstractBrand b0 in ar) {
				BasicBrand b = b0 as BasicBrand;
				if(b==null || b.Market==MarketType.B) continue;

				StringBuilder bld = new StringBuilder();
				bld.Append(b.CodeAsString);
				bld.Append(",");
				bld.Append(b.Name);
				bld.Append(",");
				bld.Append(b.Market.ToString());
				bld.Append(",");
				bld.Append(b.Unit.ToString());
				if(b.Obsolete) {
					bld.Append(",");
					bld.Append("OBS");
				}
				if(b.Nikkei225) {
					bld.Append(",");
					bld.Append("N225");
				}
				if(b.Active500) {
					bld.Append(",");
					bld.Append("A500");
				}
				if(b.SplitInfo!=null) {
					foreach(SplitInfo si in b.SplitInfo) {
						bld.Append(",");
						bld.Append(String.Format("S:{0}:{1}", si.Date, si.Ratio));
					}
				}
				wr.WriteLine(bld.ToString());
			}
			wr.Close();

		}

		//2003年10月末時点の日経平均採用銘柄
		//取得するには、http://www3.nikkei.co.jp/nkave/about/225_list.cfm
		//のHTMLを保存して、grep -E "onedown>[0-9]{4}" nikkei225htm.txt > temp.txt
		private static readonly int[] NIKKEI225 = {
													  2001,
													  2002,
													  2201,
													  2202,
													  2261,
													  2501,
													  2502,
													  2503,
													  2531,
													  2536,
													  2602,
													  2801,
													  2802,
													  2871,
													  2914,
													  3101,
													  3102,
													  3103,
													  3105,
													  3110,
													  3401,
													  3402,
													  3404,
													  3405,
													  3861,
													  3864,
													  3865,
													  3893,
													  3407,
													  4004,
													  4005,
													  4010,
													  4021,
													  4041,
													  4042,
													  4045,
													  4061,
													  4063,
													  4151,
													  4208,
													  4272,
													  4452,
													  4901,
													  4902,
													  4911,
													  4501,
													  4502,
													  4503,
													  4505,
													  4506,
													  4507,
													  4511,
													  4523,
													  4543,
													  5001,
													  5002,
													  5016,
													  5101,
													  5108,
													  5201,
													  5202,
													  5232,
													  5233,
													  5301,
													  5332,
													  5333,
													  5401,
													  5405,
													  5406,
													  5411,
													  5701,
													  5706,
													  5707,
													  5711,
													  5713,
													  5714,
													  5715,
													  5801,
													  5802,
													  5803,
													  5901,
													  5631,
													  6103,
													  6301,
													  6302,
													  6326,
													  6361,
													  6366,
													  6367,
													  6471,
													  6472,
													  6473,
													  6474,
													  7004,
													  7011,
													  6479,
													  6501,
													  6502,
													  6503,
													  6504,
													  6508,
													  6701,
													  6702,
													  6703,
													  6752,
													  6753,
													  6758,
													  6762,
													  6764,
													  6767,
													  6770,
													  6773,
													  6796,
													  6841,
													  6857,
													  6902,
													  6933,
													  6952,
													  6954,
													  6971,
													  6976,
													  6991,
													  7751,
													  7003,
													  7012,
													  7013,
													  7201,
													  7202,
													  7203,
													  7205,
													  7211,
													  7261,
													  7267,
													  7269,
													  7270,
													  7102,
													  7231,
													  7731,
													  7733,
													  7752,
													  7762,
													  7911,
													  7912,
													  7951,
													  1332,
													  1601,
													  1721,
													  1801,
													  1802,
													  1803,
													  1812,
													  1861,
													  1925,
													  1928,
													  1963,
													  2768,
													  8001,
													  8002,
													  8003,
													  8031,
													  8035,
													  8053,
													  8058,
													  2779,
													  8183,
													  8232,
													  8233,
													  8238,
													  8252,
													  8264,
													  8267,
													  8306,
													  8307,
													  8308,
													  8309,
													  8316,
													  8331,
													  8332,
													  8355,
													  8403,
													  8404,
													  8411,
													  8601,
													  8603,
													  8604,
													  8606,
													  8752,
													  8755,
													  8766,
													  8253,
													  8583,
													  8801,
													  8802,
													  8803,
													  8830,
													  9001,
													  9005,
													  9007,
													  9008,
													  9009,
													  9020,
													  9021,
													  9062,
													  9064,
													  9101,
													  9104,
													  9107,
													  9202,
													  9205,
													  9301,
													  9432,
													  9433,
													  9437,
													  9613,
													  9501,
													  9502,
													  9503,
													  9531,
													  9532,
													  4704,
													  9605,
													  9681,
													  9735,
													  9737,
													  9766
												  };

	}
}
