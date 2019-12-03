/************************************************************
Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
文件名: // code06-05-findEndPoints
作  者: // 胡少军、何东健、耿楠等
日  期: // 2014年8月4日
描  述: // 二值骨架图像的端点提取
函数列表:
1. void drawEndPoints(const Mat& maskImg, Mat &dstImg); //用圆绘制端点到目标图像
2. Mat findEndPoints(const Mat &binaryImg); //提取二值骨架图像所有端点
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//****************************************************************************
//函数名称：void drawEndPoints(const Mat& maskImg, Mat &dstImg)
//基本功能：将maskImg中的端点绘制到图像dstImg
//参数说明：参数1为提取的端点二值图像；参数2为目标图像
//返 回 值：无
//****************************************************************************
void drawEndPoints(const Mat& maskImg, Mat &dstImg)
{
	uchar pixel;
	for(int i=0; i<maskImg.rows; i++)
		for(int j=0; j<maskImg.cols; j++)
		{
			pixel = maskImg.at<uchar>(i, j);
			if(pixel==255)
				circle(dstImg, Point(j, i), 5, Scalar(255,255,255));
		}
}

//****************************************************************************
//函数名称：Mat findEndPoints(const Mat &binaryImg)
//基本功能：采用击中击不中运算提取二值骨架图像所有端点
//参数说明：输入二值图像，该图像中背景色为0，前景色(目标)为255
//返 回 值：返回包含所有端点的二值图像
//****************************************************************************
Mat findEndPoints(const Mat &binaryImg)
{
	int i, elemSize = 3;
	Mat dstImg;
	Mat maskImg = Mat::zeros(binaryImg.size(), CV_8UC1);
	/**四种结构元素
	  1 1 1		1 1 0	0 0 0	0 1 1
	  1 0 1		1 0 0	1 0 1	0 0 1
	  0 0 0		1 1 0	1 1 1	0 1 1  **/
	//设置上端点3×3结构元素S2
	Mat elemStruct(elemSize, elemSize, CV_8U, Scalar(1));
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(2, i) = 0;
	dstImg = 255 - binaryImg;
	erode(dstImg, dstImg, elemStruct);
	maskImg = binaryImg&dstImg;

	//设置左端点3×3结构元素S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(i, 2) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	//设置下端点3×3结构元素S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(0, i) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	//设置右端点3×3结构元素S2
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for(i=0; i<elemSize; i++)
		elemStruct.at<uchar>(i, 0) = 0;
	dstImg = 255 - binaryImg;
	erode( dstImg, dstImg, elemStruct);
	maskImg |= binaryImg&dstImg;

	dstImg = binaryImg;
	drawEndPoints(maskImg, dstImg);

	dstImg = 255-dstImg;

	return dstImg;
}

int main( int argc, char** argv )
{
	Mat srcImg, binaryImg, dstImg;

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

	binaryImg = 255 - binaryImg;

	dstImg = findEndPoints(binaryImg);

	imshow("Find end points", dstImg);

	waitKey(0);
	return 0;
}
