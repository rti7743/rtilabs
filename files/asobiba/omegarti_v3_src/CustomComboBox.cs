/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;
using Zanetti.Commands;

namespace Zanetti.UI
{
	internal class CustomComboBox : ComboBox
	{
		private bool _divertWheelEvent;

		public CustomComboBox() {
		}

		public bool DivertWheelEvent {
			get {
				return _divertWheelEvent;
			}
			set {
				_divertWheelEvent = value;
			}
		}
		protected override void WndProc(ref Message m) {
			if(m.Msg==Win32.WM_MOUSEWHEEL)
				Env.Command.ExecMouseWheel(m.WParam.ToInt32() >> 16);
			else
				base.WndProc (ref m);
		}



	
		/*
		protected override void OnDrawItem(DrawItemEventArgs e) {
			base.OnDrawItem (e);

			string t = (string)this.Items[e.Index];
			e.Graphics.FillRectangle(new SolidBrush(e.BackColor), e.Bounds);
			e.Graphics.DrawString(t, e.Font, new SolidBrush(e.ForeColor), e.Bounds.X, e.Bounds.Y);
		}
		*/
	}
}
