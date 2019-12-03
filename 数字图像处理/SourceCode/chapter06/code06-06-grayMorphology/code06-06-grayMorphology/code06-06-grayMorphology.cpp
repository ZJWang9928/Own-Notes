/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: // code06-06-grayMorphology
  作  者: // 胡少军、何东健、耿楠等
  日  期: // 2014年8月4日
  描  述: // 灰度图像形态学运算的OpenCV实现
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

#define GRAY_IMGAE_ERODE 100
#define GRAY_IMGAE_DILATE 101
#define GRAY_IMGAE_OPENING 102
#define GRAY_IMGAE_CLOSING 103

int main( int argc, char** argv )
{
	Mat srcImg, grayImg, morphDst;
	int elemType = MORPH_RECT; //MORPH_CROSS, MORPH_ELLIPSE
	int elemSize = 1;
	int morphOperation = GRAY_IMGAE_ERODE;

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

	grayImg.create(srcImg.size(), CV_8U);

	//将源图像转化为灰度图像
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	Mat elemStruct = getStructuringElement( elemType,
									Size( 2*elemSize + 1, 2*elemSize+1 ),
									Point( elemSize, elemSize ) );

	imshow( "Source Image", grayImg );

	morphDst = grayImg;
	switch (morphOperation)
	{
	case GRAY_IMGAE_ERODE:
		//腐蚀操作
		erode( grayImg, morphDst, elemStruct);
		imwrite("..\\figures\\figure6_16_b.bmp", morphDst);
		break;
	case GRAY_IMGAE_DILATE:
		// 膨胀操作
		dilate( grayImg, morphDst, elemStruct);
		imwrite("..\\figures\\figure6_16_c.bmp", morphDst);
		break;
	case GRAY_IMGAE_OPENING:
		//开运算
		morphologyEx(grayImg, morphDst, MORPH_OPEN, elemStruct);
		imwrite("..\\figures\\figure6_16_d.bmp", morphDst);
		break;
	case GRAY_IMGAE_CLOSING:
		//闭运算
		morphologyEx(grayImg, morphDst, MORPH_CLOSE, elemStruct);
		imwrite("..\\figures\\figure6_16_e.bmp", morphDst);
		break;
	}

	imshow( "Gray Image Morphology", morphDst );

	waitKey(0);
	return 0;
}
