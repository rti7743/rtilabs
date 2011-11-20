/*****************************************************************/
/*****************************************************************/
//ダイアログ
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef DIALOG_LIFE

#include <.\Base\rti_dialog.h>
#include <.\Base\rti_debug.h>

#include <shlobj.h>

/*****************************************************************/
//オープン
// hWnd オーナーウィンド or NULL
// Title タイトルバーに表示するヤツ. or NULL
// Path  開くパス or NULL
// Filter フィルター　ややこしいのでサンプルみてね(^^;;
// RetFullFilename MAX_PATH の配列を入れること. ここにファイル名がフルパスでくる
// FileTile        ディフォルトであるファイルタイトル or NULL
/*****************************************************************/
bool TDialog::Open(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile)
{
    OPENFILENAME ofn;
    if(!FileTile)
        ZeroMemory(RetFullFilename,MAX_PATH);
    else 
        lstrcpy(RetFullFilename,FileTile);

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = Filter;
    ofn.lpstrFile = RetFullFilename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = Title;
	ofn.lpstrInitialDir = Path;

    if (GetOpenFileName(&ofn)) {
		return true;
    }
	RetFullFilename[0] = NULL;
    return false;
}

/*****************************************************************/
//セーブ
/*****************************************************************/
bool TDialog::Save(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile)
{
    OPENFILENAME ofn;
    if(!FileTile)
        ZeroMemory(RetFullFilename,MAX_PATH);
    else 
        lstrcpy(RetFullFilename,FileTile);

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = Filter;
    ofn.lpstrFile = RetFullFilename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = Title;
	ofn.lpstrInitialDir = Path;

    if (GetSaveFileName(&ofn)) {
		return true;
    }
	RetFullFilename[0] = NULL;
    return false;
}

/*****************************************************************/
//パス選択
/*****************************************************************/
bool TDialog::Path(HWND hWnd,char *Title,char *RetPath)
{
    bool ret = false;
    IMalloc *m;
    if(SUCCEEDED(SHGetMalloc(&m)))
    {
        BROWSEINFO b;

        b.hwndOwner = hWnd;
        b.pidlRoot = NULL;
        b.pszDisplayName = RetPath;
        b.lpszTitle = Title;
        b.ulFlags = BIF_RETURNONLYFSDIRS; //フォルダのみ
        b.lpfn = NULL;
        b.lParam = 0;
        b.iImage = 0;
        LPITEMIDLIST id = SHBrowseForFolder(&b); //ダイアログの表示
        if(id)
        {
            SHGetPathFromIDList(id, RetPath); //パス名を得る

            if (RetPath[lstrlen(RetPath)-1]!='\\') 
                           strcat(RetPath,"\\"); /*最後は \\*/
            m->Free(id);
            ret = true;
        }
        m->Release();
    }
    return ret;
}
z
#endif //LIFE END
