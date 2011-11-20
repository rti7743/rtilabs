/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Runtime.InteropServices;

namespace Zanetti
{

	/// <summary>
	/// ゲバラで使うWin32API関係の宣言一式
	/// </summary>
	public class Win32 {

		//関数
		[DllImport("user32.dll", CharSet=CharSet.Auto)]
		public static extern IntPtr FindWindowEx(
			IntPtr hwndParent,      // handle to parent window
			IntPtr hwndChildAfter,  // handle to child window
			string lpszClass,    // class name
			string lpszWindow    // window name
			);
		[DllImport("user32.dll", ExactSpelling=false, CharSet=CharSet.Auto)]
		public static extern unsafe int GetWindowText(IntPtr hwnd, char* buf, int size);
		[DllImport("user32.dll")]
		public static extern int SendMessage(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll")]
		public static extern int PostMessage(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam);
		[DllImport("user32.dll")]
		public static extern IntPtr GetDesktopWindow();
		[DllImport("user32.dll")]
		public static extern IntPtr LoadIcon(IntPtr hModule, IntPtr iconName);
		[DllImport("user32.dll")]
		public static extern int GetSystemMetrics(int index);
		[DllImport("user32.dll")]
		public static extern int GetCaretBlinkTime();
		[DllImport("user32.dll")]
		public static extern int MessageBeep(int type);
		[DllImport("user32.dll")]
		public static extern short GetKeyState(int vk);
		[DllImport("user32.dll", CharSet=CharSet.Auto)]
		public static extern short VkKeyScan(char ch);
		[DllImport("user32.dll")]
		public static extern bool GetKeyboardState(byte[] data);

		
		[DllImport("kernel32.dll")]
		public static extern int GetLastError();
		[DllImport("kernel32.dll")]
		public static extern IntPtr CreateEvent(IntPtr lpSecurityAttribute, int manualReset, int initialState, IntPtr name); //ポインタだとunsafeにする必要があるので逃げる
		[DllImport("kernel32.dll")]
		public static extern bool CloseHandle(IntPtr lpSecurityAttribute);
		[DllImport("kernel32.dll")]
		public static extern int WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

		[DllImport("kernel32.dll")]
		public static extern bool GetCommState(IntPtr handle, ref DCB dcb);
		[DllImport("kernel32.dll")]
		public static extern bool SetCommState(IntPtr handle, ref DCB dcb);
		[DllImport("kernel32.dll")]
		public static extern bool GetCommTimeouts(IntPtr handle, ref COMMTIMEOUTS timeouts);
		[DllImport("kernel32.dll")]
		public static extern bool SetCommTimeouts(IntPtr handle, ref COMMTIMEOUTS timeouts);
		[DllImport("kernel32.dll")]
		public static extern bool SetCommBreak(IntPtr handle);
		[DllImport("kernel32.dll")]
		public static extern bool ClearCommBreak(IntPtr handle);
		[DllImport("kernel32.dll")]
		public static extern bool WaitCommEvent(
			IntPtr hFile,                // handle to comm device
			out int lpEvtMask,           // event type
			ref OVERLAPPED lpOverlapped   // overlapped structure
			);
		[DllImport("kernel32.dll")]
		public static extern bool ClearCommError(
			IntPtr hFile,     // handle to communications device
			out int lpErrors, // error codes
			IntPtr lpStat  // communications status (本当はCommStat)
			);
		[DllImport("kernel32.dll")]
		public static extern bool SetCommMask(
			IntPtr hFile,                // handle to comm device
			int flags
			);

		[DllImport("kernel32.dll", CharSet=CharSet.Auto)]
		public static extern IntPtr CreateFile(
			string filename,
			uint dwDesiredAccess,                      // access mode
			uint dwShareMode,                          // share mode
			IntPtr lpSecurityAttributes, // SD
			uint dwCreationDisposition,                // how to create
			uint dwFlagsAndAttributes,                 // file attributes
			IntPtr hTemplateFile                        // handle to template file
			);

		[DllImport("kernel32.dll")]
		public static extern bool ReadFile(
			IntPtr hFile,                // handle to file
			byte[] lpBuffer,             // data buffer
			int nNumberOfBytesToRead,  // number of bytes to read
			ref int lpNumberOfBytesRead, // number of bytes read
			ref OVERLAPPED lpOverlapped    // overlapped buffer
			);
		[DllImport("kernel32.dll")]
		public static extern bool WriteFile(
			IntPtr hFile,                // handle to file
			byte[] lpBuffer,             // data buffer
			int nNumberOfBytesToRead,  // number of bytes to read
			ref int lpNumberOfBytesRead, // number of bytes read
			ref OVERLAPPED lpOverlapped    // overlapped buffer
			);
		[DllImport("kernel32.dll")]
		public static extern bool GetOverlappedResult(
			IntPtr hFile,                       // handle to file, pipe, or device
			ref OVERLAPPED lpOverlapped,          // overlapped structure
			ref int lpNumberOfBytesTransferred, // bytes transferred
			bool bWait                          // wait option
			);
		[DllImport("kernel32.dll")]
		public static extern bool FlushFileBuffers(IntPtr handle);

		[DllImport("kernel32.dll", CharSet=CharSet.Ansi)] //WinExecはLPCSTRだった。"T"がない。
		public static extern int WinExec(string command, int uCmdShow);


		//描画をネイティブコードに
		[DllImport("gdi32.dll", CharSet=CharSet.Unicode)]
		public static extern unsafe int TextOut(IntPtr hdc, int x, int y, char* text, int length);
		[DllImport("user32.dll", CharSet=CharSet.Unicode)]
		public static extern int DrawText(IntPtr hdc, string text, int length, ref RECT rc, int flags);
		[DllImport("gdi32.dll", CharSet=CharSet.Unicode)]
		public static extern int GetTextExtentPoint32(IntPtr hdc, string text, int length, out SIZE size);
		[DllImport("gdi32.dll")]
		public static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);
		[DllImport("gdi32.dll")]
		public static extern int DeleteObject(IntPtr hObject);
		[DllImport("gdi32.dll")]
		public static extern IntPtr CreatePen(int style, int width, uint color);
		[DllImport("gdi32.dll")]
		public static extern IntPtr CreateSolidBrush(uint color);
		[DllImport("gdi32.dll")]
		public static extern int SetTextColor(IntPtr hDC, uint color);
		[DllImport("gdi32.dll")]
		public static extern int SetBkColor(IntPtr hDC, uint color);
		[DllImport("user32.dll")] //なぜuser32?
		public static extern int FillRect(IntPtr hDC, ref RECT rect, IntPtr brush);
		[DllImport("gdi32.dll")]
		public static extern int Rectangle(IntPtr hDC, int left, int top, int right, int bottom);
		[DllImport("gdi32.dll")]
		public static extern int MoveToEx(IntPtr hDC, int x, int y, out POINT prev);
		[DllImport("gdi32.dll")]
		public static extern int LineTo(IntPtr hDC, int x, int y);
		[DllImport("gdi32.dll")]
		public static extern int Ellipse(IntPtr hDC, int x1, int y1, int x2, int y2);
		// Bitblt
		[DllImport("gdi32.dll")]
		public static extern bool BitBlt(IntPtr hdcDest, 
			int nXDest, int nYDest, int nWidth, int nHeight, 
			IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);
     		

		
		[DllImport("gdi32.dll", CharSet=CharSet.Auto)]
		public static extern int EnumFontFamiliesEx(
			IntPtr hdc,                          // handle to DC
			ref tagLOGFONT lpLogfont,              // font information
			EnumFontFamExProc lpEnumFontFamExProc, // callback function
			IntPtr lParam,                    // additional data
			uint  dwFlags                     // not used; must be 0
			);
		
