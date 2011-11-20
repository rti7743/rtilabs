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

using Zanetti.Config;

namespace Zanetti.Forms
{
	/// <summary>
	/// SelectBookmarkFolder の概要の説明です。
	/// </summary>
	internal class SelectBookmarkFolder : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button _okButton;
		private System.Windows.Forms.Button _cancelButton;
		private System.Windows.Forms.TreeView _treeView;
		private BookmarkFolder _selectedFolder;
		private System.Windows.Forms.Button _createFolderButton;

		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public SelectBookmarkFolder()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}

		public BookmarkFolder ResultFolder {
			get {
				return _selectedFolder;
			}
		}
		public bool ShowCreateFolderButton {
			get {
				return _createFolderButton.Visible;
			}
			set {
				_createFolderButton.Visible = value;
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

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this._okButton = new System.Windows.Forms.Button();
			this._cancelButton = new System.Windows.Forms.Button();
			this._treeView = new System.Windows.Forms.TreeView();
			this._createFolderButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// _okButton
			// 
			this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._okButton.Location = new System.Drawing.Point(136, 152);
			this._okButton.Name = "_okButton";
			this._okButton.TabIndex = 0;
			this._okButton.Text = "OK";
			this._okButton.Click += new System.EventHandler(this.OnOK);
			// 
			// _cancelButton
			// 
			this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._cancelButton.Location = new System.Drawing.Point(224, 152);
			this._cancelButton.Name = "_cancelButton";
			this._cancelButton.TabIndex = 1;
			this._cancelButton.Text = "キャンセル";
			// 
			// _treeView
			// 
			this._treeView.Location = new System.Drawing.Point(0, 0);
			this._treeView.Name = "_treeView";
			this._treeView.ShowRootLines = false;
			this._treeView.Size = new System.Drawing.Size(304, 144);
			this._treeView.TabIndex = 2;
			this._treeView.ImageList = Env.ImageList16;
			// 
			// _createFolderButton
			// 
			this._createFolderButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this._createFolderButton.Location = new System.Drawing.Point(8, 152);
			this._createFolderButton.Name = "_createFolderButton";
			this._createFolderButton.Size = new System.Drawing.Size(88, 23);
			this._createFolderButton.TabIndex = 3;
			this._createFolderButton.Text = "フォルダの作成";
			this._createFolderButton.Click += new System.EventHandler(this.OnCreateFolder);
			// 
			// SelectBookmarkFolder
			// 
			this.AcceptButton = this._okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.CancelButton = this._cancelButton;
			this.ClientSize = new System.Drawing.Size(304, 182);
			this.Controls.Add(this._createFolderButton);
			this.Controls.Add(this._treeView);
			this.Controls.Add(this._cancelButton);
			this.Controls.Add(this._okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SelectBookmarkFolder";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "フォルダの選択";
			this.ResumeLayout(false);

		}
		#endregion

		protected override void OnLoad(EventArgs e) {
			base.OnLoad (e);
			InitTreeView();
		}


		private void InitTreeView() {
			TreeNode r = Env.Bookmark.Root.CreateTreeNode(true);
			_treeView.Nodes.Add(r);
			_treeView.SelectedNode = r;
			r.Expand();
		}
		private void OnOK(object sender, EventArgs args) {
			_selectedFolder = (BookmarkFolder)_treeView.SelectedNode.Tag;
		}

		private void OnCreateFolder(object sender, System.EventArgs e){
			TreeNode n = _treeView.SelectedNode;
			BookmarkFolder bfp = (BookmarkFolder)n.Tag;
			if(bfp != null) {
				BookmarkFolder bf = new BookmarkFolder(bfp);
				bf.Name = "新しいフォルダ";
				bfp.AddChild(bf,null);
				
				TreeNode r = bf.CreateTreeNode(true);
				n.Nodes.Add(r);
				n.Expand();
			}
		}
	}
}
