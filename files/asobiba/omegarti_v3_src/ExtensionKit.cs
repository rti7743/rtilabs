/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Globalization;
using System.Collections;
using System.IO;
using System.Text;
using System.Xml;
using System.Security.Cryptography;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

using Travis.PKI;
using Travis.Storage;

using Zanetti.Parser;
using Zanetti.Data;

namespace Zanetti.Indicators.Schema
{
	//拡張キット１つに対応
	internal class ExtensionKit {
		private string _id;
		private string _version;
		private string _owner;
		private string _author;
		private string _title;
		private string _filename;
		private DateTime _expiration;
		private bool _signed;
		private ArrayList _items;
		private ArrayList _oscillatorGroups;
		private ArrayList _libraries;

		public ExtensionKit(StorageNode doc, StorageNode parent, string filename) {
			_items = new ArrayList();
			_oscillatorGroups = new ArrayList();
			_libraries = new ArrayList();
			_filename = filename;
			//!!バージョン1.0チェック
			_version = Util.LoadMandatoryAttr(doc, "version");

			//byte[] hash = body.SHA1Hash;
			//if(hash!=null) VerifySignature(doc, hash);

			StorageNode parameters = null;
			foreach(StorageNode elem in doc.Children) {
				if(elem.Name=="info") {
					_owner  = elem["owner"];
					_author = elem["author"];
					_title  = Util.LoadMandatoryAttr(elem, "title");
					_id = Util.LoadMandatoryAttr(elem, "id");

					string e = elem["expires"];
					if(e==null || e=="never")
						_expiration = new DateTime(2100,1,1);
					else
						_expiration = DateTime.Parse(e, CultureInfo.CurrentUICulture.DateTimeFormat);

					if(parent!=null) parameters = parent.FindChildNode(_id);
				}
				else if(elem.Name=="indicator")
					AddItemWithNameCheck(_items, new SchemaIndicatorItem(this, elem, parameters)); 
				else if(elem.Name=="screening")
					AddItemWithNameCheck(_items, new SchemaScreeningItem(this, elem, parameters));
				else if(elem.Name=="prediction")
					AddItemWithNameCheck(_items, new SchemaPredictionItem(this, elem, parameters));
				else if(elem.Name=="auto-trading")
					AddItemWithNameCheck(_items, new SchemaAutoTradingItem(this, elem, parameters));
                else if (elem.Name == "money-planning")
                    AddItemWithNameCheck(_items, new SchemaMoneyPlanningItem(this, elem, parameters));
                else if (elem.Name == "oscillatorGroup")
					_oscillatorGroups.Add(new SchemaOscillatorGroup(this, elem));
				else if(elem.Name=="library")
					AddItemWithNameCheck(_libraries, new SchemaExtensionLibrary(this, elem));
				else if(elem.Name=="brand")
					Env.BrandCollection.Append(LoadDerivedBrand(elem));
			}
		}
		private void AddItemWithNameCheck(ArrayList col, SchemaItemBase item) {
			foreach(char ch in item.Name) {
				if(!('A'<=ch && ch<='Z') && !('a'<=ch && ch<='z') && !('0'<=ch && ch<='9') && !(ch=='_'))
					throw new FormatException(item.Name + " には無効な文字が含まれています。英数字とアンダースコアのみ使用できます。");
			}
			
			foreach(SchemaItemBase i in _items) {
				if(i.Name==item.Name) throw new FormatException(String.Format("{0}はすでに同名要素があります。", item.Name));
			}
			foreach(SchemaItemBase i in _libraries) {
				if(i.Name==item.Name) throw new FormatException(String.Format("{0}はすでに同名要素があります。", item.Name));
			}
			col.Add(item);
		}