		[DllImport("gdi32.dll", CharSet=CharSet.Auto)]
		public static extern IntPtr CreateFontIndirect(ref LOGFONT lf);
		[DllImport("gdi32.dll")]
		public static extern IntPtr CreatePolygonRgn(POINT[] points, int count, int mode);
		[DllImport("gdi32.dll")]
		public static extern int FillRgn(IntPtr hdc, IntPtr rgn, IntPtr brush);

		[DllImport("gdiplus.dll")]
		public static extern int GdipCreateFromHDC(IntPtr hdc, ref IntPtr graphics);
		[DllImport("gdiplus.dll")]
		public static extern int GdipDeleteGraphics(IntPtr graphics);
		[DllImport("gdiplus.dll")]
		public static extern int GdipSetTextRenderingHint(IntPtr graphics, int mode);
		

		[DllImport("imm32.dll")]
		public static extern IntPtr ImmGetContext(IntPtr hWnd);
		[DllImport("imm32.dll")]
		public static extern bool ImmReleaseContext(IntPtr hWnd, IntPtr hIMC);
		[DllImport("imm32.dll", CharSet=CharSet.Auto)]
		public static extern bool ImmSetCompositionFont(IntPtr hIMC, LOGFONT lf);
		[DllImport("imm32.dll")]
		public static extern bool ImmSetCompositionWindow(IntPtr hIMC, ref COMPOSITIONFORM form);
		[DllImport("imm32.dll")]
		public static extern bool ImmNotifyIME(IntPtr hIMC, int dwAction, int dwIndex, int dwValue);

