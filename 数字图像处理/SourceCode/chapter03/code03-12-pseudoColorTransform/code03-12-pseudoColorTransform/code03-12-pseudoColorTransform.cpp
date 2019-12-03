/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-12-pseudoColorTransform.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月19日
  描  述: 利用灰度变换法实现伪彩色变换。
  其  它:
  函数列表:
    1. void pseudoColorTransform() 利用灰度变换法实现伪彩色变换
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/19    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * 利用灰度变换法实现伪彩色变换
 * @param grayImg 变换前的8位灰度图像
 * @param colorImg 变换后的8位伪彩色图像
 * 红色变换：if x<L/2, then y=0; else if x<3L/4, then y=4x-2L; else y=L
 * 绿色变换：if x<L/4, then y=4x; else if x<3L/4, then y=L; else y=-4x+4L
 * 蓝色变换：if x<L/4, then y=L; else if x<L/2, then y=-2x+3L/2; else y=0
 */
void pseudoColorTransform(const Mat& grayImg, Mat& colorImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return;
	}

	float totL = 255.0f, quadL = totL/4.0f, halfL = totL/2.0f, quad3L = 3.0f*totL/4.0f;

	colorImg = cv::Mat::zeros(grayImg.size(), CV_8UC3);

	const uchar* pGrayData = NULL;
	Vec3b* pColorData = NULL;
	for(int r = 0; r < grayImg.rows; r++)
	{
		pGrayData = grayImg.ptr<uchar>(r);
		pColorData = colorImg.ptr<Vec3b>(r);

		for(int c = 0; c < grayImg.cols; c++)
		{
			if(pGrayData[c] < quadL)
			{
				pColorData[c][0] = totL;					// Blue
				pColorData[c][1] = 4.0f * pGrayData[c];		// Green
				pColorData[c][2] = 0;						// Red
			}
			else if(pGrayData[c] < halfL)
			{
				pColorData[c][0] = -2.0f * pGrayData[c] + totL + halfL;	// Blue
				pColorData[c][1] = totL;								// Green
				pColorData[c][2] = 0;									// Red
			}
			else if(pGrayData[c] < quad3L)
			{
				pColorData[c][0] = 0;				// Blue
				pColorData[c][1] = totL;		// Green
				pColorData[c][2] = 4.0f * pGrayData[c] - 2.0f * totL;	// Red
			}
			else
			{
				pColorData[c][0] = 0;								// Blue
				pColorData[c][1] = 4.0f * (totL - pGrayData[c]);	// Green
				pColorData[c][2] = totL;							// Red
			}
		}
	}
}

int main(int argc, char** argv)
{	
	/// 读入原始图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	cv::Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 转换为灰度图像
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// 显示原始图像
	imshow("Original Image", srcImg);

	/// 伪彩色变换
	cv::Mat dstImg;
	pseudoColorTransform(srcImg, dstImg);

	/// 显示变换图像
	imshow("Pseudo-Color Image", dstImg);

	waitKey();

	return 0;
}