		private DerivedBrand LoadDerivedBrand(StorageNode elem) {
			string name = Util.LoadMandatoryText(elem, "name");
			int code = Int32.Parse(Util.LoadMandatoryAttr(elem, "code"));
			if(code<Util.CustomBrandMinValue || code>=1000)
				throw new FormatException(String.Format("派生銘柄 {0} のコードは{1}から999までである必要があります", name, Util.CustomBrandMinValue));
			string expr = Util.LoadMandatoryText(elem, "expression");
			double pricescale = Util.ParseDouble(Util.LoadOptionalText(elem, "priceScale", null), 1);
			string formatstring = Util.LoadOptionalText(elem, "format", "F0");
			
			ArrayList deps = new ArrayList();
			foreach(StorageNode ch in elem.Children) {
				if(ch.Name=="param") {
					int pc = Int32.Parse(Util.LoadMandatoryAttr(ch, "code"));
					int index = Int32.Parse(Util.LoadMandatoryAttr(ch, "index"));
					AbstractBrand br = Env.BrandCollection.FindBrand(pc);
					deps.Add(br);
				}
			}
			if(deps.Count==0)
				throw new FormatException(String.Format("派生銘柄 {0} に依存先がひとつもありません。", name));

			return new DerivedBrand((AbstractBrand[])deps.ToArray(typeof(AbstractBrand)), expr, name, code, MarketType.Custom, pricescale, formatstring);
		}


		public void SaveTo(StorageNode node) {
			StorageNode ch = new StorageNode();
			ch.Name = _id;
			foreach(SchemaItem item in _items)
				item.SaveTo(ch);
			node.AddChild(ch);
		}
	
		public string Version {
			get {
				return _version;
			}
		}
		public string ID {
			get {
				return _id;
			}
		}

		public string Owner {
			get {
				return _owner;
			}
		}
		public string Author {
			get {
				return _author;
			}
		}
		public string Title {
			get {
				return _title;
			}
		}
		public DateTime Expiration {
			get {
				return _expiration;
			}
		}	  
		public bool Signed {
			get {
				return _signed;
			}
			set {
				_signed = value;
			}
		}
		public string FileName {
			get {
				return _filename;
			}
			set {
				_filename = value;
			}

		}
		public void VerifyExpressions() {
			foreach(SchemaItemBase i in _items)
				i.VerifyExpression();
			foreach(SchemaItemBase i in _libraries)
				i.VerifyExpression();
		}

		public SchemaItem FindSchemaItemByName(string name) {
			foreach(SchemaItem i in _items)
				if(i.Name==name) return i;
			return null;
		}
        //hackedby rti
        //タイトルから検索
        //タイトルはダブル場合があるので検索にはむかないが、
        //AutoTradingItem 等からは title でしか紹介できない不思議構造のため、こうする。
        public SchemaItem FindSchemaItemByTitle(string title)
        {
            foreach (SchemaItem i in _items)
                if (i.Title == title) return i;
            return null;
        }
        //----------

		public void CollectIndicatorSchema(ArrayList col) {
			foreach(SchemaItem i in _items)
				if(i is SchemaIndicatorItem) col.Add(i);
		}
		public void CollectScreeningSchema(ArrayList col) {
			foreach(SchemaItem i in _items)
				if(i is SchemaScreeningItem) col.Add(i);
		}
		public void CollectParameter(ArrayList col) {
			foreach(SchemaItem i in _items)
				i.CollectParameter(col);
		}
		public void CollectAppearance(ArrayList col) {
			foreach(SchemaItem i in _items)
				i.CollectAppearance(col);
		}
		public void CollectOscillatorGroup(ArrayList col) {
			col.AddRange(_oscillatorGroups);
		}
		public void CollectLibraryFunction(ArrayList col) {
			col.AddRange(_libraries);
		}
		public void CollectPredictionSchema(ArrayList col) {
			foreach(SchemaItem i in _items)
				if(i is SchemaPredictionItem) col.Add(i);
		}
		public void CollectAutoTradingSchema(ArrayList col) {
			foreach(SchemaItem i in _items)
				if(i is SchemaAutoTradingItem) col.Add(i);
		}
        //hacked by rti
        public void CollectMoneyPlanningSchema(ArrayList col)
        {
            foreach (SchemaItem i in _items)
                if (i is SchemaMoneyPlanningItem) col.Add(i);
        }
        //---------------

