/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-02-multiLinearGrayTransform.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年6月28日
  描  述: 通过指定两点的输入与输出灰度实现分段灰度线性变换（灰度拉伸）
  其  它:
  函数列表:
    1. bool multiLinearGrayTransform() 分段灰度线性变换（灰度拉伸）
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/06/28    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


/*************************************************
  名    称: bool multiLinearGrayTransform(const Mat& srcImg, Mat& dstImg, 
                 const Point2d& ptFirst, const Point2d& ptSecond)
  描    述: 实现分段灰度线性变换（灰度拉伸）
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: main()
  输入参数: const Mat& srcImg-->输入图像（CV_8U）
            const Point2d& ptFirst-->变换前的灰度范围[a,c]
            const Point2d& ptSecond-->变换后的灰度范围[b,d]
  输出参数: Mat& dstImg-->输出图像（CV_8U）
  返    回: 成功则返回true，否则返回false
  其    它: 无
  作    者：龙满生
  日    期：2014年6月28日
*************************************************/
bool multiLinearGrayTransform(const Mat& srcImg, Mat& dstImg, const Point2d& ptFirst, const Point2d& ptSecond)
{
	/// 为了简化代码，暂时只处理无符号字符型图像
	if(srcImg.empty() || srcImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return false;
	}
	
	/// 根据两点的输入输出灰度计算灰度变换的映射表
	uchar grayMap[256];	// 输入灰度-->输出灰度的映射表
	int i;
	
	// 针对第一段进行灰度映射
	for(i = 0; i <= ptFirst.x; i++)
	{
		if(ptFirst.x > 0)
		{
			grayMap[i] = uchar(ptFirst.y * i / ptFirst.x);
		}
		else
		{
			grayMap[i] = uchar(ptFirst.y);
		}
	}
	
	// 针对第二段进行灰度映射
	for(; i <= ptSecond.x; i++)
	{
		if(ptSecond.x > ptFirst.x)
		{
			grayMap[i] = uchar((ptSecond.y - ptFirst.y) * (i - ptFirst.x) / (ptSecond.x - ptFirst.x) + ptFirst.y);
		}
		else
		{
			grayMap[i] = uchar(ptSecond.y);
		}
	}

	// 针对第三段进行灰度映射
	for(; i < 256; i++)
	{
		if(ptSecond.x != 255)
		{
			grayMap[i] = uchar((255 - ptSecond.y) * (i - ptSecond.x) / (255 - ptSecond.x) + ptSecond.y);
		}
		else
		{
			grayMap[i] = 255;
		}
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

	/// 实施分段灰度线性变换
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = grayMap[p[c]];
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

	/// 输入两点的灰度坐标(a,c)和(b,d)
	Point2i ptFirst, ptSecond;
	cout << "Enter gray coordinate of the first point[a c]: ";
	cin >> ptFirst.x >> ptFirst.y;
	cout << "Enter gray coordinate of the second point[b d]: ";
	cin >> ptSecond.x >> ptSecond.y;

	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 灰度线性变换
	multiLinearGrayTransform(srcImg, dstImg, ptFirst, ptSecond);

	/// 显示变换图像
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	return 0;
}

