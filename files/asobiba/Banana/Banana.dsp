# Microsoft Developer Studio Project File - Name="Banana" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Banana - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Banana.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Banana.mak" CFG="Banana - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Banana - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "Banana - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "Banana - Win32 ReleaseViewOnly" ("Win32 (x86) Console Application" 用)
!MESSAGE "Banana - Win32 ReleaseMakeMap" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Banana - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Ot /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT CPP /Ox /Oi
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /machine:I386

!ELSEIF  "$(CFG)" == "Banana - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Banana - Win32 ReleaseViewOnly"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Banana___Win32_ReleaseViewOnly"
# PROP BASE Intermediate_Dir "Banana___Win32_ReleaseViewOnly"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Banana___Win32_ReleaseViewOnly"
# PROP Intermediate_Dir "Banana___Win32_ReleaseViewOnly"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT BASE CPP /Ox /Oi
# ADD CPP /nologo /MT /W3 /GX /Ox /Ot /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "VIEWONLY" /D "NOTEST" /FR /YX /FD /c
# SUBTRACT CPP /Oi /Os
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Banana - Win32 ReleaseMakeMap"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Banana___Win32_ReleaseMakeMap"
# PROP BASE Intermediate_Dir "Banana___Win32_ReleaseMakeMap"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Banana___Win32_ReleaseMakeMap"
# PROP Intermediate_Dir "Banana___Win32_ReleaseMakeMap"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ot /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT BASE CPP /Ox /Oi
# ADD CPP /nologo /MT /W3 /GX /Ot /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "NOTEST" /FR /YX /FD /c
# SUBTRACT CPP /Ox /Oi
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /machine:I386 /out:"Banana___Win32_ReleaseMakeMap/Milk.exe"

!ENDIF 

# Begin Target

# Name "Banana - Win32 Release"
# Name "Banana - Win32 Debug"
# Name "Banana - Win32 ReleaseViewOnly"
# Name "Banana - Win32 ReleaseMakeMap"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BAddressMatching.cpp
# End Source File
# Begin Source File

SOURCE=.\banana.rc
# End Source File
# Begin Source File

SOURCE=.\BBanana.cpp
# End Source File
# Begin Source File

SOURCE=.\BBananaConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\BBananaConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BClickMode.cpp
# End Source File
# Begin Source File

SOURCE=.\BConf.cpp
# End Source File
# Begin Source File

SOURCE=.\BFindAdressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BFindEkiDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BGPSConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BGPSInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BGPSSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BGPSStateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Bl2Sec.cpp
# End Source File
# Begin Source File

SOURCE=.\BMakeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BPen.cpp
# End Source File
# Begin Source File

SOURCE=.\BRotateMode.cpp
# End Source File
# Begin Source File

SOURCE=.\BUserDataEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BUserDataIconDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\comm.cpp
# End Source File
# Begin Source File

SOURCE=.\GISType.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.cpp
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.cpp
# End Source File
# Begin Source File

SOURCE=.\RBigArray2.cpp
# End Source File
# Begin Source File

SOURCE=.\RComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RComException.cpp
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

SOURCE=.\RCreateFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RCSV.cpp
# End Source File
# Begin Source File

SOURCE=.\RDartyScan.cpp
# End Source File
# Begin Source File

SOURCE=.\RDebug.cpp
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

SOURCE=.\RDiskUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.cpp
# End Source File
# Begin Source File

SOURCE=.\RDrawObject.cpp
# End Source File
# Begin Source File

SOURCE=.\REdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFileUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RFModException.cpp
# End Source File
# Begin Source File

SOURCE=.\RFont.cpp
# End Source File
# Begin Source File

SOURCE=.\RFontCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS2500.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS25000.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS2500Head.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS2500Info.cpp
# End Source File
# Begin Source File

