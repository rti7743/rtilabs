/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;

using Travis.Storage;
using Zanetti.Data;
using Zanetti.UI;

namespace Zanetti.Config
{
	internal abstract class BookmarkItem {
		protected BookmarkFolder _parent; 
		public abstract TreeNode CreateTreeNode(bool folderonly);

		public BookmarkFolder Parent {
			get {
				return _parent;
			}
		}
		public int Depth {
			get {
				if(_parent==null)
					return 0;
				else
					return 1+_parent.Depth;
			}
		}
		public virtual void Move(BookmarkFolder dest,BookmarkItem neighbor)
		{		
			_parent.Remove(this);
			dest.AddChild(this,neighbor);
			_parent = dest;
		}

		public abstract void GatherTo(ArrayList brands, GatherOption opt);
	}

	internal class BrandBookmark : BookmarkItem {
		private int _code;
		private AbstractBrand _brand;

		public BrandBookmark(BookmarkFolder parent, int code) {
			_code = code;
			_parent = parent;
		}
		public int Code {
			get {
				return _code;
			}
		}
		public override void GatherTo(ArrayList brands, GatherOption opt) {
			if(opt==GatherOption.Brand) {
				EnsureBrand();
				brands.Add(_brand);
			}
		}

		public override TreeNode CreateTreeNode(bool folderonly) {
			EnsureBrand();
			TreeNode node = new TreeNode(_brand.CodeAsString + " " + _brand.Name);
			node.Tag = this;
			node.SelectedImageIndex = node.ImageIndex = IconConst.DOCUMENT;
			return node;
		}

		private void EnsureBrand() {
			_brand = Env.BrandCollection.FindBrand(_code);
		}

	}
	internal class BookmarkFolder : BookmarkItem {
		private string _name;
		private ArrayList _children;

		public BookmarkFolder(BookmarkFolder parent) {
			_parent = parent;
			_children = new ArrayList();
		}
		public string Name {
			get {
				return _name;
			}
			set {
				_name = value;
			}
		}
		public ArrayList Children{
			get {
				return _children;
			}
		}
		
		public override void GatherTo(ArrayList brands, GatherOption opt) {
			foreach(BookmarkItem i in _children)
				i.GatherTo(brands, opt);
		}

		public void AddChild(BookmarkItem item,BookmarkItem neighbor) {
			if(item is BrandBookmark) {
				int code = ((BrandBookmark)item).Code;
				if(ContainsCode(code,false)) return; // ignore
			}
			if(neighbor!=null){
				int ii = _children.IndexOf(neighbor);
				_children.Insert(ii,item);	// 見つからない場合は考えない
			}
			else{
				_children.Add(item);
			}
			
			IBookmarkEventListener e = Env.Bookmark.EventListener;
			if(e!=null)	e.OnChildAdded(this, item, neighbor);
		}
		public void Remove(BookmarkItem item) {
			_children.Remove(item);
			IBookmarkEventListener e = Env.Bookmark.EventListener;
			if(e!=null) e.OnChildRemoved(this, item);
		}
		public bool ContainsCode(int code,bool recur) {
			foreach(BookmarkItem item in _children){
				if(item is BrandBookmark && ((BrandBookmark)item).Code==code) return true;
				if(recur && item is BookmarkFolder && ((BookmarkFolder)item).ContainsCode(code,true)) return true;
			}
			return false;
		}
		public bool ContainsItem(BookmarkItem item,bool recur){
			foreach(BookmarkItem i in _children){
				if(i == item) return true;
				if(recur && i is BookmarkFolder && ((BookmarkFolder)i).ContainsItem(item,true)) return true;
			}
			return false;
		}
		public override TreeNode CreateTreeNode(bool folderonly) {
			TreeNode node = new TreeNode(_name);
			node.Tag = this;
			node.SelectedImageIndex = node.ImageIndex = _parent==null? IconConst.STAR : IconConst.FOLDER;
			foreach(BookmarkItem item in _children)
				if(!folderonly || (item is BookmarkFolder))
					node.Nodes.Add(item.CreateTreeNode(folderonly));
			return node;
		}

		public override void Move(BookmarkFolder dest,BookmarkItem neighbor){
			if(this != dest && !this.ContainsItem(dest,true)){
				base.Move(dest,neighbor);
			}
		}

		// セーブデータがかなり冗長。
		public StorageNode Save() {
			StorageNode node = new StorageNode();
			node.Name = "bookmark-group";
			node["name"] = _name;
			foreach(BookmarkItem item in _children) {
				if(item is BookmarkFolder)
					node.AddChild(((BookmarkFolder)item).Save());
				else {
					StorageNode itemnode = new StorageNode();
					itemnode.Name = "bookmark-item";
					itemnode["code"] = ((BrandBookmark)item).Code.ToString();
					node.AddChild(itemnode);
				}
			}
			return node;
		}

		public void Load(StorageNode sec) {
			_name = sec["name"];
			foreach(StorageNode child in sec.Children) {
				if(child.Name=="bookmark-group") {
					BookmarkFolder f = new BookmarkFolder(this);
					f.Load(child);
					_children.Add(f);
				}
				else if(child.Name=="bookmark-item") {
					int code = Int32.Parse(child["code"]);
					//codeの存在を確認すべきだが、起動時には派生銘柄は読み込まれていないのでこの段階では確認不可
					BrandBookmark m = new BrandBookmark(this, code);
					_children.Add(m);
				}
			}
		}
	}

	internal class Bookmark {
		private BookmarkFolder _root;
		private IBookmarkEventListener _eventListener;

		public Bookmark() {
		}

		public BookmarkFolder Root {
			get {
				return _root;
			}
		}
		public IBookmarkEventListener EventListener {
			get {
				return _eventListener;
			}
			set {
				_eventListener = value;
			}
		}
		public ArrayList AllStockBrands {
			get {
				ArrayList r = new ArrayList();
				_root.GatherTo(r, GatherOption.Brand);
				return r;
			}
		}

		public void Clear() {
			_root = new BookmarkFolder(null);
			_root.Name = "お気に入り";
		}
		public void SaveTo(StorageNode parent) {
			parent.AddChild(_root.Save());
		}
		public void Load(StorageNode parent) {
			_root = new BookmarkFolder(null);
			_root.Load(parent);
		}
	}

	//ブックマークからコレクションを集めるときのオプション　後で必要に応じて追加する
	internal enum GatherOption {
		Brand
	}

	internal interface IBookmarkEventListener {
		void OnChildAdded(BookmarkFolder parent, BookmarkItem child, BookmarkItem neighbor);
		void OnChildRemoved(BookmarkFolder parent, BookmarkItem child);
	}
}
