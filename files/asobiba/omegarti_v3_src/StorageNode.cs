/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Collections;

namespace Travis.Storage
{
	public class StorageNode {
		private string _name;
		private string _textValue; //XMLサポートのためにやむなく登場
		private Hashtable _data;
		private ArrayList _childNodes;

		public string Name {
			get {
				return _name;
			}
			set {
				_name = value;
			}
		}
		public string TextValue {
			get {
				return _textValue;
			}
			set {
				_textValue = value;
			}
		}
		public IDictionaryEnumerator GetPairEnumerator() {
			return _data.GetEnumerator();
		}
		public string this[string name] {
			get {
				return (string)_data[name];
			}
			set {
				_data[name] = value;
			}
		}
		public string GetValue(string name, string defval) {
			object t = _data[name];
			return t==null? defval : (string)t;
		}
		public string GetValue(string name) {
			return (string)_data[name];
		}
		public bool HasChild {
			get {
				return _childNodes.Count>0;
			}
		}
		public int ChildCount {
			get {
				return _childNodes.Count;
			}
		}
		public int ValueCount {
			get {
				return _data.Count;
			}
		}
		public IEnumerable Children {
			get {
				return _childNodes;
			}
		}
		public StorageNode GetChildAt(int index) {
			if(index>=_childNodes.Count) return null;
			return (StorageNode)_childNodes[index];
		}

		public StorageNode FindChildNode(string name) {
			foreach(StorageNode s in _childNodes) {
				if(s.Name==name) return s;
			}
			return null;
		}
		public void AddChild(StorageNode node) {
			_childNodes.Add(node);
		}

		public StorageNode() {
			_data = new Hashtable();
			_childNodes = new ArrayList();

		}
        //hacked by rti
        //<inNodeName>inTextValue</inNodeName> って構造を作る.
        public StorageNode(string inNodeName , string inTextValue)
        {
            _data = new Hashtable();
            _childNodes = new ArrayList();

            StorageNode p = new StorageNode();
            p._textValue = inTextValue;
            p.Name = inNodeName;
            this.AddChild(p);
        }
        //------------------------
    }

	public abstract class NodeReader {
		public abstract StorageNode Read(); 
		public abstract void Close();
	}
	public abstract class NodeWriter {
		public abstract void Write(StorageNode node); 
		public abstract void Close();
	}

	public class TextNodeReader : NodeReader {
		private TextReader _reader;
		public TextNodeReader(TextReader reader) {
			_reader = reader;
		}
		public override void Close() {
			_reader.Close();
		}
		public override StorageNode Read() {
			StorageNode node = new StorageNode();

			string line = ReadLine();
			while(line!=null) {
				int e = line.IndexOf('=');
				if(e!=-1) {
					string name0 = Normalize(line.Substring(0, e));
					string value = e==line.Length-1? "" : Normalize(line.Substring(e+1));
					node[name0] = value;
				}
				else if(line.EndsWith("{")) {
					string[] v = line.Split(' ');
					foreach(string t in v) {
						if(t!="{") {
							StorageNode ch = Read();
							ch.Name = t;
							node.AddChild(ch);
							break;
						}
					}
				}
				else if(line.StartsWith("}"))
					break;
				line = ReadLine();
			}

			return node;
		}
		private string ReadLine() {
			string line = _reader.ReadLine();
			return Normalize(line);
		}
		private static string Normalize(string s) {
			int i=0;
			if(s==null) return null;
			do {
				if(i==s.Length) return "";
				char ch = s[i++];
				if(ch!=' ' && ch!='\t') return s.Substring(i-1);
			} while(true);
		}

	}

	public class TextNodeWriter : NodeWriter {

		private TextWriter _writer;
		private int _indent;

		public TextNodeWriter(TextWriter writer) {
			_writer = writer;
			_indent = 0;
		}
		public override void Close() {
			_writer.Close();
		}
		public override void Write(StorageNode node) {
			WriteStartSection(_writer, _indent++, node.Name);
			IDictionaryEnumerator ie = node.GetPairEnumerator();
			while(ie.MoveNext())
				WriteValue(_writer, _indent, (string)ie.Key, (string)ie.Value);
			foreach(StorageNode ch in node.Children)
				Write(ch);
			WriteEndSection(_writer, --_indent);
		}



		//書き出しのUtil
		public static void WriteStartSection(TextWriter wr, int indent, string name) {
			for(int i=0; i<indent*2; i++) wr.Write(' ');
			wr.Write(name);
			wr.WriteLine(" {");
		}
		public static void WriteEndSection(TextWriter wr, int indent) {
			for(int i=0; i<indent*2; i++) wr.Write(' ');
			wr.WriteLine("}");
		}
		public static void WriteValue(TextWriter wr, int indent, string name, string value) {
			for(int i=0; i<indent*2; i++) wr.Write(' ');
			wr.Write(name);
			wr.Write('=');
			wr.WriteLine(value);
		}
	}

	public class BinaryNodeReader : NodeReader {
		private Stream _strm;
		public BinaryNodeReader(Stream strm) {
			_strm = strm;
		}
		public override StorageNode Read() {
			StorageNode node = new StorageNode();
			node.Name = ReadString(_strm);
			node.TextValue = ReadString(_strm);
			int c = ReadShort(_strm);
			for(int i=0; i<c; i++) {
				string n = ReadString(_strm);
				node[n] = ReadString(_strm);
			}
			c = ReadShort(_strm);
			for(int i=0; i<c; i++)
				node.AddChild(Read());
			return node;
		}
		public override void Close() {
			_strm.Close();
		}

		private static short ReadShort(Stream strm) {
			byte b1 = (byte)strm.ReadByte();
			byte b2 = (byte)strm.ReadByte();
			return (short)(b2 + b1*0x100);
		}
		private static string ReadString(Stream strm) {
			byte[] t = new byte[(int)ReadShort(strm)];
			strm.Read(t, 0, t.Length);
			return Encoding.UTF8.GetString(t);
		}
	}
	public class BinaryNodeWriter : NodeWriter {
		private Stream _strm;
		public BinaryNodeWriter(Stream strm) {
			_strm = strm;
		}
		public override void Write(StorageNode node) {
			WriteString(_strm, node.Name);
			WriteString(_strm, node.TextValue==null? "" : node.TextValue);
			WriteShort(_strm, (short)node.ValueCount);
			IDictionaryEnumerator ie = node.GetPairEnumerator();
			while(ie.MoveNext()) {
				WriteString(_strm, (string)ie.Key);
				WriteString(_strm, (string)ie.Value);
			}
			WriteShort(_strm, (short)node.ChildCount);
			foreach(StorageNode ch in node.Children)
				Write(ch);
		}
		public override void Close() {
			_strm.Close();
		}

		private static void WriteString(Stream strm, string data) {
			byte[] t = Encoding.UTF8.GetBytes(data);
			WriteShort(strm, (short)t.Length);
			strm.Write(t, 0, t.Length);
		}
		private static void WriteShort(Stream strm, short value) {
			strm.WriteByte((byte)(value / 0x100));
			strm.WriteByte((byte)(value % 0x100));
		}
	}
}
