# Microsoft Developer Studio Project File - Name="filer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=filer - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "filer.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "filer.mak" CFG="filer - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "filer - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "filer - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "filer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /Yu"comm.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Msvcrt.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "filer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /FR /Yu"comm.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Msvcrtd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "filer - Win32 Release"
# Name "filer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddressHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\AtlActiveXBrige.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\comm.cpp
# ADD CPP /Yc"comm.h"
# End Source File
# Begin Source File

SOURCE=.\DeleteObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayBitmapView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayBlank.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDebugPrintf.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayListViewBigIconWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayMediaView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayNiseOLE.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayPDFView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayWebBrowserView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\FileModifyChacheLargeWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\FileModifyChacheWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\Filer.cpp
# End Source File
# Begin Source File

SOURCE=.\filer.rc
# End Source File
# Begin Source File

SOURCE=.\FilerSub.cpp
# End Source File
# Begin Source File

SOURCE=.\FMod.cpp
# End Source File
# Begin Source File

SOURCE=.\FModCallable.cpp
# End Source File
# Begin Source File

SOURCE=.\FModPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\IMyEnumString.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSentence.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiString.cpp
# End Source File
# Begin Source File

SOURCE=.\NativeSupportPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\Owners.cpp
# End Source File
# Begin Source File

SOURCE=.\PathInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\RActiveXWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\RAddressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RAddressBarWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\RArchiveException.cpp
# End Source File
# Begin Source File

SOURCE=.\RArchiveFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RArchiveLha.cpp
# End Source File
# Begin Source File

SOURCE=.\RAutoLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.cpp
# End Source File
# Begin Source File

SOURCE=.\RCacheFS.cpp
# End Source File
# Begin Source File

SOURCE=.\RCommKeyDown.cpp
# End Source File
# Begin Source File

SOURCE=.\RConferException.cpp
# End Source File
# Begin Source File

SOURCE=.\RConv.cpp
# End Source File
# Begin Source File

SOURCE=.\RDataLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.cpp
# End Source File
# Begin Source File

SOURCE=.\REasyAI.cpp
# End Source File
# Begin Source File

SOURCE=.\ReferenceCount.cpp
# End Source File
# Begin Source File

SOURCE=.\REnum.cpp
# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\REvent.cpp
# End Source File
# Begin Source File

SOURCE=.\RException.cpp
# End Source File
# Begin Source File

SOURCE=.\RExpr.cpp
# End Source File
# Begin Source File

SOURCE=.\Rfile.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileArc.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileArcBigIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileLocalBigIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileLocalReport.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileNetResource.cpp
# End Source File
# Begin Source File

SOURCE=.\RfileNetResourceBigIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilePreviewOne.cpp
# End Source File
# Begin Source File

SOURCE=.\Rfiles.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesLzh.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesPreviewOne.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesResolve.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesSMBLogon.cpp
# End Source File
# Begin Source File

SOURCE=.\RfilesWithCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RFixedColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\RFixedColumnArray.cpp
# End Source File
# Begin Source File

SOURCE=.\RFlashPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\RFModException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFModView.cpp
# End Source File
# Begin Source File

SOURCE=.\RFocus.cpp
# End Source File
# Begin Source File

SOURCE=.\RFunctionScript.cpp
# End Source File
# Begin Source File

SOURCE=.\RImage.cpp
# End Source File
# Begin Source File

SOURCE=.\RImageButton.cpp
# End Source File
# Begin Source File

SOURCE=.\RImageFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\RImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\RImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\RInputDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RInputStream.cpp
# End Source File
# Begin Source File

SOURCE=.\RIOCannotCloseException.cpp
# End Source File
# Begin Source File

SOURCE=.\RIOCannotOpenException.cpp
# End Source File
# Begin Source File

SOURCE=.\RIOCannotReadException.cpp
# End Source File
# Begin Source File

SOURCE=.\RIOCannotWriteException.cpp
# End Source File
# Begin Source File

SOURCE=.\RIOException.cpp
# End Source File
# Begin Source File

SOURCE=.\RListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RListViewWithMouseGesture.cpp
# End Source File
# Begin Source File

SOURCE=.\RLock.cpp
# End Source File
# Begin Source File

SOURCE=.\RMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\RMediaPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\RMediaView.cpp
# End Source File
# Begin Source File

SOURCE=.\RMediaViewPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\RMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\RMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RMessageBoxWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RMouseGesture.cpp
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMakeZende.cpp
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMouseCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMousePoints.cpp
# End Source File
# Begin Source File

SOURCE=.\RNiseOle.cpp
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.cpp
# End Source File
# Begin Source File

