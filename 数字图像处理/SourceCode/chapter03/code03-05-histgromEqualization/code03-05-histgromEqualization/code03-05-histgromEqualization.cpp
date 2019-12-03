/**
 * 
 * @file HistogramEqualization.cpp
 * @author 龙满生(longmansheng@126.com)
 * @date 2014-7-1
 * @note 示例代码只针对灰度图像进行直方图均衡化。若希望对彩色图像进行直方图均衡化，
 * 则可以先用split函数分离颜色通道，再用equalizeHist函数分别对各个颜色通道进行直方图
 * 均衡化，然后用merge函数将各个通道合并为彩色图像。
 */
/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-05-histgromEqualization.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月1日
  描  述: 实现直方图均衡化
  其  它:
  函数列表:
    1. void createGrayHistogram() 创建灰度直方图
    2. void drawHistogram  绘制灰度直方图
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/01    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * 创建灰度直方图
 * @param srcImg 原始图像
 * @param grayHist 灰度直方图
 */
void createGrayHistogram(Mat srcImg, Mat& grayHist)
{
	///// 转换为灰度图像
	//cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// 计算灰度直方图
	int histSize = 256;			// 直方图的箱格数
	float range[] = {0, 255};
	const float* histRange = {range};	// 直方图各维的箱格的数值范围
	calcHist(&srcImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, true, false);
}

/**
 * 绘制灰度直方图
 * @param histWinName 用于显示灰度直方图的窗口名称
 * @param grayHist 灰度直方图数据
 */
void drawHistogram(const string &histWinName, Mat &grayHist)
{
	/// 创建直方图图像
	int histWidth = 512;	// 直方图宽度
	int histHeight = 400;	// 直方图高度
	int binWidth = cvRound((double)histWidth / grayHist.rows);	// 箱格宽度
	Mat histImg(histHeight, histWidth, CV_8UC3, Scalar(0,0,0));

	/// 规范化直方图
	normalize(grayHist, grayHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

	/// 绘制直方图
	//for(int i = 1; i < grayHist.rows; i++)
	//{
	//	line(histImg, Point(binWidth*(i-1), histHeight-cvRound(grayHist.at<float>(i-1))),
	//		Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
	//		Scalar(128, 128, 128), 2, 8, 0);
	//}
	for(int i = 0; i < grayHist.rows; i++)
	{
		line(histImg, Point(binWidth*(i), histHeight),
			Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	/// 显示直方图
	imshow(histWinName, histImg);
}

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;

	/// 加载原始图像
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image." << endl;
		return -1;
	}

	/// 转换为灰度图像
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// 直方图均衡化
	equalizeHist(srcImg, dstImg);

	/// 显示原始图像与均衡化结果
	const string srcWinName = "Source image";
	const string dstWinName = "Equalized image";
	namedWindow(srcWinName);
	namedWindow(dstWinName);
	imshow(srcWinName, srcImg);
	imshow(dstWinName, dstImg);

	/// 创建均衡化前后的灰度直方图
	Mat srcGrayHist, dstGrayHist;
	createGrayHistogram(srcImg, srcGrayHist);
	createGrayHistogram(dstImg, dstGrayHist);

	/// 显示原始图像与均衡化结果的直方图
	const string srcHistWinName = "Histogram of source image";
	const string dstHistWinName = "Histogram of equalized image";
	namedWindow(srcHistWinName);
	namedWindow(dstHistWinName);
	drawHistogram(srcHistWinName, srcGrayHist);
	drawHistogram(dstHistWinName, dstGrayHist);

	waitKey(0);

	return 0;
}