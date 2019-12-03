// Character.cpp: implementation of the CCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgPro.h"
#include "Character.h"
#include "ImgProDoc.h"
#include "ImgProView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Identify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacter::CCharacter()
{
}


CCharacter::~CCharacter()
{

}

float CCharacter::GetAntennaPara()
{


	return 0;
}

float CCharacter::GetBellyPara()
{
	return 1.0;
}

int CCharacter::GetSwingPara()
{
	return 1;
}