		[DllImport("shell32.dll", CharSet=CharSet.Auto)]
		public static extern IntPtr ShellExecute(IntPtr hwnd, string verb, string file, string param, string dir, int cmd);

		//定数
		public const int WM_COPYDATA = 0x4A;
		public const int WM_NOTIFY = 0x4E;
		public const int WM_USER = 0x400;
		public const int WM_VSCROLL = 0x115;
		public const int WM_IME_STARTCOMPOSITION =        0x010D;
		public const int WM_IME_ENDCOMPOSITION   =        0x010E;
		public const int WM_MOUSEWHEEL           =        0x020A;
		public const int WM_KEYDOWN              =        0x0100;

		public const int TCN_FIRST = -550;
		public const int TCN_SELCHANGING = (TCN_FIRST - 2);

		public const int VK_LSHIFT   =      0xA0;
		public const int VK_RSHIFT   =      0xA1;
		public const int VK_LCONTROL =      0xA2;
		public const int VK_RCONTROL =      0xA3;
		public const int VK_LMENU    =      0xA4;
		public const int VK_RMENU    =      0xA5;

		public const uint GENERIC_READ  =                   (0x80000000);
		public const uint GENERIC_WRITE =                   (0x40000000);
		public const uint OPEN_EXISTING =      3;
		public const uint FILE_ATTRIBUTE_NORMAL =              0x00000080;
		public const uint FILE_FLAG_OVERLAPPED =           0x40000000;

		public const int IDI_APPLICATION =    32512;
		public const int IDI_HAND        =    32513;
		public const int IDI_QUESTION    =    32514;
		public const int IDI_EXCLAMATION =    32515;
		public const int IDI_ASTERISK    =    32516;

		public static IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);

		public const int CFS_DEFAULT =                    0x0000;
		public const int CFS_RECT =                       0x0001;
		public const int CFS_POINT =                      0x0002;
		public const int CFS_FORCE_POSITION =             0x0020;
		public const int CFS_CANDIDATEPOS =               0x0040;
		public const int CFS_EXCLUDE =                    0x0080;

		//GetSystemMetricsに渡す定数。とりあえず必要な分だけ
		public const int SM_CXVSCROLL =           2;
		public const int SM_CXEDGE    =           45;
		public const int SM_CYEDGE    =           46;

		public const int NI_COMPOSITIONSTR =              0x0015;
		public const int CPS_CANCEL =                     0x0004;

		public const int ERROR_IO_PENDING = 997;
		public const int WAIT_OBJECT_0 = 0;

/*
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11
*/

		//構造体
		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
			public unsafe struct COPYDATASTRUCT {
			public uint dwData;
			public uint cbData;
			public void* lpData;
		}