		public static ExtensionKit LoadFrom(string filename, StorageNode parameters) {
			byte[] rawdata = new byte[(int)new FileInfo(filename).Length];
			FileStream strm = new FileStream(filename, FileMode.Open, FileAccess.Read);
			strm.Read(rawdata, 0, rawdata.Length);
			strm.Close();

			StorageNode node;
			if(rawdata[0]==(byte)'<' && rawdata[1]==(byte)'?') { //xml format
				XmlDocument doc = new XmlDocument();
				doc.Load(new MemoryStream(rawdata));
				DOMNodeReader r = new DOMNodeReader(doc);
				node = r.Read();
			}
			else { //bonary format
				VerifySignature(rawdata);
				BinaryNodeReader r = new BinaryNodeReader(new MemoryStream(rawdata));
				node = r.Read();
			}
			return new ExtensionKit(node, parameters, filename);
		}
		private static void VerifySignature(byte[] rawdata) {
			RSAPublicKey pubkey = ZPublicKey.PubKeyForExtensionKit;
			byte[] hash = new SHA1CryptoServiceProvider().ComputeHash(rawdata, 0, rawdata.Length-32);
			byte[] sig  = new byte[32];
			Array.Copy(rawdata, rawdata.Length-32, sig, 0, sig.Length);
			pubkey.Verify(sig, hash); 
		}

	}

	//システムで１つのインスタンスになるスキーマ
	internal class ZanettiSchema {
		private ArrayList _files;

		public ZanettiSchema() {
			_files = new ArrayList();
		}
		public ArrayList ExtensionKits {
			get {
				return _files;
			}
		}

		public void Load(string dir, StorageNode parameters) {
			_files.Clear();
			string[] fs = Directory.GetFiles(dir, "*.omega");
			foreach(string f in fs) {
				try {
					ExtensionKit ek = ExtensionKit.LoadFrom(f, parameters);
					_files.Add(ek);
				}
				catch(Exception ex) {
					Util.SilentReportCriticalError(ex);
					MessageBox.Show(String.Format("拡張キット {0} の読み込みに失敗しました。\n{1}", f, ex.Message), "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
				}
			}
			Reorder();
		}
		public void SaveTo(StorageNode node) {
			StorageNode ch = new StorageNode();
			ch.Name = "params";
			foreach(ExtensionKit kit in _files)
				kit.SaveTo(ch);
			node.AddChild(ch);
		}
		public ExtensionKit FindByID(string id) {
			foreach(ExtensionKit k in _files)
				if(id==k.ID) return k;
			return null;
		}
		public void Reorder() {
			_files.Sort(new Sorter());
		}


		public void CollectIndicatorSchema(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectIndicatorSchema(col);
		}
		public void CollectScreeningSchema(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectScreeningSchema(col);
		}
		public void CollectParameter(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectParameter(col);
		}
		public void CollectAppearance(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectAppearance(col);
		}
		public void CollectOscillatorGroup(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectOscillatorGroup(col);
		}
		public void CollectLibraryFunction(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectLibraryFunction(col);
		}
		public void CollectPredictionSchema(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectPredictionSchema(col);
		}
		public void CollectAutoTradingSchema(ArrayList col) {
			foreach(ExtensionKit k in _files)
				k.CollectAutoTradingSchema(col);
		}
        //hacked by rti
        public void CollectMoneyPlanningSchema(ArrayList col)
        {
            foreach (ExtensionKit k in _files)
                k.CollectMoneyPlanningSchema(col);
        }
        //---------------------

		private class Sorter : IComparer {
	
			public int Compare(object x, object y) {
				//標準キットは先頭にくるようにするため
				string x1 = ((ExtensionKit)x).ID;
				if(x1=="built-in") x1="";
				string y1 = ((ExtensionKit)y).ID;
				if(y1=="built-in") y1="";
				return x1.CompareTo(y1);
			}

	
		}

	}
}
