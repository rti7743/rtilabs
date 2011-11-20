// RSetupLoad.cpp: RSetupLoad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"

#include "regex.h"

#include "RSetupLoad.h"
#include "RConv.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RSetupLoad::RSetupLoad()
{

}

RSetupLoad::~RSetupLoad()
{
	for (OneMenu::iterator i = Menus.begin() ; i != Menus.end() ; ++i)	delete (*i).second;

}

void RSetupLoad::Load(string inSetupFilename,string inSection) 
			throw(RIOCannotOpenException,RIOCannotReadException,RParseException)
{
	RStdioFile	StdFile;
	StdFile.Open(inSetupFilename ,"rb");

	string buf;

	//セクションシーク.
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':' && buf.substr(1) == inSection) break;
	}

	//キー取得.
	boost::reg_expression<char> keyRegex  = "KEY\\s*([A-Z]+)\\s*([0-9]+)\\s*(.*)$";
	//メニュー
	boost::reg_expression<char> menuRegex  = "MENU\\s*([^\\s]+)\\s*([^\\s]+)\\s*(.*)$";

	//キー
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':') break;		//別のセクションにはいったので、そのキーはないということで.

		if (buf.size() >= 5)
		{
			if ( buf.substr(0,3) == "KEY")
			{
				//キー定義.
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, keyRegex) ) 
				{
					//追加 / 挿入
					KeyDowns.SetKey( ToKey(results.str(1)), (MODIFY_KEY) atoi(results.str(2).c_str()) , results.str(3) );
				}
			}
			if (buf.substr(0,4) == "MENU")
			{
				//メニュー定義.
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, menuRegex) ) 
				{
					//メニューの管理機構を追加 / 挿入
					//管理機構の中で目的のメニューを検索.
					MenuSentence* menu;
					OneMenu::iterator j =  Menus.find( results.str(1) );
					if (j == Menus.end() )
					{
						menu = new MenuSentence;
						Menus[results.str(1)] = menu;
					}
					else
					{
						menu = ((*j).second);
					}
					//目的のメニューに項目を追加.
					string script = RConv::ltrim(results.str(3));
					if (script[0] == '@' )
					{//入れ子
						OneMenu::iterator k =  Menus.find( script.substr(1) );
						if (k == Menus.end() )
						{
							throw RParseException("ネストさせようしている %s というメニューは定義されていません",script.c_str() );
						}
						menu->CreateIncludeMenu(results.str(2) , ((*k).second)->getMenu() );
					}
					else
					{//普通.
						menu->CreateNode(results.str(2) , script );
					}
				}
			}
		}
	}

}


//キーコードの変換
unsigned char RSetupLoad::ToKey(string inKeyname) const
{
	if (inKeyname.size() == 1)
	{
		return inKeyname[0];
	}
	if (inKeyname == "RETURN" || inKeyname == "RET" )	return VK_RETURN;
	if (inKeyname == "TAB" )	return VK_TAB;
	if (inKeyname == "BACK" )	return VK_BACK;
	if (inKeyname == "ESCAPE"  || inKeyname == "ESC"  )	return VK_ESCAPE;
	if (inKeyname == "INSERT"  || inKeyname == "INS"  )	return VK_INSERT;
	if (inKeyname == "DELETE"  || inKeyname == "DEL"  )	return VK_DELETE;
	if (inKeyname == "SPACE" )	return VK_SPACE;
	if (inKeyname == "UP" )	return VK_UP;
	if (inKeyname == "RIGHT" )	return VK_RIGHT;
	if (inKeyname == "DOWN" )	return VK_DOWN;
	if (inKeyname == "LEFT" )	return VK_LEFT;
	if (inKeyname == "SPACE" )	return VK_SPACE;
	if (inKeyname == "END" )	return VK_END;
	if (inKeyname == "HOME" )	return VK_HOME;
	if (inKeyname == "LBUTTON" )	return VK_LBUTTON;
	if (inKeyname == "RBUTTON" )	return VK_RBUTTON;
	if (inKeyname == "MBUTTON" )	return VK_MBUTTON;

	if (inKeyname == "F1" )	return VK_F1;
	if (inKeyname == "F2" )	return VK_F2;
	if (inKeyname == "F3" )	return VK_F3;
	if (inKeyname == "F4" )	return VK_F4;
	if (inKeyname == "F5" )	return VK_F5;
	if (inKeyname == "F6" )	return VK_F6;
	if (inKeyname == "F7" )	return VK_F7;
	if (inKeyname == "F8" )	return VK_F8;
	if (inKeyname == "F9" )	return VK_F9;
	if (inKeyname == "F10" )	return VK_F10;
	if (inKeyname == "F11" )	return VK_F11;
	if (inKeyname == "F12" )	return VK_F12;

	return VK_SPACE;
}



//キーダウン.
bool RSetupLoad::KeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey) const throw (RParseException)
{
	return this->KeyDowns.OnKeyDown(inKeyCode , inModifyKey) ;
}

//キーダウン.
bool RSetupLoad::KeyDown(unsigned char inKeyCode ) const throw (RParseException)
{
	int modifyKey = 0;
	if (GetKeyState(VK_SHIFT) & 0x8000)		modifyKey &= MODIFY_KEY_SHIFT;
	if (GetKeyState(VK_CONTROL) & 0x8000)	modifyKey &= MODIFY_KEY_CTRL;
	if (GetKeyState(VK_MENU) & 0x8000)		modifyKey &= MODIFY_KEY_ALT;	//どうして MENU が ALT なんの? 参考 http://www.experts-exchange.com/Programming/Programming_Languages/MFC/Q_10271543.html

	return this->KeyDowns.OnKeyDown(inKeyCode , (MODIFY_KEY)modifyKey) ;
}


//メニューを出す.
string RSetupLoad::Popup(const string inMenuName, POINT inScreenPoint , HWND inOwner) const throw (RParseException,RNotFoundException,RWin32Exception)
{
	OneMenu::const_iterator i = this->Menus.find(inMenuName);
	if (i == this->Menus.end() )	
	{
		throw RNotFoundException("指定された %s という名前のメニューはありません\n", inMenuName.c_str() );
	}
	const MenuSentence* menuSentence = (*i).second;
	
	//メニューをポップアップ.
	HMENU menu = menuSentence->getMenu();
	DWORD ret = TrackPopupMenu( menu , TPM_LEFTALIGN|TPM_LEFTBUTTON | TPM_RETURNCMD,
							inScreenPoint.x , inScreenPoint.y , 0 , inOwner ,NULL);
	if (ret == 0)
	{
		throw RWin32Exception("ポップアップメニュー %s でエラーを検出しました." , inMenuName.c_str() );
	}
	return menuSentence->Run(ret);
}
