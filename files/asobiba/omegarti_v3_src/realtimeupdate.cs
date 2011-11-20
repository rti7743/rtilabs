using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using Zanetti.DDE;
using Zanetti.Data;

namespace Zanetti.RealTimeUpdate
{
    //メイン
    class Main
    {
        private RSSThread ThreadProc;
        private Thread Thread;
        public Main(IntPtr inNotifyWindowHandle, int inNotifyWindowMessage)
        {
            this.ThreadProc = new RSSThread();
            this.ThreadProc.DMZ.SetNotify(inNotifyWindowHandle, inNotifyWindowMessage);

            this.Thread = null;
        }
        ~Main()
        {
            Stop();
        }
        //スレッドの開始
        public void Start()
        {
            if (this.Thread != null)
            {
                return; //すでに動作中
            }
            this.Thread = new Thread(new ThreadStart(this.ThreadProc.Run));
            this.Thread.Priority = ThreadPriority.Lowest;   //優先度は低め
            this.Thread.IsBackground = true;
            this.Thread.Start();
        }
        //スレッドの停止
        public void Stop()
        {
            if (this.Thread == null)
            {
                return; //スレッドは動いていません!!
            }
            if (!this.Thread.IsAlive)
            {
                return; //スレッドはすでに死んでいます
            }
            //スレッドを停止させまする.
            //this.Thread.Abort(); //Interrupt で終わらせるのが正解? Abort()推奨しているページが結構あるのはなぜ?
            this.Thread.Interrupt();
            this.Thread.Join();
            this.Thread = null;
        }
        //スレッドは生きてますか?
        public bool IsAlive()
        {
            return this.Thread != null;
        }
        //株価情報の取得
        public void GetKabuka(out int outCode, out int outDate, out NewDailyData outData)
        {
            this.ThreadProc.DMZ.GetKabuka(out outCode, out  outDate, out  outData);
        }
        //株価情報の取得
        public bool SetNextCode(int inCode , MarketType inType)
        {
            string codeString = inCode + ".";
            switch (inType)
            {
                case MarketType.T1:
                case MarketType.T2:
                    codeString += "T";
                    break;
                case MarketType.O1:
                case MarketType.O2:
                    codeString += "OS";
                    break;
                case MarketType.J:
                    codeString += "Q";
                    break;
                case MarketType.M:
                    codeString += "T";
                    break;
                case MarketType.H:
                    codeString += "OJ";
                    break;
                /* マケスピが出来高を取得できないので止めた
                case MarketType.B:
                    switch (inCode)
                    {
                        case 101:   //日経平均
                            codeString = "N225";
                            break;
                        case 102:   //TOPIX
                            codeString = "TOPX";
                            break;
                        case 106:   //JASDAQ
                            codeString = "JSD";
                            break;
                        case 151:   //日経平均先物
                            codeString = "N225.FUT01.OS";
                            break;
                        default:
                            //未対応
                            this.ThreadProc.DMZ.SetNextCode(0, "");
                            return false;
                    }
                    break;
                 */
                default:
                    //未対応
                    this.ThreadProc.DMZ.SetNextCode(0, "");
                    return false;
            }
            this.ThreadProc.DMZ.SetNextCode(inCode, codeString);
            return true;
        }

    };
    internal class DMZ
    {
        private volatile int NextCode;
        private volatile string NextCodeString;
        private volatile int Code;
        private volatile int Date;
        private volatile NewDailyData Data;
        private volatile string ErrorMessage;
        //データに更新があった場合の通知先
        private volatile IntPtr NotifyWindowHandle;
        private volatile int NotifyWindowMessage;

        public void SetNextCode(int inCode,string inCodeString)
        {
            lock (this) ;
            this.NextCode = inCode;
            this.NextCodeString = inCodeString; 
        }

