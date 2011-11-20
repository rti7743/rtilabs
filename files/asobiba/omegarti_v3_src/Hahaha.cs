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

namespace Zanetti.DataSource.Specialized
{
	internal class HahahaDataSource : DailyDataSource
	{
		public HahahaDataSource(int[] dates) : base(dates) {
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
								Debug.WriteLine("Data not found(hahaha) : code="+br.Code+" market="+br.Market.ToString());
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
				string url = String.Format("http://www.bekkoame.ne.jp/ha/hahaha/data{0}/{1}.LZH", datetext.Substring(0,4), datetext);
				r = new StreamReader(ExtractData(url), Encoding.Default);

				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split(',');
					//例       0    1    2              3      4      5      6     7 8
					//2004/10/01,8316,"三井住友Ｆ",643000,660000,642000,656000,41845,1
					if(t.Length==9) { //１行目には日付があるようだ
						int code = Int32.Parse(t[1]);
						
						double pv = 1, vv = 1;
						bool skip = false;
						//コードの特例
						if(code==1001 || code==1002) { //日経平均,TOPIX
							skip = true;
						}
						else if(Env.BrandCollection.FindBrand(code)==null) { //存在しない銘柄は処理しない
							skip = true;
						}

						if(!skip) {
							NewDailyData td = new NewDailyData();
							td.volume = (int)(Double.Parse(t[7]) * vv);
							NewDailyData existing = (NewDailyData)result[code];
							//無尽蔵では複数市場に上場している銘柄には複数のエントリがある。出来高の多いほうをとる。
							if(existing==null || existing.volume < td.volume) {
								//Debug.WriteLine(line);
								td.open = (int)(Double.Parse(t[3]) * pv);
								td.high = (int)(Double.Parse(t[4]) * pv);
								td.low  = (int)(Double.Parse(t[5]) * pv);
								td.close= (int)(Double.Parse(t[6]) * pv);
								result[code] = td;
							}
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
			MemoryStream ms = new HTTPConnection(url).Open();
			ms.Close();
			byte[] input = ms.ToArray();
			ArrayList list = new ExtractLha().extract(input);
			return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
		}

	}
}
