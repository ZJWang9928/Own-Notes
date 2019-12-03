/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-04-createAndDrawHistogram.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月1日
  描  述: 创建并显示灰度直方图。
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/01     1.0     建立该文件
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
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	Mat srcImg;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 转换为灰度图像
	Mat grayImg;
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);

	/// 分离通道
	vector<Mat> bgrImg;
	split(srcImg, bgrImg);


	/**
	void calcHist(const Mat* images, int nimages, const int* channels,
	              InputArray mask, OutputArray	hist,
				  int dims, const int* histSize, const float** ranges,
				  bool uniform=true, bool accumulate=false )
	* images - 源图像数组
	* nimages - 源图像数目
	* channels - 用于计算直方图的通道列表（从0开始编号）
	* mask - 掩模图像（与非0元素对应的图像像素将用于计算直方图）
	* hist - 直方图（dims维矩阵）
	* dims - 直方图的维数（最大值为CV_MAX_DIMS，目前为32）
	* histSize - 直方图各维的大小（箱格数）
	* ranges - 直方图各维的箱格的数值范围（若箱格均匀分布，则每维只需两个值）
	* uniform - 直方图箱格是否均匀分布的标志
	* accumulate - 是否累加以前的直方图
	*/
	/// 计算直方图
	Mat bHist, gHist, rHist, grayHist;	// 直方图数据
	int histSize = 256;					// 直方图的箱格数
	float range[] = {0, 255};
	const float* histRange = {range};	// 直方图各维的箱格的数值范围
	calcHist(&grayImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, true, false);	// 亮度直方图
	//calcHist(&bgrImg[0], 1, 0, Mat(), bHist, 1, &histSize, &histRange, true, false);	// 蓝色通道直方图
	//calcHist(&bgrImg[1], 1, 0, Mat(), gHist, 1, &histSize, &histRange, true, false);	// 绿色通道直方图
	//calcHist(&bgrImg[2], 1, 0, Mat(), rHist, 1, &histSize, &histRange, true, false);	// 红色通道直方图

	// 也可用下面的语句计算红、绿、蓝通道的直方图，无需分离通道
	const int channels[] = {0, 1, 2}; // 依次为蓝、绿、红的颜色通道索引
	calcHist(&srcImg, 1, &channels[0], Mat(), bHist, 1, &histSize, &histRange, true, false);	// 蓝色通道直方图
	calcHist(&srcImg, 1, &channels[1], Mat(), gHist, 1, &histSize, &histRange, true, false);	// 绿色通道直方图
	calcHist(&srcImg, 1, &channels[2], Mat(), rHist, 1, &histSize, &histRange, true, false);	// 红色通道直方图

	/// 创建直方图图像
	int histWidth = 512;	// 直方图宽度
	int histHeight = 400;	// 直方图高度
	int binWidth = cvRound((double)histWidth / histSize);	// 箱格宽度
	Mat histImg(histHeight, histWidth, CV_8UC3, Scalar(0,0,0));

	/// 规范化直方图
	normalize(grayHist, grayHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// 亮度直方图
	normalize(bHist, bHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// 蓝色通道直方图
	normalize(gHist, gHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// 绿色通道直方图
	normalize(rHist, rHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());	// 红色通道直方图

	/// 绘制直方图
	for(int i = 1; i < histSize; i++)
	{
		// 亮度直方图
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(grayHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
			Scalar(128, 128, 128), 2, 8, 0);

		// 蓝色通道直方图
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(bHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(bHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

		// 绿色通道直方图
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(gHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(gHist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);

		// 红色通道直方图
		line(histImg, Point(binWidth*(i-1), histHeight-cvRound(rHist.at<float>(i-1))),
			Point(binWidth*i, histHeight-cvRound(rHist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// 显示原始图像和直方图
	const string srcWinName = "Source image";
	const string histWinName = "Gray histogram";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(histWinName);
	imshow(histWinName, histImg);

	waitKey(0);

	return 0;
}