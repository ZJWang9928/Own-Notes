/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-01-linearGrayTransform.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年6月27日
  描  述: 通过指定输入与输出的灰度范围实现灰度线性变换。
  其  它:
  函数列表:
    1. bool linearGrayTransform() 灰度线性变换
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/06/27    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*************************************************
  名    称: bool linearGrayTransform(const Mat& srcImg, Mat& dstImg,
                   const Vec2i& srcGrayExt, const Vec2i& dstGrayExt)
  描    述: 实现灰度线性变换
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: const Mat& srcImg-->输入图像（CV_8U）
            const Vec2i& srcGrayExt-->变换前的灰度范围[a,b]
            const Vec2i& dstGrayExt-->变换后的灰度范围[c,d]
  输出参数: Mat& dstImg-->输出图像
  返    回: 成功则返回true，否则返回false
  其    它: 无
  作    者：龙满生
  日    期：2014年6月27日
*************************************************/
bool linearGrayTransform(const Mat& srcImg, Mat& dstImg, const Vec2i& srcGrayExt, const Vec2i& dstGrayExt)
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

	/// 根据输入的原始和目标灰度范围计算灰度线性变换的斜率和截距
	double alpha, beta;
	alpha = double((dstGrayExt[1] - dstGrayExt[0])) / double((srcGrayExt[1] - srcGrayExt[0]));
	beta = dstGrayExt[0] - alpha * srcGrayExt[0];

	/* 可用语句srcImg.convertTo(dstImg, -1, alpha, beta);代替下面的系列语句，实现灰度线性变换 */

	/// 初始化目标图像
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	/// 实施灰度线性变换
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = saturate_cast<uchar>(alpha * p[c] + beta);
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

	/// 输入变换前后的灰度范围
	Vec2i inGrayExt, outGrayExt;
	cout << "Enter the source gray extent[a b]: ";
	cin >> inGrayExt[0] >> inGrayExt[1];
	cout << "Enter the target gray extent[c d]: ";
	cin >> outGrayExt[0] >> outGrayExt[1];

	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 灰度线性变换
	linearGrayTransform(srcImg, dstImg, inGrayExt, outGrayExt);

	/// 显示变换图像
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	return 0;
}

