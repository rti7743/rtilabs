/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Collections;

using Zanetti.Data;

namespace Zanetti.DataSource {

	//データの入手元の基底クラス
	internal abstract class DataSourceBase {

		public const int DATE_MASK = Int32.MinValue; //==0x80000000

		//エラーが起きたときのメッセージ取得
		public abstract string ErrorMessage { get; }

		//実行
		public abstract void Process();

		//外部への通知
		public abstract void SendMessage(int msg, int wparam, int lparam);

		//データの処理にかかるステップ数
		public abstract int TotalStep { get; }

		//中止
		public abstract void Abort();
	}

	//複数の入手元を連続的にアクセスする
	internal class CombinedDataSource : DataSourceBase {
		private  IntPtr _notifyTarget;
		private SingleDataSource[] _elements;
		private int _index;
		protected Thread _downloader1;

		public CombinedDataSource(SingleDataSource[] e) {
			_elements = e;
		}

		public void AsyncProcess(IntPtr hwnd) {
			Debug.Assert(hwnd!=IntPtr.Zero);
			_notifyTarget = hwnd;
			_downloader1 = new Thread(new ThreadStart(AsyncRoot));
			_downloader1.Start();
		}
		public int Count {
			get {
				return _elements.Length;
			}
		}


		[STAThread]
		private void AsyncRoot() {
			Process();
		}

		public override void Process() {
			try {
				_index = 0;
				while(_index < _elements.Length) {
					SingleDataSource ds = _elements[_index];
					ds.Parent = this;
					ds.Process();
					_index++;
				}
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_ERROR);
			}
		}


		public override string ErrorMessage {
			get {
				return _elements[_index].ErrorMessage;
			}
		}

		public override void SendMessage(int msg, int wparam, int lparam) {
			//最終要素でなければ完了にはならない
			if(lparam==AsyncConst.LPARAM_FINISHED && _index<_elements.Length-1)
				lparam = AsyncConst.LPARAM_PROGRESS_SUCCESSFUL;
			Win32.SendMessage(_notifyTarget, msg, new IntPtr(wparam), new IntPtr(lparam));
		}
		
		public override void Abort() {
			if(_downloader1==null) throw new Exception("illegal operation: _downloader==null");
			if(_index<_elements.Length)
				_elements[_index].Abort();
			_downloader1.Abort();
		}

