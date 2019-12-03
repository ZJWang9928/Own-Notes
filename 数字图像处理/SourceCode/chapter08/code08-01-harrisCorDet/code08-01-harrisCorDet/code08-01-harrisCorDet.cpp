/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-01-harrisCorDet.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现Harris角点检测算法。
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

/** @function main */
int main( int argc, char** argv )
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName);
	if (img.empty()||img.data==NULL)
	{
		cout << "Failed to load image: " << srcFileName << endl;
		return 1;
	}


	// 转换为灰度图像
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);

	Mat dst, dstNorm,dstNorSca;
	dst = Mat::zeros( img.size(), CV_32FC1 );

	/// Global variables
	int thresh = 120;

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris( img, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

	/// Normalizing
	normalize( dst, dstNorm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dstNorm, dstNorSca );

	/// Drawing a circle around corners
	for( int i = 0; i < dstNorm.rows ; i++ )
	{ 
		for( int j = 0; j < dstNorm.cols; j++ )
		{
			if( (int) dstNorm.at<float>(i,j) > thresh )
			{
				circle( dstNorSca, Point(j, i ), 5,  Scalar(0), 2, 8, 0 );
			}
		}
	}

	imshow("Harris corner detect",dstNorSca);
	imshow("Original Image",img);

	waitKey(0);
	return(0);
}