SOURCE=.\RNormalWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\RNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\RNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\RNotSupportException.cpp
# End Source File
# Begin Source File

SOURCE=.\ROutOfRangeException.cpp
# End Source File
# Begin Source File

SOURCE=.\RParseException.cpp
# End Source File
# Begin Source File

SOURCE=.\RPDFView.cpp
# End Source File
# Begin Source File

SOURCE=.\RProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RRealPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\RRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RSetupLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\RSpliter.cpp
# End Source File
# Begin Source File

SOURCE=.\RStatue.cpp
# End Source File
# Begin Source File

SOURCE=.\RStatueControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RStatueWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RSusieConferException.cpp
# End Source File
# Begin Source File

SOURCE=.\RSyncFileCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\RTabBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RTabBarWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RTable.cpp
# End Source File
# Begin Source File

SOURCE=.\RTableControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RTableEasy.cpp
# End Source File
# Begin Source File

SOURCE=.\RTableMuchDuplication.cpp
# End Source File
# Begin Source File

SOURCE=.\RTableOnlyOne.cpp
# End Source File
# Begin Source File

SOURCE=.\RTask.cpp
# End Source File
# Begin Source File

SOURCE=.\RTaskLS.cpp
# End Source File
# Begin Source File

SOURCE=.\RTaskManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RTaskView.cpp
# End Source File
# Begin Source File

SOURCE=.\RTaskViewWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RThreadExecuteOnMainThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\RToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RToolBarControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RToolBarWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\RWebBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\RWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileLZH.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileNiseOLE.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFilePDF.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvableFileText.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePath.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSMB.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSMBLogon.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSpecialFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\SolvablePathWeb.cpp
# End Source File
# Begin Source File

SOURCE=.\StringList.cpp
# End Source File
# Begin Source File

SOURCE=.\SusieCallable.cpp
# End Source File
# Begin Source File

SOURCE=.\SusiePlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\SusiePluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SusiePluginWapper.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Window.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.cpp
# End Source File
# Begin Source File

SOURCE=.\WndProcCallable.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddressHistory.h
# End Source File
# Begin Source File

SOURCE=.\AtlActiveXBrige.h
# End Source File
# Begin Source File

SOURCE=.\BasePanel.h
# End Source File
# Begin Source File

SOURCE=.\Channel.h
# End Source File
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\DeleteObject.h
# End Source File
# Begin Source File

SOURCE=.\Display.h
# End Source File
# Begin Source File

SOURCE=.\DisplayBitmapView.h
# End Source File
# Begin Source File

SOURCE=.\DisplayBlank.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDebugPrintf.h
# End Source File
# Begin Source File

SOURCE=.\DisplayFactory.h
# End Source File
# Begin Source File

SOURCE=.\DisplayListViewBigIconWin32.h
# End Source File
# Begin Source File

SOURCE=.\DisplayMediaView.h
# End Source File
# Begin Source File

SOURCE=.\DisplayNiseOLE.h
# End Source File
# Begin Source File

SOURCE=.\DisplayPDFView.h
# End Source File
# Begin Source File

SOURCE=.\DisplayTextView.h
# End Source File
# Begin Source File

SOURCE=.\DisplayWebBrowserView.h
# End Source File
# Begin Source File

SOURCE=.\DisplayWin32.h
# End Source File
# Begin Source File

SOURCE=.\FileModifyChacheLargeWin32.h
# End Source File
# Begin Source File

SOURCE=.\FileModifyChacheWin32.h
# End Source File
# Begin Source File

SOURCE=.\Filer.h
# End Source File
# Begin Source File

SOURCE=.\FilerSub.h
# End Source File
# Begin Source File

SOURCE=.\FMod.h
# End Source File
# Begin Source File

SOURCE=.\fmod_define.h
# End Source File
# Begin Source File

SOURCE=.\FModCallable.h
# End Source File
# Begin Source File

SOURCE=.\FModPlay.h
# End Source File
# Begin Source File

SOURCE=.\IMyEnumString.h
# End Source File
# Begin Source File

SOURCE=.\MenuSentence.h
# End Source File
# Begin Source File

SOURCE=.\MultiString.h
# End Source File
# Begin Source File

SOURCE=.\NativeSupportPicture.h
# End Source File
# Begin Source File

SOURCE=.\Owners.h
# End Source File
# Begin Source File

SOURCE=.\PathInterface.h
# End Source File
# Begin Source File

SOURCE=.\RActiveXWindow.h
# End Source File
# Begin Source File

SOURCE=.\RAddressBar.h
# End Source File
# Begin Source File

SOURCE=.\RAddressBarWin32.h
# End Source File
# Begin Source File

