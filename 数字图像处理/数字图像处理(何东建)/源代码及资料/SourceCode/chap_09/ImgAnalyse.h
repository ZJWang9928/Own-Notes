// ImgAnalyse.h: interface for the CImgAnalyse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_)
#define AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_

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

//区域标记结果的结构体
struct LabelResult
{
	int nLabelNo;			//区域标记号
	CPoint StartPoint;		//标记区域的起始点
	CPoint CenterPoint;		//区域中心坐标
	DWORD LabelArea;		//标记区域的面积
};

static CArray<LabelResult,LabelResult&> LabelArray;

#include "DibObject.h"
#include "matlib.h"

class CImgAnalyse : public CObject  
{
	DECLARE_DYNCREATE(CImgAnalyse)
public:
	//获取标记区域的质心坐标
	POINT GetCentroid();
	//区域标记
	BOOL LabelArea(CDibObject *pDibObject = NULL);
	//求傅立叶描述子
	double * FourierFactor(int *nDimSize);
	//由边界链码计算封闭区域的周长
	BOOL PeriByChainCode(float m_fXScale,float m_fYScale,float m_fXYScale,float *fP);
	//利用格林公式由边界点坐标计算封闭区域面积
	BOOL AreaByGreen(float m_fXScale,float m_fYScale,float *fArea);
	//由边界链码求面积
	BOOL AreaByChain(float *fArea);
	//获取边界点数
	int GetEdgePointNum();
	//尺寸标定
	void Calibrate(float *X_Scale, float *Y_Scale, float *XY_Scale,float realSize);
	//边界跟踪
	BOOL EdgeTrace(CDibObject *pDibObject = NULL);
	//构造函数
	CImgAnalyse();
	CImgAnalyse(CDibObject * );

	//析构函数
	virtual ~CImgAnalyse();

	//设置DibObject类对象
	void SetDibObjectClass( CDibObject * );

	//诊断调试函数
	#ifdef _DEBUG
		virtual void Dump( CDumpContext &dc) const;
		virtual void AssertValid() const;
	#endif

protected:
	CDibObject * m_pDibObject;
	//定义MatLab矩阵变量
	Mm mFourier;

};

#endif // !defined(AFX_IMGANALYSE_H__74A2A985_E51E_43DC_893F_E82EC4C920A2__INCLUDED_)
