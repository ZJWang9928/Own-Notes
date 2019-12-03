/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-11-highBoostFiltering.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月19日
  描  述: 实现高频增强滤波。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/19    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	const string srcWinName = "Source image";
	const string dstWinName = "High-Boosted image";
	Mat srcImg, dstImg;

	/// 加载原始图像
	string srcImgName;
	cout << "Enter the source image file name: ";
	cin >> srcImgName;
	srcImg = imread(srcImgName);
	cvtColor(srcImg, srcImg, CV_RGB2GRAY);

	/// 显示原始图像
	imshow("Source Image", srcImg);

	///// 输入原始图像与高频图像的加权系数
	//float srcImgCoef = 1.0f, highPassCoef = 0.0f;
	//cout << "Enter the coefficient of source image(>=0): ";
	//cin >> srcImgCoef;
	//cout << "Enter the coefficient of high-pass image(0-1):";
	//cin >> highPassCoef;

	/// 拉普拉斯高斯滤波
	float laplacianMaskData[][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
	Mat laplacianMask(3, 3, CV_32F, &laplacianMaskData[0]);
	filter2D(srcImg, dstImg, CV_8U, laplacianMask);
	imshow("Laplacian Sharpen Image", dstImg);

	/// 拉普拉斯高频增强滤波
	//dstImg = dstImg + srcImg;
	/** 或者使用下面的模板对原始图像进行模板卷积 **/
	laplacianMask.at<float>(1, 1) = 9;
	filter2D(srcImg, dstImg, CV_8U, laplacianMask);
	imshow("Laplacian High-Boost Image", dstImg);

	/// 高斯平滑高频增强滤波
	// 高斯平滑
	cv::GaussianBlur(srcImg, dstImg, Size(5, 5), 1.5, 1.5);
	// 非锐化滤波
	dstImg = srcImg - dstImg;
	// 高频增强滤波
	dstImg = srcImg + 0.6*dstImg;
	imshow("Gaussian High-Boost Image", dstImg);

	cv::waitKey(0);

	return 0;
}