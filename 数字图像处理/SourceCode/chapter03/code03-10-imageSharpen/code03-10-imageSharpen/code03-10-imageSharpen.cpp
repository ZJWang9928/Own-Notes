/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-10-imageSharpen.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月18日
  描  述: 在空间域实现图像锐化。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/18    1.0     建立该文件
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
	const string dstWinName = "Gradient image";

	/// 加载原始图像
	Mat srcImg, dstImg, hortGrad, vertGrad;
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
	imshow(srcWinName, srcImg);

	/// 应用Sobel水平梯度算子
	cv::Sobel(srcImg, hortGrad, CV_8U, 1, 0, 3, 0.4, 128);
	
	/// 显示Sobel水平梯度锐化图像
	imshow(dstWinName, hortGrad);
	cout << "Sobel 水平梯度算子" << endl;
	waitKey(0);

	/// 应用Sobel垂直梯度算子
	cv::Sobel(srcImg, vertGrad, CV_8U, 0, 1, 3, 0.4, 128);
	
	/// 显示Sobel垂直梯度锐化图像
	imshow(dstWinName, vertGrad);
	cout << "Sobel 垂直梯度算子" << endl;
	waitKey(0);

	/// 计算L1 norm：L=abs(Gx)+abs(Gy)
	dstImg = abs(hortGrad) + abs(vertGrad);

	/// 显示Sobel梯度幅度图像L1 norm
	imshow(dstWinName, dstImg);
	cout << "Sobel 梯度幅度（L1 Norm）" << endl;
	waitKey(0);

	/// 计算梯度幅度L2 norm：L=sqrt(Gx*Gx + Gy*Gy)和梯度方向
	cv::Mat gradNorm, gradDir;
	cv::Sobel(srcImg, hortGrad, CV_32F, 1, 0);	// 水平模板
	cv::Sobel(srcImg, vertGrad, CV_32F, 0, 1);	// 垂直模板
	cv::cartToPolar(hortGrad, vertGrad, gradNorm, gradDir);	// 计算梯度幅度和方向

	/// 显示Sobel梯度幅度L2 norm
	imshow(dstWinName, gradNorm);
	cout << "Sobel 梯度幅度（L2 Norm）" << endl;
	waitKey(0);

	/// 显示Sobel梯度方向
	imshow(dstWinName, gradDir);
	cout << "Sobel 梯度方向" << endl;
	waitKey(0);

	/// 应用Laplacian算子
	cv::Laplacian(srcImg, gradNorm, CV_16S, 3);
	cv::convertScaleAbs(gradNorm, dstImg);

	/// 显示Laplacian结果
	imshow(dstWinName, dstImg);
	cout << "Laplacian 结果" << endl;
	waitKey(0);

	/**
	 * 可以通过filter2D函数以模板卷积的方式实现Robert、Prewitt、Sobel、
	 * Krisch、Isotropic Sobel、Laplacian等算子的锐化，请读者自行编写。
	*/
	return 0;
}