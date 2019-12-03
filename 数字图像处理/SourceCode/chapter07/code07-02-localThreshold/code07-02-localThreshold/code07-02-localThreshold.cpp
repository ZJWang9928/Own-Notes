/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-02-localThreshold.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月24日
  描  述: 实现局部阈值分割，包括灰度－梯度散射图的计算、基于
          灰度级平均梯度变换直方图。
  函数列表:
    1. Mat grayGradient() 计算梯度幅度
    2. Mat grayGradientScatter() 计算灰度－梯度散射图
    3. Mat histDivByAverGrad() 基于灰度级平均梯度变换直方图
    4. double otsuThreshold() Otsu阈值法
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

/// 计算梯度幅度
Mat grayGradient(const Mat &grayImg);

/// 计算灰度－梯度散射图
Mat grayGradientScatter(const Mat &grayImg);

/// 基于灰度级平均梯度变换直方图
Mat histDivByAverGrad(const Mat &scatImg);

/// 利用Otsu阈值法（最大类间方差法、大津法）计算灰度图像的阈值
double otsuThreshold(const Mat &histImg);

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

	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 计算灰度－梯度散射图
	Mat scatImg = grayGradientScatter(srcImg);

	///// 显示灰度－梯度散射图
	//scatImg.convertTo(dstImg, CV_8U);
	//cv::imshow("Gray-Gradient Scatter Image", dstImg);

	/// 基于灰度级平均梯度变换直方图
	Mat newHist = histDivByAverGrad(scatImg);
	for(int r = 0; r < newHist.rows; r++)
	{
		for(int c = 0; c < newHist.cols; c++)
		{
			cout << newHist.at<float>(r, c) << ",";
		}
		cout << endl;
	}
	cout << endl;

	/// 阈值分割
	double th = otsuThreshold(newHist);
	threshold(srcImg, dstImg, th, 255, THRESH_BINARY);
	
	/// 显示分割图像
	namedWindow("Segmented Image");
	imshow("Segmented Image", dstImg);

	waitKey();

	return 0;
}

/**
 * 计算灰度梯度幅度
 * @param grayImg 单通道灰度图像
 * @return 灰度梯度幅度（浮点型）
 */
Mat grayGradient(const Mat& grayImg)
{
	double dx, dy;
	int r, c, rows = grayImg.rows - 1, cols = grayImg.cols - 1;

	Mat gradMag = Mat::zeros(grayImg.size(), CV_32F);
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			dx = grayImg.at<uchar>(r, c+1) - grayImg.at<uchar>(r, c);
			dy = grayImg.at<uchar>(r+1, c) - grayImg.at<uchar>(r, c);

			gradMag.at<float>(r, c) = (float)sqrt(dx * dx + dy * dy);
		}
	}

	return gradMag;
}

/**
 * 计算灰度－梯度幅度散射图
 * @param grayImg 单通道灰度图像
 * @return 灰度－梯度幅度散射图（浮点型，行方向为灰度索引，列方向为梯度幅度索引）
 * @note 若要显示散射图，则应利用convertTo转换为CV_8U
 */
Mat grayGradientScatter(const Mat &grayImg)
{
	/// 计算灰度梯度幅度[0-255]
	Mat gradMag = grayGradient(grayImg);

	///// 计算最小最大梯度幅度
	//double minGrad, maxGrad;
	//cv::minMaxLoc(gradMag, &minGrad, &maxGrad);

	/// 计算灰度－梯度幅度直方图（散射图）
	Mat images[] = {Mat_<float>(grayImg), gradMag};
	const int histSize[] = {256, 256};
	float grayRanges[] = {0, 256};
	float gradRanges[] = {0, 256};
	const float* histRanges[] = {grayRanges, gradRanges};
	int histChannels[] = {0, 1};
	Mat histImg;
	calcHist(images, 2, histChannels, Mat(), histImg, 2, histSize, histRanges, true, false);

	return histImg;
}

/**
 * 基于灰度级平均梯度变换直方图
 * @param scatImg 灰度－梯度幅度散射图（浮点型，行方向为灰度索引，列方向为梯度幅度索引）
 * @return 除以灰度平均梯度的灰度直方图
 */
Mat histDivByAverGrad(const Mat &scatImg)
{
	/// 创建灰直方图
	Mat histImg = Mat::zeros(scatImg.rows, 1, CV_32F);

	/// 基于灰度－梯度散射图计算灰度平均梯度变换后的直方图
	for(int r = 0; r < scatImg.rows; r++)
	{
		int curGrayPixels = 0;		// 当前灰度的像素总数
		float averGrayGrad = 0;		// 当前灰度的平均梯度
		float pixelVal;
		for(int c = 0; c < scatImg.cols; c++)
		{
			pixelVal = scatImg.at<float>(r, c);

			if(pixelVal > 0)
			{
				curGrayPixels += pixelVal;
				averGrayGrad += c * pixelVal;
			}
		}

		if(curGrayPixels > 0)
		{
			averGrayGrad /= curGrayPixels;	// 灰度级平均梯度

			histImg.at<float>(r) = float(curGrayPixels / (averGrayGrad + 0.1f)); // 避免分母为0
		}
	}

	return histImg;
}

/**
 * 利用Otsu阈值法（最大类间方差法、大津法）计算灰度图像的阈值
 * @param histImg 灰度直方图
 * @return 灰度阈值
 */
double otsuThreshold(const Mat &histImg)
{
	/// 直方图归一化（即求得像素灰度的出现概率）
	Scalar totPixels = cv::sum(histImg);
	histImg /= totPixels(0);

	/// 查找图像中的最小、最大灰度值
	int minGray = 0, maxGray = 255;
	for(minGray = 0; minGray < 256 && histImg.at<float>(minGray) <= 0; minGray++);
	for(maxGray = 255; maxGray > minGray && histImg.at<float>(maxGray) <= 0; maxGray--);

	/// 如果图像只有1种或2种灰度，则直接返回最小灰度
	if(maxGray - minGray < 2)
	{
		return minGray;
	}

	/// 求出整幅图像的平均灰度值
	double ut = 0;
	for(int i = minGray; i <= maxGray; i++)
	{
		ut += i * histImg.at<float>(i);
	}

	/// 依次取[minGray, maxGray]中的值为灰度阈值，求出使类间方差最大的阈值
	double u0 = 0, u1 = 0, w0 = 0, w1 = 0, sigma = 0, maxSigma = 0, th = minGray;
	for(int i = minGray; i < maxGray; i++)
	{
		// 第1部分的灰度概率和灰度均值（为简化计算，此处的灰度均值没有除以灰度概率）
		w0 += histImg.at<float>(i);
		u0 += histImg.at<float>(i) * i;

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