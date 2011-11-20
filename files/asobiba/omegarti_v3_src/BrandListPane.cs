/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;
using Zanetti.Data;
using Zanetti.Commands;

namespace Zanetti.UI
{
	//銘柄リストを表示できるコントロールの基底クラス。
	internal abstract class BrandListPane : UserControl
	{

		public abstract AbstractBrand NextBrand { get; }
		public abstract AbstractBrand PrevBrand { get; }

		protected override void OnGotFocus(EventArgs e) {
			base.OnGotFocus (e);

			Env.Frame.CurrentBrandListPane = this;
		}
		protected override void OnVisibleChanged(EventArgs e) {
			base.OnVisibleChanged (e);
			if(this.Visible)
				Env.Frame.CurrentBrandListPane = this;
		}
		
	}
}
