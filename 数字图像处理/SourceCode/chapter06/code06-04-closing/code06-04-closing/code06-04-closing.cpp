/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: // code06-04-closing
  作  者: // 胡少军、何东健、耿楠等
  日  期: // 2014年8月4日
  描  述: // 二值图像闭运算的OpenCV实现
  函数列表:
    1. void morphologyClosing( int, void* ); //对二值图像执行闭运算
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat srcImg, binaryImg, dstImg;

int elemType = MORPH_ELLIPSE;
int morphSize = 8;
const int maxType = MORPH_ELLIPSE;
const int maxKernelSize = 20;

char* windowName = "Morphology Closing";

void morphologyClosing( int, void* )
{
	// 设定结构元素
	Mat element = getStructuringElement( elemType, Size( 2*morphSize + 1, 2*morphSize+1 ),
																Point( morphSize, morphSize ) );
	// 执行闭运算
	morphologyEx( binaryImg, dstImg, MORPH_CLOSE, element );

	imwrite("..\\figures\\figure6_12_b.bmp", dstImg);
	imshow( windowName, dstImg );
}

int main( int argc, char** argv )
{
	/// 装载图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	Mat grayImg (srcImg.size(), CV_8U);

	//将源图像转化为灰度图像
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	binaryImg = grayImg;

	//二值化处理
	threshold(grayImg, binaryImg, 100, 255, THRESH_BINARY);

	namedWindow( windowName, CV_WINDOW_AUTOSIZE );

	createTrackbar( "结构类型", windowName,
							&elemType, maxType,
							morphologyClosing );

	createTrackbar( "大小", windowName,
							&morphSize, maxKernelSize,
							morphologyClosing );

	morphologyClosing( 0, 0 );

	waitKey(0);
	return 0;
}
