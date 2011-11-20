using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Diagnostics;
using UtilityLibrary.General;

namespace Zanetti.UI {
	internal class ToggleButtonEx : UserControl {

		bool mouseDown = false;
		bool mouseEnter = false;
		bool _checked = false;
		bool _autoToggle = false;

		private Image _image;
		private bool _showComboStyle;
		private const int COMBOAREA_WIDTH = 8;

		private BorderStyle _borderstyle;
        /*
		public BorderStyle BorderStyle {
			get {
				return _borderstyle;
			}
			set {
				_borderstyle = value;
			}
		}
         */
		public bool Checked {
			get {
				return _checked;
			}
			set {
				_checked = value;
			}
		}
		public bool AutoToggle {
			get {
				return _autoToggle;
			}
			set {
				_autoToggle = value;
			}
		}
		public Image Image {
			get {
				return _image;
			}
			set {
				_image = value;
			}
		}
		public bool ShowComboStyle {
			get {
				return _showComboStyle;
			}
			set {
				_showComboStyle = value;
			}
		}
		public int BodyWidth {
			get {
				int w = this.Width;
				if(_showComboStyle) w -= COMBOAREA_WIDTH;
				return w;
			}
		}

		public ToggleButtonEx() {
			SetStyle(ControlStyles.AllPaintingInWmPaint|ControlStyles.UserPaint|ControlStyles.DoubleBuffer, true);
			_borderstyle = BorderStyle.None;
			_checked = false;
		}

		protected override void OnPaint(PaintEventArgs pe) {
			base.OnPaint(pe);

			try {
				Graphics g = pe.Graphics;
				if ( mouseDown ) {
					DrawButtonState(g, DrawState.Pressed);
					return;
				}

				if (mouseEnter) {
					DrawButtonState(g, DrawState.Hot);
					return;
				}
			
				if ( Enabled )
					DrawButtonState(pe.Graphics, DrawState.Normal);
				else
					DrawButtonState(pe.Graphics, DrawState.Disable);
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected override void OnMouseEnter(EventArgs e) {
			try {
				base.OnMouseEnter(e);
				mouseEnter = true;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		
		}

		protected override void OnMouseLeave(EventArgs e) {
			base.OnMouseLeave(e);
			try {
			mouseEnter = false;
			Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected override void OnMouseDown(MouseEventArgs e) {
			base.OnMouseDown(e);
			try {
			mouseDown = true;
			Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e) {
			base.OnMouseUp(e);
			try {
			bool newvalue = !_checked;
			mouseDown = false;
			if(_autoToggle)
				_checked = newvalue;
			Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected void DrawButtonState(Graphics g, DrawState state) {
			DrawBackground(g, state);
			Rectangle rc = ClientRectangle;
			
			bool hasText = false;
			bool hasImage = Image != null;
			Size textSize = new Size(0,0);
			if ( Text != string.Empty && Text != "" ) {
				hasText = true;
				textSize = TextUtil.GetTextSize(g, Text, Font);
			}
			
			int imageWidth = 0;
			int imageHeight = 0;
			if ( hasImage ) {
				SizeF sizeF = Image.PhysicalDimension;
				imageWidth = (int)sizeF.Width;
				imageHeight = (int)sizeF.Height;
				// We are assuming that the button image is smaller than
				// the button itself
				if ( imageWidth > rc.Width || imageHeight > rc.Height) {
					//Debug.WriteLine("Image dimensions need to be smaller that button's dimension...");
					return;
				}
			}

			int x, y;
			if ( hasText && !hasImage ) {
				// Text only drawing
				x = (BodyWidth - textSize.Width)/2;
				y = (Height - textSize.Height)/2;
				DrawText(g, Text, state, x, y);
			}
			else if ( hasImage && !hasText ) {
				// Image only drawing
				x = (BodyWidth - imageWidth)/2;
				y = (Height - imageHeight)/2;
				DrawImage(g, state, Image, x, y);
			}
			else {
				// Text and Image drawing
				x = (BodyWidth - textSize.Width - imageWidth -2)/2;
				y = (Height - imageHeight)/2;
				DrawImage(g, state, Image, x, y);
				x += imageWidth + 2;
				y = (Height - textSize.Height)/2;
				DrawText(g, Text, state, x, y);
			}
			
			if(_showComboStyle) {
				DrawComboStyleTriangle(g, state);
			}
		}

		protected void DrawBackground(Graphics g, DrawState state) {
			Rectangle rc = ClientRectangle;
			// Draw background
			if ( state == DrawState.Normal || state == DrawState.Disable ) {
				
				g.FillRectangle(new SolidBrush(_checked? SystemColors.ControlLight : SystemColors.Control), rc);

				if(_checked) {
					ControlPaint.DrawBorder3D(g, rc, Border3DStyle.Sunken);
				}
				else if(_borderstyle!=BorderStyle.None) {
					SolidBrush rcBrush;
					if ( state == DrawState.Disable )
						rcBrush = new SolidBrush(SystemColors.ControlDark);
					else
						rcBrush = new SolidBrush(SystemColors.ControlDarkDark);
				
					// Draw border rectangle
					g.DrawRectangle(new Pen(rcBrush), rc.Left, rc.Top, rc.Width-1, rc.Height-1);
				}
			}
			else if ( state == DrawState.Hot || state == DrawState.Pressed  ) {
				// Erase whaterver that was there before
				if ( state == DrawState.Hot )
					g.FillRectangle(new SolidBrush(ColorUtil.VSNetSelectionColor), rc);
				else
					g.FillRectangle(new SolidBrush(ColorUtil.VSNetPressedColor), rc);
				// Draw border rectangle
				g.DrawRectangle(SystemPens.Highlight, rc.Left, rc.Top, rc.Width-1, rc.Height-1);
			}
		}

		protected void DrawImage(Graphics g, DrawState state, Image image, int x, int y) {
			SizeF sizeF = Image.PhysicalDimension;
			int imageWidth = (int)sizeF.Width;
			int imageHeight = (int)sizeF.Height;
			
			if(_checked) {
				x++; y++;
			}

			if ( state == DrawState.Normal ) {
				g.DrawImage(Image, x, y, imageWidth, imageHeight);
			}
			else if ( state == DrawState.Disable ) {
				ControlPaint.DrawImageDisabled(g, Image, x, y, SystemColors.Control);
			}
			else if ( state == DrawState.Pressed || state == DrawState.Hot ) {
				ControlPaint.DrawImageDisabled(g, Image, x+1, y, SystemColors.Control);
				g.DrawImage(Image, x-1, y-1, imageWidth, imageHeight);                 
			}
		}

		protected void DrawText(Graphics g, string Text, DrawState state, int x, int y) {
			if ( state == DrawState.Disable )
				g.DrawString(Text, Font, SystemBrushes.ControlDark, new Point(x, y));
			else
				g.DrawString(Text, Font, SystemBrushes.ControlText, new Point(x, y));
		}

		//!!‚±‚ê‚ÍButtonEx‚Æ“¯‚¶
		private void DrawComboStyleTriangle(Graphics g, DrawState state) {
			Pen p = state==DrawState.Disable? SystemPens.ControlDark : SystemPens.ControlText;
			int x = this.Width - COMBOAREA_WIDTH;
			int y = this.Height / 2;
			g.DrawLine(p, x,   y-1, x+5, y-1);
			g.DrawLine(p, x+1, y  , x+4, y  );
			g.DrawLine(p, x+2, y+1, x+3, y+1);

		}

	}

}
