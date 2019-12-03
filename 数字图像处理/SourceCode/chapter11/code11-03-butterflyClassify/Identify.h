// Identify.h: interface for the CIdentify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_)
#define AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////
//功能：EdgePoint是一个存储边界跟踪中边界点的坐标、当前矢量、次矢量的结构。
//      TraceArray为存储边界跟踪结果的元素为EdgePoint型的模板数组。
//说明：(1)当前矢量：前一个边界点到当前边界点的矢量链码;
//      (2)次矢量：边界上当前点到下一个边界点的矢量链码;
//	    (3)程序中采用的8方向链码，其方向定义如下：
//            3     2     1
//            4     P     0
//            5     6     7
//      (4)边界跟踪时，从下到上，从左到右，逆时针进行。
//////////////////////////////////////////////////////////////////////////////
struct EdgePoint
{
	BYTE nCurVerct;			//当前矢量，即在轮廓跟踪中的前一个搜索方向
	CPoint CurPoint;		//当前点的坐标
};

static CArray<EdgePoint,EdgePoint&> TraceArray;

class CIdentify : public CDibObject  
{
public:

	//定义昆虫相关参数
	float m_antennaPara;
	float m_bellyPara;
	int   m_swingPara;
	float m_antennaFactor;
	float m_bellyFactor;

	//边界跟踪
	double GetPeriByChainCode();
	BOOL EdgeTrace();
	//轮廓提取函数
	BOOL ContourTrace();

	//获取图象质心和二阶中心矩
	float GetSquare();
	POINT GetCentroid();

	//得到昆虫的各种参数函数
	int GetSwingPara();      
	float GetBellyPara();
	float GetAntennaPara();
	float GetArea();
	float GetEccentricity();

	//得到昆虫的形态因子函数
	float GetAntennaFactor();
	float GetBellyFactor();

	//昆虫分割函数
	BOOL SwingSep();
	BOOL BellySep();
	BOOL AntennaSep();

	//定义对象指针函数
	CIdentify * m_pIndentify;

	//构造函数
	CIdentify();
	CIdentify(CDibObject *);
	//析构函数
	virtual ~CIdentify();

};

#endif // !defined(AFX_IDENTIFY_H__1B8B902B_239B_45B5_A13A_F42BD0324522__INCLUDED_)
