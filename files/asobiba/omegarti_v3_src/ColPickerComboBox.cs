/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Windows.Forms;
using System.Drawing;

using UtilityLibrary.WinControls;

namespace Zanetti.UI
{
	//XPのUIになじませるための新ColorPicker
	internal class ColPickerComboBox : ComboBox
	{
		private ColorPickerDropDown _dropdown;

		private Color _selectedColor;
		public event ColorPickerDropDown.NewColorEventHandler NewColor;
		
		public ColPickerComboBox() {
			this.DrawMode = DrawMode.OwnerDrawFixed;
			this.DropDownStyle = ComboBoxStyle.DropDownList;
			this.Items.Add("");
			_selectedColor = Color.White;
		}

		public Color Color {
			get {
				return _selectedColor;
			}
			set {
				Color t = Color.Empty;
				if(UtilityLibrary.General.ColorUtil.IsKnownColor(value, ref t, false))
					value = t;
				else if(UtilityLibrary.General.ColorUtil.IsSystemColor(value, ref t))
					value = t;
				_selectedColor = value;
			}
		}

		private void ShowDropDown() {
			// Create color picker dropdown portion
			if(_dropdown==null) {
				_dropdown = new ColorPickerDropDown();
				ColorChangeEventHandler eh = new ColorChangeEventHandler(ColorChanged);
				_dropdown.ColorChanged += eh;
			}
			_dropdown.CurrentColor = _selectedColor;

			// Show color picker dropdown control
			Point point = new Point(0,0);
			CalculateSafeDisplayPoint(_dropdown, ref point);
			_dropdown.DesktopBounds = new Rectangle(point.X, point.Y, _dropdown.ClientRectangle.Width, _dropdown.ClientRectangle.Height);
			_dropdown.Show();
		}
		private void CalculateSafeDisplayPoint(ColorPickerDropDown dd, ref Point point) {
			
			Rectangle rc = ClientRectangle;
			rc = RectangleToScreen(rc);
			point.X = rc.Right - dd.Width;
			point.Y = rc.Bottom+1;

			/*
			int screenWidth = Screen.PrimaryScreen.Bounds.Width;
			int screenHeight = Screen.PrimaryScreen.Bounds.Height;
						
			// Correct x coordinate if necessary
			if ( point.X < 0 )
				point.X = 0;
			else if ( point.X + dd.Width  > screenWidth ) 
				point.X = screenWidth - colorDropDown.Width;
            			
			// Correct y coordinate if necessary
			if ( point.Y < 0 ) 
				point.Y = 0;
			else if ( point.Y + dd.Height  > screenHeight )
				point.Y = rc.Top -1 - colorDropDown.Height;
			*/
		}

		private void ColorChanged(object sender, ColorChangeArgs ea) {
			_selectedColor = ea.NewColor;
			Invalidate(true);
			if(NewColor!=null) NewColor(this, new NewColorArgs(_selectedColor));
		}

		protected override void OnDrawItem(DrawItemEventArgs e) {
			Rectangle box = new Rectangle(e.Bounds.Left+2, e.Bounds.Top+2, 20, e.Bounds.Height-4);
			using(Brush br = new SolidBrush(_selectedColor)) {
				e.Graphics.FillRectangle(br, box);
			}
			using(Pen pen = new Pen(this.ForeColor)) {
				e.Graphics.DrawRectangle(pen, box);
			}
			if(!_selectedColor.IsEmpty) {
				using(Brush br = new SolidBrush(e.ForeColor)) {
					if(_selectedColor.Name!="0")
						e.Graphics.DrawString(_selectedColor.Name, e.Font, br, box.Right + 4, e.Bounds.Top);
				}
			}
		}

		protected override void WndProc(ref Message m) {
			if(m.Msg==(int)UtilityLibrary.Win32.Msg.WM_LBUTTONDOWN) //これを握りつぶして独自にドロップダウン
				ShowDropDown();
			else if(m.Msg!=(int)UtilityLibrary.Win32.Msg.WM_LBUTTONUP && m.Msg!=(int)UtilityLibrary.Win32.Msg.WM_LBUTTONDBLCLK)
				base.WndProc (ref m);
		}
	}

}
