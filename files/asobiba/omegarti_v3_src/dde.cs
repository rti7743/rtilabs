//DDE
//C++用に作ったやつを強引にC#にしました。
//NDDE と msdn と C++のヘッダーを参考に作成しました。
//ちなみに NDDE からのコピペは一切ありません。
//
//予想以上に面倒だった、これなら素直に NDDEを使えばよかったかも知れぬ。
//
using System;
using System.Collections;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Zanetti.DDE
{
    //DDE関係のインポート
    public class DDESource
    {
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeInitialize(
                  ref int pidInst,          // インスタンス識別子へのポインタ
                  IntPtr pfnCallback,  // コールバック関数へのポインタ 使わないので null
                  uint afCmd,              // コマンドフラグとフィルタフラグ
                  int ulRes               // 予約済み
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeUninitialize(
                  int idInst   // インスタンスID
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static uint DdeGetLastError(
                int idInst   // インスタンス識別子
           );

        //DdeInitializeの引数
        public const uint APPCMD_CLIENTONLY = (0x00000010);
        public const uint APPCMD_FILTERINITS = (0x00000020);
        public const uint APPCMD_MASK = (0x00000FF0);

        public const uint CBF_SKIP_CONNECT_CONFIRMS = (0x00040000);
        public const uint CBF_SKIP_REGISTRATIONS = (0x00080000);
        public const uint CBF_SKIP_UNREGISTRATIONS = (0x00100000);
        public const uint CBF_SKIP_DISCONNECTS = (0x00200000);
        public const uint CBF_SKIP_ALLNOTIFICATIONS = (0x003c0000);


        //戻り値(成功とエラー)
        public const int DMLERR_NO_ERROR = 0;
        public const uint DMLERR_FIRST = 0x4000;

        public const uint DMLERR_ADVACKTIMEOUT = 0x4000;
        public const uint DMLERR_BUSY = 0x4001;
        public const uint DMLERR_DATAACKTIMEOUT = 0x4002;
        public const uint DMLERR_DLL_NOT_INITIALIZED = 0x4003;
        public const uint DMLERR_DLL_USAGE = 0x4004;
        public const uint DMLERR_EXECACKTIMEOUT = 0x4005;
        public const uint DMLERR_INVALIDPARAMETER = 0x4006;
        public const uint DMLERR_LOW_MEMORY = 0x4007;
        public const uint DMLERR_MEMORY_ERROR = 0x4008;
        public const uint DMLERR_NOTPROCESSED = 0x4009;
        public const uint DMLERR_NO_CONV_ESTABLISHED = 0x400a;
        public const uint DMLERR_POKEACKTIMEOUT = 0x400b;
        public const uint DMLERR_POSTMSG_FAILED = 0x400c;
        public const uint DMLERR_REENTRANCY = 0x400d;
        public const uint DMLERR_SERVER_DIED = 0x400e;
        public const uint DMLERR_SYS_ERROR = 0x400f;
        public const uint DMLERR_UNADVACKTIMEOUT = 0x4010;
        public const uint DMLERR_UNFOUND_QUEUE_ID = 0x4011;

        public const uint DMLERR_LAST = 0x4011;

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeCreateStringHandle(
                int idInst,  // インスタンス識別子
                string psz,    // NULL で終わる文字列へのポインタ
                uint iCodePage  // コードページ識別子
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeFreeStringHandle(
                int idInst,  // インスタンス識別子
                IntPtr hsz        // 文字列のハンドル
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeConnect(
                int idInst,      // インスタンス識別子
                IntPtr hszService,    // サービス名の文字列のハンドル
                IntPtr hszTopic,      // トピック名の文字列のハンドル
                IntPtr pCC   // コンテキストデータが入った構造体へのポインタ(今回は利用しないのでIntPtr nullを入れるように)
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeDisconnect(
                IntPtr hConv   // 対話のハンドル
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeClientTransaction(
                byte[] pData,       // サーバーに渡すデータへのポインタ
                int cbData,       // データの長さ
                IntPtr hConv,        // 対話のハンドル
                IntPtr hszItem,        // 項目名の文字列のハンドル
                uint wFmt,          // クリップボードデータ形式
                uint wType,         // トランザクション型
                int dwTimeout,    // タイムアウト時間
                ref int pdwResult   // トランザクションの結果へのポインタ
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeGetData(
                IntPtr hData,  // DDE オブジェクトのハンドル
                StringBuilder pDst,     // コピー先バッファへのポインタ
                int cbMax,     // コピーするデータの量
                int cbOff      // データの先頭へのオフセット
           );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeFreeDataHandle(
                IntPtr hData   // DDE オブジェクトのハンドル
           );

        public const uint CP_WINANSI = 1004;    /* default codepage for windows & old DDE convs. */
        public const uint CP_WINUNICODE = 1200;

        public const uint CF_TEXT = 1;

        public const uint XTYPF_NOBLOCK = 0x0002;  /* CBR_BLOCK will not work */
        public const uint XTYPF_NODATA = 0x0004;  /* DDE_FDEFERUPD */
        public const uint XTYPF_ACKREQ = 0x0008;  /* DDE_FACKREQ */

        public const uint XCLASS_MASK = 0xFC00;
        public const uint XCLASS_BOOL = 0x1000;
        public const uint XCLASS_DATA = 0x2000;
        public const uint XCLASS_FLAGS = 0x4000;
        public const uint XCLASS_NOTIFICATION = 0x8000;

        public const uint XTYP_ERROR = (0x0000 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_ADVDATA = (0x0010 | XCLASS_FLAGS);
        public const uint XTYP_ADVREQ = (0x0020 | XCLASS_DATA | XTYPF_NOBLOCK);
        public const uint XTYP_ADVSTART = (0x0030 | XCLASS_BOOL);
        public const uint XTYP_ADVSTOP = (0x0040 | XCLASS_NOTIFICATION);
        public const uint XTYP_EXECUTE = (0x0050 | XCLASS_FLAGS);
        public const uint XTYP_CONNECT = (0x0060 | XCLASS_BOOL | XTYPF_NOBLOCK);
        public const uint XTYP_CONNECT_CONFIRM = (0x0070 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_XACT_COMPLETE = (0x0080 | XCLASS_NOTIFICATION);
        public const uint XTYP_POKE = (0x0090 | XCLASS_FLAGS);
        public const uint XTYP_REGISTER = (0x00A0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_REQUEST = (0x00B0 | XCLASS_DATA);
        public const uint XTYP_DISCONNECT = (0x00C0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_UNREGISTER = (0x00D0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_WILDCONNECT = (0x00E0 | XCLASS_DATA | XTYPF_NOBLOCK);

        public const uint XTYP_MASK = 0x00F0;
        public const uint XTYP_SHIFT = 4;  /* shift to turn XTYP_ into an index */

    }

    //DDEの初期化系  一度だけやればいい
    public class DDEMasterInstance
    {
        public DDEMasterInstance()
	    {
            this.DDEInstance = 0;
	    }
	    ~DDEMasterInstance()
	    {
            Destroy();
	    }
        //作成
	    public void Create()
	    {
            Debug.Assert(this.DDEInstance == 0);

            if (DDESource.DdeInitialize(ref this.DDEInstance, IntPtr.Zero, DDESource.APPCMD_CLIENTONLY | DDESource.CBF_SKIP_ALLNOTIFICATIONS, 0) != DDESource.DMLERR_NO_ERROR) 
		    {
			    throw new ApplicationException ( GetDDEErrorMessage( ) );
		    }
	    }
        //破壊
        public void Destroy() 
	    {
            if (this.DDEInstance != 0)
		    {
                DDESource.DdeUninitialize(this.DDEInstance);
                this.DDEInstance = 0;
		    }
	    }
        //DDEインスタンスの取得
	    public int getInstance()
	    {
		    return this.DDEInstance;
	    }
        //DDEエラーの文字列化
	    public  string GetDDEErrorMessage() 
        {
            uint uiErrNo = DDESource.DdeGetLastError(this.DDEInstance);
	        switch(uiErrNo)
	        {
            case DDESource.DMLERR_NO_ERROR:
		        return "DMLERR_NO_ERROR 処理に成功しました";
            case DDESource.DMLERR_ADVACKTIMEOUT:
		        return "DMLERR_ADVACKTIMEOUT 同期アドバイズトランザクションの要求がタイムアウトになりました。 ";
            case DDESource.DMLERR_BUSY: 
		        return "DMLERR_BUSY トランザクションへの応答により DDE_FBUSY フラグがセットされました。 ";
            case DDESource.DMLERR_DATAACKTIMEOUT: 
		        return "DMLERR_DATAACKTIMEOUT 同期データトランザクションの要求がタイムアウトになりました。 ";
            case DDESource.DMLERR_DLL_NOT_INITIALIZED: 
		        return "DMLERR_DLL_NOT_INITIALIZED DdeInitialize 関数を呼び出さずに DDEML 関数を呼び出したか、DDEML 関数に渡したインスタンス識別子が無効です。 ";
            case DDESource.DMLERR_DLL_USAGE: 
		        return "DMLERR_DLL_USAGE APPCLASS_MONITORとして初期化されたアプリケーションが DDE（Dynamic Data Exchange）トランザクションを試みたか、APPCMD_CLIENTONLY として初期化されたアプリケーションがサーバートランザクションの実行を試みました。 ";
            case DDESource.DMLERR_EXECACKTIMEOUT:
		        return "DMLERR_EXECACKTIMEOUT 同期実行トランザクションの要求がタイムアウトになりました。 ";
            case DDESource.DMLERR_INVALIDPARAMETER:
		        return "DMLERR_INVALIDPARAMETER DDEMLがパラメータを有効にできませんでした。次の理由が考えられます。 \r\n" + 
				        "トランザクションが要求するものとは異なる項目名のハンドルで初期化したデータハンドルを使ったため。\r\n" + 
				        "トランザクションが要求するものとは異なるクリップボードデータ形式で初期化したデータハンドルを使ったため。\r\n" + 
				        "サーバー側関数でクライアント側の対話ハンドルを使ったか、クライアント側でサーバー側の対話ハンドルを使ったため。\r\n" + 
				        "解放されたデータハンドルまたは文字列ハンドルを使ったため。\r\n" +
				        "アプリケーションの複数のインスタンスが同じオブジェクトを使ったため。";
            case DDESource.DMLERR_LOW_MEMORY: 
		        return "DMLERR_LOW_MEMORY DDEMLアプリケーションによってサーバー先行状態（ サーバーアプリケーションがクライアントの処理能力以上にサービスを提供している状態）になり、大量のメモリが消費されました。 ";
            case DDESource.DMLERR_MEMORY_ERROR:
		        return "DMLERR_MEMORY_ERROR メモリ割り当てに失敗しました。 ";
            case DDESource.DMLERR_NO_CONV_ESTABLISHED:
		        return "DMLERR_NO_CONV_ESTABLISHED クライアントが対話を確立しようとして失敗しました。 ";
            case DDESource.DMLERR_NOTPROCESSED:
		        return "DMLERR_NOTPROCESSED トランザクションに失敗しました。 ";
            case DDESource.DMLERR_POKEACKTIMEOUT:
		        return "DMLERR_POKEACKTIMEOUT 同期ポークトランザクションの要求がタイムアウトになりました。 ";
            case DDESource.DMLERR_POSTMSG_FAILED:
		        return "DMLERR_POSTMSG_FAILED PostMessage 関数の内部呼び出しに失敗しました。 ";
            case DDESource.DMLERR_REENTRANCY:
		        return "DMLERR_REENTRANCY 同期トランザクションがすでに進行中のアプリケーションインスタンスで別の同期トランザクションを開始しようとしたか、DdeEnableCallback 関数が DDEML コールバック関数内で呼び出されました。 ";
            case DDESource.DMLERR_SERVER_DIED:
		        return "DMLERR_SERVER_DIED クライアント側が終了した対話でサーバー側トランザクションが試みられたか、トランザクションが完了する前にサーバーが終了しました。 ";
            case DDESource.DMLERR_SYS_ERROR:
		        return "DMLERR_SYS_ERROR DDEMLで内部エラーが発生しました。 ";
            case DDESource.DMLERR_UNADVACKTIMEOUT:
		        return "DMLERR_UNADVACKTIMEOUT アドバイズトランザクションの終了要求がタイムアウトになりました。 ";
            case DDESource.DMLERR_UNFOUND_QUEUE_ID:
		        return "DMLERR_UNFOUND_QUEUE_ID DDEML 関数に渡されたトランザクション識別子が無効です。アプリケーションが XTYP_XACT_COMPLETE コールバックから戻ると、そのコールバック関数のトランザクション識別子は無効になります。";
	        }

            return String.Format("未知のエラー{0}が発生しました" , uiErrNo);
        }

        private int DDEInstance;
    };

    //DDE通信に利用する文字列
    public class DDEString
    {
        public DDEString()
        {
            this.PoolDDEInstance = null;
            this.ItemDDEStringHandle = IntPtr.Zero;
        }
        ~DDEString()
        {
            Destroy();
        }
        //文字列の作成
        public IntPtr Create(DDEMasterInstance poolDDEInstance, string inItem)
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(this.ItemDDEStringHandle == IntPtr.Zero);
            Debug.Assert(poolDDEInstance != null);
            Debug.Assert(inItem.Length != 0);

            this.PoolDDEInstance = poolDDEInstance;

            //通信する文字列の作成
            //極限まで早くしたいなら、キャッシュすれば早くなるかも。
            this.ItemDDEStringHandle = DDESource.DdeCreateStringHandle(
                this.PoolDDEInstance.getInstance(), inItem, DDESource.CP_WINANSI);
            if (this.ItemDDEStringHandle == IntPtr.Zero)
            {
                throw new ApplicationException(this.PoolDDEInstance.GetDDEErrorMessage());
            }
            return this.ItemDDEStringHandle;
        }
        //開放
        public  void Destroy()
        {
            if (this.ItemDDEStringHandle != IntPtr.Zero)
            {
                Debug.Assert(this.PoolDDEInstance != null);

                DDESource.DdeFreeStringHandle(this.PoolDDEInstance.getInstance(), this.ItemDDEStringHandle);
                this.ItemDDEStringHandle = IntPtr.Zero;
            }
            this.PoolDDEInstance = null;
        }
        //ハンドルの取得
        //外で勝手に開放したりするとダメぽ
        public IntPtr Value
        {
            get
            {
                Debug.Assert(this.ItemDDEStringHandle != IntPtr.Zero);
                return this.ItemDDEStringHandle;
            }
        }

        private DDEMasterInstance PoolDDEInstance;
        private IntPtr ItemDDEStringHandle;
    };

    //DDE クライアント
    public class DDEClient  
    {
        public DDEClient()
	    {
            this.PoolDDEInstance = null;
            this.DDEConnection = IntPtr.Zero;
            this.ServiceNameStringHandle = new DDEString();
            this.TopicNameStringHandle = new DDEString();
            this.TimeOut = 6000;
	    }
	    ~DDEClient()
	    {
		    DisConnect();
	    }

        //接続
        public void Connect(DDEMasterInstance poolDDEInstance,
		    string inServiceName ,string inTopicName )
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(this.DDEConnection == IntPtr.Zero);
            Debug.Assert(poolDDEInstance != null);

            this.PoolDDEInstance = poolDDEInstance;

            ServiceNameStringHandle.Create(this.PoolDDEInstance, inServiceName);
            TopicNameStringHandle.Create(this.PoolDDEInstance, inTopicName);

            this.DDEConnection = DDESource.DdeConnect(
                this.PoolDDEInstance.getInstance(), ServiceNameStringHandle.Value, TopicNameStringHandle.Value, IntPtr.Zero);
            if (this.DDEConnection == IntPtr.Zero)
            {
                throw new ApplicationException(this.PoolDDEInstance.GetDDEErrorMessage());
            }
        }

        //切断
	    public  void DisConnect()
        {
            if (this.DDEConnection != IntPtr.Zero)
            {
                DDESource.DdeDisconnect(this.DDEConnection);
                this.DDEConnection = IntPtr.Zero;
            }
            ServiceNameStringHandle.Destroy();
            TopicNameStringHandle.Destroy();
            this.PoolDDEInstance = null;
        }

        //情報の取得
        public string Trans(DDEString inItem)
        {
            Debug.Assert(this.PoolDDEInstance != null);
            Debug.Assert(this.DDEConnection != IntPtr.Zero);
            Debug.Assert(inItem != null);

            int dummyResult = 0;
            IntPtr ddeData =
                DDESource.DdeClientTransaction(null, 0, this.DDEConnection, inItem.Value, DDESource.CF_TEXT, DDESource.XTYP_REQUEST, this.TimeOutValue, ref dummyResult);
            if (ddeData == IntPtr.Zero)
            {
                //ありうる! ので、例外にはしない
                return "";
            }

            int length = DDESource.DdeGetData(ddeData, null, 0, 0);

            StringBuilder buffer = new StringBuilder(length);
            DDESource.DdeGetData(ddeData, buffer, length, 0);

            DDESource.DdeFreeDataHandle(ddeData);

            return buffer.ToString();
        }

        //情報の取得
	    public  string Trans(string  inItem )
        {
            Debug.Assert( this.PoolDDEInstance != null );
            Debug.Assert( inItem.Length != 0 );

            //通信する文字列の作成
            DDEString ddeString = new DDEString();
            ddeString.Create(this.PoolDDEInstance , inItem);

            return this.Trans(ddeString);
        }
	    public  int TransINT(string  inItem )
	    {
            string str = Trans(inItem);

            int ret;
            if (! int.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
	    }
        public double TransDouble(string inItem)
        {
            string str = Trans(inItem);

            double ret;
            if (!Double.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public int TimeOut
        {
            get
            {
                return this.TimeOutValue;
            }
            set
            {
                this.TimeOutValue = value;
            }
        }

	    private DDEMasterInstance	PoolDDEInstance;
        private IntPtr DDEConnection;
        private DDEString ServiceNameStringHandle;
        private DDEString TopicNameStringHandle;
        private int TimeOutValue;
    };

    //DDEクライアント + DDE文字列キャッシュ
    //DDEで文字列をキャッシュするヤツ.
    //継承したっていいんだけど、、委譲が好きなんで。
    public class DDEClientWithCache
    {
        public DDEClientWithCache()
        {
            this.PoolDDEInstance = null;
            this.Client = new DDEClient();
            this.DDEStringCache = new Hashtable();
        }
        ~DDEClientWithCache()
        {
            DisConnect();
        }

        //接続
        public void Connect(DDEMasterInstance poolDDEInstance,
            string inServiceName, string inTopicName)
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(poolDDEInstance != null);

            this.PoolDDEInstance = poolDDEInstance;
            this.Client.Connect(this.PoolDDEInstance, inServiceName, inTopicName);
        }

        //切断
        public void DisConnect()
        {
            this.Client.DisConnect();
            this.PoolDDEInstance = null;
        }

        //キャッシュをクリア 使い道あるかな...
        public void CacheClear()
        {
            this.DDEStringCache = new Hashtable();
        }

        //情報の取得
        public string Trans(string inItem)
        {
            Debug.Assert(this.PoolDDEInstance != null);

            DDEString ddeString = (DDEString)this.DDEStringCache[inItem];
            if (ddeString == null)
            {
                //キャッシュにないので作成する.
                ddeString = new DDEString();
                ddeString.Create(this.PoolDDEInstance , inItem);
                this.DDEStringCache[inItem] = ddeString;
            }

            return this.Client.Trans(ddeString);
        }
        public int TransINT(string inItem)
        {
            string str = Trans(inItem);

            int ret;
            if (!int.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public double TransDouble(string inItem)
        {
            string str = Trans(inItem);

            double ret;
            if (!Double.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public int TimeOut
        {
            get
            {
                return this.Client.TimeOut;
            }
            set
            {
                this.Client.TimeOut = value;
            }
        }
        private DDEMasterInstance PoolDDEInstance;
        private DDEClient Client;
        private Hashtable DDEStringCache;
    };
    /*
    public class DDETest
    {
        public static void test()
        {
            DDEMasterInstance master = new DDEMasterInstance();
            master.Create();

            DDEClientWithCache client = new DDEClientWithCache();
            client.Connect( master , "RSS" , "5401.T");
            int a = client.TransINT("出来高");

            int b = client.TransINT("出来高");

        }
    }
    */
}
