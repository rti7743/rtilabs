using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Diagnostics;

using UtilityLibrary.General;

namespace Zanetti.UI
{
    public enum DrawState {
        Normal,
        Disable,
        Hot,
        Pressed
    }
   
    internal class ButtonEx : UserControl {

		public enum BStyle {
			Normal,
			ShowTriangle,
			ButtonAndMenu
		}

		bool mouseDown = false;
		bool mouseEnter = false;
		private Image _image;

		public event EventHandler PopupMenu;

		private BStyle _style;
		private const int COMBOAREA_WIDTH = 12;
		
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
		public Image Image {
			get {
				return _image;
			}
			set {
				_image = value;
			}
		}
		public BStyle Style {
			get {
				return _style;
			}
			set {
				_style = value;
			}
		}


		public ButtonEx() {
			SetStyle(ControlStyles.AllPaintingInWmPaint|ControlStyles.UserPaint|ControlStyles.DoubleBuffer, true);
			_borderstyle = BorderStyle.None;
			Debug.Assert(!this.InvokeRequired);
		}

		//resets UI effect and invalidates itself. for example, OnMouseLeave is never called after drag & drop operations.
		public void Reset() {
			mouseDown = mouseEnter = false;
			this.Cursor = Cursors.Default;
			Debug.Assert(!this.InvokeRequired);
			Invalidate();
		}

		public int BodyWidth {
			get {
				int w = this.Width;
				if(_style==BStyle.ButtonAndMenu || _style==BStyle.ShowTriangle) w -= COMBOAREA_WIDTH;
				return w;
			}
		}
		public bool MousePointsTriangle(int x, int y) {
			return x>=BodyWidth && x<this.Width && y>=0 && y<this.Height;
		}

