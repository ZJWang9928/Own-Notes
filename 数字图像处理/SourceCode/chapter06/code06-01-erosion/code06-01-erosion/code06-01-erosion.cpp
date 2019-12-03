/************************************************************
Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
文件名: // code06-01-erosion
作  者: // 胡少军、何东健、耿楠等
日  期: // 2014年8月4日
描  述: // 二值图像腐蚀运算的OpenCV实现
函数列表:
1. void Erosion( int, void* ); //对二值图像进行腐蚀运算
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat srcImg, binaryImg, erosionDst;

int erosionElem = MORPH_RECT;
int erosionSize = 1;
int const maxElem = 2;
int const maxKernelSize = 21;

void Erosion( int, void* )
{
	int elemType = MORPH_RECT;

	if( erosionElem == 0 ){ elemType = MORPH_RECT; }
	else if( erosionElem == 1 ){ elemType = MORPH_CROSS; }
	else if( erosionElem == 2) { elemType = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( elemType,
		Size( 2*erosionSize + 1, 2*erosionSize+1 ),
		Point( erosionSize, erosionSize ) );

	// 腐蚀操作
	erode( binaryImg, erosionDst, element);
	imwrite("..\\figures\\figure6_7_b.bmp", erosionDst);
	imshow( "Erosion", erosionDst );
}

int main( int argc, char** argv )
{
	// 装载图像
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
	namedWindow( "Erosion", CV_WINDOW_AUTOSIZE );

	// 创建滚动条
	createTrackbar( "结构元素", "Erosion",
		&erosionElem, maxElem,
		Erosion );

	createTrackbar( "大小", "Erosion",
		&erosionSize, maxKernelSize,
		Erosion );

	Erosion( 0, 0 );

	waitKey(0);
	return 0;
}
