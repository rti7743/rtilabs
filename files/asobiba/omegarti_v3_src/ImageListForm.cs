/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Zanetti.UI
{

	/// <summary>
	/// ImageListForm の概要の説明です。
	/// </summary>
	internal class ImageListForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ImageList _image16;
		private System.ComponentModel.IContainer components;


		public ImageListForm()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}

		public ImageList ImageList16 {
			get {
				return _image16;
			}
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(ImageListForm));
			this._image16 = new System.Windows.Forms.ImageList(this.components);
			// 
			// _image16
			// 
			this._image16.ImageSize = new System.Drawing.Size(16, 16);
			this._image16.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("_image16.ImageStream")));
			this._image16.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// ImageListForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(264, 246);
			this.Name = "ImageListForm";
			this.Text = "ImageListForm";

		}
		#endregion
	}
	internal class IconConst {
		//16ピクセルのアイコン用イメージ
		public const int DESKTOP = 0;
		public const int STAR = 1;
		public const int FOLDER = 2;
		public const int CHECK = 3;
		public const int COPY = 4;
		public const int NEWFOLDER = 5;
		public const int SEARCH = 6;
		public const int DOCUMENT = 7;
		public const int HELP = 8;
		public const int QUESTION = 9;
        public const int GRIDLOCK = 10;
        public const int SPLIT = 11;
        public const int UPDATE0 = 12;
		public const int UPDATE = 13;
		public const int VOLUME = 14;
		public const int OSCILLATOR1 = 15;
		public const int OSCILLATOR2 = 16;
		public const int OSCILLATOR3 = 17;
		public const int ACCUMULATIVE = 18;
		public const int INVERSE = 19;
		public const int LOG = 20;
		public const int CHARTFORMAT = 21;
		public const int CANDLEWIDTH = 22;
		public const int VALUEWINDOW = 23;
		public const int BACK = 24;
		public const int GO = 25;
	}
}
