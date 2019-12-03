# Microsoft Developer Studio Project File - Name="Insects" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Insects - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Insects.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Insects.mak" CFG="Insects - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Insects - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Insects - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Insects - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Insects - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ   /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Insects - Win32 Release"
# Name "Insects - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\Character.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DibObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGenRota.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGenTran.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGenZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMedFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMorth.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgResult.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTempSmooth.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\Identify.cpp
# End Source File
# Begin Source File

SOURCE=.\Insects.cpp
# End Source File
# Begin Source File

SOURCE=.\Insects.rc
# End Source File
# Begin Source File

SOURCE=.\InsectsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\InsectsView.cpp
# End Source File
# Begin Source File

SOURCE=.\LapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Pretreatment.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AFXTEMPL.H
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ButtonStruct.h
# End Source File
# Begin Source File

SOURCE=.\Character.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DibObject.h
# End Source File
# Begin Source File

SOURCE=.\DlgCharacter.h
# End Source File
# Begin Source File

SOURCE=.\DlgGenRota.h
# End Source File
# Begin Source File

SOURCE=.\DlgGenTran.h
# End Source File
# Begin Source File

SOURCE=.\DlgGenZoom.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgLine.h
# End Source File
# Begin Source File

SOURCE=.\DlgMedFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgMorth.h
# End Source File
# Begin Source File

SOURCE=.\DlgResult.h
# End Source File
# Begin Source File

SOURCE=.\DlgTempSmooth.h
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.h
# End Source File
# Begin Source File

SOURCE=.\GeoTrans.h
# End Source File
# Begin Source File

SOURCE=.\Identify.h
# End Source File
# Begin Source File

SOURCE=.\ImageErrors.h
# End Source File
# Begin Source File

SOURCE=.\Imageload.h
# End Source File
# Begin Source File

SOURCE=.\Insects.h
# End Source File
# Begin Source File

SOURCE=.\InsectsDoc.h
# End Source File
# Begin Source File

SOURCE=.\InsectsView.h
# End Source File
# Begin Source File

SOURCE=.\LapDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MorphProc.h
# End Source File
# Begin Source File

SOURCE=.\Pretreatment.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Insects.ico
# End Source File
# Begin Source File

SOURCE=.\res\Insects.rc2
# End Source File
# Begin Source File

SOURCE=.\res\InsectsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\help.chm
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\Thumbs.db
# End Source File
# Begin Source File

SOURCE=.\~VCB1.tmp
# End Source File
# Begin Source File

SOURCE=.\~VCB6.tmp
# End Source File
# Begin Source File

SOURCE=.\ImageLoad.lib
# End Source File
# End Target
# End Project
