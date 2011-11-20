/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Collections;
using System.IO.Compression;

using Zanetti.Data;

namespace Zanetti.DataSource.Specialized
{
	internal class DreamVisorDataSource : FullDataSource {

		public DreamVisorDataSource(CodeEnumerator ce) : base(ce) {
		}
		public override byte[] DownloadFullData(BasicBrand br) {
            int code = br.Code;
            GZipStream gs = new GZipStream(Util.HttpDownload(FormatDailyTimeSeriesURL(code, br.Market)), CompressionMode.Decompress);
            MemoryStream result = new MemoryStream();
            try {
                Util.CopyStream(gs, result);
                return result.ToArray();
            }
            finally {
                gs.Close();
                result.Close();
            }
		}


		public override void ImportFullData(BasicBrand br, byte[] buf) {
			FileStream d = null;
			try {
				int code = br.Code;
				string filename = Util.GetDailyDataFileName(code);
				d = new FileStream(filename, FileMode.Create);
				int head = buf[0] + (buf[1]*256); //最初の２バイトでレコード数を示している?
				
				int offset = 4;
				for(int i=0; i<head; i++) {
					d.Write(buf, offset+0,   24); //日付、４本値、出来高のあと12バイトの空白
					d.Write(buf, offset+24,   8); //出力は信用の分だけ進める
					offset += 36;
				}
			}
			finally {
				if(d!=null) d.Close();
				//Debug.WriteLine("DL END");
			}
		}

		private static string FormatDailyTimeSeriesURL(int code, MarketType mt) {
			if(mt==MarketType.B) //指数は長期
				return String.Format("http://www.dreamvisor.com/chart/gzip.pl?L{0}", MarketTypeToFileName((BuiltInIndex)code));
			else
                return String.Format("http://www.dreamvisor.com/chart/gzip.pl?S{0}", code);
		}

		private static string MarketTypeToFileName(BuiltInIndex m) {
			switch(m) {
				case BuiltInIndex.Nikkei225:
					return "0101";
				case BuiltInIndex.Nikkei225_F:
					return "0102";
				case BuiltInIndex.TOPIX:
					return "0151";
				case BuiltInIndex.TOPIX_F:
					return "0152";
				case BuiltInIndex.JASDAQ:
					return "0190";
				case BuiltInIndex.LONGTERM_INTEREST:
					return "0551";
				case BuiltInIndex.Dow:
					return "0601";
				case BuiltInIndex.SP500:
					return "0602";
				case BuiltInIndex.Nasdaq:
					return "0603";
				case BuiltInIndex.JPYUSD:
					return "0501";
				case BuiltInIndex.JPYEUR:
					return "0502";
				default:
					if((int)BuiltInIndexGroup.SectorIndexStart<=(int)m && (int)m<=(int)BuiltInIndexGroup.SectorIndexEnd)
						return String.Format("0{0}", (int)m - (int)BuiltInIndexGroup.SectorIndexStart + 321);
					else
						throw new ArgumentException("unexpected market type " + m);
			}
		}
		private static bool VolumeIsAvailable(int code) {
			return true;
		}
	}
}
