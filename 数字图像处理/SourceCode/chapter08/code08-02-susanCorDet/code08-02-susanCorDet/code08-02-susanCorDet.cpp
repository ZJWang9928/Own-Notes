/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-02-susanCorDet.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现susan角点检测算法。
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
#include <math.h>

using namespace cv;
using namespace std;

void susanArea(Mat src, Mat &dst,int k,int t, int g);
void localMaxDet(Mat src, Mat &dst, int k);

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

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE );
	imshow("Original Image", img );

	int nRad=3;  // 检测半径
	int t=15;    // 相似度阈值
	int g=20;    // susan面积阈值

	Mat susImg,susLocMax;
	//dst=Mat::zeros(img.rows,img.cols,CV_32F);
	susanArea(img,susImg,nRad,t,g);       // 检测潜在的suan候选点
	//imwrite("f:\\susImg.bmp",susImg);
	localMaxDet(susImg,susLocMax,nRad);   // 局部极大值检测

	namedWindow("Susan Image", CV_WINDOW_AUTOSIZE );
	imshow("Susan Image", susImg );

	// 在角点周围做标记
	for( int i = 0; i < susLocMax.rows ; i++ )
	{ 
		for( int j = 0; j < susLocMax.cols; j++ )
		{
			if( susLocMax.at<char>(i,j) > 0 )
			{
				circle(img, Point( j, i ), 5, Scalar(0), 2, 8, 0 );
			}
		}
	}
	imshow("Susan Corner Image", img );
	waitKey(0);
  return(0);
}

void susanArea(Mat src, Mat &dst,int k,int t, int g)
{
	int nHeight=src.rows,nWidth=src.cols;
	int i,j,m,n;

	// susan算法的搜索范围
	dst=Mat::zeros(nHeight,nWidth,CV_32F);

	// 遍历图像的每一个点，计算其susan面积
	for (i=k;i<nHeight-k;i++)
	{
		for (j=k;j<nWidth-k;j++)
		{
			for (m=-k;m<=k;m++)
			{
				for (n=-k;n<=k;n++)
				{
					if ((m*m+n*n<=k*k) && abs(src.at<char>(i+m,j+n)-src.at<char>(i,j))<=t)
						dst.at<float>(i,j)=dst.at<float>(i,j)+1;	
				}
			}
		}	
	}
	// 如果susan面积小于一个阈值，则认为其是角点候选点，否则置为0
	for (i=k;i<nHeight-k;i++)
	{
		for (j=k;j<nWidth-k;j++)
		{
			if (dst.at<float>(i,j)<=g && dst.at<float>(i,j)>0)
				dst.at<float>(i,j)=g-dst.at<float>(i,j);
			else
				dst.at<float>(i,j)=0;
		}
	}
}

void localMaxDet(Mat src, Mat &dst,int k)
{
	int i,j,flag;
	int m,n;
	dst=Mat::zeros(src.rows,src.cols,CV_8UC1);
	for (i=k;i<src.rows-k;i++)
	{
		for (j=k;j<src.cols-k;j++)
		{
			// 取出当前点的值
			if (src.at<float>(i,j)<=13)
				continue;
			flag=1;
			
			// 遍历当前点的3*3邻域，检查其是否是极大值
			for (m=-2;m<=2;m++)
			{
				for (n=-2;n<=2;n++)
				{
					if (!(m==0 && n==0) && src.at<float>(i+m,j+n)>=src.at<float>(i,j))
					{
						flag=0;
						break;
					}
				}
				if (flag==0)
					break;
			}

			// 如果标志未变化，则表明该点是极大值点
			if (flag==1)
				dst.at<char>(i,j)=1;
		}
	}
	imwrite("f:\\susImg3.bmp",dst);
}