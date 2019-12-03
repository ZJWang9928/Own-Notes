/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-08-regionLabel.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月7日
  描  述: 利用OpenCV的findContours函数实现二值图像的轮廓提取，
          利用drawContours函数绘制并填充轮廓，从而实现区域标记功能。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/07    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
	/// 输入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName << endl;
		return -1;
	}

	/// 阈值分割
	Mat bwImg;
	cv::threshold(srcImg, bwImg, 50, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	namedWindow("Source image");
	imshow("Source image", bwImg);

	/// 轮廓提取
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( bwImg, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	/// 轮廓绘制（填充方式）
	Mat dstImg = Mat::zeros(bwImg.size(), CV_8UC3);
	if( !contours.empty() && !hierarchy.empty() )
	{
		// 遍历顶层轮廓，并以随机色绘制（填充方式）各轮廓
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			Scalar color((rand()&255), (rand()&255), (rand()&255));
			drawContours(dstImg, contours, idx, color, CV_FILLED, 8, hierarchy);
		}
	}
	namedWindow("Labeled image");
	imshow("Labeled image", dstImg);

	waitKey(0);
	return 0;
}
