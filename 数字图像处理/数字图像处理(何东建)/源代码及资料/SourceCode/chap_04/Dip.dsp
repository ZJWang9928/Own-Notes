# Microsoft Developer Studio Project File - Name="Dip" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Dip - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dip.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dip.mak" CFG="Dip - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dip - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Dip - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Dip - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
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

!ELSEIF  "$(CFG)" == "Dip - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ImageLoad.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Dip - Win32 Release"
# Name "Dip - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AreaPro.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CombineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DibObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Dip.cpp
# End Source File
# Begin Source File

SOURCE=.\Dip.rc
# End Source File
# Begin Source File

SOURCE=.\DipDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DipView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIntensity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinerPara.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMedianFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSmooth.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeDetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PointPro.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AreaPro.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CombineDlg.h
# End Source File
# Begin Source File

SOURCE=.\DibObject.h
# End Source File
# Begin Source File

SOURCE=.\Dip.h
# End Source File
# Begin Source File

SOURCE=.\DipDoc.h
# End Source File
# Begin Source File

SOURCE=.\DipView.h
# End Source File
# Begin Source File

SOURCE=.\DlgColor.h
# End Source File
# Begin Source File

SOURCE=.\DlgIntensity.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinerPara.h
# End Source File
# Begin Source File

SOURCE=.\DlgMedianFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.h
# End Source File
# Begin Source File

SOURCE=.\DlgSmooth.h
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.h
# End Source File
# Begin Source File

SOURCE=.\EdgeDetDlg.h
# End Source File
# Begin Source File

SOURCE=.\LapDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PointPro.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Dip.ico
# End Source File
# Begin Source File

