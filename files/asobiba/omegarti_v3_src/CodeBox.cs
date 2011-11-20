/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Windows.Forms;

namespace Zanetti.Forms
{
	/// <summary>
	/// CodeBox ÇÃäTóvÇÃê‡ñæÇ≈Ç∑ÅB
	/// </summary>
	internal class CodeBox : TextBox
	{
		public event EventHandler CodeComplete;
		private int _code;

		public int StockCode {
			get {
				return _code;
			}
			set {
				_code = value;
				this.Text = value.ToString();
				Select(1, 0);
			}
		}

		protected override bool IsInputChar(char ch) {
			return IsInterestingChar(ch);
		}
		protected override bool ProcessDialogChar(char ch) {
			if(IsInterestingChar(ch))
				return false;
			else
				return true;
		}
		private bool IsInterestingChar(char ch) {
			return ('0'<=ch && ch<='9') || (int)ch==8 || ch=='\n';
		}


		protected override void OnKeyPress(KeyPressEventArgs args) {
			base.OnKeyPress(args);
			char ch = args.KeyChar;
			if('0'<=ch && ch<='9') {
				if(this.Text.Length==3) {
					_code = Int32.Parse(this.Text + ch);
					if(CodeComplete!=null) CodeComplete(this, args);
				}
			}
			else if(ch=='\n') {
				_code = Int32.Parse(this.Text);
				if(CodeComplete!=null) CodeComplete(this, args);
			}
		}


	}
}
