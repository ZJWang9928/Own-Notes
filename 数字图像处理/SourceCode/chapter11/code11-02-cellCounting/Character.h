// Character.h: interface for the CCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTER_H__5A3BFA87_CED4_48C3_A19F_2DA89F2A65DE__INCLUDED_)
#define AFX_CHARACTER_H__5A3BFA87_CED4_48C3_A19F_2DA89F2A65DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DibObject.h"

class CCharacter : public CDibObject  
{
public:
	int GetSwingPara();
	float GetBellyPara();
	float GetAntennaPara();
	CCharacter();
	virtual ~CCharacter();

};

#endif // !defined(AFX_CHARACTER_H__5A3BFA87_CED4_48C3_A19F_2DA89F2A65DE__INCLUDED_)
