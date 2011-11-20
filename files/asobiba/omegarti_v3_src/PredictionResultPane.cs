/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

using Zanetti.Commands;
using Zanetti.Indicators;
using Zanetti.Data;
using Zanetti.Prediction;

namespace Zanetti.UI {
	internal class PredictionResultPane : System.Windows.Forms.UserControl {
		private VerificationResult _result;
		private int _requiredWidth;

		private System.Windows.Forms.Label _explanation;
		private ListViewWithCustomKey _listView;
		private System.Windows.Forms.ColumnHeader _colDate;
		private System.Windows.Forms.ColumnHeader _colResult;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PredictionResultPane(VerificationResult result) {
			// この呼び出しは、Windows.Forms フォーム デザイナで必要です。
			InitializeComponent();
			this.SetStyle(ControlStyles.DoubleBuffer|ControlStyles.AllPaintingInWmPaint, true);

			_result = result;
			// TODO: InitializeComponent 呼び出しの後に初期化処理を追加します。
			InitList();
		}

		public int RequiredWidth {
			get {
				return _requiredWidth;
			}
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if(components != null) {
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
		private void InitializeComponent() {
			this._explanation = new System.Windows.Forms.Label();
			this._listView = new ListViewWithCustomKey();
			this._colDate = new System.Windows.Forms.ColumnHeader();
			this._colResult = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// _explanation
			// 
			this._explanation.Dock = System.Windows.Forms.DockStyle.Top;
			this._explanation.Location = new System.Drawing.Point(0, 0);
			this._explanation.Name = "_explanation";
			this._explanation.Size = new System.Drawing.Size(150, 23);
			this._explanation.TabIndex = 0;
			this._explanation.TextAlign = ContentAlignment.MiddleLeft;
			this._explanation.Height = 48;
			// 
			// _listView
			// 
			this._listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						this._colDate,
																						this._colResult});
			this._listView.Dock = System.Windows.Forms.DockStyle.Fill;
			this._listView.Location = new System.Drawing.Point(0, 23);
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
			// _colDate
			// 
			this._colDate.Text = "日付";
			this._colDate.Width = 120;
			// 
			// _colResult
			// 
			this._colResult.Text = "結果";
			this._colResult.Width = 40;
			// 
			// ScreeningResultPane
			// 
			this.Controls.Add(this._listView);
			this.Controls.Add(this._explanation);
			this.Name = "ScreeningResultPane";
			this.ResumeLayout(false);

		}
		#endregion

		private void InitList() {
			_requiredWidth = _colDate.Width + _colResult.Width;
			_explanation.Text = String.Format("銘柄:{0}\n {1}\nセオリー成立割合 {2}/{3}({4}%)", _result.Brand.Name, _result.Item.Header, _result.TrueCount, _result.HitCount, _result.TrueCount*100/_result.HitCount);
			_listView.SuspendLayout();
			_listView.BeginUpdate();
			foreach(VerificationResultEntry e in _result.Entries) {
				AddItem(e);
			}
			_listView.EndUpdate();

			_listView.ResumeLayout();
		}

		private void AddItem(VerificationResultEntry ent) {
			string[] values = new string[2];
			values[0] = Util.FormatShortDate(ent.Date);
			values[1] = ent.Result==PredictionResult.True? "○" : "×";
			ListViewItem li = new ListViewItem(values);
			li.Tag = ent;
			_listView.Items.Add(li);
		}

		private void OnItemActivated(object sender, EventArgs args) {
			ListView.SelectedListViewItemCollection t = _listView.SelectedItems;
			if(t.Count>0) {
				ListViewItem li = t[0];
				VerificationResultEntry ent = li.Tag as VerificationResultEntry;
				if(ent!=null) {
					AbstractBrand br = Env.Frame.ChartCanvas.GetBrand();
					if(br!=_result.Brand) CommandExec.ShowBrand(_result.Brand);
					Env.Frame.ChartCanvas.ForceVisibleDate(ent.Date,false);
				}
			}
		}
	}
}
