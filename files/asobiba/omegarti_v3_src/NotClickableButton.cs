/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Windows.Forms;

namespace Zanetti.UI
{
	/// <summary>
	/// NotClickableButton ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal class NotClickableButton : Button
	{
		protected override void WndProc(ref Message m) {
			int n = m.Msg;
			if(n!=(int)UtilityLibrary.Win32.Msg.WM_LBUTTONDOWN && n!=(int)UtilityLibrary.Win32.Msg.WM_LBUTTONUP && n!=(int)UtilityLibrary.Win32.Msg.WM_LBUTTONDBLCLK)
				base.WndProc (ref m);
		}



	}
}
