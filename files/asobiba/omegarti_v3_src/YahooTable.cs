using System;
using System.Text;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Text.RegularExpressions;

using Zanetti.Data;
using Travis.Archive;
using Travis.Http;
using Zanetti.CodeExtendData;

namespace Zanetti.DataSource.Specialized
{
    internal class YahooDataSource : DailyDataSource {

        //上場廃止銘柄のダウンロードを行わない
        public bool IsIgnoreObsolete = true;

        public YahooDataSource(int[] dates)  : base(dates)
        {
		}

        public override void Run() {

            //ダウンロード開始日と終了日
            int startDate = GetStartDate(this._dates);
            int endDate = GetEndDate(this._dates);

            Hashtable newdata = new Hashtable();

            //データをダウンロード
            IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
            while (ie.MoveNext())
            {
                AbstractBrand br = (AbstractBrand)ie.Value;
                if ( br.Market == MarketType.Custom) continue;
                if (this.IsIgnoreObsolete)
                {//上場廃止は無視する設定の場合
                    if (br is BasicBrand && ((BasicBrand)br).Obsolete == true)
                    {
                        continue;
                    }
                }

                if (endDate - startDate > 20)
                {//あんまり間が空くとこっちで縦断爆撃しないといけない...
                    newdata[br.Code] = DownloadOldDate(br, startDate, endDate);
                }
                else
                {//そんなに間が空いていないなら、Yahoo!に負荷がかからないと思われる方法で
                    newdata[br.Code] = DownloadCurrentDate(br);
                }

                SendMessage(AsyncConst.WM_ASYNCPROCESS, (startDate & DataSourceBase.DATE_MASK), AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
            }

            //データをインポート
            
            ie = Env.BrandCollection.GetEnumerator();
            while (ie.MoveNext())
            {
                AbstractBrand br = (AbstractBrand)ie.Value;
                if (br.Market == MarketType.Custom) continue;

                Hashtable tr = (Hashtable)newdata[br.Code];
                if (tr == null)
                {
                    continue;
                }
                ArrayList dates = new ArrayList(tr.Keys);
                dates.Sort();

                bool trace_flag = false;
                using (DailyDataFarm f = (DailyDataFarm)br.CreateDailyFarm(dates.Count ))
                {
                    foreach(int day in dates)
                    {
                        NewDailyData td = (NewDailyData)tr[day];
                        if (td == null)
                        {
                            if (!trace_flag)
                            {
                                trace_flag = true;
                                Debug.WriteLine("Data not found(yahoo) : code=" + br.Code + " market=" + br.Market.ToString());
                            }
                        }
                        else
                            f.UpdateDataFarm(day, td);
                    }
                    f.Save(Util.GetDailyDataFileName(br.Code));
                }
                SendMessage(AsyncConst.WM_ASYNCPROCESS, br.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
            }

        }

        //開始日検索
        int GetStartDate(int[] inDateArray)
        {
            Debug.Assert(inDateArray.Length >= 1);

            int r = inDateArray[0];

            for (int i = 1; i < inDateArray.Length; i++)
            {
                if (r > inDateArray[i])
                {
                    r = inDateArray[i];
                }
            }
            return r;
        }
        //終了日検索
        int GetEndDate(int[] inDateArray)
        {
            Debug.Assert(inDateArray.Length >= 1);

            int r = inDateArray[0];

            for (int i = 1; i < inDateArray.Length; i++)
            {
                if (r < inDateArray[i])
                {
                    r = inDateArray[i];
                }
            }
            return r;
        }
        //コードをyahoo形式で取得する  5401 -> 5401.t
        //未対応の場合 空白になる
        string GetYahooCode(int inCode, MarketType inType)
        {
            string codeString = inCode + ".";
            switch (inType)
            {
                case MarketType.T1:
                case MarketType.T2:
                    codeString += "t";
                    break;
                case MarketType.O1:
                case MarketType.O2:
                    codeString += "o";
                    break;
                case MarketType.J:
                    codeString += "q";
                    break;
                case MarketType.M:
                    codeString += "t";
                    break;
                case MarketType.H:
                    codeString += "j";
                    break;
                case MarketType.B:
                    switch (inCode)
                    {
                        case (int)BuiltInIndex.Nikkei225:   //日経平均
                            codeString = "998407.O";
                            break;
                        case (int)BuiltInIndex.TOPIX:   //TOPIX
                            codeString = "998405.T";
                            break;
                        case (int)BuiltInIndex.JASDAQ:   //JASDAQ
                            codeString = "23337.Q";
                            break;
                        case (int)BuiltInIndex.Nikkei225_F:   //日経平均先物
                            codeString = "5040469.O";
                            break;
                        default:
                            //未対応
                            codeString = "";
                            break;
                    }
                    break;
                default:
                    //未対応
                    codeString = "";
                    break;
            }

            return codeString;
        }

        //古いデータをダウンロード
        //Hashtable[日時] = NewDailyData 形式
        Hashtable DownloadOldDate(AbstractBrand inBR, int inStartDate, int inEndDate)
        {
            DateTime d2 = Util.IntToDate(inStartDate);
            DateTime e2 = Util.IntToDate(inEndDate);
            string codeString = GetYahooCode(inBR.Code , inBR.Market);
            int count = 0;

            Hashtable ret = new Hashtable();
            if (codeString == "")
            {
                return ret; //未対応
            }

            while (true)
            {
                string url = String.Format("http://table.yahoo.co.jp/t?c={0}&a={1}&b={2}&f={3}&d={4}&e={5}&g=d&s={6}&y={7}&z={6}", d2.Year, d2.Month, d2.Day, e2.Year, e2.Month, e2.Day, codeString, count);
                string html = Download(url);

                //ダウンロードした htmlの解析
                if (!ParseHTML(ret, inBR.Code, html))
                {
                    break;
                }

                //次のデータがある?
                if (html.IndexOf("次の") < 0)
                {
                    break;  //ないなら終わる
                }
                count += 50;
            }
            return ret;
        }

        //数日の新しいデータをダウンロード
        //Hashtable[日時] = NewDailyData 形式
        Hashtable DownloadCurrentDate(AbstractBrand inBR)
        {
            string codeString = GetYahooCode(inBR.Code, inBR.Market);

            Hashtable ret = new Hashtable();
            if (codeString == "")
            {
                return ret; //未対応
            }

            string url = String.Format("http://table.yahoo.co.jp/t?s={0}&g=d", codeString);
            string html = Download(url);

            //ダウンロードした htmlの解析
            ParseHTML(ret, inBR.Code, html);

            return ret;
        }

        string Download(string inUrl)
        {
            try
            {
                MemoryStream ms = Util.HttpDownload(inUrl);
                ms.Close();
                return Encoding.GetEncoding("euc-jp").GetString(ms.ToArray());
            }
            catch(Exception e)
            {
                throw new Exception("URL(" + inUrl + ")をダウンロード中にエラーが発生しました。\r\n例外メッセージ:" + e.Message, e);
            }
        }

        bool ParseHTML(Hashtable ioReslut ,int inCode ,string inHTML)
        {
            int kaishine = inHTML.IndexOf("始値");
            if (kaishine < 0)
            {
                return false;
            }
            int tableStart = inHTML.LastIndexOf("<table ", kaishine);
            if (tableStart < 0)
            {
                return false;
            }

            double vv = 1, pv = 1;
            //倍率調整
            if (IsDomesticIndex(inCode))
            {
                vv = 0.001; //DreamVisorのものにあわせる格好で。1000株単位かな
                pv = 100;
            }
            if (inCode == (int)BuiltInIndex.TOPIX_F)
            { //TOPIX先物は整数単位で記録されている
                pv = 10;
            }

            //テーブルの取り込み
            ArrayList y = TableToCsv(inHTML.Substring(tableStart));
            for (int yCount = 1 ; yCount < y.Count ; yCount ++ )
            {
                NewDailyData td = new NewDailyData();
                ArrayList x = (ArrayList)y[yCount];
                if (((string)x[1]).IndexOf("分割") >= 0)
                {
                    //分割?
                    continue;
                }
                int date = Util.StringDateToInt((string)x[0]);
                td.open =  (int) (double.Parse((string)x[1]) * pv);
                td.high = (int) (double.Parse((string)x[2]) * pv);
                td.low = (int) (double.Parse((string)x[3]) * pv);
                td.close = (int) (double.Parse((string)x[4]) * pv);
                if (x.Count > 5)
                {
                    td.volume = (int) (double.Parse((string)x[5]) * vv);
                }
                else
                {
                    td.volume = 0;
                }
                ioReslut[date] = td;
            }
            return true;
        }
        private static bool IsDomesticIndex(int code)
        {
            return code == (int)BuiltInIndex.Nikkei225 || code == (int)BuiltInIndex.TOPIX || code == (int)BuiltInIndex.JASDAQ;
        }

        //table タグを csv に変換
        //複雑なテーブルには未対応です。
        //ArrayList[y] = {ArrayList[x]} 形式です.
        ArrayList TableToCsv(string inHTML)
        {
            ArrayList y = new ArrayList();

            //枝刈り
            inHTML = inHTML.Replace("<TABLE", "<table");
            inHTML = inHTML.Replace("</TABLE", "</table");
            inHTML = inHTML.Replace("<TR", "<tr");
            inHTML = inHTML.Replace("</TR", "</tr");
            inHTML = inHTML.Replace("<TD", "<td");
            inHTML = inHTML.Replace("</TD", "</td");
            inHTML = inHTML.Replace("<TH", "<td");   //TD扱いにする
            inHTML = inHTML.Replace("</TH", "</td"); //TD扱いにする
            inHTML = inHTML.Replace("<th", "<td");   //TD扱いにする
            inHTML = inHTML.Replace("</th", "</td"); //TD扱いにする

            int tableStart = inHTML.IndexOf("<table");
            if (tableStart < 0)
            {
                return y;
            }
            int tableEnd = inHTML.IndexOf("</table>", tableStart);
            if (tableEnd < 0)
            {
                return y;
            }
            int trLoop = tableStart;
            while(true)
            {
                //TRタグ 行の取得
                int trStart = inHTML.IndexOf("<tr", trLoop, tableEnd - trLoop);
                if (trStart < 0)
                {
                    break;
                }
                int trEnd = inHTML.IndexOf("</tr>", trStart, tableEnd - trStart);
                if (trEnd < 0)
                {
                    break;
                }
                trLoop = trEnd + 1;

                //TDタグ 列の取得
                ArrayList x = new ArrayList();
                int tdLoop = trStart;
                while (true)
                {
                    int tdStart = inHTML.IndexOf("<td", tdLoop, trEnd - tdLoop);
                    if (tdStart < 0)
                    {
                        break;
                    }
                    int tdEnd = inHTML.IndexOf("</td>", tdStart, trEnd - tdStart);
                    if (tdEnd < 0)
                    {
                        break;
                    }
                    tdLoop = tdEnd + 1;

                    //中身を保存.
                    string node = inHTML.Substring(tdStart , tdEnd - tdStart);
                    x.Add(KillTag(node));
                }
                y.Add(x);
            }
            return y;
        }
        string KillTag(string inHTML)
        {
            string retString = "";
            int tagLoop = 0;

            while (true)
            {
                int tagStart = inHTML.IndexOf('<', tagLoop);
                if (tagStart < 0)
                {
                    break;
                }
                //タグが始まるまでのテキストの取り込み
                if (tagStart - tagLoop > 0)
                {
                    retString += inHTML.Substring(tagLoop, tagStart - tagLoop);
                }
                int tagEnd = inHTML.IndexOf('>', tagStart);
                if (tagEnd < 0)
                {
                    break;  //タグを閉じていない??
                }
                tagLoop = tagEnd + 1;
            }

            return retString;
        }
    }
}
