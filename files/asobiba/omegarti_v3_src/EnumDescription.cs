/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Collections;
using System.Reflection;

namespace Travis.Util
{
	//整数のenum値に表記をつけたり相互変換したりする　構造上
	[AttributeUsage(AttributeTargets.Enum)]
	internal class EnumDescAttribute : Attribute {
		protected string[] _descriptions;
		protected Hashtable _descToValue;
		protected string[] _names;
		protected Hashtable _nameToValue;
		public EnumDescAttribute(Type t) { //これはコンストラクタからもらってこなくても良さそうなものだが
			MemberInfo[] ms = t.GetMembers();
			_descToValue = new Hashtable();
			_nameToValue = new Hashtable();

			ArrayList descriptions = new ArrayList();
			ArrayList names = new ArrayList();

			int expected = 0;
			foreach(MemberInfo mi in ms) {
				FieldInfo fi = mi as FieldInfo;
				if(fi!=null && fi.IsStatic && fi.IsPublic) {
					int intVal = (int)fi.GetValue(null); //int以外をベースにしているEnum値はサポート外
					if(intVal!=expected) throw new Exception("unexpected enum value order"); //０から順に並ぶenum値のみ対応
					EnumValueAttribute a = (EnumValueAttribute)(fi.GetCustomAttributes(typeof(EnumValueAttribute), false)[0]);
					
					string desc = a.Description;
					descriptions.Add(desc);
					_descToValue[desc] = intVal;

					string name = fi.Name;
					names.Add(name);
					_nameToValue[name] = intVal;

					expected++;
				}
			}

			_descriptions = (string[])descriptions.ToArray(typeof(string));
			_names        = (string[])names.ToArray(typeof(string));
		}

		public virtual string GetDescription(ValueType i) {
			return _descriptions[(int)i];
		}
		public virtual ValueType FromDescription(string v) {
			return (ValueType)_descToValue[v];
		}
		public virtual ValueType FromDescription(string v, ValueType d) {
			if(v==null) return d;
			ValueType t = (ValueType)_descToValue[v];
			return t==null? d : t;
		}
		public virtual string GetName(ValueType i) {
			return _names[(int)i];
		}
		public virtual ValueType FromName(string v) {
			return (ValueType)_nameToValue[v];
		}
		public virtual ValueType FromName(string v, ValueType d) {
			if(v==null) return d;
			ValueType t = (ValueType)_nameToValue[v];
			return t==null? d : t;
		}

		public virtual string[] DescriptionCollection() {
			return _descriptions;
		}

		//アトリビュートを取得する
		private static Hashtable _typeToAttr = new Hashtable();
		public static EnumDescAttribute For(Type type) {
			EnumDescAttribute a = _typeToAttr[type] as EnumDescAttribute;
			if(a==null) {
				a = (EnumDescAttribute)(type.GetCustomAttributes(typeof(EnumDescAttribute), false)[0]);
				_typeToAttr.Add(type, a);
			}
			return a;
		}

		/*
		public static void Test() {
			Type t = typeof(ETest);
			EnumDescAttribute a = EnumDescAttribute.For(t);
			string x = a.GetDescription(ETest.B);
			ETest x2 = (ETest)a.FromDescription("CCC");
			string x3 = a.GetName(ETest.C);
			ETest x4 = (ETest)a.FromName("A");

			ValueType x5 = a.FromName("RRRRRRRRRRRRRR");
			ETest x6 = (ETest)x5;

		}
		*/
	}

	[AttributeUsage(AttributeTargets.Field)]
	internal class EnumValueAttribute : Attribute {
		protected string _description;
		public string Description {
			get {
				return _description;
			}
			set {
				_description = value;
			}
		}
	}

}
