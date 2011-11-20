/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;
using System.Web;
using System.Drawing;
using System.Net;
using System.Diagnostics;
using System.Text;

using Travis.Storage;
using Travis.Util;
using Travis.Http;

using Zanetti.UI;
using Zanetti.Data;


namespace Zanetti
{
	internal enum PrimitiveIndicator {
		Date,
		Open,
		High,
		Low,
		Close,
		Volume,
		CreditLong,
		CreditShort,
		LAST
	}
	[EnumDesc(typeof(ChartFormat))]
	internal enum ChartFormat {
        [EnumValue(Description = "日足")] Daily,
        [EnumValue(Description="週足")] Weekly,
		[EnumValue(Description="月足")] Monthly
#if DOJIMA
		, [EnumValue(Description = "半日足")] HalfDaily
#endif
	}

	internal enum FormatModifier {
		Nop,
		Mul100,
		Percent
	}
	internal enum CompanyInfoSite {
		Yahoo,
		Infoseek,
		Nikkei,
		Livedoor
	}


	/// <summary>
	/// Util の概要の説明です。
	/// </summary>
	internal class Util
	{
		//ユーザ定義銘柄のコード最小値
		public const int CustomBrandMinValue = 400;

		public static void ReportCriticalError(Exception ex) {
			SilentReportCriticalError(ex);
			//メッセージボックスで通知
			string dir = Env.GetAppDir();
			MessageBox.Show(String.Format("内部エラーが発生しました。" + dir + "error.logファイルにエラー位置が記録されました。\n{0}", ex.Message), "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
		}
		public static void SilentReportCriticalError(Exception ex) {
			Debug.WriteLine(ex.Message);
			Debug.WriteLine(ex.StackTrace);
			//エラーファイルに追記
			string dir = Env.GetAppDir();
			StreamWriter sw = new StreamWriter(dir+"error.log", true);
			sw.WriteLine(DateTime.Now.ToString() + " : " + ex.Message);
			sw.WriteLine(ex.StackTrace);
			sw.Close();
		}

		public static MemoryStream HttpDownload(string url) {
			/* 2005/2/12より、ケンミレはクッキーを要求するようになった。WinHTTP経由だとこれに対応できない。
			 * スクリプトによるプロキシ設定はあきらめざるをえないが仕方ないか。
			if(HTTPConnection.IsWinHTTPAvailable && Env.Options.ProxyConfig.UseIESetting) {
				return new HTTPConnection(url).Open();
			}
			*/

			WebProxy proxy;
            if(Env.Options.ProxyConfig.UseIESetting)
				proxy = WebProxy.GetDefaultProxy();
			else
                proxy = new WebProxy(Env.Options.ProxyConfig.Address, Env.Options.ProxyConfig.Port);
			HttpWebRequest rq = (HttpWebRequest)WebRequest.Create(url);
			//rq.CookieContainer = _cookieContainer;
			rq.ProtocolVersion = new Version(1,1);
			rq.Method = "GET";
			rq.Proxy = proxy;
			HttpWebResponse rs = (HttpWebResponse)rq.GetResponse();
	
			//効率はわるいがWinHTTPが使えない環境用なのでまあいいだろう
			MemoryStream strm = new MemoryStream(0x18000);
			CopyStream(rs.GetResponseStream(), strm);
			strm.Close();
			return new MemoryStream(strm.ToArray());
		}

		public static void CopyStream(Stream input, Stream output) {
			byte[] buffer = new byte[0x20000];
			int n = input.Read(buffer, 0, buffer.Length);
			while(n > 0) {
				output.Write(buffer, 0, n);
				n = input.Read(buffer, 0, buffer.Length);
			}
		}
		public static void StreamToFile(Stream input, string filename) {
			FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
			CopyStream(input, fs);
		}

		//COLORREFに対応した整数を返す
		public static uint ToCOLORREF(Color c) {
			uint t = (uint)c.ToArgb();
			//COLORREFは0x00BBGGRR、ToArgbは0x00RRGGBB
			uint r = (t & 0x00FF0000) >> 16;
			uint b = (t & 0x000000FF) << 16;
			t &= 0x0000FF00;
			return t | r | b;
		}
		public static Color ToLightColor(Color c) {
			return Color.FromArgb((255+c.R)/2, (255+c.G)/2, (255+c.B)/2);
		}
		public static Color ToDarkColor(Color c) {
			return Color.FromArgb(c.R/2, c.G/2, c.B/2);
		}
		//c1*v + c2*(1-v)
		public static Color MergeColor(Color c1, Color c2, double v) {
			double r1 = (double)c1.R;
			double g1 = (double)c1.G;
			double b1 = (double)c1.B;
			double r2 = (double)c2.R;
			double g2 = (double)c2.G;
			double b2 = (double)c2.B;

			return Color.FromArgb((int)(r1*v+r2*(1-v)), (int)(g1*v+g2*(1-v)), (int)(b1*v+b2*(1-v)));
		}
		public static string FormatColor(Color col) {
			if(col.IsNamedColor)
				return col.Name;
			else
				return "#"+col.Name;
		}
		public static Color ParseColor(string src, Color def) {
			if(src.StartsWith("#")) {
				try {
					return Color.FromArgb(Int32.Parse(src.Substring(1), System.Globalization.NumberStyles.HexNumber));
				}
				catch(Exception) {
					return def;
				}
			}
			else {
				Color c = Color.FromName(src);
				return c;
			}
		}

        //hacked by rti
        //signal 等の式の中に // でコメントをかけるようにします。
        //本当はgrammaticaの書式で対応するべきなんでしょうが、
        //grammatica で COMMENT なるものは存在しないらしい、、
        //よって、強引にコメントを実装する
        public static string ParseComment(string src)
        {
            string[] lineArray = src.Replace("\r\n", "\n").Split('\n'); 
            string retString = "";
            for (int i = 0; i < lineArray.Length; i ++ )
            {
                string oneLine = lineArray[i];
                int commentPosstion = oneLine.IndexOf("//");
                if (commentPosstion == -1)
                {
                    //コメントは存在しないので、全文利用できる.
                    retString += oneLine + "\r\n";
                }
                else
                {
                    retString += oneLine.Substring(0, commentPosstion) + "\r\n";
                }
            }

            return retString;
        }
        //atoiみたいに空気を読んでくれる変換
        public static int atoi(string str)
        {
            int ret;
            if (! int.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        //空気を読んでくれる 日付 -> yyyymmdd 形式 変換
        public static int StringDateToInt(string str)
        {
            DateTime d;
            if (!DateTime.TryParse(str, out d))
            {
                return 0;
            }

            return Util.DateToInt(d);
        }
        //--- 


		public static string GetDailyDataFileName(int code) {
			if(code<1000)
				return Env.GetAppDir()+"data\\0"+code;
			else
				return Env.GetAppDir()+"data\\"+code;
		}
        //hacked by rti
        //コード拡張のファイル名を求める
        public static string GetExtDataFileName(int code)
        {
            return Env.GetAppDir() + "data\\" + code + ".ext.dat";
        }
        //翌日
        public static int NextDay(int date)
        {
            DateTime dt = Util.IntToDate(date);
            return Util.DateToInt( dt.AddDays(1) );
        }
        //--------------------------

		public static bool IsDailyBased(ChartFormat fmt) {
#if DOJIMA
			return fmt==ChartFormat.Daily || fmt==ChartFormat.HalfDaily;
#else
			return fmt==ChartFormat.Daily;
#endif
		}

		private static string[] _dayOfWeek = {"日", "月", "火", "水", "木", "金", "土" };

		//内部形式の日付のフォーマット
		public static string FormatFullDate(int date) {
			DateTime d = new DateTime(date / 10000, (date % 10000) / 100, (date % 100));
			return String.Format("{0}年{1:d2}月{2:d2}日({3})", d.Year, d.Month, d.Day, _dayOfWeek[(int)d.DayOfWeek]);
		}
		public static string FormatFullDate(int date, ChartFormat format) {
			DateTime d = new DateTime(date / 10000, (date % 10000) / 100, (date % 100));
			if(IsDailyBased(format))
				return String.Format("{0}年{1:d2}月{2:d2}日({3})", d.Year, d.Month, d.Day, _dayOfWeek[(int)d.DayOfWeek]);
			else if(format==ChartFormat.Weekly) {
				d = d.AddDays(5); //金曜日の位置で決めるのが自然
				return String.Format("{0}年{1:d2}月{2}週", d.Year, d.Month, (d.Day-1)/7+1);
			}
			else // Monthly
				return String.Format("{0}年{1:d2}月", d.Year, d.Month);
		}
		public static string FormatShortDate(int date) {
			DateTime d = new DateTime(date / 10000, (date % 10000) / 100, (date % 100));
			return String.Format("{0}/{1:d2}/{2:d2}", d.Year, d.Month, d.Day);
		}
		public static DayOfWeek GetDayOfWeek(int date) {
			DateTime d = new DateTime(date / 10000, (date % 10000) / 100, (date % 100));
			return d.DayOfWeek;
		}

		//len文字の右詰文字
		public static string FormatFixedLenValue(double value, int len, string format, FormatModifier mod) {
			if(Double.IsNaN(value))
				return new string(' ', len-1) + '-';
			else {
				if(mod==FormatModifier.Mul100 || mod==FormatModifier.Percent) value *= 100;
				string t = Math.Abs(value)<100000? value.ToString(format) : value.ToString("F0"); //あまりでかい数に小数点つけても仕方ない
				if(mod==FormatModifier.Percent) t += "%";
				if(t.Length<len) t = new string(' ', len-t.Length) + t;
				
				return t;
			}
		}
		//一般的な数値フォーマット
		public static string FormatValue(double value, string fs, FormatModifier mod) {
			switch(mod) {
				case FormatModifier.Nop:
					return value.ToString(fs);
				case FormatModifier.Mul100:
					return (value*100).ToString(fs);
				case FormatModifier.Percent:
					return (value*100).ToString(fs)+"%";
				default:
					return "format error";
			}
		}

		//データが利用可能と思われる最終日付を取得
		public static DateTime GuessLatestTradeDate() {
			DateTime dt = DateTime.UtcNow.AddHours(9); //日本時間を常に取得
			return GuessLatestTradeDate(dt);
		}
		//last以前で最後の日付を取得
		public static DateTime GuessLatestTradeDate(DateTime last) {
			while(!IsMarketOpenDate(last)) last = last.AddDays(-1);
			return new DateTime(last.Year, last.Month, last.Day, 0, 0, 0);
		}
		public static int DateToInt(DateTime dt) {
			return dt.Year*10000 + dt.Month*100 + dt.Day;
		}
		public static int DateToInt(int year, int month, int day) {
			return year*10000 + month*100 + day;
		}
		public static DateTime IntToDate(int dt) {
			return new DateTime(dt / 10000, (dt % 10000) / 100, (dt % 100));
		}

		public static bool IsMarketOpenDate(DateTime dt) {
			if(dt.DayOfWeek==DayOfWeek.Sunday || dt.DayOfWeek==DayOfWeek.Saturday) return false;

			int y = dt.Year;
			int m = dt.Month;
			int d = dt.Day;
			if(IsHoliday(y, m, d, false))
				return false;
			else if(dt.DayOfWeek==DayOfWeek.Monday) {
				if(IsHoliday(y, m, d, true)) //月曜の特殊祝日
					return false;
				else if(IsHoliday(y, m, d-1, false))
					return false; //振り替え休日
			}

			return true; 
		}

		public static bool IsHoliday(int year, int month, int day, bool monday) {
            //春分・秋分は本当は年により違う。このルールは2006年現在での、2010年までのもの
			switch(month) {
				case 1:
					return day==1 || day==2 || day==3 || (monday && 8<=day && day<=14);
				case 2:
					return day==11;
				case 3:
					if(day==20)
						return year==2008 || year==2009; //2008,9年は20日が春分
					else if(day==21)
						return year==2006 || year==2007 || year==2010;
					else
						return false;
				case 4:
					return day==29;
				case 5:
					return 3<=day && day<=5;
				case 7:
					return monday && 15<=day && day<=21;
				case 9:
					return (monday && 15<=day && day<=21) || day==23;
				case 10:
					return monday && 8<=day && day<=14;
				case 11:
					return day==3 || day==23;
				case 12:
					return day==23 || day==31; //31日は祝日ではないが、市場は休み
				default:
					return false;
			}
		}
		public static double Yobine(MarketType mt, double v) {
			//呼値を計算
			double r;
			if(mt==MarketType.B)
				return 1;
			/* 旧JASDAQ版
				if(v <= 1000)
					r = 1;
				else if(v < 10000)
					r = 10;
				else if(v < 100000)
					r = 100;
				else if(v < 1000000)
					r = 1000;
				else if(v < 10000000)
					r = 10000;
				else
					r = 50000;
			}
			else {
			*/
			if(v <= 2000)
				r = 1;
			else if(v <= 3000)
				r = 5;
			else if(v <= 30000)
				r = 10;
			else if(v <= 50000)
				r = 50;
			else if(v <= 100000)
				r = 100;
			else if(v <= 1000000)
				r = 1000;
			else if(v <= 20000000)
				r = 10000;
			else if(v <= 30000000)
				r = 50000;
			else
				r = 100000;
			return r;
		}
		//呼値の整数倍になるように丸める
		public static double RoundToYobine(MarketType mt, double v) {
			double y = Yobine(mt, v);
			if(y==1) return v; //ショートカット
			double n = Math.Round(v / y);
			return n * y;
		}

		public static void Warning(IWin32Window parent, string msg) {
			MessageBox.Show(parent, msg, Env.Constants.AppTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
		}
		public static void Warning(string msg) {
			MessageBox.Show(msg, Env.Constants.AppTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
		}
		public static void Information(IWin32Window parent, string msg) {
			MessageBox.Show(parent, msg, Env.Constants.AppTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
		}
		public static DialogResult AskUserYesNo(IWin32Window parent, string msg) {
			return MessageBox.Show(parent, msg, Env.Constants.AppTitle, MessageBoxButtons.YesNo, MessageBoxIcon.Question);
		}

		public static void AddMenuBar(Menu parent) {
			ZMenuItem mi = new ZMenuItem();
			mi.Text = "-";
			mi.Index = parent.MenuItems.Count;
			parent.MenuItems.Add(mi);
		}
		public static void AddMenuItem(Menu parent, string text, EventHandler handler) {
			ZMenuItem mi = new ZMenuItem();
			mi.Text = text;
			mi.Index = parent.MenuItems.Count;
			mi.Click += handler;
			parent.MenuItems.Add(mi);
		}

		public static double ParseDouble(string value, double def) {
			try {
				if(value==null || value.Length==0)
					return def;
				else
					return Double.Parse(value);
			}
			catch(Exception) {
				return def;
			}
		}
		public static double[] ParseDoubles(string value) {
			string[] t = value.Split(',');
			double[] r = new double[t.Length];
			for(int i=0; i<t.Length; i++)
				r[i] = Double.Parse(t[i]);
			return r;
		}
		public static int ParseInt(string value) {
			try {
				return Int32.Parse(value);
			}
			catch(Exception) {
				Debug.WriteLine("Int32 parse error " + value);
				throw;
			}
		}
		public static int ParseInt(string value, int def) {
			try {
				if(value==null || value.Length==0)
					return def;
				else
					return Int32.Parse(value);
			}
			catch(Exception) {
				Debug.WriteLine("Int32 parse error " + value);
				return def;
			}
		}
        //hacked by rti
        public static Int64 ParseInt64(string value, Int64 def)
        {
            try
            {
                if (value == null || value.Length == 0)
                    return def;
                else
                    return Int64.Parse(value);
            }
            catch (Exception)
            {
                Debug.WriteLine("Int64 parse error " + value);
                return def;
            }
        }
        //----------------
		public static int ParseHexInt(string value, int def) {
			try {
				if(value==null || value.Length==0)
					return def;
				else
					return Int32.Parse(value, System.Globalization.NumberStyles.HexNumber); //先頭に0xがついていてはだめ
			}
			catch(Exception) {
				Debug.WriteLine("Int32 parse error " + value);
				return def;
			}
		}
		public static float ParseFloat(string value, float def) {
			try {
				if(value==null || value.Length==0)
					return def;
				else
					return Single.Parse(value);
			}
			catch(Exception) {
				return def;
			}
		}
		public static bool ParseBool(string value) {
			return value=="True";
		}
		public static bool ParseBool(string value, bool def) {
			if(value=="True" || value=="true")
				return true;
			else if(value=="False" || value=="false")
				return false;
			else
				return def;
		}
		public static HeightConfig ParseHeightConfig(string value, HeightConfig def) {
			try {
				if(value==null || value.Length==0) return def; //shortcut
				return (HeightConfig)Enum.Parse(typeof(HeightConfig), value);
			}
			catch(Exception) {
				return def;
			}
		}

		public static int IndexOf(object[] values, object value) {
			for(int i=0; i<values.Length; i++)
				if(values[i]==value) return i;
			return -1;
		}

		public static string LoadMandatoryAttr(StorageNode node, string name) {
			string r = node[name];
			if(r==null || r.Length==0)
				throw new FormatException(String.Format("{0}に必須アトリビュート{1}がないか、空です。", node.Name, name));
			return r;
		}
		public static string LoadMandatoryText(StorageNode node, string name) {
			StorageNode r = node.FindChildNode(name);
			if(r==null)
				throw new FormatException(String.Format("{0}に必須要素{1}がありません。", node.Name, name));
			else if(r.TextValue==null)
				throw new FormatException(String.Format("{0}の必須要素{1}が空です。", node.Name, name));
			return r.TextValue;
		}
		public static string LoadOptionalText(StorageNode node, string name, string def) {
			StorageNode r = node.FindChildNode(name);
			if(r==null)
				return def;
			else if(r.TextValue==null)
				return def;
			else
				return r.TextValue;
		}

		public static int SafeArgLength(Array a) {
			return a==null? 0 : a.Length;
		}

		public static Keys ParseKey(string s) {
			if(s.Length==0)
				return Keys.None;
			/*
			 !!もしここの処理が遅ければ自前で書きなおせる
			else if(s.Length==1) {
				char ch = s[0];
				if('0'<=ch && ch<='9')
					return Keys.D0 + (ch - '0');
			}
			*/

			return (Keys)Enum.Parse(typeof(Keys), s);
		}
		public static string FormatShortcut(Keys key) {
			if(key==Keys.None) return "";

			Keys modifiers = key & Keys.Modifiers;
			StringBuilder b = new StringBuilder();
			if((modifiers & Keys.Control)!=Keys.None) {
				b.Append("Ctrl");
			}
			if((modifiers & Keys.Shift)!=Keys.None) {
				if(b.Length>0) b.Append('+');
				b.Append("Shift");
			}
			if((modifiers & Keys.Alt)!=Keys.None) {
				if(b.Length>0) b.Append('+');
				b.Append("Alt");
			}
			if(b.Length>0)
				b.Append('+');

			b.Append(KeyString(key & Keys.KeyCode));
			return b.ToString();
		}
		public static string KeyString(Keys key) {
			int ik = (int)key;
			if((int)Keys.D0<=ik && ik<=(int)Keys.D9)
				return new string((char)('0' + (ik-(int)Keys.D0)), 1);
			else {
				switch(key) {
					case Keys.None:
						return "";
					case Keys.Prior:
						return "PageUp";
					case Keys.Next:
						return "PageDown";
						//Oemほにゃららがうざったい
					case Keys.OemBackslash:
						return "Backslash";
					case Keys.OemCloseBrackets:
						return "CloseBrackets";
					case Keys.Oemcomma:
						return "Comma";
					case Keys.OemMinus:
						return "Minus";
					case Keys.OemOpenBrackets:
						return "OpenBrackets";
					case Keys.OemPeriod:
						return "Period";
					case Keys.OemPipe:
						return "Pipe";
					case Keys.Oemplus:
						return "Plus";
					case Keys.OemQuestion:
						return "Question";
					case Keys.OemQuotes:
						return "Quotes";
					case Keys.OemSemicolon:
						return "Semicolon";
					case Keys.Oemtilde:
						return "Tilde";
					default:
						return key.ToString();
				}
			}
		}

        public static void Swap(ref int x, ref int y) {
            int t = x;
            x = y;
            y = t;
        }

    }

	internal abstract class Trans {
		protected double _a;
		protected double _b;

		public Trans(double a, double b) {
			_a = a;
			_b = b;
		}
		public double A {
			get {
				return _a;
			}
		}
		public double B {
			get {
				return _b;
			}
		}
		public static Trans Solve(double x1, double y1, double x2, double y2,bool logscale,bool inverseupdown){
			if(logscale){
				if(inverseupdown)
				{
					return LogTrans.SolveUpsideDown(x1,y1,x2,y2);
				}
				else
				{
					return LogTrans.Solve(x1,y1,x2,y2);
				}
			}
			else{
				if(inverseupdown){
					return LinearTrans.SolveUpsideDown(x1,y1,x2,y2);
				}
				else{
					return LinearTrans.Solve(x1,y1,x2,y2);
				}
			}
		}
		public abstract double TransValue(double x);
		public abstract double Inverse(double y);
	}
	
	//y = ax + b の変換をする
	internal class LinearTrans : Trans
	{
		public LinearTrans(double a,double b) : base(a,b) {			
		}
	
		public override double TransValue(double x) {
			return _a * x + _b;
		}
		public override double Inverse(double y) {
			return (y - _b) / _a;
		}

		//0除算や精度はあまり気にせず連立１次方程式を解く
		public static LinearTrans Solve(double x1, double y1, double x2, double y2) {
			double a = (y1 - y2) / (x1 - x2);
			return new LinearTrans(a, y1 - a * x1);
		}
		public static LinearTrans SolveUpsideDown(double x1, double y1, double x2, double y2) 
		{
			double a = - (y1 - y2) / (x1 - x2);
			return new LinearTrans(a, y1 - a * x2);
		}
	}
	
	internal class LogTrans : Trans
	{
		public LogTrans(double a,double b) : base(a,b) 
		{			
		}
		public override double TransValue(double x) 
		{
			// ボリバンなどがはみ出てもトリミング
			if(x < 1)
				x = 1;
			return _a * Math.Log10(x) + _b;
		}
		public override double Inverse(double y) 
		{
			return Math.Pow(10,(y - _b) / _a);
		}
		public static LogTrans Solve(double x1, double y1, double x2, double y2) 
		{
			// 負の部分はトリミング
			if(x2 < 1){
				y2 = LinearTrans.Solve(x1,y1,x2,y2).TransValue(1);
				x2 = 1;
			}
		
			double a = (y1 - y2) / Math.Log10(x1 / x2);
			double b = y1 - a * Math.Log10(x1);
						
			return new LogTrans(a, b);
		}
		public static LogTrans SolveUpsideDown(double x1, double y1, double x2, double y2) 
		{
			// 負の部分はトリミング
			if(x2 < 1)
			{
				y2 = LinearTrans.Solve(x1,y1,x2,y2).TransValue(1);
				x2 = 1;
			}
		
			double a = - (y1 - y2) / Math.Log10(x1 / x2);
			double b = y1 - a * Math.Log10(x2);
						
			return new LogTrans(a, b);
		}	
	}
	

	//時間のかかる動作のときに途中経過を知らせるための定数　SendMessageを使う
	internal class AsyncConst 
	{
		public const int WM_ASYNCPROCESS = Win32.WM_USER+1;
		public const int LPARAM_PROGRESS_SUCCESSFUL = 1;
		public const int LPARAM_PROGRESS_FAILURE = 2;
		public const int LPARAM_FINISHED = 3;
		public const int LPARAM_ERROR = 4;
	}

	//Look & Feelを変更するための仕組み
	internal class ThemeUtil {
		public enum Theme {
			Unspecified,
			Luna
		}

		private static Theme _theme;

		public static Theme CurrentTheme {
			get {
				return _theme;
			}
		}
		
		[DllImport("uxtheme.dll", CharSet=CharSet.Unicode)]
		private static extern int GetCurrentThemeName(char[] filename, int filenamelen, char[] colorbuff, int colornamelen, char[] sizebuff, int sizebufflen);

		private static void SpecifyThemeUnderWinXP() {
			try {
				char[] fn = new char[256];
				char[] cb = new char[256];
				char[] sz = new char[256];
				int r = GetCurrentThemeName(fn, 256, cb, 256, sz, 256);
				if(r==0) {
					string theme_name = new string(fn);
					if(theme_name.IndexOf("Luna")!=-1)
						_theme = Theme.Luna;
				}
				//Debug.WriteLine(String.Format("FN={0} Color={1} Size={2}", new string(fn), new string(cb), new string(sz)));
			}
			catch(Exception) {
			}
		}
		
		public static void Init() {
			_theme = Theme.Unspecified;
			OperatingSystem os = System.Environment.OSVersion;
			if(os.Platform==PlatformID.Win32NT && os.Version.CompareTo(new Version(5,1))>=0)
				SpecifyThemeUnderWinXP();
		}

		public static Color TabPaneBackColor {
			get {
				if(_theme==Theme.Luna)
					return Color.FromKnownColor(KnownColor.ControlLightLight);
				else
					return Color.FromKnownColor(KnownColor.Control);
			}
		}


	}
    //hacked by rti
    //バイナリデータを扱うのに便利なやつ
    //Data.cs からコピペ
    internal class BinaryUtil
    {

		public static int GetInt(byte[] rawdata, int offset) {
			Debug.Assert(rawdata.Length>0);
			unsafe {
				fixed(byte* p = &rawdata[0]) {
					return *(int*)(p+offset);
				}
			}
		}
        public static void SetInt(byte[] rawdata, int offset, int value)
        {
			unsafe {
				fixed(byte* p = &rawdata[0]) {
					*(int*)(p+offset) = value;
				}
			}
		}
    };

    //統計関係
    //いちみにオイラは統計って食べれるんですか?って人です。
    //式はすべてぱくりです。
    internal class Statistics
    {
        //RSQ
        //ピアソンの積率相関係数の2乗
        public static  double RSQ(double[] inX, double[] inY)
        {
            double r = CORREF(inX, inY);
            return r * r;
        }
        //CORREF
        //ピアソンの積率相関係数
        //http://aoki2.si.gunma-u.ac.jp/JavaScript/src/corr2.html
        public static  double CORREF(double[] inX, double[] inY)
        {
            Debug.Assert (inX.Length == inY.Length);

            int i = 0;
            int n = inX.Length;
            double mx = 0;
            double my = 0;
            for (i = 0; i < n; i++) 
            {
                mx += inX[i];
                my += inY[i];
            }
            mx /= n;
            my /= n;

            double vx = 0;
            double vy = 0;
            double vxy = 0;
            for (i = 0; i < n; i++) 
            {
                vx += Math.Pow(inX[i]-mx, 2);
                vy += Math.Pow(inY[i]-my, 2);
                vxy += (inX[i]-mx)*(inY[i]-my);
            }
            vx /= n-1;
            vy /= n-1;
            vxy /= n-1;
            double sdx = Math.Sqrt(vx);
            double sdy = Math.Sqrt(vy);

            if (sdx*sdy == 0)
            {//計算不能
                return double.NaN;
            }
            //ピアソンの積率相関係数
            return vxy/sdx/sdy;
        }
        /*
        public static void test()
        {
            double[] x = { 1 , 2 , 3,  4 , 5};
            double[] y = { 80, 50, 90, 100, 120 };

            int r;
            Debug.Assert( (r = (int)(CORREF(x, y) * 100)) == 79);
            Debug.Assert( (r = (int)(RSQ(x, y) * 100)) == 63);
        }
        */
    }
 
    //-------------------
}
