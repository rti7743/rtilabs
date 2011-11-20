; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CEvalDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "eval.h"

ClassCount=2
Class1=CEvalApp
Class2=CEvalDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Resource2=IDD_EVAL_DIALOG

[CLS:CEvalApp]
Type=0
HeaderFile=eval.h
ImplementationFile=eval.cpp
Filter=N

[CLS:CEvalDlg]
Type=0
HeaderFile=evalDlg.h
ImplementationFile=evalDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EVALSTRING_EDIT



[DLG:IDD_EVAL_DIALOG]
Type=1
Class=CEvalDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EVALSTRING_EDIT,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_ANSWER_EDIT,edit,1350631552
Control5=IDRECURSIVE_DESCENT_PARSING,button,1342242816
Control6=IDCHEART,button,1342242816