SOURCE=.\RGIS2500IntArray.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISAddressMng.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISAddressToCode.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISArc.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISAtr.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISCaption.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISChimei.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISCVCut.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISCVCutDataIml.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISCvLight.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISDouro.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISEki.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISFindCityToNumberConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISFindHintAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISFindHintEki.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISFindNameDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISGPS.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISGPSLog.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISGyoseiD.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISGyousei.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISHashi.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISIntelligentCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISJoin.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISKawa.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISKeiidoData.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISKoudo2.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISKoukyou.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISMath.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISMathAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNameDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNameDatabaseDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNBlockManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNDouro.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNMEA0183.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISNode.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISPgn.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISPnt.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSlm.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSlmINdex.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSlp.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmall.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallChimei.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDouro.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDrawEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDupKill.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFileRead.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindCityToNumberConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindEki.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindNameDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindObject.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindUserData.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallGlobalToBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallHeaderImp.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallIntelligentCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKawa.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKoudo2.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKoukyou.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallMaster.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallName.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNameDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNameDataBase1.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNavigation.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallParent.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallParentImp.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallPathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallPenFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallShareImage.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallSuiikiKai.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallTatemono.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallTetudo.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallType.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallUserData.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallUserDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSmallZyouti.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISSuiikiKai.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISTetudo.cpp
# End Source File
# Begin Source File

SOURCE=.\RGISTonneru.cpp
# End Source File
# Begin Source File

SOURCE=.\RGraphics.cpp
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

SOURCE=.\RIStreamble.cpp
# End Source File
# Begin Source File

SOURCE=.\RListenner.cpp
# End Source File
# Begin Source File

SOURCE=.\RListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RMathAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\RMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RMmap.cpp
# End Source File
# Begin Source File

SOURCE=.\RNeoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RNeoIFStreamUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RNeoIStreamble.cpp
# End Source File
# Begin Source File

SOURCE=.\RNeoMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\RNeoSerialPort.cpp
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

SOURCE=.\RRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\RResorceImageLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\RScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RState.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioFileBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioMemoryWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\RStdioSerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\RStringRingBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\RSujiFontCache.cpp
# End Source File
# Begin Source File

SOURCE=.\RTextLib.cpp
# End Source File
# Begin Source File

SOURCE=.\RThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\RToken.cpp
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

SOURCE=.\RZlib.cpp
# End Source File
# Begin Source File

SOURCE=.\Sec2Utm.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BAddressMatching.h
# End Source File
# Begin Source File

SOURCE=.\BBanana.h
# End Source File
# Begin Source File

SOURCE=.\BBananaConfig.h
# End Source File
# Begin Source File

SOURCE=.\BBananaConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\BClickMode.h
# End Source File
# Begin Source File

SOURCE=.\BConf.h
# End Source File
# Begin Source File

SOURCE=.\BFindAdressDlg.h
# End Source File
# Begin Source File

SOURCE=.\BFindEkiDlg.h
# End Source File
# Begin Source File

SOURCE=.\BGPSConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\BGPSInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\BGPSSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\BGPSStateDlg.h
# End Source File
# Begin Source File

SOURCE=.\Bl2Sec.h
# End Source File
# Begin Source File

SOURCE=.\BMakeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BPen.h
# End Source File
# Begin Source File

SOURCE=.\BRotateMode.h
# End Source File
# Begin Source File

SOURCE=.\BUserDataEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\BUserDataIconDlg.h
# End Source File
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\GISType.h
# End Source File
# Begin Source File

SOURCE=.\RAlreadyException.h
# End Source File
# Begin Source File

SOURCE=.\RAutoLibraryWithException.h
# End Source File
# Begin Source File

SOURCE=.\RBigArray2.h
# End Source File
# Begin Source File

SOURCE=.\RComboBox.h
# End Source File
# Begin Source File

SOURCE=.\RComException.h
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

SOURCE=.\RCreateFile.h
# End Source File
# Begin Source File

