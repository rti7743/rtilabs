using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using UtilityLibrary.Win32;

namespace Zanetti.UI
{
	/// <summary>
	/// ソート機能付き ListView。
	/// </summary>
	internal class SortableListView : ListView
	{
		private int _listSortKey;	// リストのソートキー (0:初期状態, 正:列番号昇順, 負:列番号降順)
		private ListSorter _listSorter;

		public SortableListView()
		{
			_listSortKey = 0;
			_listSorter = new ListSorter(this);
			ColumnClick += new ColumnClickEventHandler(OnColumnClicked);
		}

		/// <summary>
		/// ListViewItem の比較。
		/// </summary>
		/// <param name="item1">比較対象1</param>
		/// <param name="item2">比較対象2</param>
		/// <param name="sortKey">比較するためのキー（0:初期状態, 正:列番号昇順, 負:列番号降順）</param>
		/// <param name="columnHeader">ソート対象の ColumnHeader</param>
		/// <returns>比較結果</returns>
		public virtual int CompareItem(ListViewItem item1, ListViewItem item2, int sortKey, ColumnHeader columnHeader) {
			if(sortKey != 0){
				// 項目の昇順 or 降順ソート
				int index = Math.Abs(sortKey) - 1;
				int illegal1 = 0;
				int illegal2 = 0;
				string str1 = item1.SubItems[index].Text;
				string str2 = item2.SubItems[index].Text;
				int result = 0;
				if(columnHeader.TextAlign == HorizontalAlignment.Right){
					// 右寄せの場合は数値ソート
					double d1 = 0.0;
					double d2 = 0.0;
					int len1 = GetNumberLen(str1);
					int len2 = GetNumberLen(str2);
					if(len1 > 0) d1 = double.Parse(str1.Substring(0, len1));
					else illegal1 = 1;
					if(len2 > 0) d2 = double.Parse(str2.Substring(0, len2));
					else illegal2 = 1;
					if(d1 < d2) result = -1;
					else if(d1 > d2) result = 1;

					// 共に非数の場合の処理
					if((illegal1 & illegal2) == 1){
						illegal1 = illegal2 = 0;
						if(str1.Length == 0) illegal1 = 1;
						if(str2.Length == 0) illegal2 = 1;
						result = str1.CompareTo(str2);
					}
				}
				else{
					// 右寄せ以外の場合は文字列ソート
					if(str1.Length == 0) illegal1 = 1;
					if(str2.Length == 0) illegal2 = 1;
					result = str1.CompareTo(str2);
				}

				// 順序が決定する場合はそれを返す
				if(illegal1 != illegal2){
					// 空白や非数は後の方に追い出したい
					return illegal1 - illegal2;
				}
				if(result != 0){
					return sortKey * result;
				}
			}
			
			// デフォルトのソート
			if((item1.Tag is IComparable) && (item2.Tag is IComparable)){
				IComparable c1 = (IComparable) item1.Tag;
				IComparable c2 = (IComparable) item2.Tag;
				return c1.CompareTo(c2);
			}

			return 0;
		}

		private void OnColumnClicked(object sender, ColumnClickEventArgs e) {
			int sortKey = e.Column + 1;
			ListHeader header = getHeader();
			HDITEM hdi = new HDITEM();
			int index;

			hdi.mask = (uint)HeaderItemFlags.HDI_FORMAT;

			// ヘッダーの以前のソート表示を元に戻す
			if(_listSortKey != 0){
				index = Math.Abs(_listSortKey) - 1;
				header.GetItem(index, ref hdi);
				hdi.fmt &= ~(int)(HeaderControlFormats.HDF_SORTDOWN | HeaderControlFormats.HDF_SORTUP);
				header.SetItem(index, ref hdi);
			}

			if(_listSortKey == sortKey){
				// 現在昇順なら降順にする
				_listSortKey *= -1;
			}
			else if(_listSortKey == -sortKey){
				// 現在降順なら初期状態に戻す
				_listSortKey = 0;
			}
			else{
				// その他の場合は、項目で昇順ソート
				_listSortKey = sortKey;
			}

			// ヘッダーにソート表示（▲▼）を行う
			if(_listSortKey != 0){
				index = Math.Abs(_listSortKey) - 1;
				header.GetItem(index, ref hdi);
				hdi.fmt |= (int)(_listSortKey > 0 ?
									HeaderControlFormats.HDF_SORTUP :
									HeaderControlFormats.HDF_SORTDOWN);
				header.SetItem(index, ref hdi);
			}

			// ソート
			_listSorter.SetKey(_listSortKey);
			ListViewItemSorter = _listSorter;
			Sort();
		}

		// 数値として有効な文字数を返す
		private static int GetNumberLen(string str){
			int n = str.Length;
			if(n <= 0){
				return 0;
			}
			char c = str[0];
			if((c < '0' || '9' < c) && c != '-' && c != '+'){
				return 0;
			}
			for(int i = 1; i < n; i++){
				c = str[i];
				if((c < '0' || '9' < c) && c != '.'){
					return i;
				}
			}
			
			return n;
		}

		private ListHeader getHeader() {
			// TODO: 可能な場合は ListHeader をキャッシュすると少し速くなる。
			return new ListHeader(Handle);
		}

		private class ListSorter : IComparer {
			int _sortKey; // ソートキー (0:初期状態, 正:列番号昇順, 負:列番号降順)
			ColumnHeader _columnHeader; // ソート対象の ColumnHeader
			SortableListView _list; // ソート対象の ListView

			public ListSorter(SortableListView list) {
				_list = list;
			}

			public void SetKey(int sortKey) {
				_sortKey = sortKey;

				if(_sortKey != 0){
					int index = Math.Abs(_sortKey) - 1;
					_columnHeader = (ColumnHeader)_list.Columns[index];
				}
				else{
					_columnHeader = null;
				}
			}

			public int Compare(object x, object y) {
				return _list.CompareItem((ListViewItem)x, (ListViewItem)y,
											_sortKey, _columnHeader);
			}
		}

		private class ListHeader {
			private IntPtr _hwnd;

			public ListHeader(IntPtr hwndList){
				_hwnd = WindowsAPI.SendMessage(hwndList, ListViewMessages.LVM_GETHEADER, 0, 0);
				Debug.Assert(_hwnd != IntPtr.Zero);
			}

			public int GetItemCount(){
				if(_hwnd == IntPtr.Zero){
					return 0;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_GETITEMCOUNT, 0, 0);
			}

			public bool GetItem(int i, ref HDITEM phdi){
				if(_hwnd == IntPtr.Zero){
					return false;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_GETITEMW, i, ref phdi);
			}

			public bool SetItem(int i, ref HDITEM phdi){
				if(_hwnd == IntPtr.Zero){
					return false;
				}
				return WindowsAPI.SendMessage(_hwnd, HeaderControlMessages.HDM_SETITEMW, i, ref phdi);
			}
		}
	}
}

