/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;
using System.Drawing;

using Travis.Util;

namespace Zanetti.UI
{
	//グラフィックに必要なクラス群

	internal abstract class ZGDIObject : IDisposable {
		protected Color _color;
		protected IntPtr _handle;

		public IntPtr Handle {
			get {
				if(_handle==IntPtr.Zero) CreateObject();
				return _handle;
			}
		}
		public Color Color {
			get {
				return _color;
			}
		}
		public virtual void Dispose() {
			if(_handle!=IntPtr.Zero) Win32.DeleteObject(_handle);
			_handle = IntPtr.Zero;
		}

		protected abstract void CreateObject();
	}

	internal class ZPen : ZGDIObject {
		/*
#define PS_SOLID            0
#define PS_DASH             1       /* -------  
#define PS_DOT              2       /* .......  
#define PS_DASHDOT          3       /* _._._._  
*/
		public const int PS_SOLID   = 0;
		public const int PS_DASH    = 1;
		public const int PS_DOT     = 2;
		public const int PS_DASHDOT = 3;

		protected PenStyle _style;
		
		public ZPen(Color col, PenStyle style) {
			_color = col;
			_style = style;
		}
		protected override void CreateObject() {
			_handle = Win32.CreatePen(_style==PenStyle.Dotted? PS_DOT : PS_SOLID, _style==PenStyle.Bold? 2 : 1, Util.ToCOLORREF(_color));
		}

		public void Update(Color col, PenStyle style) {
			Dispose();
			_color = col;
			_style = style;
		}
		public PenStyle Style {
			get {
				return _style;
			}
		}

		[EnumDesc(typeof(PenStyle))]
			public enum PenStyle {
			[EnumValue(Description="実線")] Normal,
			[EnumValue(Description="点線")] Dotted,
			[EnumValue(Description="太線")] Bold
		}
	}

	internal class ZBrush : ZGDIObject {
		private IntPtr _lightPen;
		private IntPtr _darkPen;

		public ZBrush(Color col) {
			_color = col;
		}

		public IntPtr LightPen {
			get {
				if(_lightPen==IntPtr.Zero) CreateObject();
				return _lightPen;
			}
		}
		public IntPtr DarkPen {
			get {
				if(_darkPen==IntPtr.Zero) CreateObject();
				return _darkPen;
			}
		}

		protected override void CreateObject() {
			_handle = Win32.CreateSolidBrush(Util.ToCOLORREF(_color));
			_lightPen = Win32.CreatePen(0, 1, Util.ToCOLORREF(Util.ToLightColor(_color)));
			_darkPen  = Win32.CreatePen(0, 1, Util.ToCOLORREF(Util.ToDarkColor(_color)));
		}
		public void Update(Color col) {
			Dispose();
			_color = col;
		}

		public override void Dispose() {
			if(_lightPen!=IntPtr.Zero) Win32.DeleteObject(_lightPen);
			_lightPen = IntPtr.Zero;
			if(_darkPen!=IntPtr.Zero)  Win32.DeleteObject(_darkPen);
			_darkPen = IntPtr.Zero;
			base.Dispose();
		}
	}

	internal class ZCandlePen : ZGDIObject {
		private IntPtr _lightPen;
		private IntPtr _darkPen;
		private IntPtr _darkDarkPen;
		private IntPtr _brush;
		private Color _backColor;

		public ZCandlePen(Color col, Color back) {
			_color = col;
			_backColor = back;
		}

		protected override void CreateObject() {
			_handle = Win32.CreatePen(0, 1, Util.ToCOLORREF(_color));
			_brush = Win32.CreateSolidBrush(Util.ToCOLORREF(_color));
			_lightPen = Win32.CreatePen(0, 1, Util.ToCOLORREF(Util.MergeColor(_color, _backColor, 0.75)));
			_darkPen = Win32.CreatePen(0, 1, Util.ToCOLORREF(Util.MergeColor(_color, _backColor, 0.5)));
			_darkDarkPen = Win32.CreatePen(0, 1, Util.ToCOLORREF(Util.MergeColor(_color, _backColor, 0.375)));
		}
		public void Update(Color col, Color back) {
			Dispose();
			_color = col;
			_backColor = back;
		}

		public override void Dispose() {
			if(_lightPen!=IntPtr.Zero) Win32.DeleteObject(_lightPen);
			_lightPen = IntPtr.Zero;
			if(_darkPen!=IntPtr.Zero)  Win32.DeleteObject(_darkPen);
			_darkPen = IntPtr.Zero;
			if(_darkDarkPen!=IntPtr.Zero)  Win32.DeleteObject(_darkDarkPen);
			_darkDarkPen = IntPtr.Zero;
			if(_brush!=IntPtr.Zero) Win32.DeleteObject(_brush);
			base.Dispose();
		}

		public IntPtr LightPen {
			get {
				return _lightPen;
			}
		}
		public IntPtr DarkPen {
			get {
				return _darkPen;
			}
		}
		public IntPtr DarkDarkPen {
			get {
				return _darkDarkPen;
			}
		}
		public IntPtr Brush {
			get {
				return _brush;
			}
		}
	}
}
