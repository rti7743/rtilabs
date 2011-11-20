# Microsoft Developer Studio Project File - Name="slit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=slit - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "slit.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "slit.mak" CFG="slit - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "slit - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "slit - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "slit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "slit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "slit - Win32 Release"
# Name "slit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\comm.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MVConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\MVControl.cpp
# End Source File
# Begin Source File

SOURCE=.\MVFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\MVImage.cpp
# End Source File
# Begin Source File

SOURCE=.\MVImagePreConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\MVInputData.cpp
# End Source File
# Begin Source File

SOURCE=.\MVMain.cpp
# End Source File
# Begin Source File

SOURCE=.\MVPageData.cpp
# End Source File
# Begin Source File

SOURCE=.\MVReaderThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MVSettingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MVStaticZLib.cpp
# End Source File
# Begin Source File

SOURCE=.\MVThreadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.cpp
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.cpp
# End Source File
# Begin Source File

SOURCE=.\RComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RComInit.cpp
# End Source File
# Begin Source File

SOURCE=.\RConferException.cpp
# End Source File
# Begin Source File

SOURCE=.\RConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RConv.cpp
# End Source File
# Begin Source File

SOURCE=.\RDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RDialogUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RDIBBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.cpp
# End Source File
# Begin Source File

SOURCE=.\RException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFModException.cpp
# End Source File
# Begin Source File

SOURCE=.\RImage.cpp
# End Source File
# Begin Source File

SOURCE=.\RImgdecmp.cpp
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

SOURCE=.\RMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.cpp
# End Source File
# Begin Source File

SOURCE=.\RNotFoundException.cpp
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

SOURCE=.\RStdioFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\RThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RWaitCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\RWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\RWindowBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\MVConfig.h
# End Source File
# Begin Source File

SOURCE=.\MVControl.h
# End Source File
# Begin Source File

SOURCE=.\MVFileList.h
# End Source File
# Begin Source File

SOURCE=.\MVImage.h
# End Source File
# Begin Source File

SOURCE=.\MVImagePreConvert.h
# End Source File
# Begin Source File

SOURCE=.\MVInputData.h
# End Source File
# Begin Source File

SOURCE=.\MVMain.h
# End Source File
# Begin Source File

SOURCE=.\MVPageData.h
# End Source File
# Begin Source File

SOURCE=.\MVReaderThread.h
# End Source File
# Begin Source File

SOURCE=.\MVSettingDialog.h
# End Source File
# Begin Source File

SOURCE=.\MVStaticZLib.h
# End Source File
# Begin Source File

SOURCE=.\MVThreadManager.h
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.h
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.h
# End Source File
# Begin Source File

SOURCE=.\RComboBox.h
# End Source File
# Begin Source File

SOURCE=.\RComInit.h
# End Source File
# Begin Source File

SOURCE=.\RConferException.h
# End Source File
# Begin Source File

SOURCE=.\RConfigFile.h
# End Source File
# Begin Source File

SOURCE=.\RConv.h
# End Source File
# Begin Source File

SOURCE=.\RDialog.h
# End Source File
# Begin Source File

SOURCE=.\RDialogUtil.h
# End Source File
# Begin Source File

SOURCE=.\RDIBBitmap.h
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resrc1.h
# End Source File
# Begin Source File

SOURCE=.\RException.h
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\RFModException.h
# End Source File
# Begin Source File

SOURCE=.\RImage.h
# End Source File
# Begin Source File

SOURCE=.\RImgdecmp.h
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

SOURCE=.\RMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.h
# End Source File
# Begin Source File

SOURCE=.\RNotFoundException.h
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

SOURCE=.\RStdioFile.h
# End Source File
# Begin Source File

SOURCE=.\RStdioMemory.h
# End Source File
# Begin Source File

SOURCE=.\RThread.h
# End Source File
# Begin Source File

SOURCE=.\RWaitCursor.h
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.h
# End Source File
# Begin Source File

SOURCE=.\RWindow.h
# End Source File
# Begin Source File

SOURCE=.\RWindowBase.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ceux1.bin
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
