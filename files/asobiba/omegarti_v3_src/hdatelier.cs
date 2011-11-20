/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Collections;
using System.Diagnostics;


using Zanetti.Data;
using Travis.Archive;
using Travis.Http;

namespace Zanetti.DataSource.Specialized {
	internal class HdatelierDataSource : DailyDataSource {
		public HdatelierDataSource(int[] dates) : base(dates) {
		}

		public override void Run() {
			Hashtable[] newdata = new Hashtable[_dates.Length];
			//データをまずダウンロード
			for(int i=0; i<_dates.Length; i++)
				newdata[i] = FillData(_dates[i]);

			//各データの追加と保存
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = (AbstractBrand)ie.Value;
				if(br.Market==MarketType.B || br.Market==MarketType.Custom) continue;

				using(DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(_dates.Length)) {
					bool trace_flag = false;
					for(int i=0; i<_dates.Length; i++) {
						NewDailyData td = (NewDailyData)newdata[i][br.Code];
						if(td==null) {
							if(!trace_flag) {
								trace_flag = true;
								Debug.WriteLine("Data not found(hdetalier) : code="+br.Code+" market="+br.Market.ToString());
							}
						}
						else
							f.UpdateDataFarm(_dates[i], td);
					}

					f.Save(Util.GetDailyDataFileName(br.Code));
				}
				SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}
		}


		//ある日付に対して、コードのint値からPanRollingTradeDataへのハッシュテーブルを構築して返す
		private Hashtable FillData(int date) {
			TextReader r = null;
			try {
				Hashtable result = new Hashtable();
				string datetext = date.ToString();
				string url = String.Format("http://homepage1.nifty.com/hdatelier/data/{0}.lzh", datetext);
				r = new StreamReader(ExtractData(url), Encoding.Default);

				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split(',');
					//例       0    1    2  3   4   5      6
                    //2004/10/08,1301,199,202,199,200,817000
					if(t.Length==7 && t[1].Length==4) { //指数類があるので
						int code = Util.ParseInt(t[1]);
						
						bool skip = false;
						if(Env.BrandCollection.FindBrand(code)==null) { //存在しない銘柄は処理しない
							skip = true;
						}

						double vv = 1, pv = 1;
						if(!skip) {
							NewDailyData td = new NewDailyData();
							td.volume = (int)(Double.Parse(t[6]) * vv);
							NewDailyData existing = (NewDailyData)result[code];
							//Debug.WriteLine(line);
							td.open = (int)(Double.Parse(t[2]) * pv);
							td.high = (int)(Double.Parse(t[3]) * pv);
							td.low  = (int)(Double.Parse(t[4]) * pv);
							td.close= (int)(Double.Parse(ToClosePrice(t[5], td.volume)) * pv);
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

		private static Stream ExtractData(string url) {
			MemoryStream ms = Util.HttpDownload(url);
			ms.Close();
			byte[] input = ms.ToArray();
			ArrayList list = new ExtractLha().extract(input);
			return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
		}

		//出来高が０だと最終気配等の文字が入るため調整
		private static string ToClosePrice(string src, double vol) {
			if(vol!=0) return src;

			int t = src.IndexOf(' ');
			if(t==-1)
				return src;
			else
				return src.Substring(0, t);
		}

	}
}
