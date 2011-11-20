# Microsoft Developer Studio Project File - Name="youjyo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=youjyo - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "youjyo.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "youjyo.mak" CFG="youjyo - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "youjyo - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "youjyo - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "youjyo - Win32 ReleaseNoTEST" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "youjyo - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_MT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imagehlp.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "youjyo - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_MT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imagehlp.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "youjyo - Win32 ReleaseNoTEST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "youjyo___Win32_ReleaseNoTEST"
# PROP BASE Intermediate_Dir "youjyo___Win32_ReleaseNoTEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "youjyo___Win32_ReleaseNoTEST"
# PROP Intermediate_Dir "youjyo___Win32_ReleaseNoTEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_NO_TEST_MODE" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_MT"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_MT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imagehlp.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imagehlp.lib /nologo /subsystem:console /machine:I386

!ENDIF 

# Begin Target

# Name "youjyo - Win32 Release"
# Name "youjyo - Win32 Debug"
# Name "youjyo - Win32 ReleaseNoTEST"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blowfish.cpp
# End Source File
# Begin Source File

SOURCE=.\comm.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\RAlphabetHash.cpp
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.cpp
# End Source File
# Begin Source File

SOURCE=.\RAnsyncStreamUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.cpp
# End Source File
# Begin Source File

SOURCE=.\RCollect.cpp
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

SOURCE=.\RDateConv.cpp
# End Source File
# Begin Source File

SOURCE=.\RDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.cpp
# End Source File
# Begin Source File

SOURCE=.\REvent.cpp
# End Source File
# Begin Source File

SOURCE=.\RException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileDB.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFModException.cpp
# End Source File
# Begin Source File

SOURCE=.\RHtmlTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttribute.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttributeDate.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttributeRange.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPPOSTParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RHTTPResponse.cpp
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

SOURCE=.\RIPFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\RMimeType.cpp
# End Source File
# Begin Source File

SOURCE=.\RMmap.cpp
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.cpp
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

SOURCE=.\RPackSuper.cpp
# End Source File
# Begin Source File

SOURCE=.\RParseException.cpp
# End Source File
# Begin Source File

SOURCE=.\RRemoteHost.cpp
# End Source File
# Begin Source File

SOURCE=.\RRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\RSequenceFileDB.cpp
# End Source File
# Begin Source File

SOURCE=.\RSimpleThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=.\RSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\RSocketAsyncTransmitFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RSocketForWinSock2.cpp
# End Source File
# Begin Source File

SOURCE=.\RSpinLock.cpp
# End Source File
# Begin Source File

SOURCE=.\RStream.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamAnSyncSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamAsync.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RTcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\RThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=.\RToken.cpp
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\RWorkerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\YBBS.cpp
# End Source File
# Begin Source File

SOURCE=.\YBBSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\YBuildThread.cpp
# End Source File
# Begin Source File

SOURCE=.\YConnectNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\YDAT.cpp
# End Source File
# Begin Source File

SOURCE=.\YDatCache.cpp
# End Source File
# Begin Source File

SOURCE=.\YDATCache1.cpp
# End Source File
# Begin Source File

SOURCE=.\YDATReader.cpp
# End Source File
# Begin Source File

SOURCE=.\YDATWritter.cpp
# End Source File
# Begin Source File

SOURCE=.\YDeleteWait.cpp
# End Source File
# Begin Source File

SOURCE=.\YFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\YHtmlTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\YHttpdConf.cpp
# End Source File
# Begin Source File

SOURCE=.\YIndexHTML.cpp
# End Source File
# Begin Source File

SOURCE=.\YLocalRule.cpp
# End Source File
# Begin Source File

SOURCE=.\YMain.cpp
# End Source File
# Begin Source File

SOURCE=.\YReadCGITemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\YRemoteHost.cpp
# End Source File
# Begin Source File

SOURCE=.\YRequestPath.cpp
# End Source File
# Begin Source File

SOURCE=.\YServer.cpp
# End Source File
# Begin Source File

SOURCE=.\YSettingTXT.cpp
# End Source File
# Begin Source File

SOURCE=.\YSubBackHTML.cpp
# End Source File
# Begin Source File

SOURCE=.\YSubject.cpp
# End Source File
# Begin Source File

SOURCE=.\YTextFile.cpp
# End Source File
# Begin Source File

SOURCE=.\YThreadWithTls.cpp
# End Source File
# Begin Source File

SOURCE=.\YTransmitFile.cpp
# End Source File
# Begin Source File

SOURCE=.\YWriteHTML.cpp
# End Source File
# Begin Source File

SOURCE=.\YYoujyoGC.cpp
# End Source File
# Begin Source File

SOURCE=.\YYoujyoKiller.cpp
# End Source File
# Begin Source File

SOURCE=.\YYoujyoLogger.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\blowfish.h
# End Source File
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\RAlphabetHash.h
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.h
# End Source File
# Begin Source File

SOURCE=.\RAnsyncStreamUtil.h
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.h
# End Source File
# Begin Source File

SOURCE=.\RCollect.h
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

SOURCE=.\RDateConv.h
# End Source File
# Begin Source File