		[StructLayout(LayoutKind.Sequential)]
			public struct NMHDR { 
			public IntPtr hwndFrom; 
			public uint idFrom; 
			public int code; 
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct DCB { 
			public uint DCBlength; 
			public uint BaudRate;
			public uint Misc;
			/*
			DWORD fBinary: 1; 
			DWORD fParity: 1; 
			DWORD fOutxCtsFlow:1; 
			DWORD fOutxDsrFlow:1; 
			DWORD fDtrControl:2; 
			DWORD fDsrSensitivity:1; 
			DWORD fTXContinueOnXoff:1; 
			DWORD fOutX: 1; 
			DWORD fInX: 1; 
			DWORD fErrorChar: 1; 
			DWORD fNull: 1; 
			DWORD fRtsControl:2; 
			DWORD fAbortOnError:1; 
			DWORD fDummy2:17; 
			*/
			public ushort wReserved; 
			public ushort XonLim; 
			public ushort XoffLim; 
			public byte ByteSize; 
			public byte Parity; 
			public byte StopBits; 
			public byte XonChar; 
			public byte XoffChar; 
			public byte ErrorChar; 
			public byte EofChar; 
			public byte EvtChar; 
			public ushort wReserved1; 
		} 

		//Font#ToLogFontに渡すためにはstructではだめでclassにしないといかん
		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		public class LOGFONT {
			public int lfHeight;
			public int lfWidth;
			public int lfEscapement;
			public int lfOrientation;
			public int lfWeight;
			public byte lfItalic;
			public byte lfUnderline;
			public byte lfStrikeOut;
			public byte lfCharSet;
			public byte lfOutPrecision;
			public byte lfClipPrecision;
			public byte lfQuality;
			public byte lfPitchAndFamily;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
			public string lfFaceName;
		}
		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		public struct tagLOGFONT {
			public int lfHeight;
			public int lfWidth;
			public int lfEscapement;
			public int lfOrientation;
			public int lfWeight;
			public byte lfItalic;
			public byte lfUnderline;
			public byte lfStrikeOut;
			public byte lfCharSet;
			public byte lfOutPrecision;
			public byte lfClipPrecision;
			public byte lfQuality;
			public byte lfPitchAndFamily;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
			public string lfFaceName;
		}

		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		public struct ENUMLOGFONTEX {
			//LOGFONT part
			public int lfHeight;
			public int lfWidth;
			public int lfEscapement;
			public int lfOrientation;
			public int lfWeight;
			public byte lfItalic;
			public byte lfUnderline;
			public byte lfStrikeOut;
			public byte lfCharSet;
			public byte lfOutPrecision;
			public byte lfClipPrecision;
			public byte lfQuality;
			public byte lfPitchAndFamily;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
			public string lfFaceName;
			//ENUMLOGFONTEX part
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=64)]
			public string elfFullName;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
			public string elfStyle;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst=32)]
			public string elfScript;
		}

		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		public struct NEWTEXTMETRIC {
			public int   tmHeight; 
			public int   tmAscent; 
			public int   tmDescent; 
			public int   tmInternalLeading; 
			public int   tmExternalLeading; 
			public int   tmAveCharWidth; 
			public int   tmMaxCharWidth; 
			public int   tmWeight; 
			public int   tmOverhang; 
			public int   tmDigitizedAspectX; 
			public int   tmDigitizedAspectY; 
			public char  tmFirschar; 
			public char  tmLaschar; 
			public char  tmDefaulchar; 
			public char  tmBreakChar; 
			public byte   tmItalic; 
			public byte   tmUnderlined; 
			public byte   tmStruckOut; 
			public byte   tmPitchAndFamily; 
			public byte   tmCharSet; 
			public uint  ntmFlags; 
			public uint   ntmSizeEM; 
			public uint   ntmCellHeight; 
			public uint   ntmAvgWidth; 
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct FONTSIGNATURE {
			[MarshalAs(UnmanagedType.ByValArray, SizeConst=4)]
			public uint[]  fsUsb;
			[MarshalAs(UnmanagedType.ByValArray, SizeConst=2)]
			public uint[]  fsCsb;
		}

		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		public struct NEWTEXTMETRICEX {
			public NEWTEXTMETRIC ntmTm;
			public FONTSIGNATURE ntmFontSig;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct COMMTIMEOUTS {  
			public uint ReadIntervalTimeout; 
			public uint ReadTotalTimeoutMultiplier; 
			public uint ReadTotalTimeoutConstant; 
			public uint WriteTotalTimeoutMultiplier; 
			public uint WriteTotalTimeoutConstant; 
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct POINT {
			public int x;
			public int y;
		}
		[StructLayout(LayoutKind.Sequential)]
			public struct SIZE {
			public int width;
			public int height;
		}
		[StructLayout(LayoutKind.Sequential)]
		public struct RECT {
			public int left;
			public int top;
			public int right;
			public int bottom;
		}
		public static RECT CreateRect(int left, int top, int width, int height) {
			RECT r = new RECT();
			r.left = left;
			r.top  = top;
			r.right = left+width;
			r.bottom = top+height;
			return r;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct COMPOSITIONFORM {
			public int dwStyle;
			public POINT ptCurrentPos;
			public RECT  rcArea;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct OVERLAPPED { 
			public int  Internal; 
			public int  InternalHigh; 
			public int  Offset; 
			public int  OffsetHigh; 
			public IntPtr hEvent; 
		}

		//callbacks
		public delegate int EnumFontFamExProc(ref ENUMLOGFONTEX lpelfe,    // logical-font data
						 ref NEWTEXTMETRICEX lpntme,  // physical-font data
						 uint FontType,           // type of font
						 IntPtr lParam             // application-defined data
						 );


		public enum VK {
			LBUTTON		= 0x01,
			CANCEL		= 0x03,
			BACK			= 0x08,
			TAB			= 0x09,
			CLEAR		= 0x0C,
			RETURN		= 0x0D,
			SHIFT		= 0x10,
			CONTROL		= 0x11,
			MENU			= 0x12,
			CAPITAL		= 0x14,
			ESCAPE		= 0x1B,
			SPACE		= 0x20,
			PRIOR		= 0x21,
			NEXT			= 0x22,
			END			= 0x23,
			HOME			= 0x24,
			LEFT			= 0x25,
			UP			= 0x26,
			RIGHT		= 0x27,
			DOWN			= 0x28,
			SELECT		= 0x29,
			EXECUTE		= 0x2B,
			SNAPSHOT		= 0x2C,
			HELP			= 0x2F,
			D0			= 0x30,
			D1			= 0x31,
			D2			= 0x32,
			D3			= 0x33,
			D4			= 0x34,
			D5			= 0x35,
			D6			= 0x36,
			D7			= 0x37,
			D8			= 0x38,
			D9			= 0x39,
			A			= 0x41,
			B			= 0x42,
			C			= 0x43,
			D			= 0x44,
			E			= 0x45,
			F			= 0x46,
			G			= 0x47,
			H			= 0x48,
			I			= 0x49,
			J			= 0x4A,
			K			= 0x4B,
			L			= 0x4C,
			M			= 0x4D,
			N			= 0x4E,
			O			= 0x4F,
			P			= 0x50,
			Q			= 0x51,
			R			= 0x52,
			S			= 0x53,
			T			= 0x54,
			U			= 0x55,
			V			= 0x56,
			W			= 0x57,
			X			= 0x58,
			Y			= 0x59,
			Z			= 0x5A,
			NUMPAD0		= 0x60,
			NUMPAD1		= 0x61,
			NUMPAD2		= 0x62,
			NUMPAD3		= 0x63,
			NUMPAD4		= 0x64,
			NUMPAD5		= 0x65,
			NUMPAD6		= 0x66,
			NUMPAD7		= 0x67,
			NUMPAD8		= 0x68,
			NUMPAD9		= 0x69,
			MULTIPLY		= 0x6A,
			ADD			= 0x6B,
			SEPARATOR	= 0x6C,
			SUBTRACT		= 0x6D,
			DECIMAL		= 0x6E,
			DIVIDE		= 0x6F,
			ATTN			= 0xF6,
			CRSEL		= 0xF7,
			EXSEL		= 0xF8,
			EREOF		= 0xF9,
			PLAY			= 0xFA,  
			ZOOM			= 0xFB,
			NONAME		= 0xFC,
			PA1			= 0xFD,
			OEM_CLEAR	= 0xFE,
			LWIN			= 0x5B,
			RWIN			= 0x5C,
			APPS			= 0x5D,   
			LSHIFT		= 0xA0,   
			RSHIFT		= 0xA1,   
			LCONTROL		= 0xA2,   
			RCONTROL		= 0xA3,   
			LMENU		= 0xA4,   
			RMENU		= 0xA5
		}
	}
}
