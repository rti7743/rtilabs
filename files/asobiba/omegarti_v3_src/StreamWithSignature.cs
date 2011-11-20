/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Security.Cryptography;
using System.Diagnostics;

namespace Zanetti
{
/*
 * 任意のテキストデータ（典型的にはXML）に簡易署名機構を追加する。
 * XML Signatureみたいなのをマジで実装するのは大変なので、末尾のコメントに
 * <!-- sig:??????? -->
 * の形式の行をつける。署名対象はファイルの先頭からこの行の直前の行末までとする
 */

	internal class StreamWithSignature {

		private byte[] _rawData;

		public StreamWithSignature(string filename) {
			_rawData = new byte[(int)new FileInfo(filename).Length];
			FileStream s = new FileStream(filename, FileMode.Open, FileAccess.Read);
			s.Read(_rawData, 0, _rawData.Length);
			s.Close();
		}
		public MemoryStream AsStream() {
			return new MemoryStream(_rawData, false);
		}
		public byte[] SHA1Hash {
			get {
				int e = FindEndOfBody();
				//Debug.WriteLine("Signature Offset " + e);
				if(e==-1)
					return null;
				else
					return new SHA1CryptoServiceProvider().ComputeHash(_rawData, 0, e);
			}
		}

		private int FindEndOfBody() {
			//utf16だとこれではだめだがね
			byte[] targets = System.Text.Encoding.ASCII.GetBytes("<!-- sig:");
			int c = targets.Length-1;
			for(int i=_rawData.Length-1; i>=0; i--) {
				byte d = _rawData[i];
				if(d==targets[c]) {
					if(c==0) return i;
					else c--;
				}
				else
					c = targets.Length-1;
			}
			return -1;

		}

	}
}
