/*
 * Copyright (c) Tasuku SUENAGA, Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;
using System.Collections;

namespace Travis.Archive {
	/// <summary>
	/// Extract の概要の説明です。
	/// </summary>
	public abstract class Extract {
	
		public class FileBuffer{
			public string filename;
			public byte[] buffer;
		};
	
		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi,Pack=4)]
			public struct INDIVIDUALINFO{
			public uint dwOriginalSize;
			public uint dwCompressedSize;
			public uint dwCRC;
			public uint uFlag;
			public uint uOSType;
			public ushort wRatio;
			public ushort wDate;
			public ushort wTime;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=513)]
			public string szFileName;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=3)]
			public string dummy1;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=8)]
			public string szAttribute;
			[MarshalAs(UnmanagedType.ByValTStr,SizeConst=8)]
			public string szMode;
		}
	
		public Extract() {
		}
		public abstract uint getVersion();
		public abstract bool getRunning();
		public abstract bool checkArchive(string filename,int mode);
		public abstract string getDllName();
		public abstract int execute(string cmdline,string output,int size);
		
		public abstract int extract(IntPtr harc,string filename);
		public abstract IntPtr openArchive(string filename,uint mode);
		public abstract int closeArchive(IntPtr harc);
		public abstract int findFirst(IntPtr harc,ref INDIVIDUALINFO info);
		public abstract int findNext(IntPtr harc,ref INDIVIDUALINFO info);
		public abstract int getFileName(IntPtr harc,ref string filename);
		public abstract int extractMem(string filename,byte[] outbuf);
		
		public bool throwException(string msg) {
			throw new ApplicationException(getDllName() + " : " + msg);
		}

		public ArrayList extract(byte[] inbuf){
		
			string tmpfn = Path.GetTempFileName();
			FileStream fst = File.OpenWrite(tmpfn);
			BinaryWriter bw = new BinaryWriter(fst);
			bw.Write(inbuf);
			bw.Close();
			fst.Close();

			ArrayList retval = extract(tmpfn);
			
			File.Delete(tmpfn);
			
			return retval;
		}

		public ArrayList extract(string infile) {
			//指定されたファイルがあるか調べる
			if (!System.IO.File.Exists(infile)) {
				throw new ApplicationException("ファイルが見つかりません");
			}

			//DLLのチェック
			try {
				uint ver = getVersion();
			}
			catch {
				throwException("解凍を行うDLLが見つかりません。\nhttp://www.madobe.net/archiver/lib/main.htmlからインストールして下さい。");
			}

			if (getRunning()) {
				throwException("他のアプリケーションで使用中です。");
			}

			if (!checkArchive(infile, 0)) {
				throwException("このDLLでは解凍できないファイルです。");
			}

			if (infile.IndexOf(' ') > 0) {
				infile = "\"" + infile + "\"";
			}
			
			// ファイルを全部さらう
			// memo:あとで個別解凍より、
			// 一個ずつテンポラリファイルに書き出して読み込んだほうがパフォーマンス高くなるかも
			ArrayList fbar = new ArrayList();
			INDIVIDUALINFO info = new INDIVIDUALINFO();
			IntPtr harc = openArchive(infile,0x02000100); // M_CHECK_ALL_PATH | M_BAR_WINDOWS_OFF
			
			if(findFirst(harc,ref info) != -1){
				do{
					// ディレクトリは除く
					if(info.dwOriginalSize != 0){
						FileBuffer fb = new FileBuffer();
						fb.filename = info.szFileName;
						fb.buffer = new byte[info.dwOriginalSize];
						fbar.Add(fb);
					}
				}while(findNext(harc,ref info) != -1);
			}
			closeArchive(harc);

			// さらったあとで個別に解凍
			foreach(FileBuffer fb in fbar){
				if(extractMem(infile + " \"" + fb.filename + "\"",fb.buffer) != 0){
					throwException("ファイルの解凍に失敗しました。");
				}
			}

			return fbar;
		}
	}

	public class ExtractLha : Extract {
		[DllImport("unlha32")]
		private extern static UInt16 UnlhaGetVersion();
		[DllImport("unlha32")]
		private extern static bool UnlhaGetRunning();
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private extern static bool UnlhaCheckArchive(string szFileName, int iMode);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private extern static int Unlha(int hwnd, string szCmdLine, string szOutput, int dwSize);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private extern static int UnlhaExtract(IntPtr harc,string szFileName,string szDirName,uint dwMode);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private extern static int UnlhaExtractMem(int hwnd, string szCmdLine, byte[] szBuffer, int dwSize, IntPtr lpTime, IntPtr lpwAttr, IntPtr lpdwWriteSize);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private static extern IntPtr UnlhaOpenArchive(IntPtr hwnd, string szFileName, uint dwMode);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private static extern int UnlhaCloseArchive(IntPtr harc);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private static extern int UnlhaFindFirst(IntPtr harc, string szWildName, ref INDIVIDUALINFO lpSubInfo);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private static extern int UnlhaFindNext(IntPtr harc, ref INDIVIDUALINFO lpSubInfo);
		[DllImport("unlha32", CharSet=CharSet.Ansi)]
		private static extern int UnlhaGetFileName(IntPtr harc, StringBuilder lpBuffer, int nsize);
				
		public override uint getVersion(){return (uint)UnlhaGetVersion();}
		public override bool getRunning(){return UnlhaGetRunning();}
		public override bool checkArchive(string filename,int mode){return UnlhaCheckArchive(filename,mode);}
		public override int execute(string cmdline,string output,int size){return Unlha(0,cmdline,output,size);}
		public override string getDllName(){return "Unlha32.dll";}


		public override int extract(IntPtr harc,string filename){
			// 未実装
			return 0;
		}
		public override int extractMem(string filename,byte[] outbuf) {
			return UnlhaExtractMem(0,"-n " + filename,outbuf,outbuf.Length,IntPtr.Zero,IntPtr.Zero,IntPtr.Zero);
		}		
		public override IntPtr openArchive(string filename,uint mode){return UnlhaOpenArchive(IntPtr.Zero,filename,0);}
		public override int closeArchive(IntPtr harc){return UnlhaCloseArchive(harc);}
		public override int findFirst(IntPtr harc,ref INDIVIDUALINFO info){return UnlhaFindFirst(harc,"*.*",ref info);}
		public override int findNext(IntPtr harc,ref INDIVIDUALINFO info){return UnlhaFindNext(harc,ref info);}
		public override int getFileName(IntPtr harc,ref string filename){
			StringBuilder sb = new StringBuilder(256);
			int retval = UnlhaGetFileName(harc, sb, 256);
			filename = sb.ToString();
			return retval;
		}
	}
	public class ExtractZip : Extract {
		[DllImport("unzip32")]
		private extern static UInt16 UnZipGetVersion();
		[DllImport("unzip32")]
		private extern static bool UnZipGetRunning();
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private extern static bool UnZipCheckArchive(string szFileName, int iMode);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private extern static int UnZip(int hwnd, string szCmdLine, string szOutput, int dwSize);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private extern static int UnZipExtract(IntPtr harc,string szFileName,string szDirName,uint dwMode);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private extern static int UnZipExtractMem(int hwnd, string szCmdLine, byte[] szBuffer, int dwSize, IntPtr lpTime, IntPtr lpwAttr, IntPtr lpdwWriteSize);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private static extern IntPtr UnZipOpenArchive(IntPtr hwnd, string szFileName, uint dwMode);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private static extern int UnZipCloseArchive(IntPtr harc);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private static extern int UnZipFindFirst(IntPtr harc, string szWildName, ref INDIVIDUALINFO lpSubInfo);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private static extern int UnZipFindNext(IntPtr harc, ref INDIVIDUALINFO lpSubInfo);
		[DllImport("unzip32", CharSet=CharSet.Ansi)]
		private static extern int UnZipGetFileName(IntPtr harc, StringBuilder lpBuffer, int nsize);

		public override uint getVersion(){return (uint)UnZipGetVersion();}
		public override bool getRunning(){return UnZipGetRunning();}
		public override bool checkArchive(string filename,int mode){return UnZipCheckArchive(filename,mode);}
		public override int execute(string cmdline,string output,int size){return UnZip(0,cmdline,output,size);}
		public override string getDllName(){return "UnZip32.dll";}

		public override int extract(IntPtr harc,string filename){
			// 未実装
			return 0;
		}
		public override int extractMem(string filename,byte[] outbuf) {
			return UnZipExtractMem(0,"--i " + filename,outbuf,outbuf.Length,IntPtr.Zero,IntPtr.Zero,IntPtr.Zero);
		}
		public override IntPtr openArchive(string filename,uint mode){return UnZipOpenArchive(IntPtr.Zero,filename,0);}
		public override int closeArchive(IntPtr harc){return UnZipCloseArchive(harc);}
		public override int findFirst(IntPtr harc,ref INDIVIDUALINFO info){return UnZipFindFirst(harc,"*.*",ref info);}
		public override int findNext(IntPtr harc,ref INDIVIDUALINFO info){return UnZipFindNext(harc,ref info);}
		public override int getFileName(IntPtr harc,ref string filename) {
			StringBuilder sb = new StringBuilder(256);
			int retval = UnZipGetFileName(harc, sb, 256);
			filename = sb.ToString();
			return retval;
		}
	}
}

