/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-06-edgeDetect.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月5日
  描  述: 实现边缘检测：梯度算子、高斯－拉普拉斯算子、Canny算子。
  其  它:
  函数列表:
    1. void edgeRober() Robert边缘检测
    2. void edgePrewitt() Prewitt边缘检测
    3. void edgeCanny() Canny边缘检测
    4. void edgeLog() LOG边缘检测
	5. void edgeSobel() Sobel边缘检测
	6. void edgeKrisch() Krisch边缘检测
	7. void onThresholdTrackbar() 阈值滑块的回调函数
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/05    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int g_threshold = 50;

/**
 * Robert边缘检测
 * @param src 输入图像
 * @param dst 输出图像：Rorbert边缘幅度
 */
void edgeRobert(Mat src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// 水平方向梯度
	Mat gradX, absGradX;
	Mat kernelX = (Mat_<int>(2, 2) << 0, -1, 1, 0);
	filter2D(src, gradX, CV_16S, kernelX, Point(0, 0));
	convertScaleAbs(gradX, absGradX);

	/// 垂直方向梯度
	Mat gradY, absGradY;
	Mat kernelY = (Mat_<int>(2, 2) << -1, 0, 0, 1);	
	filter2D(src, gradY, CV_16S, kernelY, Point(0, 0));
	convertScaleAbs(gradY, absGradY);

	/// 总梯度
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// 近似梯度
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;
}

/* Prewitt边缘检测
 * @param src 输入图像
 * @param dst 输出图像：Prewitt边缘幅度
 */