		public override int TotalStep {
			get {
				int r = 0;
				foreach(DataSourceBase ds in _elements)
					r += ds.TotalStep;
				return r;
			}
		}

	}

	internal abstract class SingleDataSource : DataSourceBase {
		protected string _errorMessage;
		private DataSourceBase _parent;

		public override string ErrorMessage {
			get {
				return _errorMessage;
			}
		}
		public DataSourceBase Parent {
			get {
				return _parent;
			}
			set {
				_parent = value;
			}
		}

		public override void SendMessage(int msg, int wparam, int lparam) {
			Debug.Assert(_parent!=null); //現状はSingleDataSourceは単独では行動できない。
			_parent.SendMessage(msg, wparam, lparam);
		}


	}

	//各銘柄について、全日付データを取得するタイプのDataSource
	internal abstract class FullDataSource : SingleDataSource {
		//ダウンロードとインポートを別個にできるようにメソッドを分ける
		public abstract byte[] DownloadFullData(BasicBrand br);
		public abstract void ImportFullData(BasicBrand br, byte[] data);
		
		protected CodeEnumerator _codeEnumerator;
		protected AbstractBrand _targetBrand;
		protected AsyncImporter _importer;

		protected FullDataSource(CodeEnumerator ce) {
			_codeEnumerator = ce;
		}
		
		public override void Process() {
			_importer = new AsyncImporter(this);
			try {
				_targetBrand = _codeEnumerator.Next;
				while(_targetBrand!=null) {
					if(_targetBrand is BasicBrand) {
						try {
							byte[] data = DownloadFullData((BasicBrand)_targetBrand);
							ImportFullData((BasicBrand)_targetBrand, data); //single thread
							_importer.ImportBrand((BasicBrand)_targetBrand, data);
							SendMessage(AsyncConst.WM_ASYNCPROCESS, _targetBrand.Code, AsyncConst.LPARAM_PROGRESS_SUCCESSFUL);
						}
						catch(Exception ex) {
							Debug.WriteLine("Download failed code " + _targetBrand.Code);
							Util.SilentReportCriticalError(ex);
							_errorMessage = ex.Message;
							SendMessage(AsyncConst.WM_ASYNCPROCESS, _targetBrand.Code, AsyncConst.LPARAM_PROGRESS_FAILURE);
						}
					}
					_targetBrand = _codeEnumerator.Next;
				}
				_importer.WaitFinish();
				SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_FINISHED);
			}
			catch(ThreadAbortException) { //Abortのときは何もしない
				Debug.WriteLine("Download thread aborted");
			}
			catch(Exception ex) {
				Util.SilentReportCriticalError(ex);
				_errorMessage = ex.Message;
				SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_ERROR);
			}

			_importer.Finish();
		}

		public override int TotalStep {
			get {
				return _codeEnumerator.Count;
			}
		}
		public override void Abort() {
			if(_importer!=null)
				_importer.Abort();
		}


		public class AsyncImporter {
			private FullDataSource _parent;
			private BasicBrand _brand;
			private byte[] _data;
			private AutoResetEvent _importStartEvent;
			private AutoResetEvent _importFinishEvent;
			private bool _finished;
			private Thread _slave;

			public AsyncImporter(FullDataSource ds) {
				_parent = ds;
				_importStartEvent = new AutoResetEvent(false);
				_importFinishEvent = new AutoResetEvent(true);
				_slave = new Thread(new ThreadStart(Run));
				_slave.Start();
			}
			public void Finish() {
				_finished = true;
				_importStartEvent.Set();
			}
			public void WaitFinish() {
				_importFinishEvent.WaitOne();
			}

			//現状FinishとAbortに区別はない
			public void Abort() {
				_finished = true;
				_importStartEvent.Set();
			}
			public void ImportBrand(BasicBrand br, byte[] data) {
				_importFinishEvent.WaitOne();
				_brand = br;
				_data = data;
				_importStartEvent.Set(); //slave threadを起動
			}

			public void Run() {
				while(true) {
					try {
						_importStartEvent.WaitOne();
						if(_finished) break;
						_parent.ImportFullData(_brand, _data);
						_importFinishEvent.Set();
					}
					catch(ThreadAbortException) { //Abortのときは何もしない
						Debug.WriteLine("Download thread aborted");
					}
					catch(Exception ex) {
						Util.SilentReportCriticalError(ex);
						_parent._errorMessage = ex.Message;
						_parent.SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_ERROR);
					}
				}

				CleanUp();
			}

			private void CleanUp() {
				_importStartEvent.Close();
				_importFinishEvent.Close();
			}
		}

	}


	//ダウンロードしたい銘柄をリストアップするクラス
	internal abstract class CodeEnumerator {
		public abstract AbstractBrand Next { get; }
		public abstract int Count { get; }

		internal class AllCodeEnumerator : CodeEnumerator {
			private IDictionaryEnumerator _e;
			public AllCodeEnumerator() {
				_e = Env.BrandCollection.GetEnumerator();
			}
			public override AbstractBrand Next {
				get {
					return _e.MoveNext()? (AbstractBrand)_e.Value : null;
				}
			}
			public override int Count {
				get {
					return Env.BrandCollection.StockBrandCount;
				}
			}
		}
		//個別銘柄のみのEnumerator
		internal class AllStockEnumerator : CodeEnumerator {
			private IDictionaryEnumerator _e;
			public AllStockEnumerator() {
				_e = Env.BrandCollection.GetEnumerator();
			}
			public override AbstractBrand Next {
				get {
					AbstractBrand br = null;
					do {
						br = _e.MoveNext()? (AbstractBrand)_e.Value : null;
					} while(br!=null && br.IsBuiltIn);
					return br;
				}
			}
			public override int Count {
				get {
					return Env.BrandCollection.StockBrandCount;
				}
			}
		}
		internal class BookmarkCodeEnumerator : CodeEnumerator {
			private ArrayList _codes;
			private int _index;
			public BookmarkCodeEnumerator() {
				_codes = Env.Bookmark.AllStockBrands;
				_index = 0;
			}
			public override AbstractBrand Next {
				get {
					return _index==_codes.Count? null : (AbstractBrand)_codes[_index++];
				}
			}
			public override int Count {
				get {
					return _codes.Count;
				}
			}
		}
		internal class Single : CodeEnumerator {
			private AbstractBrand _target;
			public Single(AbstractBrand br) {
				_target =br;
			}
			public override AbstractBrand Next {
				get {
					//これで１回だけ返すことができる
					AbstractBrand b = _target;
					_target = null;
					return b;
				}
			}
			public override int Count {
				get {
					return 1;
				}
			}
		}

		//ビルトインの指数を列挙する基底
		internal abstract class BuiltInEnumerator : CodeEnumerator {
			private int[] _codes;
			private int _index;
			public BuiltInEnumerator(int[] codes) {
				_codes = codes;
				_index = 0;
			}
			public override AbstractBrand Next {
				get {
					return _index==_codes.Length? null : Env.BrandCollection.FindBrand(_codes[_index++]);
				}
			}
			public override int Count {
				get {
					return _codes.Length;
				}
			}
		}
		//指数
		internal class DomesticIndicesEnumerator : BuiltInEnumerator {
			protected static int[] _codes;
			public DomesticIndicesEnumerator() : base(Init()) {
			}
			private static int[] Init() {
				if(_codes!=null) return _codes;
				ArrayList ar = new ArrayList();
				ar.Add((int)BuiltInIndex.Nikkei225);
				ar.Add((int)BuiltInIndex.TOPIX);
				ar.Add((int)BuiltInIndex.JASDAQ);
				//ar.Add((int)BuiltInIndex.LONGTERM_INTEREST);
				ar.Add((int)BuiltInIndex.Nikkei225_F);
				ar.Add((int)BuiltInIndex.TOPIX_F);
				_codes = (int[])ar.ToArray(typeof(int));
				return _codes;
			}
		}
		internal class ForeignIndicesEnumerator : BuiltInEnumerator {
			protected static int[] _codes;
			public ForeignIndicesEnumerator() : base(Init()) {
			}
			private static int[] Init() {
				if(_codes!=null) return _codes;
				ArrayList ar = new ArrayList();
				ar.Add((int)BuiltInIndex.Dow);
				ar.Add((int)BuiltInIndex.Nasdaq);
				ar.Add((int)BuiltInIndex.SP500);
				_codes = (int[])ar.ToArray(typeof(int));
				return _codes;
			}
		}
		//為替
		internal class CurrenciesEnumerator : BuiltInEnumerator {
			protected static int[] _codes;
			public CurrenciesEnumerator() : base(Init()) {
			}
			private static int[] Init() {
				if(_codes!=null) return _codes;
				ArrayList ar = new ArrayList();
				ar.Add((int)BuiltInIndex.JPYUSD);
				ar.Add((int)BuiltInIndex.JPYEUR);
				_codes = (int[])ar.ToArray(typeof(int));
				return _codes;
			}
		}
		//業種別
		internal class SectorsEnumerator : BuiltInEnumerator {
			protected static int[] _codes;
			public SectorsEnumerator() : base(Init()) {
			}
			private static int[] Init() {
				if(_codes!=null) return _codes;
                _codes = new int[(int)BuiltInIndexGroup.SectorIndexEnd - (int)BuiltInIndexGroup.SectorIndexStart + 1];
                for(int i = 0; i < _codes.Length; i++)
                    _codes[i] = (int)BuiltInIndexGroup.SectorIndexStart + i;
                return _codes;
			}
		}
#if DOJIMA
        //商品
        internal class CommodityEnumerator : BuiltInEnumerator {
            protected static int[] _codes;
            public CommodityEnumerator() : base(Init()) {
            }
            private static int[] Init() {
                if(_codes != null) return _codes;
                _codes = new int[CommodityUtil.INDEX_END - CommodityUtil.INDEX_START + 1];
                for(int i = 0; i < _codes.Length; i++)
                    _codes[i] = CommodityUtil.INDEX_START + i;
                return _codes;
            }
        }
#endif

    }

	//個別銘柄の日次データをダウンロードしてまとめて追加する
	internal abstract class DailyDataSource : SingleDataSource {
		protected int[] _dates;

		public DailyDataSource(int[] dates) {
			_dates = dates;
		}

		public override void Process() {
			try {
				Run();
				SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_FINISHED);
			}
			catch(ThreadAbortException) { //Abortのときは何もしない
				Debug.WriteLine("Download thread aborted");
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.Message);
				Debug.WriteLine(ex.StackTrace);
				_errorMessage = ex.Message;
				Util.SilentReportCriticalError(ex);
				SendMessage(AsyncConst.WM_ASYNCPROCESS, 0, AsyncConst.LPARAM_ERROR);
			}
		}

		public override int TotalStep {
			get {
				return Env.BrandCollection.StockBrandCount;
			}
		}

		public abstract void Run();

		//DailyDataSourceではマルチスレッドにはならないので何もしない
		public override void Abort() {

		}

	}
}
