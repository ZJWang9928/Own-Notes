/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-03-tempImgMat.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现模板图像匹配。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宁纪锋  2014/04/20    1.0     建立该文件
***********************************************************/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	Mat img; Mat temp; Mat result;

	int max_Trackbar = 5;

	// 装载原图像和模板图像
	string firstFile, secondFile;
	cout << "请输入原图像文件名: ";
	cin >> firstFile;
	cout << "请输入模板图像文件名: ";
	cin >> secondFile;
	
	img = imread(firstFile);
	if(img.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	temp = imread(secondFile);
	if(tmp.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}

	// 转换为灰度图像
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);
	if (temp.channels()>=3)
		cvtColor(temp,temp,CV_RGB2GRAY);

	// 创建窗口
	namedWindow( "Source Image", CV_WINDOW_AUTOSIZE );
	namedWindow( "Result window", CV_WINDOW_AUTOSIZE );	// 将被显示的原图像

	Mat imgDis;

	// 创建输出结果的矩阵
	int resultCols =  img.cols - temp.cols + 1;
	int resultRows = img.rows - temp.rows + 1;

	result.create( resultCols, resultRows, CV_32FC1 );

	/// 进行匹配和标准化
	int i;   // i:模板匹配方法
	for (i=0;i<5;i++)
	{
		matchTemplate( img, temp, result, i );
		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

		// 通过函数 minMaxLoc 定位最匹配的位置
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		// 对于方法 SQDIFF 和 SQDIFF_NORMED, 取最小值作为匹配结果，而对于其他方法, 最大值作为匹配结果
		if( i  == CV_TM_SQDIFF || i == CV_TM_SQDIFF_NORMED )
		{
			matchLoc = minLoc; 
		}
		else
		{ 
			matchLoc = maxLoc; 
		}

		img.copyTo( imgDis );

		// 显示最终结果
		rectangle( imgDis, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(255), 2, 8, 0 );
		rectangle( result, matchLoc, Point( matchLoc.x + temp.cols , matchLoc.y + temp.rows ), Scalar::all(255), 2, 8, 0 );

		imshow( "Source Image", imgDis );
		imshow( "Result window", result );

		waitKey(0);
	}
	return 0;
}