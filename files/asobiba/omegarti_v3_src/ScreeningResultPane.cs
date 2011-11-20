/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using Zanetti.Commands;
using Zanetti.Config;
using Zanetti.Indicators;
using Zanetti.Data;
using Zanetti.SystemTrading.Screening;
using Zanetti.Forms;			// for select BookmarkFolder

using UtilityLibrary.WinControls;

namespace Zanetti.UI
{
	internal class ListViewUtil {
		public delegate bool ItemChecker(ListViewItem li);

		public static void CopyListViewContentToClipboard(ListView lv, ItemChecker checker) {
			StringBuilder bld = new StringBuilder();
			bool first = true;
			foreach(ColumnHeader h in lv.Columns) {
				if(!first) bld.Append("\t");
				bld.Append(h.Text);
				first = false;
			}
			bld.Append("\r\n");
			foreach(ListViewItem item in lv.Items) {
				if(checker(item)) {
					first = true;
					foreach(ListViewItem.ListViewSubItem si in item.SubItems) {
						if(!first) bld.Append("\t");
						bld.Append(si.Text);
						first = false;
					}
					bld.Append("\r\n");
				}
			}
			Clipboard.SetDataObject(bld.ToString(), true);
		}

		public static void SaveListViewContentToFile(ListView lv, ItemChecker checker) {
			SaveFileDialog dlg = new SaveFileDialog();
			dlg.Title = "スクリーニング結果の保存";
			dlg.Filter = "CSV Files(*.csv)|*.csv";
			dlg.DefaultExt = "csv";
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				StreamWriter st = null;
				try {
					st = new StreamWriter(dlg.FileName, false, Encoding.Default);
					bool first = true;
					foreach(ColumnHeader h in lv.Columns) {
						if(!first) st.Write(",");
						st.Write(h.Text);
						first = false;
					}
					st.WriteLine();
					foreach(ListViewItem item in lv.Items) {
						if(checker(item)) {
							first = true;
							foreach(ListViewItem.ListViewSubItem si in item.SubItems) {
								if(!first) st.Write(",");
								st.Write(si.Text);
								first = false;
							}
							st.WriteLine();
						}
					}
				}
				catch(Exception ex) {
					Debug.WriteLine(ex.StackTrace);
					Util.Warning(Env.Frame, ex.Message);
				}
				finally {
					if(st!=null) st.Close();
				}
			}
		}
	}

	internal class ListViewWithCustomKey : SortableListView {

        protected bool _editingLabel;

        public ListViewWithCustomKey() {
            this.BeforeLabelEdit += new LabelEditEventHandler(OnBeforeLabelEdit);
            this.AfterLabelEdit += new LabelEditEventHandler(OnAfterLabelEdit);
        }

        protected override bool ProcessKeyMessage(ref Message m) {
			if(!_editingLabel && Env.Command.Exec(ref m)!=CommandResult.Ignored)
				return true;
			else
				return base.ProcessKeyMessage(ref m);
		}
		/*
		protected override void WndProc(ref Message m) {
			if(m.Msg==Win32.WM_MOUSEWHEEL) {
				if(Env.Command.Exec(ref m)!=CommandResult.Ignored)
					return;
			}

			base.WndProc (ref m);
		}
		*/

        private void OnBeforeLabelEdit(object sender, LabelEditEventArgs args) {
            _editingLabel = true;
        }
        private void OnAfterLabelEdit(object sender, LabelEditEventArgs args) {
            _editingLabel = false;
        }
    }


    /// <summary>
	/// ScreeningResultPane の概要の説明です。
	/// </summary>
	internal class ScreeningResultPane : BrandListPane
	{
		protected class ItemTag {
			public int group; //-1:上位、0:セパレータ、1:下位
			public ScreeningResultEntry entry;

			public ItemTag(int g, ScreeningResultEntry e) {
				group = g;
				entry = e;
			}

			public const int TOP = -1;
			public const int SEPARATOR = 0;
			public const int BOTTOM = 1;
		}

		private class ScreeningListView : ListViewWithCustomKey
		{
			// ListViewItem の比較
			//   sortKey: 比較するためのキー（0:初期状態, 正:列番号昇順, 負:列番号降順）
			//   columnHeader: ソート対象の ColumnHeader
			public override int CompareItem(ListViewItem item1, ListViewItem item2, int sortKey, ColumnHeader columnHeader) {
				ItemTag t1 = item1.Tag as ItemTag;
				ItemTag t2 = item2.Tag as ItemTag;
				int ret;

				if(t1.group!=t2.group) {
					return t1.group - t2.group;
				}

				int index = Math.Abs(sortKey) - 1;
				if(index==0) { //コード
					ret = t1.entry.Brand.Code - t2.entry.Brand.Code;
					if(ret != 0){
						return ret * sortKey;
					}
				}
				else if(index==2) { //結果
					ret = t1.entry.PrimaryResult.DoubleVal.CompareTo(t2.entry.PrimaryResult.DoubleVal);
					if(ret != 0){
						return ret * sortKey;
					}
				}
				else if(index==-1) { //初期状態
					if(t1.group==ItemTag.TOP)
						ret = t2.entry.PrimaryResult.DoubleVal.CompareTo(t1.entry.PrimaryResult.DoubleVal); //TOPグループでは降順
					else
						ret = t1.entry.PrimaryResult.DoubleVal.CompareTo(t2.entry.PrimaryResult.DoubleVal);
					if(ret != 0){
						return ret;
					}
				}

				// SortableListView に任せる
				return base.CompareItem(item1, item2, sortKey, columnHeader);
			}
		}

		private ContextMenu _contextMenu;
		private ScreeningOrder _result;
		private int _requiredWidth;

		private System.Windows.Forms.Label _explanation;
		private Button _exportButton;
		private ScreeningListView _listView;
		private System.Windows.Forms.ColumnHeader _colCode;
		private System.Windows.Forms.ColumnHeader _colName;
        //hacked by rti
        private System.Windows.Forms.ColumnHeader _colKounyuDekiruKabuSuu;
        //------

		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ScreeningResultPane(ScreeningOrder result)
		{
			// この呼び出しは、Windows.Forms フォーム デザイナで必要です。
			InitializeComponent();
			this.SetStyle(ControlStyles.DoubleBuffer|ControlStyles.AllPaintingInWmPaint, true);

			_result = result;
			// TODO: InitializeComponent 呼び出しの後に初期化処理を追加します。
			InitList(_result.PreferredResultCountType);
		}

		public int RequiredWidth {
			get {
				return _requiredWidth;
			}
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region コンポーネント デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this._explanation = new System.Windows.Forms.Label();
			this._exportButton = new Button();
			this._listView = new ScreeningListView();
			this._colCode = new System.Windows.Forms.ColumnHeader();
			this._colName = new System.Windows.Forms.ColumnHeader();
            //hacked by rti
            //予算で購入できる枚数を表示する.
            this._colKounyuDekiruKabuSuu = new System.Windows.Forms.ColumnHeader();
            //---
			this.SuspendLayout();
			// 
			// _explanation
			// 
			this._explanation.Location = new System.Drawing.Point(0, 0);
			this._explanation.Name = "_explanation";
			this._explanation.Size = new System.Drawing.Size(150, 23);
			this._explanation.TabIndex = 0;
			this._explanation.TextAlign = ContentAlignment.MiddleLeft;
			// 
			// _exportButton
			// 
			this._exportButton.Location = new System.Drawing.Point(24, 23);
			this._exportButton.Name = "_exportButton";
			this._exportButton.Size = new System.Drawing.Size(96, 23);
			this._exportButton.TabIndex = 1;
			this._exportButton.FlatStyle = FlatStyle.System;
			this._exportButton.TextAlign = ContentAlignment.MiddleLeft;
			this._exportButton.Click += new EventHandler(OnExportButtonClicked);
			this._exportButton.Text = "エクスポート(&E)...";
			// 
			// _listView
			// 
            //hacked by rti
            this._listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																							   this._colCode,
																							   this._colName,
                                                                                               this._colKounyuDekiruKabuSuu});
            //---
            this._listView.Location = new System.Drawing.Point(0, 48);
			this._listView.Name = "_listView";
			this._listView.TabIndex = 1;
			this._listView.FullRowSelect = true;
			this._listView.GridLines = true;
			this._listView.Name = "_listView";
			this._listView.TabIndex = 1;
			this._listView.MultiSelect = false;
			this._listView.Activation = ItemActivation.OneClick;
			this._listView.View = System.Windows.Forms.View.Details;
			this._listView.SelectedIndexChanged += new EventHandler(OnItemActivated);
			// 
			// _colCode
			// 
			this._colCode.Text = "コード";
			this._colCode.Width = 40;
			// 
			// _colName
			// 
			this._colName.Text = "名前";
			this._colName.Width = 80;
            //hacked by rti
            // 
            // _colKounyuDekiruKabuSuu
            // 
            this._colKounyuDekiruKabuSuu.Text = "購入できる株数";
            this._colKounyuDekiruKabuSuu.Width = 80;
            //---
            // 
			// ScreeningResultPane
			// 
			this.Controls.Add(this._listView);
			this.Controls.Add(this._exportButton);
			this.Controls.Add(this._explanation);
			this.Name = "ScreeningResultPane";
			this.ResumeLayout(false);

		}
		#endregion

		private void InitList(ResultCountType type) {
			_listView.SuspendLayout();
			_requiredWidth = _colCode.Width + _colName.Width;
			_explanation.Text = String.Format("検索対象日 {0}\n対象銘柄数 {1}", Util.FormatShortDate(_result.OriginDate), _result.Result.ResultCount);
			AddColumn(_result.PrimaryItem.Header);
			foreach(ScreeningItem item in _result.SecondaryItems)
				AddColumn(item.Header);
			this.Width = _listView.Width+8;
			this.Height = Env.Frame.ChartCanvas.Height - 40;

            //hacked by rti 
            //本日買い付けに回せる金額かせ、1銘柄にいくらまでかけれるか計算します。
            Int64 useLimit = Env.Preference.TodayBuyCach / _result.Result.ResultCount;
            //----
            
			_listView.BeginUpdate();
			ScreeningResult sr = _result.Result as ScreeningResult;
			switch(type) {
				case ResultCountType.Top100:
					for(int i=sr.ResultCount-1; i>=Math.Max(sr.ResultCount-101, 0); i--)
						AddItem(sr.GetAtS(i),useLimit, ItemTag.TOP);
					break;
                case ResultCountType.Top1000:
                    for(int i = sr.ResultCount - 1; i >= Math.Max(sr.ResultCount - 1001, 0); i--)
                        AddItem(sr.GetAtS(i), useLimit, ItemTag.TOP);
                    break;
                case ResultCountType.Bottom100:
                    for(int i=0; i<Math.Min(sr.ResultCount, 100); i++)
                        AddItem(sr.GetAtS(i), useLimit, ItemTag.BOTTOM);
					break;
                case ResultCountType.Bottom1000:
                    for(int i = 0; i < Math.Min(sr.ResultCount, 1000); i++)
                        AddItem(sr.GetAtS(i), useLimit, ItemTag.BOTTOM);
                    break;
                case ResultCountType.Both50:
                    for(int i=sr.ResultCount-1; i>=Math.Max(sr.ResultCount-51, 0); i--)
                        AddItem(sr.GetAtS(i), useLimit, ItemTag.TOP);
					AddSeparator();
					for(int i=0; i<Math.Min(sr.ResultCount, 50); i++)
                        AddItem(sr.GetAtS(i), useLimit, ItemTag.BOTTOM);
					break;
			}
			_listView.EndUpdate();

			_listView.ResumeLayout();
		}
		private void AddColumn(string name) {
			ColumnHeader ch = new ColumnHeader();
			ch.Text = name;
			ch.Width = 60;
			ch.TextAlign = HorizontalAlignment.Right; // 数値なので右寄せ
			_requiredWidth += ch.Width;
			_listView.Columns.Add(ch);
		}
		private void AddItem(ScreeningResultEntry ent,Int64 useLimit, int group) {
			string[] values = new string[_listView.Columns.Count];
			AbstractBrand br = ent.Brand;
			values[0] = br.CodeAsString;
			values[1] = br.Name;
            values[2] = GetHonzituKaeruKabuSuu(ent, useLimit).ToString();
			values[3] = Util.FormatValue(ent.PrimaryResult.DoubleVal, _result.PrimaryItem.FormatString, _result.PrimaryItem.FormatModifier);
			for(int i=0; i<_result.SecondaryItemCount; i++)
				values[4+i] = Util.FormatValue(ent.SecondaryResults[i].DoubleVal, _result.GetSecondaryItem(i).FormatString, _result.GetSecondaryItem(i).FormatModifier);
			ListViewItem li = new ListViewItem(values);
			li.Tag = new ItemTag(group, ent);
			_listView.Items.Add(li);
		}

        //本日買い付けできる株数を求める
        private Int64 GetHonzituKaeruKabuSuu(ScreeningResultEntry ent,Int64 useLimit)
        {
            //単元数を取ります..
            Int64 unit = (ent.Brand is BasicBrand) ? ((BasicBrand)ent.Brand).Unit : 1;
            if (unit <= 0)
            {
                unit = 1;    //指数など普通は売買しないものを売買しようとするとこうなるらしい。
            }

            //購入出来る枚数
            Int64 mai = (Int64)(useLimit / ent.EndPrice / unit);
            if (mai <= 0)
            {//残高不足で仕掛けられませんでした
                return 0;
            }
            //購入できる枚数 * 単元
            return mai * unit;
        }

		private void AddSeparator() {
			string[] values = new string[_result.SecondaryItemCount+4]; //code,name,primaryの分で３を足す
			values[0] = "-";
			values[1] = "---";
			values[2] = "-";
            values[3] = "-";
            for (int i = 0; i < _result.SecondaryItemCount; i++)
				values[4+i] = "-";
			ListViewItem li = new ListViewItem(values);
			li.Tag = new ItemTag(ItemTag.SEPARATOR, null);
			_listView.Items.Add(li);
		}

		protected override void OnResize(EventArgs e) {
			base.OnResize (e);
			int w = this.ClientSize.Width;
			_explanation.Width = w;
			_exportButton.Left = w - _exportButton.Width;
			_listView.Width = w;
			_listView.Height = this.ClientSize.Height - _exportButton.Bottom;
		}

		private void InitContextMenu() {
			_contextMenu = new ContextMenu();
			Util.AddMenuItem(_contextMenu, "お気に入りへ追加(&B)...", new EventHandler(OnAddToBookmark));
			Util.AddMenuItem(_contextMenu, "クリップボードへコピー(&C)", new EventHandler(OnCopyResult));
			Util.AddMenuItem(_contextMenu, "CSV形式で保存(&S)...", new EventHandler(OnSaveResult));
		}
		private void OnExportButtonClicked(object sender, EventArgs args) {
			if(_contextMenu==null) InitContextMenu();
			_contextMenu.Show(this, new Point(_exportButton.Left, _exportButton.Bottom));
		}

		private void OnItemActivated(object sender, EventArgs args) {
			ListView.SelectedListViewItemCollection t = _listView.SelectedItems;
			if(t.Count==1) { //複数選択のときはこの機能は使わない
				ListViewItem li = t[0];
				ItemTag tag = li.Tag as ItemTag;
				if(tag!=null) {
					ScreeningResultEntry e = tag.entry;
					if(e!=null)
						CommandExec.ShowBrand(e.Brand);
				}
			}
		}
		private void OnAddToBookmark(object sender, EventArgs args) {
			SelectBookmarkFolder dlg = new SelectBookmarkFolder();
			if(dlg.ShowDialog(Env.Frame)==DialogResult.OK) {
				BookmarkFolder f = dlg.ResultFolder;
				foreach(ListViewItem item in _listView.Items) {
					ItemTag tag = item.Tag as ItemTag;
					if(tag!=null && tag.entry!=null) {
						AbstractBrand br = tag.entry.Brand;
						f.AddChild(new BrandBookmark(f, br.Code), null);
					}
				}
			}
		}
		private void OnCopyResult(object sender, EventArgs args) {
			ListViewUtil.CopyListViewContentToClipboard(_listView, new ListViewUtil.ItemChecker(ListViewItemChecker));
		}
		private void OnSaveResult(object sender, EventArgs args) {
			ListViewUtil.SaveListViewContentToFile(_listView, new ListViewUtil.ItemChecker(ListViewItemChecker));
		}
		private bool ListViewItemChecker(ListViewItem li) {
			ItemTag t = li.Tag as ItemTag;
			return t!=null && t.entry!=null;
		}

		//!!以下２つはプロパティだが選択状態の変更を伴う。ちょっと汚い
		public override AbstractBrand NextBrand {
			get {
				if(_listView.SelectedItems.Count==0) return null;

				ListViewItem li = _listView.SelectedItems[0];
				if(li.Index < _listView.Items.Count-1) {
					_listView.Items[li.Index].Selected = false;
					ListViewItem next = _listView.Items[li.Index+1];
					next.Selected = true;
					ItemTag tag = next.Tag as ItemTag;
					if(tag.entry!=null) return tag.entry.Brand;
				}
				return null;
			}
		}
		public override AbstractBrand PrevBrand {
			get {
				if(_listView.SelectedItems.Count==0) return null;

				ListViewItem li = _listView.SelectedItems[0];
				if(li.Index > 0) {
					_listView.Items[li.Index].Selected = false;
					ListViewItem next = _listView.Items[li.Index-1];
					next.Selected = true;
					ItemTag tag = next.Tag as ItemTag;
					if(tag.entry!=null) return tag.entry.Brand;
				}
				return null;
			}
		}


	}
}
