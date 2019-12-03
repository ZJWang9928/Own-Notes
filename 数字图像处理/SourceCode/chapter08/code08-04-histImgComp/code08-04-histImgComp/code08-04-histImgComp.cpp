/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-04-histImgComp.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现直方图匹配。
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
#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	Mat srcBas,srcTest1;

	// 装载两张背景环境不同的图像
	string firstFile, secondFile;
	cout << "Enter the first image file name: ";
	cin >> firstFile;
	cout << "Enter the second image file name: ";
	cin >> secondFile;
	
	srcBas=imread(firstFile);
	if(srcBas.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	srcTest1=imread(secondFile);
	if(srcTest1.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}

	/// 在RGB图像空间，量化为16*16*16
	int rBins = 16; int gBins = 16;int bBins = 16;
	int histSize[] = { rBins, gBins, bBins};

	// 等间隔量化
	float rRngs[] = { 0, 256 };
	float gRngs[] = { 0, 256 };
	float bRngs[] = { 0, 256 };

	const float* ranges[] = { rRngs, gRngs, bRngs };

	// 使用第0，1，2通道
	int channels[] = { 0, 1, 2 };

	/// 直方图
	MatND histBas;
	MatND histTes1;

	/// 计算RGB图像的直方图
	calcHist( &srcBas, 1, channels, Mat(), histBas, 2, histSize, ranges, true, false );
	normalize( histBas, histBas, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &srcTest1, 1, channels, Mat(), histTes1, 2, histSize, ranges, true, false );
	normalize( histTes1, histTes1, 0, 1, NORM_MINMAX, -1, Mat() );

	///应用不同的直方图对比方法
	for( int i = 0; i < 4; i++ )
	{ 
		int compare_method = i;
		double base_test1 = compareHist( histBas, histTes1, compare_method );
		cout<<" Method "<<i<<" Base-Test1  "<< base_test1<<endl;
	}
	
	imshow("HistMatch",srcBas);  
	waitKey(-1);
  return(0);
}