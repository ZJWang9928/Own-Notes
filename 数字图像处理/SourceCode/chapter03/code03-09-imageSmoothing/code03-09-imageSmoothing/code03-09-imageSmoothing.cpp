/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-09-imageSmoothing.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月12日
  描  述: 实现图像平滑（包括均值滤波、双边滤波、高斯滤波、中值滤波）。
  其  它:
  函数列表:
    1. void onKernelSizeTrackbar() 滑块条的回调函数
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/12    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// 窗口名称
const string szWinName = "Smoothing Demo";

// 平滑核最大半径
const int maxKernelRadius = 15;

// 平滑核半径
int kernelRadius = 1;

// 平滑方法
char smoothMethod = 'M';

/// 平滑核尺寸的回调函数
void onKernelSizeTrackbar(int iVal, void* pUserData);

int main(int argc, char** argv)
{
	cout << "用法：H - 均值滤波；B - 双边滤波；G - 高斯滤波；M - 中值滤波；Esc - 退出。" << endl;

	/// 加载原始图像
	Mat srcImg;
	string srcImgName;
	cout << "Enter the source image file name: ";
	cin >> srcImgName;
	srcImg = imread(srcImgName);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image." << endl;
		return -1;
	}

	/// 显示原始图像
	namedWindow(szWinName);
	imshow(szWinName, srcImg);

	/// 创建平滑核尺寸的滑块
	createTrackbar("平滑核半径", szWinName, &kernelRadius, maxKernelRadius, onKernelSizeTrackbar, &srcImg);

	/// 根据按键设定平滑方法
	int keyVal = 0;
	while(1)
	{
		keyVal = waitKey(0);
		if(keyVal == 27)
		{
			break;
		}

		switch (keyVal)
		{
		case 'H':
		case 'h':
			smoothMethod = 'H';
			break;
		case 'B':
		case 'b':
			smoothMethod = 'B';
			break;
		case 'G':
		case 'g':
			smoothMethod = 'G';
			break;
		case 'M':
		case 'm':
			smoothMethod = 'M';
			break;
		default:
			smoothMethod = 'H';
			break;
		}

		/// 显示平滑结果
		onKernelSizeTrackbar(kernelRadius, &srcImg);
	}

	return 0;
}

/**
* 平滑核半径滑块的回调函数
* @param iVal 滑块位置（从0开始），代表平滑核半径
* @param pUserData 滑块事件的用户数据，此处用作原图像矩阵
*/
void onKernelSizeTrackbar(int iVal, void* pUserData)
{
	/// 从滑块事件的用户数据中获取原图像数据
	Mat* pSrcImg = (Mat*)pUserData;
	if(pSrcImg == NULL)
	{
		return;
	}

	/// 基于原图像数据创建目标图像
	Mat dstImg = pSrcImg->clone();

	/// 平滑核窗口尺寸
	int kernelSize = 2 * iVal + 1;

	/// 根据按键选择相应平滑方法实现图像平滑
	switch (smoothMethod)
	{
	case 'H':	// Homogeneous Blur（均值滤波）
		blur(*pSrcImg, dstImg, Size(kernelSize, kernelSize), Point(-1, -1));
		putText(dstImg, "Homogeneous Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	case 'B':	// Bilateral Blur（双边滤波）
		bilateralFilter(*pSrcImg, dstImg, kernelSize, kernelSize*2, kernelSize/2); 
		putText(dstImg, "Bilateral Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break; 
	case 'G':	// Gaussian Blur（高斯平滑）
		GaussianBlur(*pSrcImg, dstImg, Size(kernelSize, kernelSize), 0, 0);
		putText(dstImg, "Gaussian Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	case 'M':	// Median Blur（中值滤波）
		medianBlur(*pSrcImg, dstImg, kernelSize);
		putText(dstImg, "Median Blur", Point(dstImg.cols/4, dstImg.rows/2), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255,255,255));
		break;
	}

	/// 显示平滑结果
	imshow(szWinName, dstImg);
}