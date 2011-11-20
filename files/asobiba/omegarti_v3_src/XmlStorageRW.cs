/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Xml;

using Travis.Util;

namespace Travis.Storage
{
	public class DOMNodeReader : NodeReader {
		private XmlDocument _doc;
		public DOMNodeReader(XmlDocument doc) {
			_doc = doc;
		}
		public override StorageNode Read() {
			return ReadNode(_doc.DocumentElement);
		}
		private StorageNode ReadNode(XmlElement elem) {
			StorageNode node = new StorageNode();
			node.Name = elem.LocalName;
			foreach(XmlAttribute attr in elem.Attributes)
				node[attr.LocalName] = attr.Value;
			foreach(XmlNode ch in elem.ChildNodes) {
				if(ch is XmlElement)
					node.AddChild(ReadNode((XmlElement)ch));
				else if(ch is XmlText || ch is XmlCDataSection)
					node.TextValue = ch.Value;
			}
			return node;

		}
		public override void Close() {
		}

	}
}
