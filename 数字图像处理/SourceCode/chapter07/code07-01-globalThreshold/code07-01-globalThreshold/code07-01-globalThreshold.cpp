/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-01-globalThreshold.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月24日
  描  述: 实现全局阈值分割，包括迭代阈值法、Otsu阈值法、p参数阈值法。
  函数列表:
    1. double iterativeThreshold() 迭代阈值法
    2. double otsuThreshold() Otsu阈值法
    3. double pTileThreshold() p参数阈值法
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/24     1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// 迭代阈值
double iterativeThreshold(const Mat &grayImg);

/// Otsu阈值
double otsuThreshold(const Mat &grayImg);

/// p参数阈值法
double pTileThreshold(const Mat &grayImg, double objRatio);

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;
	
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	///// 计算阈值（迭代阈值法）
	//double th = iterativeThreshold(srcImg);

	/// 计算阈值（Otsu阈值法）
	//double th = otsuThreshold(srcImg);

	/// 计算阈值（p-参数阈值法）
	double th = pTileThreshold(srcImg, 0.65);

	cout << "分割阈值：" << th << endl; 

	/// 阈值分割
	threshold(srcImg, dstImg, th, 255, THRESH_BINARY);
	
	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 显示分割图像
	namedWindow("Segmented Image");
	imshow("Segmented Image", dstImg);

	waitKey();

	return 0;
}

/**
 * 利用迭代阈值法计算灰度图像的阈值
 * @param grayImg 单通道灰度图像
 * @return 灰度阈值
 */
double iterativeThreshold(const Mat &grayImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// 计算灰度直方图
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// 查找图像中的最小、最大灰度值
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg(maxGray) <= 0; maxGray--);

	/// 如果图像只有1种或2种灰度，则直接返回最小灰度
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// 以最小与最大灰度值的中间值作为初始阈值
	double thOld = (minGray + maxGray) / 2;

	/// 迭代求解阈值
	double thNew = thOld;
	double leftMean, rightMean, leftSum, rightSum;
	bool bDone = false;
	while(!bDone)
	{
		// 根据上一步的阈值将图像分割为两部分并求出两部分的灰度均值
		leftMean = 0;
		leftSum = 0;
		int i = minGray;
		for(; i <= thOld; i++)
		{
			leftSum += histImg(i);
			leftMean += histImg(i) * i;
		}
		leftMean /= leftSum;

		rightMean = 0;
		rightSum = 0;
		for(; i <= maxGray; i++)
		{
			rightSum += histImg(i);
			rightMean += histImg(i) * i;
		}
		rightMean /= rightSum;

		// 基于两部分的灰度均值更新阈值
		thNew = (leftMean + rightMean) / 2.0;

		bDone = abs(thNew - thOld) < 1.0;
		thOld = thNew;
	}

	return thOld;
}

/**
 * 利用Otsu阈值法（最大类间方差法、大津法）计算灰度图像的阈值
 * @param grayImg 单通道灰度图像
 * @return 灰度阈值
 */
double otsuThreshold(const Mat &grayImg)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// 计算灰度直方图
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// 直方图归一化（即求得像素灰度的出现概率）
	int totPixels = grayImg.rows * grayImg.cols;
	histImg /= totPixels;

	/// 查找图像中的最小、最大灰度值
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg(maxGray) <= 0; maxGray--);

	/// 如果图像只有1种或2种灰度，则直接返回最小灰度
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// 求出整幅图像的平均灰度值
	double ut = 0;
	for(int i = minGray; i <= maxGray; i++)
	{
		ut += i * histImg(i);
	}

	/// 依次取[minGray, maxGray]中的值为灰度阈值，求出使类间方差最大的阈值
	double u0 = 0, u1 = 0, w0 = 0, w1 = 0, sigma = 0, maxSigma = 0, th = minGray;
	for(int i = minGray; i < maxGray; i++)
	{
		// 第1部分的灰度概率和灰度均值（为简化计算，此处的灰度均值没有除以灰度概率）
		w0 += histImg(i);
		u0 += histImg(i) * i;

		// 第2部分的灰度概率和灰度均值（为简化计算，此处的灰度均值没有除以灰度概率）
		w1 = 1.0 - w0;
		u1 = ut - u0;

		// 计算类间方差
		sigma = w0 * w1 * pow(u0/w0 - u1/w1, 2);

		// 求出最大类间方差及其对应的灰度阈值
		if(sigma > maxSigma)
		{
			maxSigma = sigma;
			th = i;
		}
	}

	return th;
}

/**
 * 利用p-参数法计算灰度图像的阈值
 * @param grayImg 单通道灰度图像
 * @param objPercent 前景像素（假定为高灰度）占像素总数的比例（0－1）
 * @return 灰度阈值
*/
double pTileThreshold(const Mat &grayImg, double objRatio)
{
	if(grayImg.empty() || grayImg.depth() != CV_8U)
	{
		cerr << "Invalid image format." << endl;
		return 0;
	}

	/// 计算灰度直方图
	const int histSize[] = {256};
	float grayRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges};
	int histChannels[] = {0};
	Mat_<float> histImg;
	calcHist(&grayImg, 1, histChannels, Mat(), histImg, 1, histSize, histRanges, true, false);

	/// 图像像素总数
	int totPixels = grayImg.rows * grayImg.cols;

	/// 如果背景像素占比达到指定值，则返回当前灰度值为分割阈值
	double backRatio = 1.0 - objRatio;
	double backPixels = 0;
	int i;
	for(i = 0; i < 255 && (backPixels <= backRatio * totPixels); i++)
	{
		backPixels += histImg(i);
	}

	return i;
}