		protected override void OnPaint(PaintEventArgs pe)
		{
			try {
				base.OnPaint(pe);
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

		protected override void OnMouseEnter(EventArgs e)
		{
			try {
				base.OnMouseEnter(e);
				mouseEnter = true;
				this.Cursor = Cursors.Hand;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		
		}

		protected override void OnMouseLeave(EventArgs e)
		{
			try {
				mouseEnter = false;
				base.OnMouseLeave(e);
				this.Cursor = Cursors.Default;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected override void OnMouseDown(MouseEventArgs e)
		{
			try {
				base.OnMouseDown(e);
				mouseDown = true;
				if(_style==BStyle.ShowTriangle || (_style==BStyle.ButtonAndMenu && MousePointsTriangle(e.X, e.Y)))
					if(PopupMenu!=null) PopupMenu(this, e);
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			try {
				base.OnMouseUp(e);
				mouseDown = false;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}
		protected override void OnGotFocus(EventArgs e)
		{
			try {
				base.OnGotFocus(e);
				//gotFocus = true;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}
        
		protected override void OnLostFocus(EventArgs e)
		{
			try {
				base.OnLostFocus(e);
				//gotFocus = false;
				Invalidate();
			}
			catch(Exception ex) {
				Debug.WriteLine(ex.StackTrace);
				Debugger.Break();
			}
		}

		protected void DrawButtonState(Graphics g, DrawState state)
		{
			DrawBackground(g, state);
			Rectangle rc = ClientRectangle;
			
			bool hasText = false;
			bool hasImage = Image != null;
			Size textSize = new Size(0,0);
			if ( Text != string.Empty && Text != "" )
			{
				hasText = true;
				textSize = TextUtil.GetTextSize(g, Text, Font);
			}
			
			int imageWidth = 0;
			int imageHeight = 0;
			if ( hasImage )
			{
				SizeF sizeF = Image.PhysicalDimension;
				imageWidth = (int)sizeF.Width;
				imageHeight = (int)sizeF.Height;
				// We are assuming that the button image is smaller than
				// the button itself
				if ( imageWidth > rc.Width || imageHeight > rc.Height)
				{
					//Debug.WriteLine("Image dimensions need to be smaller that button's dimension...");
					return;
				}
			}

			int x, y;
			if ( hasText && !hasImage )
			{
				// Text only drawing
				x = (BodyWidth - textSize.Width)/2;
				y = (Height - textSize.Height)/2;
				DrawText(g, Text, state, x, y);
			}
			else if ( hasImage && !hasText )
			{
				// Image only drawing
				x = (BodyWidth - imageWidth)/2;
				y = (Height - imageHeight)/2;
				DrawImage(g, state, Image, x, y);
			}
			else if(hasImage && hasText)
			{
				// Text and Image drawing
				x = 1; //aligned to left
				//x = (Width - textSize.Width - imageWidth -2)/2; //original: center aligned
				y = (Height - imageHeight)/2;
				DrawImage(g, state, Image, x, y);
				x += imageWidth + 2;
				y = (Height - textSize.Height)/2;
				DrawText(g, Text, state, x, y);
			}

			if(_style==BStyle.ShowTriangle || _style==BStyle.ButtonAndMenu) {
				DrawComboStyleTriangle(g, state);
			}
			
		}

		protected void DrawBackground(Graphics g, DrawState state)
		{
			Rectangle rc = ClientRectangle;
			// Draw background
			if ( state == DrawState.Normal || state == DrawState.Disable )
			{
				g.FillRectangle(new SolidBrush(this.BackColor), rc);
				if(_borderstyle!=BorderStyle.None) {
					Pen pen;
					if ( state == DrawState.Disable )
						pen = SystemPens.ControlDark;
					else
						pen = SystemPens.ControlDarkDark;
				
					// Draw border rectangle
					g.DrawRectangle(pen, rc.Left, rc.Top, rc.Width-1, rc.Height-1);
				}
			}
			else if ( state == DrawState.Hot || state == DrawState.Pressed  )
			{
				// Erase whaterver that was there before
				if ( state == DrawState.Hot )
					g.FillRectangle(new SolidBrush(ColorUtil.VSNetSelectionColor), rc);
				else
					g.FillRectangle(new SolidBrush(ColorUtil.VSNetPressedColor), rc);
				// Draw border rectangle
				g.DrawRectangle(SystemPens.Highlight, rc.Left, rc.Top, rc.Width-1, rc.Height-1);
				if(_style==BStyle.ButtonAndMenu)
					g.DrawLine(SystemPens.Highlight, rc.Right-COMBOAREA_WIDTH, rc.Top, rc.Right-COMBOAREA_WIDTH, rc.Height);
			}
		}

		protected void DrawImage(Graphics g, DrawState state, Image image, int x, int y)
		{
			SizeF sizeF = Image.PhysicalDimension;
			int imageWidth = (int)sizeF.Width;
			int imageHeight = (int)sizeF.Height;
			
			if ( state == DrawState.Normal )
			{
				g.DrawImage(Image, x, y, imageWidth, imageHeight);
			}
			else if ( state == DrawState.Disable )
			{
				ControlPaint.DrawImageDisabled(g, Image, x, y, SystemColors.Control);
			}
			else if ( state == DrawState.Pressed || state == DrawState.Hot )
			{
				ControlPaint.DrawImageDisabled(g, Image, x+1, y, SystemColors.Control);
				g.DrawImage(Image, x-1, y-1, imageWidth, imageHeight);                 
			}
		}

		protected void DrawText(Graphics g, string Text, DrawState state, int x, int y)
		{
			if ( state == DrawState.Disable )
				g.DrawString(Text, Font, SystemBrushes.ControlDark, new Point(x, y));
			else {
                /*
				if(_headText!=null) {
					g.DrawString(_headText, Font, SystemBrushes.ControlDark, new Point(x, y));
					x+=11; //Should it be configurable?
				}
                */
				g.DrawString(Text, Font, SystemBrushes.ControlText, new Point(x, y));
			}
		}

		private void DrawComboStyleTriangle(Graphics g, DrawState state) {
			Pen p = state==DrawState.Disable? SystemPens.ControlDark : SystemPens.ControlText;
			int x = this.Width - COMBOAREA_WIDTH + 2;
			int y = this.Height / 2;
			g.DrawLine(p, x,   y-1, x+5, y-1);
			g.DrawLine(p, x+1, y  , x+4, y  );
			g.DrawLine(p, x+2, y+1, x+3, y+1);

		}

	}

}
