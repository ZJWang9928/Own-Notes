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
# ADD LINK32 ImageLoad.lib mdv4501v.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

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

SOURCE=.\BtnST.cpp
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

SOURCE=.\DlgFouriorFilte.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoRota.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoTran.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGeoZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHitMiss.cpp
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

SOURCE=.\DlgMorph.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSeedFill.cpp
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

SOURCE=.\FreqPro.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoTrans.cpp
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

SOURCE=.\BtnST.h
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

SOURCE=.\DlgFouriorFilte.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoRota.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoTran.h
# End Source File
# Begin Source File

SOURCE=.\DlgGeoZoom.h
# End Source File
# Begin Source File

SOURCE=.\DlgHitMiss.h
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

SOURCE=.\DlgMorph.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.h
# End Source File
# Begin Source File

SOURCE=.\DlgProject.h
# End Source File
# Begin Source File

SOURCE=.\DlgSeedFill.h
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

SOURCE=.\FreqPro.h
# End Source File
# Begin Source File

SOURCE=.\GeoTrans.h
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

SOURCE=.\res\hitmiss_.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
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
# Begin Source File

SOURCE=.\mdv4501v.lib
# End Source File
# End Target
# End Project
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
# Section Dip : {99682F6C-6B2F-484A-B4BF-54C87490A279}
# 	2:15:CLASS: CFourior:CFourior
# 	2:7:dft.cpp:dft.cpp
# 	2:5:dft.h:dft.h
# 	2:15:CLASS: CComplex:CComplex
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {9D8D3438-2110-47B9-ADCA-036F4DC51ECC}
# 	1:15:IDD_DLG_GEORota:115
# 	2:16:Resource Include:resource.h
# 	2:14:DlgGeoRota.cpp:DlgGeoRota.cpp
# 	2:15:IDD_DLG_GEORota:IDD_DLG_GEORota
# 	2:10:ENUM: enum:enum
# 	2:18:CLASS: CDlgGeoRota:CDlgGeoRota
# 	2:12:DlgGeoRota.h:DlgGeoRota.h
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
# Section Dip : {5BED8D21-C967-4776-8F78-7D0A56B3450E}
# 	1:22:IDD_DLG_Fourior_Filter:118
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_DLG_Fourior_Filter:IDD_DLG_Fourior_Filter
# 	2:19:DlgFouriorFilte.cpp:DlgFouriorFilte.cpp
# 	2:23:CLASS: CDlgFouriorFilte:CDlgFouriorFilte
# 	2:17:DlgFouriorFilte.h:DlgFouriorFilte.h
# 	2:10:ENUM: enum:enum
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
# Section Dip : {ECF17037-AF09-49B8-A358-03FA027228D4}
# 	2:21:CLASS: CImageGeoTrans:CImageGeoTrans
# 	2:17:ImageGeoTrans.cpp:ImageGeoTrans.cpp
# 	2:15:ImageGeoTrans.h:ImageGeoTrans.h
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {2164712E-391B-4FC3-A36E-D8DEDA9E60F1}
# 	2:11:MorphProc.h:MorphProc.h
# 	2:16:CLASS: CMorphPro:CMorphPro
# 	2:13:MorphProc.cpp:MorphProc.cpp
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {A31CB364-1AF4-4BB3-89D7-97AE810A07AC}
# 	1:15:IDD_DLG_GEOZoom:117
# 	2:18:CLASS: CDlgGeoZoom:CDlgGeoZoom
# 	2:16:Resource Include:resource.h
# 	2:14:DlgGeoZoom.cpp:DlgGeoZoom.cpp
# 	2:12:DlgGeoZoom.h:DlgGeoZoom.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:dip.h
# 	2:15:IDD_DLG_GEOZoom:IDD_DLG_GEOZoom
# End Section
# Section Dip : {3B3D4D22-989E-47AC-BBE7-7D39F8F09287}
# 	2:15:CLASS: CFourior:CFourior
# 	2:7:dft.cpp:dft1.cpp
# 	2:5:dft.h:dft1.h
# 	2:15:CLASS: CComplex:CComplex
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {C19B376A-2C2B-4A23-917D-A6154271CF9B}
# 	2:15:CLASS: CFreqPro:CFreqPro
# 	2:12:ImagePro.cpp:ImagePro.cpp
# 	2:10:ImagePro.h:ImagePro.h
# 	2:19:Application Include:dip.h
# End Section
# Section Dip : {D49C3844-B40B-4F66-9BD9-ABEF664A05C0}
# 	1:13:IDD_DLG_MORPH:119
# 	2:16:CLASS: CDlgMorph:CDlgMorph
# 	2:16:Resource Include:resource.h
# 	2:10:DlgMorph.h:DlgMorph.h
# 	2:13:IDD_DLG_MORPH:IDD_DLG_MORPH
# 	2:10:ENUM: enum:enum
# 	2:12:DlgMorph.cpp:DlgMorph.cpp
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
# Section Dip : {379274BF-9E84-485D-B502-E7B6E0EFA88C}
# 	1:16:IDD_DLG_GEOTrans:116
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_DLG_GEOTrans:IDD_DLG_GEOTrans
# 	2:18:CLASS: CDlgGeoTran:CDlgGeoTran
# 	2:10:ENUM: enum:enum
# 	2:12:DlgGeoTran.h:DlgGeoTran.h
# 	2:19:Application Include:dip.h
# 	2:14:DlgGeoTran.cpp:DlgGeoTran.cpp
# End Section
