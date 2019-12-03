/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: // code06-02-dilate
  作  者: // 胡少军、何东健、耿楠等
  日  期: // 2014年8月4日
  描  述: // 二值图像膨胀运算的OpenCV实现
  函数列表:
    1. void Dilation( int, void* ); //对二值图像进行膨胀运算
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

// 全局变量
Mat srcImg, binaryImg, dilationDst;

int dilationElem = MORPH_RECT;
int dilationSize = 1;
int const maxElem = 2;
int const maxKernelSize = 21;

void Dilation( int, void* )
{
  int elemType = MORPH_RECT;
  if( dilationElem == 0 ){ elemType = MORPH_RECT; }
  else if( dilationElem == 1 ){ elemType = MORPH_CROSS; }
  else if( dilationElem == 2) { elemType = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( elemType,
                                       Size( 2*dilationSize + 1, 2*dilationSize+1 ),
                                       Point( dilationSize, dilationSize ) );
  //膨胀操作
  dilate( binaryImg, dilationDst, element );
  imwrite("..\\figures\\figure6_9_c.bmp", dilationDst);
  imshow( "Dilation", dilationDst );
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

  // 创建显示窗口
  namedWindow( "Dilation", CV_WINDOW_AUTOSIZE );

  // 创建滚动条
  createTrackbar( "结构元素", "Dilation",
                  &dilationElem, maxElem,
                  Dilation );

  createTrackbar( "大小", "Dilation",
                  &dilationSize, maxKernelSize,
                  Dilation );

  Dilation( 0, 0 );

  waitKey(0);
  return 0;
}
