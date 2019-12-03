/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-08-templateConvolution.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月12日
  描  述: 利用filter2D函数在空间域实现模板卷积。
  其  它: 
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/11    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
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

	/// 输入模板原点 
	Point kernelOrigin;
	cout << "Enter the convolution template origin(x and y): ";
	cin >> kernelOrigin.x >> kernelOrigin.y;

	/// 输入模板尺度
	Size kernelSize;
	cout << "Enter the convolution template size(w and h): ";
	cin >> kernelSize.width >> kernelSize.height;

	/// 输入模板比例（分母）
	double kernelScale = 1.0;
	cout << "Enter the kernel scale(denominator): ";
	cin >> kernelScale;

	/// 输入模板系数
	Mat kernelCoef;
	kernelCoef = Mat::zeros(kernelSize, CV_32F);
	cout << "Enter the kernel coefficients: " << endl;
	for(int r = 0; r < kernelSize.height; r++)
	{
		for(int c = 0; c < kernelSize.width; c++)
		{
			cin >> kernelCoef.at<float>(r, c);
		}
	}

	/// 进行模板卷积运算
	Mat dstImg;
	kernelCoef /= kernelScale;
	filter2D(srcImg, dstImg, -1, kernelCoef, kernelOrigin, 0, BORDER_DEFAULT);

	/// 显示卷积前后的图像
	const string srcWinName = "Source image";
	const string dstWinName = "Result image";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(dstWinName);
	imshow(dstWinName, dstImg);

	waitKey(0);

	return 0;
}