SOURCE=.\RDebug.h
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.h
# End Source File
# Begin Source File

SOURCE=.\REvent.h
# End Source File
# Begin Source File

SOURCE=.\RException.h
# End Source File
# Begin Source File

SOURCE=.\RFileDB.h
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\RFModException.h
# End Source File
# Begin Source File

SOURCE=.\RHtmlTemplate.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeader.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttribute.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttributeDate.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPHeaderAttributeRange.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPPOSTParser.h
# End Source File
# Begin Source File

SOURCE=.\RHTTPResponse.h
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

SOURCE=.\RIPFilter.h
# End Source File
# Begin Source File

SOURCE=.\RLock.h
# End Source File
# Begin Source File

SOURCE=.\RMimeType.h
# End Source File
# Begin Source File

SOURCE=.\RMmap.h
# End Source File
# Begin Source File

SOURCE=.\RNoComplateException.h
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

SOURCE=.\RPackSuper.h
# End Source File
# Begin Source File

SOURCE=.\RParseException.h
# End Source File
# Begin Source File

SOURCE=.\RRemoteHost.h
# End Source File
# Begin Source File

SOURCE=.\RRequest.h
# End Source File
# Begin Source File

SOURCE=.\RSequenceFileDB.h
# End Source File
# Begin Source File

SOURCE=.\RSimpleThreadPool.h
# End Source File
# Begin Source File

SOURCE=.\rsocket.h
# End Source File
# Begin Source File

SOURCE=.\RSocketAsyncTransmitFile.h
# End Source File
# Begin Source File

SOURCE=.\RSocketForWinSock2.h
# End Source File
# Begin Source File

SOURCE=.\RSpinLock.h
# End Source File
# Begin Source File

SOURCE=.\RStream.h
# End Source File
# Begin Source File

SOURCE=.\RStreamAnSyncSocket.h
# End Source File
# Begin Source File

SOURCE=.\RStreamAsync.h
# End Source File
# Begin Source File

SOURCE=.\RStreamMemory.h
# End Source File
# Begin Source File

SOURCE=.\RStreamSocket.h
# End Source File
# Begin Source File

SOURCE=.\RStreamUtil.h
# End Source File
# Begin Source File

SOURCE=.\RTcpSocket.h
# End Source File
# Begin Source File

SOURCE=.\RThread.h
# End Source File
# Begin Source File

SOURCE=.\RThreadPool.h
# End Source File
# Begin Source File

SOURCE=.\RToken.h
# End Source File
# Begin Source File

SOURCE=.\RWin32Exception.h
# End Source File
# Begin Source File

SOURCE=.\RWorkerThread.h
# End Source File
# Begin Source File

SOURCE=.\YBBS.h
# End Source File
# Begin Source File

SOURCE=.\YBBSManager.h
# End Source File
# Begin Source File

SOURCE=.\YBuildThread.h
# End Source File
# Begin Source File

SOURCE=.\YConnectNotify.h
# End Source File
# Begin Source File

SOURCE=.\YDAT.h
# End Source File
# Begin Source File

SOURCE=.\YDatCache.h
# End Source File
# Begin Source File

SOURCE=.\YDATCache1.h
# End Source File
# Begin Source File

SOURCE=.\YDATReader.h
# End Source File
# Begin Source File

SOURCE=.\YDATWritter.h
# End Source File
# Begin Source File

SOURCE=.\YDeleteWait.h
# End Source File
# Begin Source File

SOURCE=.\YFilter.h
# End Source File
# Begin Source File

SOURCE=.\YHtmlTemplate.h
# End Source File
# Begin Source File

SOURCE=.\YHttpdConf.h
# End Source File
# Begin Source File

SOURCE=.\YIndexHTML.h
# End Source File
# Begin Source File

SOURCE=.\YLocalRule.h
# End Source File
# Begin Source File

SOURCE=.\YMain.h
# End Source File
# Begin Source File

SOURCE=.\YReadCGITemplate.h
# End Source File
# Begin Source File

SOURCE=.\YRemoteHost.h
# End Source File
# Begin Source File

SOURCE=.\YRequestPath.h
# End Source File
# Begin Source File

SOURCE=.\YServer.h
# End Source File
# Begin Source File

SOURCE=.\YSettingTXT.h
# End Source File
# Begin Source File

SOURCE=.\YSubBackHTML.h
# End Source File
# Begin Source File

SOURCE=.\YSubject.h
# End Source File
# Begin Source File

SOURCE=.\YTextFile.h
# End Source File
# Begin Source File

SOURCE=.\YThreadWithTls.h
# End Source File
# Begin Source File

SOURCE=.\YTransmitFile.h
# End Source File
# Begin Source File

SOURCE=.\YWriteHTML.h
# End Source File
# Begin Source File

SOURCE=.\YYoujyoGC.h
# End Source File
# Begin Source File

SOURCE=.\YYoujyoKiller.h
# End Source File
# Begin Source File

SOURCE=.\YYoujyoLogger.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\blowfish.h2
# End Source File
# Begin Source File

SOURCE=".\YLocal}"
# End Source File
# End Target
# End Project