SOURCE=.\RArchive.h
# End Source File
# Begin Source File

SOURCE=.\RArchiveException.h
# End Source File
# Begin Source File

SOURCE=.\RArchiveFactory.h
# End Source File
# Begin Source File

SOURCE=.\RArchiveLha.h
# End Source File
# Begin Source File

SOURCE=.\RAutoLibrary.h
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.h
# End Source File
# Begin Source File

SOURCE=.\RCacheFS.h
# End Source File
# Begin Source File

SOURCE=.\RCommKeyDown.h
# End Source File
# Begin Source File

SOURCE=.\RConferException.h
# End Source File
# Begin Source File

SOURCE=.\RConv.h
# End Source File
# Begin Source File

SOURCE=.\RDataLoad.h
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.h
# End Source File
# Begin Source File

SOURCE=.\REasyAI.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceCount.h
# End Source File
# Begin Source File

SOURCE=.\regex.h
# End Source File
# Begin Source File

SOURCE=.\REnum.h
# End Source File
# Begin Source File

SOURCE=.\Request.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\REvent.h
# End Source File
# Begin Source File

SOURCE=.\RException.h
# End Source File
# Begin Source File

SOURCE=.\RExpr.h
# End Source File
# Begin Source File

SOURCE=.\Rfile.h
# End Source File
# Begin Source File

SOURCE=.\RfileArc.h
# End Source File
# Begin Source File

SOURCE=.\RfileArcBigIcon.h
# End Source File
# Begin Source File

SOURCE=.\RFileCanNotOpenException.h
# End Source File
# Begin Source File

SOURCE=.\RfileFactory.h
# End Source File
# Begin Source File

SOURCE=.\RfileLocal.h
# End Source File
# Begin Source File

SOURCE=.\RfileLocalBigIcon.h
# End Source File
# Begin Source File

SOURCE=.\RfileLocalReport.h
# End Source File
# Begin Source File

SOURCE=.\RfileNetResource.h
# End Source File
# Begin Source File

SOURCE=.\RfileNetResourceBigIcon.h
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\RfilePreviewOne.h
# End Source File
# Begin Source File

SOURCE=.\RFiles.h
# End Source File
# Begin Source File

SOURCE=.\RfilesDebug.h
# End Source File
# Begin Source File

SOURCE=.\RfilesFactory.h
# End Source File
# Begin Source File

SOURCE=.\RfilesLocal.h
# End Source File
# Begin Source File

SOURCE=.\RfilesLzh.h
# End Source File
# Begin Source File

SOURCE=.\RfilesPreviewOne.h
# End Source File
# Begin Source File

SOURCE=.\RfilesResolve.h
# End Source File
# Begin Source File

SOURCE=.\RfilesSMBLogon.h
# End Source File
# Begin Source File

SOURCE=.\RfilesWithCache.h
# End Source File
# Begin Source File

SOURCE=.\RFixedColumn.h
# End Source File
# Begin Source File

SOURCE=.\RFixedColumnArray.h
# End Source File
# Begin Source File

SOURCE=.\RFlashPlayer.h
# End Source File
# Begin Source File

SOURCE=.\RFModException.h
# End Source File
# Begin Source File

SOURCE=.\RFModView.h
# End Source File
# Begin Source File

SOURCE=.\RFocus.h
# End Source File
# Begin Source File

SOURCE=.\RFunctionScript.h
# End Source File
# Begin Source File

SOURCE=.\RImage.h
# End Source File
# Begin Source File

SOURCE=.\RImageButton.h
# End Source File
# Begin Source File

SOURCE=.\RImageFilter.h
# End Source File
# Begin Source File

SOURCE=.\RImageList.h
# End Source File
# Begin Source File

SOURCE=.\RImageViewer.h
# End Source File
# Begin Source File

SOURCE=.\RInputDialog.h
# End Source File
# Begin Source File

SOURCE=.\RInputStream.h
# End Source File
# Begin Source File

SOURCE=.\RIOCannotCloseException.h
# End Source File
# Begin Source File

SOURCE=.\RIOCannotOpenException.h
# End Source File
# Begin Source File

SOURCE=.\RIOCannotReadException.h
# End Source File
# Begin Source File

SOURCE=.\RIOCannotWriteException.h
# End Source File
# Begin Source File

SOURCE=.\RIOException.h
# End Source File
# Begin Source File

SOURCE=.\RListView.h
# End Source File
# Begin Source File

SOURCE=.\RListViewWithMouseGesture.h
# End Source File
# Begin Source File

SOURCE=.\RLock.h
# End Source File
# Begin Source File

SOURCE=.\RMedia.h
# End Source File
# Begin Source File

