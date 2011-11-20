// SolvableFileManager.h: SolvableFileManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILEMANAGER_H__546C69E0_3084_4FEC_8196_92ACADB73A77__INCLUDED_)
#define AFX_SOLVABLEFILEMANAGER_H__546C69E0_3084_4FEC_8196_92ACADB73A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class SolvablePathManager;

#include "comm.h"


#include "SolvableFile.h"
#include "SolvableFileText.h"
#include "SolvableFileBitmap.h"
#include "SusiePluginManager.h"
#include "SolvableFileLZH.h"
#include "SolvableFileMedia.h"
#include "SolvableFilePDF.h"
#include "SolvableFileNiseOLE.h"
#include "Rfiles.h"
#include "VirtualPath.h"

class SolvableFileManager  
{
public:
	SolvableFileManager()
	{
		SolvableFileMapping["TXT"] = & this->NiseOLE;
		SolvableFileMapping["DOC"] = & this->NiseOLE;
		SolvableFileMapping["XLS"] = & this->NiseOLE;
		SolvableFileMapping["EXE"] = & this->NiseOLE;

//		SolvableFileMapping["TXT"] = & this->Text;
		SolvableFileMapping["INI"] = & this->Text;

		SolvableFileMapping["BMP"] = & this->Bitmap;
		SolvableFileMapping["JPG"] = & this->Bitmap;
		SolvableFileMapping["GIF"] = & this->Bitmap;
		SolvableFileMapping["PNG"] = & this->Bitmap;

		SolvableFileMapping["SWF"] = & this->Media;

		SolvableFileMapping["MID"] = & this->Media;
		SolvableFileMapping["S3M"] = & this->Media;
		SolvableFileMapping["XM"] = & this->Media;
		SolvableFileMapping["IT"] = & this->Media;
		SolvableFileMapping["RMI"] = & this->Media;
		SolvableFileMapping["SGT"] = & this->Media;
		SolvableFileMapping["WAV"] = & this->Media;
		SolvableFileMapping["MP2"] = & this->Media;
		SolvableFileMapping["MP3"] = & this->Media;
		SolvableFileMapping["OGG"] = & this->Media;
		SolvableFileMapping["WMA"] = & this->Media;
		SolvableFileMapping["ASF"] = & this->Media;
		SolvableFileMapping["AVI"] = & this->Media;
		SolvableFileMapping["MPG"] = & this->Media;
		SolvableFileMapping["RA"] = & this->Media;
		SolvableFileMapping["RM"] = & this->Media;

		SolvableFileMapping["PDF"] = & this->PDF;

		SolvableFileMapping["LZH"] = & this->Lzh;


		//SUSIE PLUGIN
		SusiePluginManager * susiePluginManager = SusiePluginManager::getInstance();
		SUSIE_EXT extes = susiePluginManager->getSupportExt();
		for(SUSIE_EXT::iterator ext = extes.begin() ; ext != extes.end() ; ++ext)
		{
			SolvableFileMapping[*ext] = &this->Bitmap;
		}
	}
	virtual ~SolvableFileManager()
	{
	}

public:
	//作成 singleton
	static SolvableFileManager* getInstance()
	{
		static SolvableFileManager spm;
		return &spm;
	}

	//解決してほすぃ.
	//解決できる人が見つかったら、そいつのポインタ
	//誰も解決できなかったら NULL
	SolvableFile*	DoSolvable(string inPath)
	{
		//拡張子の取得.
		string ext = Rfiles::Filename2Ext( inPath );
		if ( ! ext.empty() )
		{
			SolvableFile* p = SolvableFileMapping[ext];
			if (p != NULL)
			{
				return p->PleaseSolve( inPath );
			}
		}

		//ネストしているかもしれないので調べる.
		VirtualPath		vp(inPath);
		if (vp.getPathDilimiterCount() == 0) return NULL;	//お手上げ
		string newPath = vp.getNativePath();

		ext = Rfiles::Filename2Ext( newPath );
		if ( ! ext.empty() )
		{
			SolvableFile* p = SolvableFileMapping[ext];
			if (p != NULL)
			{
				return p->PleaseSolve( inPath );
			}
		}

		return NULL;
	}

private:
	typedef map<string , SolvableFile*>	SOLVABLEFILEMAPPING;
	SOLVABLEFILEMAPPING SolvableFileMapping;

	SolvableFileText	Text;
	SolvableFileBitmap	Bitmap;
	SolvableFileMedia	Media;
	SolvableFilePDF		PDF;
	SolvableFileNiseOLE	NiseOLE;

	SolvableFileLZH		Lzh;

};

#endif // !defined(AFX_SOLVABLEFILEMANAGER_H__546C69E0_3084_4FEC_8196_92ACADB73A77__INCLUDED_)
