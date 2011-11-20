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
using Zanetti.CodeExtendData;

namespace Zanetti.DataSource.Specialized {
	internal class MujinzouDataSource : DailyDataSource {

		private bool _includesDomesticIndices;
        private bool _IsFundamentalDownload = false;

		public MujinzouDataSource(int[] dates) : base(dates) {
		}

		public bool IncludesDomesticIndices {
			get {
				return _includesDomesticIndices;
			}
			set {
				_includesDomesticIndices = value;
			}
		}
        public bool IsFundamentalDownload
        {
            get
            {
                return _IsFundamentalDownload;
            }
            set
            {
                _IsFundamentalDownload = value;
            }
        }


		public override void Run() {

            //hacked by rti
            //業績情報
            ArrayList newCodeExtendsDataGK = new ArrayList();   //会社決算発表
            ArrayList newCodeExtendsDataG = new ArrayList();   //業績修正
            //------------------------------------

			Hashtable[] newdata = new Hashtable[_dates.Length];
			//データをまずダウンロード
			for(int i=0; i<_dates.Length; i++) {
				newdata[i] = FillData(_dates[i]);
                //hacked by rti
                //業績情報をダウンロード
                if (this._IsFundamentalDownload)
                {
                    if (IsDownloadExtendsData(_dates[i]))
                    {
                        newCodeExtendsDataGK.Add(DownloadExtendsDataGK(_dates[i]));
                        newCodeExtendsDataG.Add(DownloadExtendsDataG(_dates[i]));
                    }
                }
                //------------------------
                SendMessage(AsyncConst.WM_ASYNCPROCESS, (_dates[i] & DataSourceBase.DATE_MASK), AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}

			//各データの追加と保存
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = (AbstractBrand)ie.Value;
				if((br.Market==MarketType.B && !IsMujinzouSupportedIndices(br.Code)) || br.Market==MarketType.Custom) continue;
			
				bool trace_flag = false;
				using(DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(_dates.Length)) {
					for(int i=0; i<_dates.Length; i++) {
						NewDailyData td = (NewDailyData)newdata[i][br.Code];
						if(td==null) {
							if(!trace_flag) {
								trace_flag = true;
								Debug.WriteLine("Data not found(mujinzou) : code="+br.Code+" market="+br.Market.ToString());
							}
						}
						else
							f.UpdateDataFarm(_dates[i], td);
					}
                    //hacked by rti
                    //決算情報(GK) 
                    foreach (ArrayList dates in newCodeExtendsDataGK)
                    {//GK乙
                        foreach (CodeExtendData.DataAccounts da in dates)
                        {
                            if (da.Code == br.Code)
                            {
                                f.CodeExtend.Update(da);
                            }
                        }
                    }
                    //業績修正(G) 
                    foreach (ArrayList dates in newCodeExtendsDataG)
                    {
                        foreach (CodeExtendData.DataRevised dr in dates)
                        {
                            if (dr.Code == br.Code)
                            {
                                f.CodeExtend.Update(dr);
                            }
                        }
                    }
                    //拡張情報の保存
                    f.CodeExtend.Write(br.Code);
                    //----------------------------------
					f.Save(Util.GetDailyDataFileName(br.Code));
				}
				SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
			}
		}