SOURCE=.\RCSV.h
# End Source File
# Begin Source File

SOURCE=.\RDartyScan.h
# End Source File
# Begin Source File

SOURCE=.\RDebug.h
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

SOURCE=.\RDiskUtil.h
# End Source File
# Begin Source File

SOURCE=.\RDoNotInitaliseException.h
# End Source File
# Begin Source File

SOURCE=.\RDrawObject.h
# End Source File
# Begin Source File

SOURCE=.\REdit.h
# End Source File
# Begin Source File

SOURCE=.\RException.h
# End Source File
# Begin Source File

SOURCE=.\RFileNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\RFileUtil.h
# End Source File
# Begin Source File

SOURCE=.\RFModException.h
# End Source File
# Begin Source File

SOURCE=.\RFont.h
# End Source File
# Begin Source File

SOURCE=.\RFontCache.h
# End Source File
# Begin Source File

SOURCE=.\RGIS.h
# End Source File
# Begin Source File

SOURCE=.\RGIS2500.h
# End Source File
# Begin Source File

SOURCE=.\RGIS25000.h
# End Source File
# Begin Source File

SOURCE=.\RGIS2500Head.h
# End Source File
# Begin Source File

SOURCE=.\RGIS2500Info.h
# End Source File
# Begin Source File

SOURCE=.\RGIS2500IntArray.h
# End Source File
# Begin Source File

SOURCE=.\RGISAddress.h
# End Source File
# Begin Source File

SOURCE=.\RGISAddressMng.h
# End Source File
# Begin Source File

SOURCE=.\RGISAddressToCode.h
# End Source File
# Begin Source File

SOURCE=.\RGISArc.h
# End Source File
# Begin Source File

SOURCE=.\RGISAtr.h
# End Source File
# Begin Source File

SOURCE=.\RGISCaption.h
# End Source File
# Begin Source File

SOURCE=.\RGISChimei.h
# End Source File
# Begin Source File

SOURCE=.\RGISCVCut.h
# End Source File
# Begin Source File

SOURCE=.\RGISCVCutDataIml.h
# End Source File
# Begin Source File

SOURCE=.\RGISCvLight.h
# End Source File
# Begin Source File

SOURCE=.\RGISDouro.h
# End Source File
# Begin Source File

SOURCE=.\RGISDraw.h
# End Source File
# Begin Source File

SOURCE=.\RGISEki.h
# End Source File
# Begin Source File

SOURCE=.\RGISFindCityToNumberConvert.h
# End Source File
# Begin Source File

SOURCE=.\RGISFindHintAddress.h
# End Source File
# Begin Source File

SOURCE=.\RGISFindHintEki.h
# End Source File
# Begin Source File

SOURCE=.\RGISFindNameDataBase.h
# End Source File
# Begin Source File

SOURCE=.\RGISGPS.h
# End Source File
# Begin Source File

SOURCE=.\RGISGPSLog.h
# End Source File
# Begin Source File

SOURCE=.\RGISGyoseiD.h
# End Source File
# Begin Source File

SOURCE=.\RGISGyousei.h
# End Source File
# Begin Source File

SOURCE=.\RGISHashi.h
# End Source File
# Begin Source File

SOURCE=.\RGISIntelligentCache.h
# End Source File
# Begin Source File

SOURCE=.\RGISJoin.h
# End Source File
# Begin Source File

SOURCE=.\RGISKawa.h
# End Source File
# Begin Source File

SOURCE=.\RGISKeiidoData.h
# End Source File
# Begin Source File

SOURCE=.\RGISKoudo2.h
# End Source File
# Begin Source File

SOURCE=.\RGISKoukyou.h
# End Source File
# Begin Source File

SOURCE=.\RGISMath.h
# End Source File
# Begin Source File

SOURCE=.\RGISMathAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\RGISNameDatabase.h
# End Source File
# Begin Source File

SOURCE=.\RGISNameDatabaseDisk.h
# End Source File
# Begin Source File

