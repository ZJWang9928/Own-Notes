/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-13-homoFilter.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月23日
  描  述: 利用同态滤波增强图像。
  其  它:
  函数列表:
    1. Mat computeDFT() 傅立叶变换
    2. void shiftDFT() 矩阵数据的双线性插值
    3. Mat createSpectrumMagnitudeForDisplay()  创建用于显示的频谱图像
    4. Mat createHomoFilter() 创建同态滤波器
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/23    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// 傅立叶变换
Mat computeDFT(const Mat &grayImg);

/// 频谱平移
void shiftDFT(Mat &dftImg);

/// 创建用于显示的频谱图像
Mat createSpectrumMagnitudeForDisplay(const Mat &complexImg, bool bShift = true);

/// 创建同态滤波器
Mat createHomoFilter(Size filterSize, double d0, double c, double rL, double rH);

int main(int argc, char** argv)
{	
	/// 读入原始图像（灰值化）
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 显示原图像
	imshow("Original Image", srcImg);

	/// 计算傅立叶频谱
	Mat complexImg = computeDFT(srcImg);

	/// 显示滤波前的频谱幅度
	Mat magImg = createSpectrumMagnitudeForDisplay(complexImg);
	imshow("Fourier Spectrum", magImg);

	/// 频谱平移
	// 注：如果滤波前需要平移频谱，则应在创建滤波器前调用，以保证图像与滤波器的大小一致，
	// 否则mulSpectrums可能出错，因shiftDFT可能会改变图像尺寸。
	shiftDFT(complexImg);

	/// 创建同态滤波器
	Mat homoFilter = createHomoFilter(complexImg.size(), 10, 4, 0.5, 2);	

	/// 频域滤波
	mulSpectrums(complexImg, homoFilter, complexImg, 0);

	/// 频谱平移
	shiftDFT(complexImg);

	/// 傅立叶反变换
	idft(complexImg, complexImg);

	/// 指数变换


	/// 显示滤波后的图像
	Mat planes[2];
	split(complexImg, planes);
	Mat dstImg;
	normalize(planes[0], dstImg, 0, 1, CV_MINMAX);
	imshow("Filtered Image", dstImg);

	waitKey(0);

	return 0;
}

/**
 * 计算傅立叶频谱
 * @param grayImg 单通道灰度图像
 * @return 双通道浮点型的傅立叶频谱数据（实部+虚部）
 */
Mat computeDFT(const Mat &grayImg)
{
	// 优化图像大小
	int m = getOptimalDFTSize(grayImg.rows);
	int n = getOptimalDFTSize(grayImg.cols);

	// 扩展图像，扩展像素用0填充
	Mat paddedImg;
	copyMakeBorder(grayImg, paddedImg, 0, m - grayImg.rows, 0, n - grayImg.cols, BORDER_CONSTANT, Scalar(0));

	// 傅立叶变换（要求输入图像为浮点型）
	Mat complexImg;
	dft(Mat_<float>(paddedImg), complexImg, DFT_COMPLEX_OUTPUT);

	// 返回复数频域数据
	return complexImg;
}


/**
 * 频谱平移
 * @param dftImg DFT频谱数据或频谱幅度
 * @return 无
 */
void shiftDFT(Mat &dftImg)
{
	// 若行数或列数为奇数，则裁剪到偶数行数和列数
	dftImg = dftImg(Rect(0, 0, dftImg.cols & -2, dftImg.rows & -2));

	// 频谱平移操作（交换两对角象限）
    // 0 | 1      3 | 2
    // -----  ==> -----
    // 2 | 3      1 | 0
	int cx = dftImg.cols / 2;
	int cy = dftImg.rows / 2;
	Mat tmpMat;
	Mat topLeft = dftImg(Rect(0, 0, cx, cy));
	Mat downLeft = dftImg(Rect(0, cy, cx, cy));
	Mat topRight = dftImg(Rect(cx, 0, cx, cy));
	Mat downRight = dftImg(Rect(cx, cy, cx, cy));
	topLeft.copyTo(tmpMat); downRight.copyTo(topLeft); tmpMat.copyTo(downRight);
	topRight.copyTo(tmpMat); downLeft.copyTo(topRight); tmpMat.copyTo(downLeft);
}

/**
 * 创建显示用的傅立叶频谱幅度图像（经过对数变换和归一化）
 * @param complexImg 傅立叶频谱数据（双通道，浮点型，实部+虚部）
 * @param bShift 频谱平移标志（若为true，则将频谱平移至图像中心）
 * @return 显示用的频谱幅度图像（经过对数变换和归一化）
 */
Mat createSpectrumMagnitudeForDisplay(const Mat &complexImg, bool bShift)
{
	// 通道分离
	Mat planes[2];
	split(complexImg, planes);

	// 计算幅度：sqrt(Re(DFT(img))^2 + Im(DFT(img))^2)
	Mat magImg;
	magnitude(planes[0], planes[1], magImg);

	// 对数变换：log(1 + mag)
	magImg += Scalar::all(1);
	log(magImg, magImg);

	// 频谱平移至图像中心
	if(bShift)
	{
		shiftDFT(magImg);
	}

	// 频谱归一化到[0, 1]
	normalize(magImg, magImg, 0, 1, CV_MINMAX);

	// 返回频谱幅度图像
	return magImg;
}

/**
 * 创建同态滤波器
 * @param filterSize 滤波半径
 * @param d0 截止半径
 * @param c 用于控制滤波器斜面的锐化程度
 * @param rL 用于控制滤波强度（下界）
 * @param rH 用于控制滤波强度（上界）
 * @return 双通道浮点型的同态滤波器
 */
Mat createHomoFilter(Size filterSize, double d0, double c, double rL, double rH)
{
	Mat tmpFilter = Mat::zeros(filterSize, CV_32F);
	int cx = filterSize.width / 2;
	int cy = filterSize.height / 2;

	double d = 0;
	for(int i = 0; i < filterSize.height; i++)
	{
		for(int j = 0; j < filterSize.width; j++)
		{
			d = sqrt((j - cx) * (j - cx) + (i - cy) * (i - cy));
			tmpFilter.at<float>(i, j) = (float)((rH - rL) * (1 - exp(-c * pow(d / d0, 2))) + rL);
		}
	}

	Mat planes[2] = {tmpFilter, tmpFilter};
	Mat filterImg;
	merge(planes, 2, filterImg);

	return filterImg;
}