        //ある日付に対して、コードのint値からPanRollingTradeDataへのハッシュテーブルを構築して返す
        private Hashtable FillData(int date)
        {
			TextReader r = null;
			try {
				Hashtable result = new Hashtable();
				DateTime d2 = Util.IntToDate(date);
                //無尽蔵にはデータが２種類あり、日経先物等の有無、名証銘柄の有無、出来高０のときの扱い、が異なるようだ。先物のデータがとれるようにこちらを使用する
                //2007/3/1, eonetからのデータが入手不能になる。標準をinfoseekに切り替えるとともに、ついでに設定ファイルで切り替え可能にした。相場が大荒れの時期に面倒くさいことが起きたものだ。
                //2007/3/31, さらにinfoseekが移動
                string url = Env.Options.MujinzouUsesKakoData ?
                    String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/T{3:D2}{4:D2}{5:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month, d2.Day) :
                    String.Format("http://souba-data.com/data_day/{0}d/{1:D2}_{2:D2}d/T{3:D2}{4:D2}{5:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month, d2.Day);
                //hacked by rti
                //r = new StreamReader(ExtractData(url), Encoding.Default);
                Stream st = ExtractData(url);
                if (st == null)
                {
                    //ファイルがなかった!!
                    return new Hashtable();
                }
                r = new StreamReader(st, Encoding.Default);
                //------

				string line = r.ReadLine();
				while(line!=null) {
					string[] t = line.Split(',');
					//例      0    1  2             3     4     5     6     7,         8,       9
					//2005/5/20,1001,11,1001 日経平均,11104,11110,11034,11037,1257840000,東証１部
					if(t.Length==10 && t[1].Length==4) {
						int code = ParseCode(t[1]);

						BasicBrand br = Env.BrandCollection.FindBrand(code) as BasicBrand;
						if(br!=null && CheckMarket(br, Util.ParseInt(t[2], 0))) {

							double vv = 1, pv = 1;
							//倍率調整
							if(IsDomesticIndex(code)) {
								vv = 0.001; //DreamVisorのものにあわせる格好で。1000株単位かな
								pv = 100;
							}
							if(code==(int)BuiltInIndex.TOPIX_F) { //TOPIX先物は整数単位で記録されている
								pv = 10;
							}

							NewDailyData td = new NewDailyData();
							td.volume = (int)(Double.Parse(t[8]) * vv);
							NewDailyData existing = (NewDailyData)result[code];
							//Debug.WriteLine(line);
							td.open = (int)(Double.Parse(t[4]) * pv);
							td.high = (int)(Double.Parse(t[5]) * pv);
							td.low  = (int)(Double.Parse(t[6]) * pv);
							td.close= (int)(Double.Parse(t[7]) * pv);
							result[code] = td;
						}
					}
					line = r.ReadLine();
				}
				return result;
			}
            finally
            {
				if(r!=null) r.Close();
			}
		}

        //hackedby rti
//        private static Stream ExtractData(string url)
//        {
//            MemoryStream ms = Util.HttpDownload(url);
//            ms.Close();
//            byte[] input = ms.ToArray();
//            ArrayList list = new ExtractLha().extract(input);
//            return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
//        }
        private static Stream ExtractData(string url)
        {
            MemoryStream ms = null;
            try
            {
                ms = Util.HttpDownload(url);
                ms.Close();
            }
            catch (System.Net.WebException webException)
            {
                //404Not Found?
                //つーか、例外を飛ばさない 404の通知も作ってくれよ >> ビル
                //デバッガで例外を監視すると面倒になるぢゃないか。
                //そもそも、例外というのは、本当に例外の時意外には飛ばしちゃダメだってママから教わらなかったかい
                if (webException.Status == System.Net.WebExceptionStatus.ProtocolError &&
                    ((System.Net.HttpWebResponse)webException.Response).StatusCode == System.Net.HttpStatusCode.NotFound)
                {
                    return null;
                }
            }
            //無尽蔵の移転先は 404の時に 404のページを 200 OK で返してくれる素敵仕様なので対応する.
            //C# ってメモリ比較とかどーやるのか、よくわからんので適当にやってみる
            byte[] input = ms.ToArray();
            string a = Encoding.ASCII.GetString(input);
            if (Encoding.ASCII.GetString(input).IndexOf("File Not Found") != -1 )
            {
                return null;
            }
            
            ArrayList list = new ExtractLha().extract(input);
            return new MemoryStream(((Extract.FileBuffer)list[0]).buffer);
        }
        //--------------

        private static bool CheckMarket(BasicBrand br, int muj)
        {
            MarketType mt = br.Market;
            switch (mt)
            {
                //2006/12/26のデータより、東証はすべてコード11, 大証１・２部は21, ヘラクレスは23に変更になった。旧データを読んだときでも大丈夫なように古い判定も残しておく
                case MarketType.B:
                    return muj == 11; //指数は東証一部として記録されている
                case MarketType.T1:
                    return muj == 11;
                case MarketType.T2:
                    return muj == 11 || muj == 12;
                case MarketType.O1:
                    return muj == 21;
                case MarketType.O2:
                    return muj == 21 || muj == 22;
                case MarketType.M:
                    return muj == 21 || muj == 13;
                case MarketType.J:
                    return muj == 91;
                case MarketType.H:
                    return muj == 23 || muj == 24;
                default:
                    return false;
            }
        }

        private static bool IsMujinzouSupportedIndices(int code)
        {
			return code==(int)BuiltInIndex.Nikkei225 ||
				code==(int)BuiltInIndex.TOPIX ||
				code==(int)BuiltInIndex.JASDAQ ||
				code==(int)BuiltInIndex.Nikkei225_F ||
				code==(int)BuiltInIndex.TOPIX_F;
		}
		private static int ParseCode(string code) {
			int t = Util.ParseInt(code);
			//以下に該当するものでなければデータの取り込みは行われない
			if(t<1300) {
				switch(t) {
					case 1001: return (int)BuiltInIndex.Nikkei225;
					case 1002: return (int)BuiltInIndex.TOPIX;
					case 1004: return (int)BuiltInIndex.Nikkei225_F;
					case 1005: return (int)BuiltInIndex.JASDAQ;
					case 1006: return (int)BuiltInIndex.TOPIX_F;
					default: return 0;
				}
			}
			else
				return t; //ふつうの銘柄
		}
		private static bool IsDomesticIndex(int code) {
			return code==(int)BuiltInIndex.Nikkei225 || code==(int)BuiltInIndex.TOPIX || code==(int)BuiltInIndex.JASDAQ;
		}

        //hacked by rti
        //拡張データをダウンロードするべきか?
        private bool IsDownloadExtendsData(int inDate)
        {
            //月の最後だったら取得する.
            DateTime d = Util.IntToDate(inDate);
            if (d.AddDays(1).Day == 1)
            {
                return true;
            }

            //取得する期間の最後の日だったら、取得する.
            if (_dates[_dates.Length - 1] == inDate)
            {
                return true;
            }

            //まだ取得しなくてもいい
            return false;
        }

        //決算情報
        private ArrayList DownloadExtendsDataGK(int inDate) //GK乙
        {
            Debug.Assert(IsDownloadExtendsData(inDate) == true);

            ArrayList result = new ArrayList();
            DateTime d2 = Util.IntToDate(inDate);

            string url = String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/GK{3:D2}{4:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month);
            //ダウンロード xls中身を展開.
            ArrayList al = ExtractXLSData(url);
            if (al == null)
            {
                return result;
            }

            //xlsの内容をクラスに割り振る
            for (int i = 1; i < al.Count; i++)
            {
                ArrayList line = (ArrayList)al[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }

                //以下の順番
                //0日付	1コード	2市場区分	3銘柄名	4前期決算	5決算区分	6前期売上（百万）	7経常利益（百万）	8利益百万）	9一株益（銭）	10配当金（銭）	今期決算	決算区分	前期売上（百万）	経常利益（百万）	利益　　（百万）	一株益（銭）	配当金（銭）	次期決算	決算区分	前期売上（百万）	経常利益（百万）	利益　　（百万）	一株益（銭）	配当金（銭）	一株純資産（銭）
                CodeExtendData.DataAccounts ext = new CodeExtendData.DataAccounts();
                ext.Code = Util.atoi((string)line[1]);
                ext.Date = Util.StringDateToInt((string)line[0]);
                ext.Kubun = ((string)line[5] == "1") ? 1 : 3;    //無尽蔵は 1 だったら中間   2 だったら本決算
                ext.ZenkiKettusan = Util.StringDateToInt((string)line[4]);       //前期決算日
//                    ext.ZenkiUriage = Util.atoi((string)line[6]);         //前期売上(百万円)
//                    ext.ZenkiKeijyourieki = Util.atoi((string)line[7]);   //前期経常利益(百万円)
//                    ext.Zenkirieki = Util.atoi((string)line[8]);          //前期利益(百万円)
//                    ext.ZenkiHitoikabueki = Util.atoi((string)line[9]);   //前期一株益(銭)
//                    ext.ZenkiHaitoukin = Util.atoi((string)line[10]);      //前期配当金(銭)
                ext.KonkiKettusan = Util.StringDateToInt((string)line[11]);       //今期決算
                ext.KonkiUriage = Util.atoi((string)line[13]);         //今期売上(百万円)
                ext.KonkiKeijyourieki = Util.atoi((string)line[14]);   //今期経常利益(百万円)
                ext.Konkirieki = Util.atoi((string)line[15]);          //今期利益(百万円)
                ext.KonkiHitoikabueki = Util.atoi((string)line[16]);   //今期一株益(銭)
                ext.KonkiHaitoukin = Util.atoi((string)line[17]);      //今期配当金(銭)
                ext.JikiKettusan = Util.StringDateToInt((string)line[18]);        //次期決算
                ext.JikiUriage = Util.atoi((string)line[20]);          //次期売上(百万円)
                ext.JikiKeijyourieki = Util.atoi((string)line[21]);    //次期経常利益(百万円)
                ext.Jikirieki = Util.atoi((string)line[22]);           //次期利益(百万円)
                ext.JikiHitoikabueki = Util.atoi((string)line[23]);    //次期一株益(銭)
                ext.JikiHaitoukin = Util.atoi((string)line[24]);       //次期配当金(銭)
                ext.Hitokabujyunsisan = Util.atoi((string)line[25]);   //一株純資産

                result.Add(ext);
            }
            al = null;
            return result;
        }
        //業績修正
        private ArrayList DownloadExtendsDataG(int inDate)
        {
            Debug.Assert(IsDownloadExtendsData(inDate) == true);

            ArrayList result = new ArrayList();
            DateTime d2 = Util.IntToDate(inDate);

            string url = String.Format("http://souba-data.com/data/{0}/{1:D2}_{2:D2}/G{3:D2}{4:D2}.lzh", d2.Year, (d2.Year % 100), d2.Month, (d2.Year % 100), d2.Month);
            //ダウンロード xls中身を展開.
            ArrayList al = ExtractXLSData(url);
            if (al == null)
            {
                return result;
            }

            //xlsの内容をクラスに割り振る
            for (int i = 2; i < al.Count; i++)
            {
                ArrayList line = (ArrayList)al[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }

                //0発表日	1コード	2銘柄名	3決算区分	4変則	5決算期	6修正前売上高	7売上高（百万円）8変化率（％）9営業益（百万円）	10修正前経常益	11経常益（百万円）12変化率（％）	13修正前税引益	14税引益　（百万円）	15変化率（％）	16一株 17修正前配当	18配当　　（円）	変化率（％）	純資産	摘要	市場																																																																																																																																																																																																																																									
                CodeExtendData.DataRevised ext = new CodeExtendData.DataRevised();
                ext.Code = Util.atoi((string)line[1]);
                ext.Date = Util.StringDateToInt((string)line[0]);
                ext.SyuseimaeUriage = Util.atoi((string)line[6]);           //修正前売上
                ext.Uriage = Util.atoi((string)line[7]);                   //売上
                ext.SyuseimaeKeijyourieki = Util.atoi((string)line[10]);           //修正前経常利益
                ext.Keijyourieki = Util.atoi((string)line[11]);                    //経常利益
                ext.SyuseimaeZeibikieki = Util.atoi((string)line[13]);           //修正前税引益
                ext.Zeibikieki = Util.atoi((string)line[14]);                    //税引益
                ext.SyuseimaeHaitou = Util.atoi((string)line[17]);           //修正前配当
                ext.Haitou = Util.atoi((string)line[18]);                    //配当

                //単体決算なら連結決算がないか探す、、あれば単体は無視する、文句あっか
                if (ExtendsDataGIsTantai((string)line[3]))
                {
                    //で、連結はあるわけ?
                    if (ExtendsDataGFindRenketu(al, i + 1, ext.Code))
                    {//連結アルヂャン、じゃ無視ね
                        continue;
                    }
                }

                result.Add(ext);
            }
            al = null;
            return result;
        }
        //単体
        private bool ExtendsDataGIsTantai(string inLine)
        {
            return inLine.IndexOf("単") != -1;
        }
         //業績修正
        private bool ExtendsDataGFindRenketu(ArrayList inCsv , int inSearchLine , int inCode)
        {
            for (int i = inSearchLine ; i < inCsv.Count; i++)
            {
                ArrayList line = (ArrayList)inCsv[i];
                if (((string)line[0]).Length == 0)
                {
                    continue;
                }
                if (inCode == Util.atoi((string)line[1]))
                {
                    if (! ExtendsDataGIsTantai( (string) line[3] ) )
                    {
                        //連結見っけ!!
                        return true;
                    }
                }
            }
            return false;
        }
        
        //lzhで圧縮されているデータをダウンロードして、 xlsファイルを読み込みます
        private static ArrayList ExtractXLSData(string url)
        {
            MemoryStream ms = null;
            try
            {
                ms = Util.HttpDownload(url);
                ms.Close();
            }
            catch (System.Net.WebException webException)
            {
                //404Not Found?
                //つーか、例外を飛ばさない 404の通知も作ってくれよ >> ビル
                //デバッガで例外を監視すると面倒になるぢゃないか。
                //そもそも、例外というのは、本当に例外の時意外には飛ばしちゃダメだってママから教わらなかったかい
                if (webException.Status == System.Net.WebExceptionStatus.ProtocolError &&
                    ((System.Net.HttpWebResponse)webException.Response).StatusCode == System.Net.HttpStatusCode.NotFound)
                {
                    return null;
                }
            }
            //無尽蔵の移転先は 404の時に 404のページを 200 OK で返してくれる素敵仕様なので対応する.
            //C# ってメモリ比較とかどーやるのか、よくわからんので適当にやってみる
            byte[] input = ms.ToArray();
            if (Encoding.ASCII.GetString(input).IndexOf("File Not Found") != -1)
            {
                return null;
            }

            ArrayList list = new ExtractLha().extract(input);
            
            //展開したバッファをテンポラリに書き込む.
            //ADOってメモリ上のエクセルデータに対しても有効なのか?? よくわからんので一度ファイルを経由する.
   			string tmpfn = Path.GetTempFileName();

			FileStream fst = File.OpenWrite(tmpfn);
            BinaryWriter bw = new BinaryWriter(fst);
            bw.Write( ((Extract.FileBuffer)list[0]).buffer );
			bw.Close();
			fst.Close();

            try
            {
                Hashtable xls = CodeExtendData.UtilX.XLSReader(tmpfn);

                //最初のページを返す.
                foreach (string key in xls.Keys)
                {
                    return (ArrayList)xls[key];
                }
            }
            finally
            {
			    File.Delete(tmpfn);
            }
            return null;
        }
        //-------------------------
	}
}
