/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

using Zanetti.Data;
using Zanetti.Commands;

namespace Zanetti.Forms
{
	/// <summary>
	/// SearchBrandDialog の概要の説明です。
	/// </summary>
	internal class SearchBrandDialog : System.Windows.Forms.Form
	{
		private static ArrayList _sortedBrands;

		private System.Windows.Forms.TextBox _textBox;
		private System.Windows.Forms.ListBox _listBox;
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;
		
		public SearchBrandDialog()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}
		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			_textBox.Focus();

			if(_sortedBrands==null) FillBrands();
			foreach(AbstractBrand br in _sortedBrands)
				AddBrand(br);
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

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this._textBox = new System.Windows.Forms.TextBox();
			this._listBox = new System.Windows.Forms.ListBox();
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _textBox
			// 
			this._textBox.Location = new System.Drawing.Point(8, 8);
			this._textBox.Name = "_textBox";
			this._textBox.Size = new System.Drawing.Size(208, 19);
			this._textBox.TabIndex = 0;
			this._textBox.Text = "";
			this._textBox.TextChanged += new System.EventHandler(this.OnTextChanged);
			// 
			// _listBox
			// 
			this._listBox.ItemHeight = 12;
			this._listBox.Location = new System.Drawing.Point(8, 32);
			this._listBox.Name = "_listBox";
			this._listBox.Size = new System.Drawing.Size(208, 184);
			this._listBox.TabIndex = 1;
			this._listBox.DoubleClick += new System.EventHandler(this._listBox_DoubleClick);
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(64, 224);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 2;
			this._okButton.Text = "OK";
			this._okButton.Click += new System.EventHandler(this.OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(144, 224);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 3;
			this._cancelButton.Text = "キャンセル";
			// 
			// SearchBrandDialog
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(226, 256);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.Controls.Add(this._listBox);
			this.Controls.Add(this._textBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SearchBrandDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "名前から検索";
			this.ResumeLayout(false);

		}
		#endregion

		private static void FillBrands() {
			_sortedBrands = new ArrayList();
			//新方式ではコード順でまるでOKだった
			IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
			while(ie.MoveNext()) {
				AbstractBrand br = ie.Value as AbstractBrand;
				if(br.Market!=MarketType.B) _sortedBrands.Add(br);
			}
			_sortedBrands.Reverse();
			//_sortedBrands.Sort(new BrandComparer());
		}
		private class BrandComparer : IComparer {
			public int Compare(object x, object y) {
				return ((AbstractBrand)x).Name.CompareTo(((AbstractBrand)y).Name);
			}
		}

		private void OnTextChanged(object sender, EventArgs args) {
			string i = _textBox.Text;
			if(i.Length>0) {
				_listBox.Items.Clear();
				Refill(i);
			}
			/*
			int i = FindNearest(_textBox.Text);
			if(i!=-1)
				_listBox.SelectedIndex = i;
			*/
		}

		private void OnOK(object sender, EventArgs args) {
			ShowBrand();
		}

		private void ShowBrand() {
			if(_listBox.SelectedIndex!=-1) 
			{
				string sel = (string)_listBox.Items[_listBox.SelectedIndex];
				int sp = sel.IndexOf(' ');
				AbstractBrand br = Env.BrandCollection.FindBrand(Int32.Parse(sel.Substring(0, sp)));
				CommandExec.ShowBrand(br);
			}
		}

		private void Refill(string name) {
			CompareInfo ci = CompareInfo.GetCompareInfo("ja-JP");
			foreach(AbstractBrand br in _sortedBrands)
			{
				int c = ci.IndexOf(br.Name,name,CompareOptions.IgnoreCase | CompareOptions.IgnoreKanaType | CompareOptions.IgnoreNonSpace | CompareOptions.IgnoreWidth);
				if(c!=-1) AddBrand(br);
			}
		}

		private void AddBrand(AbstractBrand br) {
			_listBox.Items.Add(String.Format("{0} {1}", br.CodeAsString, br.Name));
		}

		private static int FindNearest(string text) {
			if(text.Length==0) return 0;
			else return FindNearest(text, 0, _sortedBrands.Count);
		}

		private void _listBox_DoubleClick(object sender, System.EventArgs e)
		{
			ShowBrand();
			this.Close();
		}
	
		private static int FindNearest(string text, int begin, int end) {
			if(end-begin<=1) {
				if(end<_sortedBrands.Count) return end;
				else return _sortedBrands.Count-1;
			}

			int m = (begin+end)/2;
			string t = ((AbstractBrand)_sortedBrands[m]).Name;
			// 検索範囲を広げても現実的な銘柄名では二分探索に失敗しないという仮定あり
			int c = text.CompareTo(t);
			if(c<0)
				return FindNearest(text, begin, m);
			else if(c>0)
				return FindNearest(text, m, end);
			else
				return m;
		}

	}
}
