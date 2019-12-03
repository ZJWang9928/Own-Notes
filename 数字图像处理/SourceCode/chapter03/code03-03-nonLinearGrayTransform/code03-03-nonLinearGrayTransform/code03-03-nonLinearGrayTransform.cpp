/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-03-nonLinearGrayTransform.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年6月28日
  描  述: 利用对数函数、指数函数、幂函数等实现非线性灰度变换。
  其  它:
  函数列表:
    1. bool nonLinearGrayTransform() 循环平移矩阵的行列数据
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/06/28    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

/**
  灰度变换函数基类
 */
class GrayTransform
{
public:
	virtual uchar operator()(uchar gray)
	{
		return 0;
	}
	virtual ~GrayTransform()
	{
	}
};

/**
  对数变换函数类：y = a + log(x+1)/(c*logb)
  如：a = 0, b = 2.0, c = 8/255 = 0.03137
 */
class LogGrayTransform : public GrayTransform
{
public:
	LogGrayTransform(float fA, float fB, float fC) : a(fA), b(fB), c(fC)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(a + logf(gray + 1.0f) / (c * logf(b)));
	}
private:
	/// 对数变换函数参数
	float a, b, c;
};

/**
  指数变换函数类：y = b^[c*(x-a)] - 1
  如：a = 0, b = 2.0, c = 8/255 = 0.03137
 */
class ExpGrayTransform : public GrayTransform
{
public:
	ExpGrayTransform(float fA, float fB, float fC) : a(fA), b(fB), c(fC)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(powf(b, c * (gray - a)) - 1.0f);
	}
private:
	/// 指数变换函数参数
	float a, b, c;
};

/**
  幂律变换函数类：y = c * x^gama
  如：gama = 0.4, c = 255^(1-gama) = 27.792
 */
class PowGrayTransform : public GrayTransform
{
public:
	PowGrayTransform(float fC, float fGama) : c(fC), gama(fGama)
	{
	}

	uchar operator()(uchar gray)
	{
		return uchar(c * powf(gray, gama));
	}
private:
	/// 幂律变换函数参数
	float c, gama;
};

/*************************************************
  名    称: bool nonLinearGrayTransform(const Mat& srcImg, Mat& dstImg, GrayTransform& grayFunc)
  描    述: 根据指定的灰度变换函数实现非线性灰度变换
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: const Mat& srcImg-->输入图像（CV_8U）
            GrayTransform& grayFunc-->非线性灰度变换的函数对象，可充当形如uchar (*pF)(uchar gray)的函数
  输出参数: Mat& dstImg-->输出图像（CV_8U）
  返    回: 成功则返回true，否则返回false
  其    它: 无
  作    者：龙满生
  日    期：2014年6月28日
*************************************************/
bool nonLinearGrayTransform(const Mat& srcImg, Mat& dstImg, GrayTransform& grayFunc)
{
	/// 为了简化代码，暂时只处理无符号字符型图像
	if(srcImg.empty() || srcImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return false;
	}
	
	/// 图像通道数、行数、列数
	int iChns, iRows, iCols;
	iChns = srcImg.channels();
	iRows = srcImg.rows;
	iCols = srcImg.cols * iChns;
	
	if(srcImg.isContinuous())
	{
		iCols *= iRows;
		iRows = 1;
	}

	/// 初始化目标图像
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	/// 实施非线性灰度变换
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = grayFunc(p[c]);
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;
	
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 输入需要变换的函数类型及其对应参数
	int funcType = 1;
	float a, b, c;
	GrayTransform *pGrayFunc = NULL;
	cout << "Enter the transform function type: \n"
		<< "1-Logarithmic function;\n"
		<< "2-Exponential function;\n"
		<< "3-Power function;\n: ";
	cin >> funcType;
	switch (funcType)
	{
	case 1:
		cout << "Enter parameters a, b and c in logarithmic function y = a + log(x+1)/(c*logb): ";
		cin >> a >> b >> c;
		pGrayFunc = new LogGrayTransform(a, b, c);
		break;
	case 2:
		cout << "Enter parameters a, b and c in exponential function y = b^[c*(x-a)] - 1: ";
		cin >> a >> b >> c;
		pGrayFunc = new ExpGrayTransform(a, b, c);
		break;
	case 3:
		cout << "Enter parameters c and gama in power function y = c * x^gama: ";
		cin >> a >> b;
		pGrayFunc = new PowGrayTransform(a, b);
		break;
	default:
		pGrayFunc = new GrayTransform();
		break;
	}
	
	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 灰度线性变换
	nonLinearGrayTransform(srcImg, dstImg, *pGrayFunc);

	/// 显示变换图像
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	/// 销毁函数对象
	delete pGrayFunc;

	return 0;
}

