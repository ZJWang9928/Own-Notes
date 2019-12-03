/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-11-meanshiftSegmentation.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月9日
  描  述: 基于Meanshift算法的图像的分割。
  其  它:
  参考文献:
    1. OpenCV样例代码：samples/cpp/meanshift_segmentation.cpp
  函数列表:
    1. void floodFillSegmentation() 根据Meanshift算法的运行结果（收敛点）填充图像区域
    2. void meanShiftSegmentation() 滑块的回调函数，实现Meanshift分割与显示分割结果
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/09    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/// 坐标空间的核函数窗口半径
int spatialRadius = 10;

/// 颜色空间的核函数窗口半径
int colorRadius = 10;

/// 分割金字塔的最大层数
int maxPyrLevel = 1;

/// 输入图像
Mat srcImg;

/// 输出图像
Mat dstImg;

/// 窗口名称
string winName = "Meanshift";

/**
 * 根据Meanshift算法的运行结果（收敛点）填充图像区域
 * @param img Meanshift算法的输出结果
 * @param colorDiff 种子填充时相邻像素的颜色最大差异
 */
static void floodFillSegmentation(Mat& img, const Scalar& colorDiff = Scalar::all(1))
{
	CV_Assert(!img.empty());
	RNG rng = theRNG();
	Mat mask(img.rows+2, img.cols+2, CV_8UC1, Scalar::all(0));
	for(int y = 0; y < img.rows; y++)
	{
		for(int x = 0; x < img.cols; x++)
		{
			if(mask.at<uchar>(y+1, x+1) == 0)
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(img, mask, Point(x,y), newVal, 0, colorDiff, colorDiff);
			}
		}
	}
}

/**
 * 滑块的回调函数，实现Meanshift分割与显示分割结果
 */
static void meanShiftSegmentation(int, void*)
{
	cout << "spatialRad=" << spatialRadius << "; "
		 << "colorRad=" << colorRadius << "; "
		 << "maxPyrLevel=" << maxPyrLevel << endl;

	pyrMeanShiftFiltering(srcImg, dstImg, spatialRadius, colorRadius, maxPyrLevel);
	floodFillSegmentation(dstImg, Scalar::all(2));

	imshow(winName, dstImg);
}

int main(int argc, char** argv)
{
	/// 加载图像
	string fileName;
	cout << "Enter the source image file name: " << endl;
	cin >> fileName;
	srcImg = imread(fileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << fileName << endl;
		return -1;
	}
	imshow("Original image", srcImg);

	/// 创建窗口
	namedWindow(winName);

	/// 创建滑块条并设置相应的回调函数
	createTrackbar("spatialRad", winName, &spatialRadius, 80, meanShiftSegmentation);
	createTrackbar("colorRad", winName, &colorRadius, 60, meanShiftSegmentation);
	createTrackbar("maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation);

	/// 初始分割
	meanShiftSegmentation(0, 0);

	waitKey();

	return 0;
}