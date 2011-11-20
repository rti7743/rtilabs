/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Xml;

namespace Travis.Util
{
	/// <summary>
	/// XmlUtil ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal class XmlUtil
	{
		public static string GetAttribute(XmlElement elem, string name) {
			string v = elem.GetAttribute(name);
			return v;
		}
		public static XmlElement GetFirstChildElement(XmlElement parent) {
			XmlNode n = parent.FirstChild;
			while(n!=null && n.NodeType!=XmlNodeType.Element)
				n = n.NextSibling;
			return (XmlElement)n;
		}
		public static XmlElement GetNextElement(XmlElement elem) {
			XmlNode n = elem.NextSibling;
			while(n!=null && n.NodeType!=XmlNodeType.Element)
				n = n.NextSibling;
			return (XmlElement)n;
		}
		public static bool ParseBool(string v) {
			return v=="true"? true : false;
		}
		public static string GetElementValue(XmlElement parent, string name) {
			XmlNodeList nl = parent.GetElementsByTagName(name);
			if(nl.Count==0)
				return "";
			else
				return nl[0].InnerText;
		}

		public static XmlDocument LoadDOM(string file) {
			XmlDocument doc = new XmlDocument();
			doc.Load(file);
			return doc;
		}
	}
}