SOURCE=.\RGISNBlock.h
# End Source File
# Begin Source File

SOURCE=.\RGISNBlockManager.h
# End Source File
# Begin Source File

SOURCE=.\RGISNDouro.h
# End Source File
# Begin Source File

SOURCE=.\RGISNMEA0183.h
# End Source File
# Begin Source File

SOURCE=.\RGISNode.h
# End Source File
# Begin Source File

SOURCE=.\RGISPgn.h
# End Source File
# Begin Source File

SOURCE=.\RGISPnt.h
# End Source File
# Begin Source File

SOURCE=.\RGISSlm.h
# End Source File
# Begin Source File

SOURCE=.\RGISSlmINdex.h
# End Source File
# Begin Source File

SOURCE=.\RGISSlp.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmall.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallAddress.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallChimei.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDouro.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDraw.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDrawEffect.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallDupKill.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFileRead.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindAddress.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindCityToNumberConvert.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindEki.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindNameDatabase.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindObject.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallFindUserData.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallGlobalToBlock.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallHeaderImp.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallIntelligentCache.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKawa.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKoudo2.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallKoukyou.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallMaster.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallName.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNameDatabase.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNameDataBase1.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallNavigation.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallParent.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallParentImp.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallPathFinder.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallPenFile.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallShareImage.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallSuiikiKai.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallTatemono.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallTetudo.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallType.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallUserData.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallUserDataFile.h
# End Source File
# Begin Source File

SOURCE=.\RGISSmallZyouti.h
# End Source File
# Begin Source File

SOURCE=.\RGISSuiikiKai.h
# End Source File
# Begin Source File

SOURCE=.\RGISTetudo.h
# End Source File
# Begin Source File

SOURCE=.\RGISTonneru.h
# End Source File
# Begin Source File

SOURCE=.\RGraphics.h
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

SOURCE=.\RIStreamble.h
# End Source File
# Begin Source File

SOURCE=.\RListenner.h
# End Source File
# Begin Source File

SOURCE=.\RListView.h
# End Source File
# Begin Source File

SOURCE=.\RLock.h
# End Source File
# Begin Source File

SOURCE=.\RMathAngle.h
# End Source File
# Begin Source File

SOURCE=.\RMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\RMmap.h
# End Source File
# Begin Source File

SOURCE=.\RNeoFile.h
# End Source File
# Begin Source File

SOURCE=.\RNeoIFStreamUtil.h
# End Source File
# Begin Source File

SOURCE=.\RNeoIStreamble.h
# End Source File
# Begin Source File

SOURCE=.\RNeoMemory.h
# End Source File
# Begin Source File

SOURCE=.\RNeoSerialPort.h
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

SOURCE=.\RRegistry.h
# End Source File
# Begin Source File

SOURCE=.\RResorceImageLoad.h
# End Source File
# Begin Source File

SOURCE=.\RScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\RState.h
# End Source File
# Begin Source File

SOURCE=.\RStdioFile.h
# End Source File
# Begin Source File

SOURCE=.\RStdioFileBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RStdioMemory.h
# End Source File
# Begin Source File

SOURCE=.\RStdioMemoryWrite.h
# End Source File
# Begin Source File

SOURCE=.\RStdioSerialPort.h
# End Source File
# Begin Source File

SOURCE=.\RStringRingBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RSujiFontCache.h
# End Source File
# Begin Source File

SOURCE=.\RTextLib.h
# End Source File
# Begin Source File

SOURCE=.\RThread.h
# End Source File
# Begin Source File

SOURCE=.\RTimer.h
# End Source File
# Begin Source File

SOURCE=.\RToken.h
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

SOURCE=.\RZlib.h
# End Source File
# Begin Source File

SOURCE=.\Sec2Utm.h
# End Source File
# Begin Source File

SOURCE=.\WindowsMain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\ceux1.bin
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\SatellitesIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_bit.bmp
# End Source File
# End Group
# End Target
# End Project
