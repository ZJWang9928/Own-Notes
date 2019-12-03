/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-03-adaptiveThreshold.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月24日
  描  述: 分块自适应阈值分割（动态阈值）。
  函数列表:
    1. double otsuThreshold() Otsu阈值法
    2. void blockAdaptiveThreshold() 分块自适应阈值分割
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

/// Otsu阈值
double otsuThreshold(const Mat &grayImg);

/// 分块自适应阈值分割
void blockAdaptiveThreshold(const Mat &srcImg, Mat& dstImg, Size blockSize = Size(64, 64));

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

	/// 自适应阈值分割
	/**
		void adaptiveThreshold(InputArray src, OutputArray dst, double maxValue, int adaptiveMethod,
				int thresholdType, int blockSize, double C)
	*/
	//adaptiveThreshold(srcImg, dstImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);
	//adaptiveThreshold(srcImg, dstImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 5);

	/// 分块自适应阈值分割
	blockAdaptiveThreshold(srcImg, dstImg);

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
 * 分块自适应阈值分割
 * @param srcImg 原始图像
 * @param dstImg 分割结果
 * @param blockSize 分块尺寸
 */
void blockAdaptiveThreshold(const Mat &srcImg, Mat& dstImg, Size blockSize)
{
	dstImg = srcImg.clone();

	/// 计算分块数目
	int xBlocks = (srcImg.cols + blockSize.width - 1) / blockSize.width;
	int yBlocks = (srcImg.rows + blockSize.height - 1) / blockSize.height;

	/// 逐块阈值分割
	for(int y = 0; y < yBlocks-1; y++)
	{
		for(int x = 0; x < xBlocks-1; x++)
		{
			// 获得分块数据
			Mat blockImg = dstImg(Rect(x * blockSize.width, y * blockSize.height, blockSize.width, blockSize.height));

			// 计算分块阈值
			double t = otsuThreshold(blockImg);

			// 实现分块分割
			threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
		}
	}
	for(int x = 0; x < xBlocks-1; x++)
	{// 下侧分块
		// 获得分块数据
		Mat blockImg = dstImg(Rect(x * blockSize.width, (yBlocks - 1) * blockSize.height, blockSize.width, dstImg.rows - (yBlocks - 1) * blockSize.height));

		// 计算分块阈值
		double t = otsuThreshold(blockImg);

		// 实现分块分割
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
	for(int y = 0; y < yBlocks-1; y++)
	{// 右侧分块
		// 获得分块数据
		Mat blockImg = dstImg(Rect((xBlocks - 1) * blockSize.width, y * blockSize.height, dstImg.cols - (xBlocks - 1) * blockSize.width, blockSize.height));

		// 计算分块阈值
		double t = otsuThreshold(blockImg);

		// 实现分块分割
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
	{// 右下角分块
		// 获得分块数据
		Mat blockImg = dstImg(Rect((xBlocks - 1) * blockSize.width, (yBlocks - 1) * blockSize.height,
			dstImg.cols - (xBlocks - 1) * blockSize.width, dstImg.rows - (yBlocks - 1) * blockSize.height));

		// 计算分块阈值
		double t = otsuThreshold(blockImg);

		// 实现分块分割
		threshold(blockImg, blockImg, t, 255, THRESH_BINARY);
	}
}