SOURCE=.\RMediaPlayer.h
# End Source File
# Begin Source File

SOURCE=.\RMediaView.h
# End Source File
# Begin Source File

SOURCE=.\RMediaViewPlayer.h
# End Source File
# Begin Source File

SOURCE=.\RMenu.h
# End Source File
# Begin Source File

SOURCE=.\RMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\RMessageBoxWin32.h
# End Source File
# Begin Source File

SOURCE=.\RMouseGesture.h
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMakeZende.h
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMouseCapture.h
# End Source File
# Begin Source File

SOURCE=.\RMouseGestureMousePoints.h
# End Source File
# Begin Source File

SOURCE=.\RNiseOle.h
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.h
# End Source File
# Begin Source File

SOURCE=.\RNormalWindow.h
# End Source File
# Begin Source File

SOURCE=.\RNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\RNotify.h
# End Source File
# Begin Source File

SOURCE=.\RNotSupportException.h
# End Source File
# Begin Source File

SOURCE=.\ROutOfRangeException.h
# End Source File
# Begin Source File

SOURCE=.\RParseException.h
# End Source File
# Begin Source File

SOURCE=.\RPDFView.h
# End Source File
# Begin Source File

SOURCE=.\RProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\RRealPlayer.h
# End Source File
# Begin Source File

SOURCE=.\RRichEdit.h
# End Source File
# Begin Source File

SOURCE=.\RSetupLoad.h
# End Source File
# Begin Source File

SOURCE=.\RSpliter.h
# End Source File
# Begin Source File

SOURCE=.\RStatue.h
# End Source File
# Begin Source File

SOURCE=.\RStatueControl.h
# End Source File
# Begin Source File

SOURCE=.\RStatueWin32.h
# End Source File
# Begin Source File

SOURCE=.\RStdioFile.h
# End Source File
# Begin Source File

SOURCE=.\RSusieConferException.h
# End Source File
# Begin Source File

SOURCE=.\RSyncFileCopy.h
# End Source File
# Begin Source File

SOURCE=.\RTabBar.h
# End Source File
# Begin Source File

SOURCE=.\RTabBarWin32.h
# End Source File
# Begin Source File

SOURCE=.\RTabControl.h
# End Source File
# Begin Source File

SOURCE=.\RTable.h
# End Source File
# Begin Source File

SOURCE=.\RTableControl.h
# End Source File
# Begin Source File

SOURCE=.\RTableEasy.h
# End Source File
# Begin Source File

SOURCE=.\RTableMuchDuplication.h
# End Source File
# Begin Source File

SOURCE=.\RTableOnlyOne.h
# End Source File
# Begin Source File

SOURCE=.\RTask.h
# End Source File
# Begin Source File

SOURCE=.\RTaskLS.h
# End Source File
# Begin Source File

SOURCE=.\RTaskManager.h
# End Source File
# Begin Source File

SOURCE=.\RTaskView.h
# End Source File
# Begin Source File

SOURCE=.\RTaskViewWin32.h
# End Source File
# Begin Source File

SOURCE=.\RThread.h
# End Source File
# Begin Source File

SOURCE=.\RThreadExecuteOnMainThread.h
# End Source File
# Begin Source File

SOURCE=.\RTimer.h
# End Source File
# Begin Source File

SOURCE=.\RToolBar.h
# End Source File
# Begin Source File

SOURCE=.\RToolBarControl.h
# End Source File
# Begin Source File

SOURCE=.\RToolBarWin32.h
# End Source File
# Begin Source File

SOURCE=.\RWebBrowser.h
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.h
# End Source File
# Begin Source File

SOURCE=.\RWindow.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFile.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileBitmap.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileLZH.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileManager.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileMedia.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileNiseOLE.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFilePDF.h
# End Source File
# Begin Source File

SOURCE=.\SolvableFileText.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePath.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathDebug.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathLocal.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathManager.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSMB.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSMBLogon.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathSpecialFolder.h
# End Source File
# Begin Source File

SOURCE=.\SolvablePathWeb.h
# End Source File
# Begin Source File

SOURCE=.\StringList.h
# End Source File
# Begin Source File

SOURCE=.\SusieCallable.h
# End Source File
# Begin Source File

SOURCE=.\SusiePlugin.h
# End Source File
# Begin Source File

SOURCE=.\SusiePluginManager.h
# End Source File
# Begin Source File

SOURCE=.\SusiePluginWapper.h
# End Source File
# Begin Source File

SOURCE=.\VirtualPath.h
# End Source File
# Begin Source File

SOURCE=.\Window.h
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.h
# End Source File
# Begin Source File

SOURCE=.\WndProcCallable.h
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
