/************************************************************
Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
文件名: // code06-08-findCorners
作  者: // 胡少军、何东健、耿楠等
日  期: // 2014年8月4日
描  述: // 灰度图像角点检测
函数列表:
1. void drawCircles(const Mat& maskImg, Mat &dstImg); //用圆绘制端点到目标图像
2. Mat findCorners(const Mat &inImg); //提取灰度图像中的角点
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//****************************************************************************
//函数名称：void drawCircles(const Mat& maskImg, Mat &dstImg)
//基本功能：将maskImg中的角点绘制到图像dstImg
//参数说明：参数1为提取的角点图像；参数2为目标图像
//返 回 值：无
//****************************************************************************
void drawCircles(const Mat& maskImg, Mat &dstImg)
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
//函数名称：Mat findCorners(const Mat &binaryImg)
//基本功能：提取灰度图像中的角点
//参数说明：输入灰度图像
//返 回 值：返回包含角点的二值图像
//****************************************************************************
Mat findCorners(const Mat &inImg)
{
	int i, elemSize = 5;
	Mat dstImg1, dstImg2, diffImg;

	//设置十字形结构元素
	Mat crossStruct = getStructuringElement( MORPH_CROSS,
									  Size(elemSize, elemSize ),
									  Point(elemSize/2, elemSize/2));
	//设置矩形结构元素
	Mat rectStruct = getStructuringElement( MORPH_RECT,
									  Size(elemSize, elemSize ),
									  Point(elemSize/2, elemSize/2));
	//自定义X形结构元素
	Mat xStruct(elemSize, elemSize, CV_8U, Scalar(0));
	for(i=0; i<elemSize; i++)
	{
		xStruct.at<uchar>(i, i) = 1;
		xStruct.at<uchar>(4-i, i) = 1;
	}
	//自定义菱形结构元素
	Mat diamondStruct(elemSize, elemSize, CV_8U, Scalar(1));
	diamondStruct.at<uchar>(0, 0) = 0;
	diamondStruct.at<uchar>(0, 1) = 0;
	diamondStruct.at<uchar>(1, 0) = 0;
	diamondStruct.at<uchar>(4, 4) = 0;
	diamondStruct.at<uchar>(3, 4) = 0;
	diamondStruct.at<uchar>(4, 3) = 0;
	diamondStruct.at<uchar>(4, 0) = 0;
	diamondStruct.at<uchar>(4, 1) = 0;
	diamondStruct.at<uchar>(3, 0) = 0;
	diamondStruct.at<uchar>(0, 4) = 0;
	diamondStruct.at<uchar>(0, 3) = 0;
	diamondStruct.at<uchar>(1, 4) = 0;

	//用十字形结构元素膨胀图像
	dilate(inImg, dstImg1, crossStruct);
	//用菱形结构元素腐蚀图像
	erode(dstImg1, dstImg1, diamondStruct);

	//用X形结构元素膨胀图像
	dilate(inImg, dstImg2, xStruct);
	//用矩形结构元素腐蚀图像
	erode(dstImg2, dstImg2, rectStruct);

	absdiff(dstImg2, dstImg1, diffImg);

	threshold(diffImg, dstImg1, 60, 255, THRESH_BINARY);

	dstImg2 = inImg;

	drawCircles(dstImg1, dstImg2);

	return dstImg2;
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

	Mat grayImg (srcImg.size(), CV_8U), diaImg;

	//将源图像转化为灰度图像
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	binaryImg = grayImg;

	dstImg = findCorners(binaryImg);

	imshow("Find corners", dstImg);

	waitKey(0);
	return 0;
}
