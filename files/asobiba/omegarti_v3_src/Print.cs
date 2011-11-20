/*
 * Copyright (c) Tasuku SUENAGA, Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;

using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Printing;
using System.Drawing.Imaging;

namespace Zanetti
{
	internal class PrintSupport {
		private Bitmap _printBitmap;
		
		public void ShowPrintDialog() {
			PrintDocument pd = new PrintDocument();
			pd.PrintPage += new PrintPageEventHandler(OnPrintPage);
			pd.DefaultPageSettings.Landscape = true;
			pd.DocumentName = "OmegaChart";
			PrintDialog pdlg = new PrintDialog();
			pdlg.Document = pd;
			_printBitmap = CaptureChart();
			if (pdlg.ShowDialog(Env.Frame) == DialogResult.OK){
				pd.Print();
			}
		}

		// Ç»ÇÒÇ©ëfíºÇ…forÇ≈x,yÉãÅ[ÉvÇ‹ÇÌÇµÇΩÇŸÇ§Ç™Ç¢Ç¢ÇÊÇ§Ç»
		private void OnPrintPage(object sender, PrintPageEventArgs args){

			// to grayscale
			ColorMatrix cm = new ColorMatrix();

			float[][] grayMatrix = new float[][]{
													new float[]{1/3f,1/3f,1/3f,0,0},
													new float[]{1/3f,1/3f,1/3f,0,0},
													new float[]{1/3f,1/3f,1/3f,0,0},
													new float[]{0,0,0,1,0},
													new float[]{0,0,0,0,1}
												};
			ColorMatrix grayColorMatrix = new ColorMatrix(grayMatrix);

			// background <- white , white <- black
			ColorMap[] cms =
				new ColorMap[] { 
					 new ColorMap(),
						new ColorMap(),
			};
			cms[0].OldColor = Color.White;
			cms[0].NewColor = Color.Black;
			cms[1].OldColor = Env.Preference.BackBrush.Color;
			cms[1].NewColor = Color.White;
			
			// gray level >= 5% then black
			ImageAttributes ia = new ImageAttributes();
			ia.SetRemapTable(cms);
			ia.SetColorMatrix(grayColorMatrix);
			ia.SetOutputChannel(ColorChannelFlag.ColorChannelK);
			ia.SetThreshold((float)0.95);
			
			Rectangle dest = args.PageBounds;
			dest.Inflate(-30, -30); //ó]îí
			args.Graphics.DrawImage(_printBitmap, dest, 0, 0, 
				_printBitmap.Width, _printBitmap.Height, GraphicsUnit.Pixel,ia);
		}
		
		Bitmap CaptureChart(){
			Graphics gs = Env.Frame.ChartCanvas.CreateGraphics();
			Size s = Env.Frame.ChartCanvas.Size;
			Bitmap bmp = new Bitmap(s.Width, s.Height, gs);
			Graphics gd = Graphics.FromImage(bmp);
			IntPtr dcs = gs.GetHdc();
			IntPtr dcd = gd.GetHdc();
			Win32.BitBlt(dcd, 0, 0, Env.Frame.ChartCanvas.Width, Env.Frame.ChartCanvas.Height,
				dcs, 0, 0, 13369376);
			gs.ReleaseHdc(dcs);
			gd.ReleaseHdc(dcd);
			
			return bmp;
		}
	}
}
