// RImage.h: RImage クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGE_H__03468318_34EF_4823_80BC_6A1B83DC0C15__INCLUDED_)
#define AFX_RIMAGE_H__03468318_34EF_4823_80BC_6A1B83DC0C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "comm.h"
#include "RException.h"

#ifdef _WIN32_WCE
		#include "RImgdecmp.h"

		class RImage
		{
		private:
		public:
			RImage();
			virtual ~RImage();

			void Release();

			HDC getDC() const
			{
				return this->MemoryDC;
			}
			int getWidth() const
			{
				return this->Width;
			}
			int getHeight() const
			{
				return this->Height;
			}

			void RImage::ChangeScale(int inWidth, int inHegiht);
			void Load(const char * inBuffer , int inSize) throw(RException);


		private:
			static DWORD CALLBACK RImage::GetImageData( LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam ) ;
			static void CALLBACK RImage::ImageProgress(    IImageRender* , BOOL ,  LPARAM  ) ;

			struct BufferControl
			{
				const char*		buffer;
				int				size;
				int				readOf;
			};

			RImgdecmp	Imgdecmp;

			int			Width , Height;
			HBITMAP		HBitmap;
			HDC			MemoryDC;

		};
#else
		#include <olectl.h>
		#include "RNotSupportException.h"
		#include "RWin32Exception.h"

		class RImage
		{
		private:
		public:
			RImage();
			virtual ~RImage();

			void Release();

			HDC getDC() const
			{
				return this->MemoryDC;
			}
			int getWidth() const
			{
				return this->Width;
			}
			int getHeight() const
			{
				return this->Height;
			}

			void RImage::ChangeScale(int inWidth, int inHegiht);
			void Load(const char * inBuffer , int inSize) throw(RException);
		private:
			int			Width , Height;
			HBITMAP		HBitmap;
			HDC			MemoryDC;
		};
#endif
#endif // !defined(AFX_RIMAGE_H__03468318_34EF_4823_80BC_6A1B83DC0C15__INCLUDED_)
