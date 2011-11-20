/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Windows.Forms;
using System.Text;

using Zanetti.Commands;
using Travis.Storage;

namespace Zanetti.Config
{
	internal enum DirectionKey {
		None,
		PageUpDown,
		UpDown,
		LeftRight,
		Wheel
	}

	//方向キーがらみのキー設定を行う。マウスホイールもキーの一種として扱っていることに注意
	internal class KeyConfig {
		internal class DirectionalEntry {
			private CID _cid; //「戻る」側のコマンド。１を足したら進むコマンドになるように
			private DirectionKey _key;
			private Keys _modifier;
			private string _tag;

			public string Tag {
				get {
					return _tag;
				}
			}
			public CID Command {
				get {
					return _cid;
				}
			}
			public DirectionKey Key {
				get {
					return _key;
				}
				set {
					_key = value;
				}
			}
			public Keys Modifiers {
				get {
					return _modifier;
				}
				set {
					_modifier = value;
				}
			}

			internal DirectionalEntry() {}

			public DirectionalEntry(CID cid, StorageNode node, string name, DirectionKey key) {
				_cid = cid;
				_key = key;
				_tag = name;
				_modifier = Keys.None;
				
				string t = node==null? null : node.GetValue(name);
				if(t!=null) {
					foreach(string e in t.Split('+')) {
						if(e=="Ctrl")
							_modifier |= Keys.Control;
						else if(e=="Shift")
							_modifier |= Keys.Shift;
						else
							_key = ParseDirectionKey(e);
					}
				}
			}

			public string FormatKey() {
				StringBuilder bld = new StringBuilder();
				if((_modifier & Keys.Control)!=Keys.None) bld.Append("Ctrl");
				if((_modifier & Keys.Shift)!=Keys.None) {
					if(bld.Length>0) bld.Append('+');
					bld.Append("Shift");
				}

				if(bld.Length>0) bld.Append('+');
				bld.Append(_key.ToString());
				return bld.ToString();
			}
			
			public DirectionalEntry Clone() {
				DirectionalEntry e = new DirectionalEntry();
				e._cid = _cid;
				e._key = _key;
				e._tag = _tag;
				e._modifier = _modifier;
				return e;
			}
				
		}

		private ArrayList _config;

		public void Load(StorageNode parent) {
			StorageNode node = parent==null? null : parent.FindChildNode("key-config");
			_config = new ArrayList();
			_config.Add(new DirectionalEntry(CID.MoveToPrevPage, node, "page", DirectionKey.PageUpDown));
			_config.Add(new DirectionalEntry(CID.MoveToPrevDate, node, "date", DirectionKey.LeftRight));
			_config.Add(new DirectionalEntry(CID.ShowPrevBrand,  node, "brand", DirectionKey.UpDown));
			_config.Add(new DirectionalEntry(CID.ShowPrevCode,   node, "code",  DirectionKey.Wheel));
		}
		public void Reset() {
			Load(null);
		}

		public void SaveTo(StorageNode parent) {
			StorageNode n = new StorageNode();
			n.Name = "key-config";
			foreach(DirectionalEntry e in _config)
				n[e.Tag] = e.FormatKey();
			parent.AddChild(n);
		}

		public DirectionalEntry Find(CID command) {
			foreach(DirectionalEntry e in _config)
				if(e.Command==command) return e;
			return null;
		}
		public KeyConfig Clone() {
			KeyConfig c = new KeyConfig();
			ArrayList ar = new ArrayList();
			foreach(DirectionalEntry e in _config)
				ar.Add(e.Clone());
			c._config = ar;
			return c;
		}
		public int EntryCount {
			get {
				return _config.Count;
			}
		}
		public DirectionalEntry GetAt(int i) {
			return _config[i] as DirectionalEntry;
		}

		public CID Translate(Keys key) {
			Keys mod = key & Keys.Modifiers;
			foreach(DirectionalEntry e in _config) {
				if(e.Modifiers!=mod) continue;
				if(TestKeyMatch(e.Key, key & Keys.KeyCode)) return e.Command;
				if(TestKeyMatchInv(e.Key, key & Keys.KeyCode)) return (CID)(e.Command+1); //次のコマンドを返す
			}
			return CID.None;
		}
		public CID TranslateWheel(int delta) {
			Keys mod = Control.ModifierKeys;
			foreach(DirectionalEntry e in _config) {
				if(e.Modifiers!=mod) continue;
				if(e.Key==DirectionKey.Wheel) return delta>0? e.Command : (CID)(e.Command+1);
			}
			return CID.None;
		}


		public static DirectionKey ParseDirectionKey(string e) {
			if(e=="PageUpDown") return DirectionKey.PageUpDown;
			if(e=="UpDown")     return DirectionKey.UpDown;
			if(e=="LeftRight")  return DirectionKey.LeftRight;
			if(e=="Wheel")      return DirectionKey.Wheel;
			return DirectionKey.None;
		}
		public static bool TestKeyMatch(DirectionKey dk, Keys input) {
			switch(dk) {
				case DirectionKey.PageUpDown:
					return input==Keys.PageUp;
				case DirectionKey.UpDown:
					return input==Keys.Up;
				case DirectionKey.LeftRight:
					return input==Keys.Left;
			}
			return false;
		}
		public static bool TestKeyMatchInv(DirectionKey dk, Keys input) {
			switch(dk) {
				case DirectionKey.PageUpDown:
					return input==Keys.PageDown;
				case DirectionKey.UpDown:
					return input==Keys.Down;
				case DirectionKey.LeftRight:
					return input==Keys.Right;
			}
			return false;
		}
	}
}
