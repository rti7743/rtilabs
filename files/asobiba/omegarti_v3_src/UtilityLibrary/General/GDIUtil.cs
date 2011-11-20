using System;
using System.Drawing;
using UtilityLibrary.Win32;

namespace UtilityLibrary.General
{
	/// <summary>
	/// Summary description for GDIUtil.
	/// </summary>
	public class GDIUtil
	{
		// We won't instatiate any object
		private GDIUtil()
		{
			
		}

		static  public void Draw3DRect(Graphics g, Rectangle rc, Color topLeft, Color bottomRight)
		{
			Draw3DRect(g, rc.Left, rc.Top, rc.Width, rc.Height,  topLeft, bottomRight);

		}

		static  public void Draw3DRect(Graphics g, int x, int y, int width, int height, Color topLeft, Color bottomRight)
		{
			g.FillRectangle(new SolidBrush(topLeft), x, y, width - 1, 1);
			g.FillRectangle(new SolidBrush(topLeft), x, y, 1, height - 1);
			g.FillRectangle(new SolidBrush(bottomRight), x + width, y, -1, height);
			g.FillRectangle(new SolidBrush(bottomRight), x, y + height, width, -1);
		}

		static public void StrechBitmap(Graphics gDest, Rectangle rcDest, Bitmap bitmap)
		{

			// Draw From bitmap
			IntPtr hDCTo = gDest.GetHdc();
			WindowsAPI.SetStretchBltMode(hDCTo, (int)StrechModeFlags.COLORONCOLOR);
			IntPtr hDCFrom = WindowsAPI.CreateCompatibleDC(hDCTo);
					
			IntPtr hOldFromBitmap = WindowsAPI.SelectObject(hDCFrom, bitmap.GetHbitmap());
			WindowsAPI.StretchBlt(hDCTo, rcDest.Left , rcDest.Top, rcDest.Width, rcDest.Height, hDCFrom, 
				0 , 0, bitmap.Width, bitmap.Height, (int)PatBltTypes.SRCCOPY);
                
			// Cleanup
			WindowsAPI.SelectObject(hDCFrom, hOldFromBitmap);
			gDest.ReleaseHdc(hDCTo);

		}

		static public Bitmap GetStrechedBitmap(Graphics gDest, Rectangle rcDest, Bitmap bitmap)
		{

			// Draw To bitmap
			Bitmap newBitmap = new Bitmap(rcDest.Width, rcDest.Height);
			Graphics gBitmap = Graphics.FromImage(newBitmap); 
			IntPtr hDCTo = gBitmap.GetHdc();
			WindowsAPI.SetStretchBltMode(hDCTo, (int)StrechModeFlags.COLORONCOLOR);
			IntPtr hDCFrom = WindowsAPI.CreateCompatibleDC(hDCTo);
									
			IntPtr hOldFromBitmap = WindowsAPI.SelectObject(hDCFrom, bitmap.GetHbitmap());
			WindowsAPI.StretchBlt(hDCTo, rcDest.Left , rcDest.Top, rcDest.Width, rcDest.Height, hDCFrom, 
				0 , 0, bitmap.Width, bitmap.Height, (int)PatBltTypes.SRCCOPY);
                
			// Cleanup
			WindowsAPI.SelectObject(hDCFrom, hOldFromBitmap);
			gBitmap.ReleaseHdc(hDCTo);

			return newBitmap;

		}


		static public Bitmap GetTileBitmap(Rectangle rcDest, Bitmap bitmap)
		{

			Bitmap tiledBitmap = new Bitmap(rcDest.Width, rcDest.Height);
			using ( Graphics g = Graphics.FromImage(tiledBitmap) )
			{
				for ( int i = 0; i < tiledBitmap.Width; i += bitmap.Width )
				{
					for ( int j = 0; j < tiledBitmap.Height; j += bitmap.Height )
					{
						g.DrawImage(bitmap, new Point(i, j));					

					}
				}
			}
			return tiledBitmap;
		}
		


	}
}