        public string GetNextCodeString()
        {
            lock (this) ;
            return this.NextCodeString;
        }
        public int GetNextCode()
        {
            lock (this) ;
            return this.NextCode;
        }
        public void SetKabuka(int inCode,int inDate ,NewDailyData inData)
        {
            lock (this) ;
            this.Code = inCode;
            this.Date = inDate;
            this.Data = inData;
        }
        public void GetKabuka(out int outCode, out int outDate, out NewDailyData outData)
        {
            lock (this) ;
            outCode = this.Code;
            outDate = this.Date;
            outData = this.Data;
        }
        public void SetError(string inErrorMessage)
        {
            lock (this) ;
            this.ErrorMessage = inErrorMessage;
        }
        public void SetNotify(IntPtr inNotifyWindowHandle,int inNotifyWindowMessage)
        {
            lock (this) ;
            this.NotifyWindowHandle = inNotifyWindowHandle;
            this.NotifyWindowMessage = inNotifyWindowMessage;
        }
        public void Notify()
        {
            IntPtr notifyWindowHandle;
            int notifyWindowMessage;
            {
                lock (this) ;
                notifyWindowHandle = this.NotifyWindowHandle;
                notifyWindowMessage = this.NotifyWindowMessage;
            }
            Win32.PostMessage(notifyWindowHandle, notifyWindowMessage, IntPtr.Zero, IntPtr.Zero);
        }
    };
    //スレッド
    internal class RSSThread
    {
        //処理している証券コード
        private int  Code;
        //現在値キャッシュ
        private int GenzaineDekidaka;

        //データの受け渡し場所
        public DMZ DMZ;

        //DDE親玉
        DDE.DDEMasterInstance DDEMaster;
        DDE.DDEClientWithCache DDEClient;



        public RSSThread()
        {
            this.DMZ = new DMZ();
        }


        //スレッドメイン
        public void Run()
        {
            this.Code = 0;
            this.GenzaineDekidaka = 0;
            this.DDEMaster = new DDE.DDEMasterInstance();
            this.DDEClient = new DDE.DDEClientWithCache();
            this.DDEMaster.Create();

            try
            {
                while (true)
                {
                    try
                    {
                        if (RSS())
                        {//うまく取得できたのでほんの少し休みます
                            Thread.Sleep(1000);
                        }
                        else
                        {//うまく取得できなかったので結構休みます
                            Thread.Sleep(3000);    //3秒ぐらい
                        }
                    }
                    catch (ApplicationException ex)
                    {//うまく取得できなかったので結構休みます
                        this.DMZ.SetError(ex.Message);
                        Thread.Sleep(3000);    //3秒ぐらい
                    }
                }
            }
            catch (ThreadInterruptedException e)
            {
                //おしまい.
            }
        }
        //スレッドメイン
        private bool RSS()
        {
            //現在接続している銘柄の確認
            int nextCode = this.DMZ.GetNextCode();
            if (nextCode == 0)
            {
                return false;
            }
            if (this.Code != nextCode)
            {//ユーザーは別の銘柄をご指定
                this.Code = nextCode;
                this.DDEClient.DisConnect();
                this.DDEClient.Connect(this.DDEMaster, "RSS", this.DMZ.GetNextCodeString());
            }

            //今何時?
            string genzaihizuke = this.DDEClient.Trans("現在日付");
            if (genzaihizuke == "")
            {
                //切断されたらしい、再接続する.
                this.DDEClient.DisConnect();
                this.DDEClient.Connect(this.DDEMaster, "RSS", this.DMZ.GetNextCodeString());

                //リトライ
                genzaihizuke = this.DDEClient.Trans("現在日付");
                if (genzaihizuke == "")
                {//やっぱりだめ
                    throw new  ApplicationException ( "RSSが切断されました" );
                }
            }

            int dekidaka = this.DDEClient.TransINT("出来高");
            //出来高が増えていない限り、更新はないよ
            if (this.GenzaineDekidaka == dekidaka)
            {
                return true;
            }
            this.GenzaineDekidaka = dekidaka;
            //寄り付いていない
            if (dekidaka == 0)
            {
                return true;
            }

            int genzaine = (int)this.DDEClient.TransDouble("現在値");
            int hatune = (int)this.DDEClient.TransDouble("始値");
            int takane = (int)this.DDEClient.TransDouble("高値");
            int yasune = (int)this.DDEClient.TransDouble("安値");

            //異常なデータ?
            if (hatune == 0 || takane == 0 || yasune == 0 || genzaine == 0)
            {
                return false;
            }
            //データの整形
            int date = Util.StringDateToInt(genzaihizuke);

            NewDailyData data = new NewDailyData();
            data.open = hatune;
            data.high = takane;
            data.low = yasune;
            data.close = genzaine;
            data.volume = dekidaka;
            this.DMZ.SetKabuka(this.Code, date , data);
            this.DMZ.Notify();

            return true;
        }
    };
}
