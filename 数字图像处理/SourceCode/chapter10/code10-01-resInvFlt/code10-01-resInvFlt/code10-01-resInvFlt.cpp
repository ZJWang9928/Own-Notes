/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code10-01-resInvFlt.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月19日
  描  述: 实现逆滤波图像复原
  其  它:
  函数列表:
    1. void creKerTemp() 创建卷积模板
    2. void fftConImg()  频域中进行傅里叶为换
    3. void invFilt() 频域中进行逆滤波运算
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宁纪锋   2014/04/18   1.0   建立该文件
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

void creKerTemp(Mat &dst,int nHeight, int nWidth,int nTemSize);
void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg);
void invFilt(Mat *imgDFT,Mat *kerTempDFT, Mat *dst);

int main( int argc, char** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	// 转换为灰度图像
	if (img.channels()>=3)
		cvtColor(img,img,CV_RGB2GRAY);

	int nHeight=img.rows;
	int nWidth=img.cols;

	namedWindow("Original Image",CV_WINDOW_AUTOSIZE);
	imshow("Original Image",img);  // 显示原始图像
	waitKey();


	// 构造模板图像
	int nkerSize=9;
	Mat kerTemp;
	creKerTemp(kerTemp,nHeight,nWidth,nkerSize);

	// 对原图像进行傅里叶变换
	Mat img2;
	img2=img.clone();
	img2=Mat_<float>(img2);
	
	// 定义一个一维数组，两个元素，每个元素的类型是Mat
	Mat planesImg[]={img2,Mat::zeros(img2.size(),img2.type())};
	Mat complexImg;
	merge(planesImg,2,complexImg);
	dft(complexImg,complexImg);	
	split(complexImg, planesImg);  
     
	// 对模板进行傅里叶变换
	Mat planesKerTemp[]={kerTemp,Mat::zeros(kerTemp.size(),kerTemp.type())};
	Mat complexKerTemp;
	merge(planesKerTemp,2,complexKerTemp);
	dft(complexKerTemp,complexKerTemp);
	split(complexKerTemp,planesKerTemp);

	// 求频域图像的卷积结果
	Mat conImg;
	Mat planesTmp[]={Mat::zeros(kerTemp.size(),kerTemp.type()),Mat::zeros(kerTemp.size(),kerTemp.type())};
	fftConImg(planesImg,planesKerTemp,planesTmp);	
    merge(planesTmp, 2, complexImg);
	idft(complexImg,complexImg);
	split(complexImg,planesTmp);


	normalize(planesTmp[0], img2, 0, 1, CV_MINMAX); // 规范化图像

	namedWindow("Convulution Image",CV_WINDOW_AUTOSIZE);
	imshow("Convulution Image",img2);  // 显示图像卷积模糊后的结果
	waitKey();

	// 对卷积模糊后的图像进行傅里叶变换
	planesImg[0]=img2;
	planesImg[1]=Mat::zeros(img2.size(),img2.type());
	merge(planesImg,2,complexImg);
	dft(complexImg,complexImg);	
	split(complexImg, planesImg);

	// 调用逆滤波函数进行图像复原
	Mat planesInvFilt[]={Mat::zeros(kerTemp.size(),kerTemp.type()),Mat::zeros(kerTemp.size(),kerTemp.type())};
	invFilt(planesImg,planesKerTemp,planesInvFilt);  // 逆滤波输出planesInvFilt仍为频域

	merge(planesInvFilt, 2, complexImg); // 数据合并，方便进行傅里叶变换
	idft(complexImg,complexImg);         // 逆傅里叶变换
	split(complexImg,planesInvFilt);

	normalize(planesInvFilt[0], img2, 0, 1, CV_MINMAX); // 规范化图像

	namedWindow("Inverse Filter Image Restoration",CV_WINDOW_AUTOSIZE);
	imshow("Inverse Filter Image Restoration",img2);  // 显示图像卷积模糊后的结果

    waitKey();
	return 0;
}

/*************************************************
  名    称: void creKerTemp(Mat &dst,int nHeight, int nWidth，int nTemSize)
  描    述: 创建一个模板图像
  调用函数: 无
  主调函数: 主函数
  输入参数: nHeight--> 图像高度
			nWidth --> 图像宽度  
			nTemSize --> 模板尺寸
  输出参数: dst--> 输出图像
  返    回: 无
  其    它: 无
  日    期：2014年4月20日
*************************************************/
void creKerTemp(Mat &dst,int nHeight, int nWidth,int nkerSize)
{
	int i,j;
	float nCoef = 1.0/(nkerSize*nkerSize);
	dst = Mat::zeros(nHeight,nWidth, CV_32FC1);

	for (i=0;i<nkerSize;i++)
	{
		for(j=0;j<nkerSize;j++)
		{
			dst.at<float>(i,j)=nCoef;
		}
	}	

}

/*************************************************
  名    称: void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg)
  描    述: 在频域对图像进行卷积，输出仍为频域
  调用函数: 无
  主调函数: 主函数
  输入参数: imgDFT --> 图像的傅里叶变换
            kerTempDFT --> 模板的傅里叶变换
  输出参数: conImg --> 频域卷积结果
  返    回: 无
  其    它: 无
  日    期：2014年4月20日
*************************************************/
void fftConImg(Mat *imgDFT,Mat *kerTempDFT, Mat *conImg)
{
	int nHeight,nWidth;
	nHeight=imgDFT[0].rows;
	nWidth=imgDFT[0].cols;

	//conImg=Mat::zeros(nHeight,nWidth,CV_32F);
	double a,b,c,d;
	int i,j;
	for (i=0;i<nHeight;i++)
	{
		for (j=0;j<nWidth;j++)
		{
			a=imgDFT[0].at<float>(i,j);             // 图像傅里叶变换结果的实部
			b=imgDFT[1].at<float>(i,j);  
			c=kerTempDFT[0].at<float>(i,j);         // 模板傅里叶变换结果的实部和虚部
			d=kerTempDFT[1].at<float>(i,j);

			conImg[0].at<float>(i,j)=a*c-b*d;
			conImg[1].at<float>(i,j)=a*d+b*c;
		}
	}

}

/*************************************************
  名    称: void invFilt(Mat *imgDFT,Mat *kerTempDFT, Mat *dst)
  描    述: 在频域对图像进行逆滤波，输出仍为频域
  调用函数: 无
  主调函数: 主函数
  输入参数: imgDFT --> 图像的傅里叶变换
            kerTempDFT --> 模板的傅里叶变换
  输出参数: dst --> 逆滤波的频域处理结果
  返    回: 无
  其    它: 无
  日    期：2014年4月20日
*************************************************/
void invFilt(Mat *imgDFT,Mat *kerTempDFT, Mat *dst)
{
	int nHeight,nWidth;
	nHeight=imgDFT[0].rows;
	nWidth=imgDFT[0].cols;

	double a,b,c,d;
	int i,j;
	for (i=0;i<nHeight;i++)
	{
		for (j=0;j<nWidth;j++)
		{
			a=imgDFT[0].at<float>(i,j);                 // 图像傅里叶变换结果的实部
			b=imgDFT[1].at<float>(i,j);  
			c=kerTempDFT[0].at<float>(i,j);             // 模板傅里叶变换结果的实部和虚部
			d=kerTempDFT[1].at<float>(i,j);

			dst[0].at<float>(i,j)=(a*c+b*d)/(c*c+d*d);  //逆滤波
			dst[1].at<float>(i,j)=(b*c-a*d)/(c*c+d*d);  // 
		}
	}
}