void edgePrewitt(Mat src, Mat &dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// 水平方向梯度
	Mat gradX, absGradX;
	Mat kernelX = (Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
	filter2D(src, gradX, CV_16S, kernelX);
	convertScaleAbs(gradX, absGradX);

	/// 垂直方向梯度
	Mat gradY, absGradY;
	Mat kernelY = (Mat_<int>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
	filter2D(src, gradY, CV_16S, kernelY);
	convertScaleAbs(gradY, absGradY);

	/// 总梯度
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// 近似梯度
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;	
}

/* Canny边缘检测
 * @param src 输入图像
 * @param dst 输出图像：canny边缘检测结果
 * @param lowThresh 低阈值
 * @param ratio 高阈值与低阈值之比（建议取2－3）
 */
void edgeCanny(Mat src, Mat& dst, double lowThresh, double ratio = 3)
{
	CV_Assert(src.depth() == CV_8U);

	/// 图像平滑
	Mat edge;
	cv::blur(src, dst, Size(3, 3));

	/// 应用Canny算子
	cv::Canny(dst, dst, lowThresh, lowThresh * ratio, 3);
}

/*
 * LOG边缘检测
 * @param src 输入图像
 * @param dst 输出图像：LOG边缘检测结果
 * @param kernelSize 平滑核尺寸（默认为3）
 * @param sigma 高斯平滑核的均方差（默认为0）
 * @note 常用的5*5LOG算子：
		-2, -4, -4, -4, -2,
		-4,  0,  8,  0, -4,
		-4,  8, 24,  8, -4,
		-4,  0,  8,  0, -4,
		-2, -4, -4, -4, -2
 */
void edgeLog(Mat src, Mat& dst, int kernelSize = 5, double sigma = 0)
{
	CV_Assert(src.depth() == CV_8U);

	/// 利用高斯滤波降噪
	Mat smooth;
	GaussianBlur(src, smooth, Size(kernelSize, kernelSize), sigma, sigma);

	/// 应用拉普拉斯算子（因输入图像为CV_8U，此处使用CV_16S避免外溢）
	Laplacian(smooth, dst, CV_16S, kernelSize);

	/// 将输出图像转换为CV_8U
	convertScaleAbs(dst, dst);
}

/*
 * Sobel边缘检测
 * @param src 输入图像
 * @param dst 输出图像：Sobel边缘幅度
 */
void edgeSobel(Mat src, Mat& dst)
{
	CV_Assert(src.depth() == CV_8U);

	/// 水平方向梯度
	Mat gradX, absGradX;
	Sobel(src, gradX, CV_16S, 1, 0);
	cv::convertScaleAbs(gradX, absGradX);

	/// 垂直方向梯度
	Mat gradY, absGradY;
	cv::Sobel(src, gradY, CV_16S, 0, 1);
	cv::convertScaleAbs(gradY, absGradY);

	/// 总梯度
	//cv::sqrt(gradX * gradX + gradY * gradY, grad);
	//cv::convertScaleAbs(grad, grad);

	/// 近似梯度
	Mat grad;
	addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

	dst = grad;
}

/**
 * Krisch边缘检测：利用8个Krisch边缘检测算子依次对图像进行模板卷积，
 * 取最大值作为边缘幅度图像的输出。
 * @param src 输入图像
 * @param dst 输出图像：Krisch边缘幅度
 */
void edgeKrisch(Mat src, Mat& dst)
{
	CV_Assert(src.depth() == CV_8U);

	Mat grad, gradMax, kernel;

	/// 方向1的卷积
	kernel = (Mat_<int>(3, 3) << 5, 5, 5, -3, 0, -3, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = grad.clone();

	/// 方向2的卷积
	kernel = (Mat_<int>(3, 3) << -3, 5, 5, -3, 0, 5, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向3的卷积
	kernel = (Mat_<int>(3, 3) << -3, -3, 5, -3, 0, 5, -3, -3, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向4的卷积
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, -3, 0, 5, -3, 5, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向5的卷积
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, -3, 0, -3, 5, 5, 5);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向6的卷积
	kernel = (Mat_<int>(3, 3) << -3, -3, -3, 5, 0, -3, 5, 5, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向7的卷积
	kernel = (Mat_<int>(3, 3) << 5, -3, -3, 5, 0, -3, 5, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 方向8的卷积
	kernel = (Mat_<int>(3, 3) << 5, 5, -3, 5, 0, -3, -3, -3, -3);
	filter2D(src, grad, CV_16S, kernel);
	gradMax = cv::max(grad, gradMax);

	/// 将输出图像转换为CV_8U
	convertScaleAbs(gradMax, dst);
}

/// 自定义的用户数据类型
struct UserData
{
	string dstWinName; // 边缘窗口名称
	int keyVal;	// 字符键值
	Mat srcImg;	// 原始图像
	Mat dstImg;	// 边缘图像
};

/**
 * 阈值滑块的回调函数：按照用户数据指定的方法和阈值进行边缘检测
 * @param th 阈值滑块位置值，即阈值
 * @param userdata UserData型的用户数据指针，包含有按键信息及输入输出图像
 */
static void onThresholdTrackbar(int th, void* userdata)
{
	UserData* pUserData = (UserData*)userdata;
	if(!pUserData)
	{
		return;
	}

	switch (pUserData->keyVal)
	{
	case 'r':
		cout << "Robert edge detector" << endl;
		edgeRobert(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, th, 255, THRESH_TOZERO);
		break;
	case 'p':
		cout << "Prewitt edge detector" << endl;
		edgePrewitt(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 's':
		cout << "Sobel edge detector" << endl;
		edgeSobel(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'k':
		cout << "Krisch edge detector" << endl;
		edgeKrisch(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'l':
		cout << "LOG edge detector" << endl;
		edgeLog(pUserData->srcImg, pUserData->dstImg);
		threshold(pUserData->dstImg, pUserData->dstImg, g_threshold, 255, THRESH_TOZERO);
		break;
	case 'c':
		cout << "Canny edge detector" << endl;
		edgeCanny(pUserData->srcImg, pUserData->dstImg, th);
		break;
	default:
		break;
	}

	// 显示边缘
	imshow(pUserData->dstWinName, pUserData->dstImg);
}

int main(int argc, char** argv)
{
	UserData userData;

	/// 输出帮助信息
	cout << "r - Robert edge detector\n"
		 << "s - Sobel edge detector\n"
		 << "p - Prewitt edge detector\n"
		 << "k - Krisch edge detector\n"
		 << "l - LOG edge detector\n"
		 << "c - Canny edge detector\n"
		 << "Esc - Quit\n";

	/// 输入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	userData.srcImg = imread("rice.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if(userData.srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// 创建显示窗口
	const string srcWinName = "Source image";
	userData.dstWinName = "Edge map";
	namedWindow(srcWinName);
	namedWindow(userData.dstWinName, CV_WINDOW_AUTOSIZE);
	
	/// 创建阈值滑动条
	createTrackbar("Threshold:", userData.dstWinName, &g_threshold, 100, onThresholdTrackbar, &userData);

	/// 显示原始图像
	imshow(srcWinName, userData.srcImg);

	/// 应用边缘检测算子
	userData.keyVal = 'c';
	while(userData.keyVal != 27)
	{
		// 边缘检测
		onThresholdTrackbar(g_threshold, &userData);

		// 获取键值
		userData.keyVal = waitKey(0);
	}

	waitKey(0);

	return 0;
}