SOURCE=.\res\Dip.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DipDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\ledoff1.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\ledon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\midpoint.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Dip : {99682F6C-6B2F-484A-B4BF-54C87490A279}
# 	2:15:CLASS: CFourior:CFourior
# 	2:7:dft.cpp:dft.cpp
# 	2:5:dft.h:dft.h
# 	2:15:CLASS: CComplex:CComplex
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {54BBEBFD-2FF4-4838-9C9E-FAF0A6897874}
# 	1:14:IDD_DLG_Smooth:106
# 	2:16:Resource Include:resource.h
# 	2:14:IDD_DLG_Smooth:IDD_DLG_Smooth
# 	2:10:ENUM: enum:enum
# 	2:11:DlgSmooth.h:DlgSmooth.h
# 	2:13:DlgSmooth.cpp:DlgSmooth.cpp
# 	2:17:CLASS: CDlgSmooth:CDlgSmooth
# 	2:19:Application Include:Dip.h
# End Section
# Section Dip : {5BC68459-B5F9-4967-8E20-6AC9657EA1FB}
# 	1:17:IDD_DLG_LinerPara:103
# 	2:20:CLASS: CDlgLinerPara:CDlgLinerPara
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_DLG_LinerPara:IDD_DLG_LinerPara
# 	2:16:DlgLinerPara.cpp:DlgLinerPara.cpp
# 	2:14:DlgLinerPara.h:DlgLinerPara.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:Dip.h
# End Section
# Section Dip : {ECF17037-AF09-49B8-A358-03FA027228D4}
# 	2:21:CLASS: CImageGeoTrans:CImageGeoTrans
# 	2:17:ImageGeoTrans.cpp:ImageGeoTrans.cpp
# 	2:15:ImageGeoTrans.h:ImageGeoTrans.h
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {1F98B094-15AB-4286-8272-F939BA70F9EB}
# 	1:17:IDD_DLG_PointStre:104
# 	2:16:Resource Include:resource.h
# 	2:16:DlgPointStre.cpp:DlgPointStre.cpp
# 	2:14:DlgPointStre.h:DlgPointStre.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:Dip.h
# 	2:20:CLASS: CDlgPointStre:CDlgPointStre
# 	2:17:IDD_DLG_PointStre:IDD_DLG_PointStre
# End Section
# Section Dip : {3B3D4D22-989E-47AC-BBE7-7D39F8F09287}
# 	2:15:CLASS: CFourior:CFourior
# 	2:7:dft.cpp:dft1.cpp
# 	2:5:dft.h:dft1.h
# 	2:15:CLASS: CComplex:CComplex
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {996E8DF5-291B-467A-AD49-F61BDA651B5B}
# 	1:19:IDD_CHANNEL_COMBINE:113
# 	2:16:Resource Include:resource.h
# 	2:19:IDD_CHANNEL_COMBINE:IDD_CHANNEL_COMBINE
# 	2:12:CombineDlg.h:CombineDlg.h
# 	2:14:CombineDlg.cpp:CombineDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:CLASS: CCombineDlg:CCombineDlg
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {A52FBF06-2780-4231-853C-0EB3D118F32A}
# 	1:22:IDD_DLG_Fourior_Filter:112
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_DLG_Fourior_Filter:IDD_DLG_Fourior_Filter
# 	2:19:DlgFouriorFilte.cpp:DlgFouriorFilte.cpp
# 	2:23:CLASS: CDlgFouriorFilte:CDlgFouriorFilte
# 	2:17:DlgFouriorFilte.h:DlgFouriorFilte.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {35F3399D-DAA5-4ECF-803D-E2440D2DE7F3}
# 	1:17:IDD_DLG_Intensity:102
# 	2:16:Resource Include:resource.h
# 	2:14:DlgIntensity.h:DlgIntensity.h
# 	2:20:CLASS: CDlgIntensity:CDlgIntensity
# 	2:17:IDD_DLG_Intensity:IDD_DLG_Intensity
# 	2:10:ENUM: enum:enum
# 	2:16:DlgIntensity.cpp:DlgIntensity.cpp
# 	2:19:Application Include:Dip.h
# End Section
# Section Dip : {2DDB1FEA-8E08-4806-B174-4FA2EB1B90A0}
# 	1:20:IDD_DLG_MORPHErosion:111
# 	2:16:Resource Include:resource.h
# 	2:20:IDD_DLG_MORPHErosion:IDD_DLG_MORPHErosion
# 	2:18:cDlgMorphErosion.h:cDlgMorphErosion.h
# 	2:10:ENUM: enum:enum
# 	2:20:cDlgMorphErosion.cpp:cDlgMorphErosion.cpp
# 	2:23:CLASS: cDlgMorphErosion:cDlgMorphErosion
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {A0FF6743-F6B7-4444-88CE-8909905258F8}
# 	2:26:CLASS: CImageFreqProcesses:CImageFreqProcesses
# 	2:22:ImageFreqProcesses.cpp:ImageFreqProcesses.cpp
# 	2:19:Application Include:dip.h
# 	2:20:ImageFreqProcesses.h:ImageFreqProcesses.h
# End Section
# Section Dip : {1DA14DDE-BB3D-4BFC-8413-0C3B80B91144}
# 	1:17:IDD_DLG_THRESHOLD:105
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CDlgThreshold:CDlgThreshold
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_DLG_THRESHOLD:IDD_DLG_THRESHOLD
# 	2:14:DlgThreshold.h:DlgThreshold.h
# 	2:16:DlgThreshold.cpp:DlgThreshold.cpp
# 	2:19:Application Include:Dip.h
# End Section
# Section Dip : {41225781-290A-4809-AA1E-386DAFF91299}
# 	1:13:IDD_DLG_Color:108
# 	2:16:Resource Include:resource.h
# 	2:13:IDD_DLG_Color:IDD_DLG_Color
# 	2:12:DlgColor.cpp:DlgColor.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CDlgColor:CDlgColor
# 	2:19:Application Include:Dip.h
# 	2:10:DlgColor.h:DlgColor.h
# End Section
