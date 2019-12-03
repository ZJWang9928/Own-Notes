/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-07-houghTransform.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月7日
  描  述: 利用Hough变换检测图像中的直线或圆。
  其  它:
  函数列表:
    1. void stHoughLine() 标准Hough变换检测直线
    2. void pbHoughLine() 概率Hough变换检测直线
    3. void stHoughCircle() Hough变换检测圆
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/07    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * 标准Hough变换检测直线
 * @param src 原始灰度图像(单通道、8位)
 * @param dst 检测结果图像
 * @param accumThreshold 累加数组元素阈值（只保留累加数组中大于该阈值的元素）
 */
void stHoughLine(const Mat& src, Mat& dst, int accumThreshold)
{
	CV_Assert(src.depth() == CV_8U);

	/// 初始化输出图像
	cvtColor(src, dst, CV_GRAY2BGR);

	/// 按照指定的极径、极角分辨率进行标准Hough变换检测直线，检测结果（rho, theta）保存在lines中
	vector<Vec2f> lines;
	HoughLines(src, lines, 1, CV_PI/180, accumThreshold, 0, 0);

	/////////////////////////////////////////////
	//   0------> x
	//   |
	//   |
	//   y

	/// 在输出图像中绘制检测到的直线
	/** 方法一 **/
	//for(size_t i = 0; i < lines.size(); i++)
	//{
	//	float rho = lines[i][0];	// 直线对应的极径
	//	float theta = lines[i][1];	// 直线对应的极角（极线与x轴的夹角）

	//	double a = cos(theta);
	//	double b = sin(theta);

	//	// 设置当前直线的两个端点坐标（以便显示）
	//	Point pt1, pt2;
	//	if(fabs(a) < 0.0001)
	//	{
	//		// 当极角接近PI/2时，近似为水平线
	//		pt1.y = pt2.y = cvRound(rho);
	//		pt1.x = 0;
	//		pt2.x = src.cols;
	//	}
	//	else if(fabs(b) < 0.0001)
	//	{
	//		// 当极角接近0时，近似为垂直线
	//		pt1.x = pt2.x = cvRound(rho);
	//		pt1.y = 0;
	//		pt2.y = src.rows;
	//	}
	//	else
	//	{
	//		pt1.x = cvRound(rho / a);
	//		pt1.y = 0;
	//		pt2.x = cvRound(rho / a - src.rows * b / a);
	//		pt2.y = src.rows;
	//	}

	//	// 绘制当前直线
	//	line(dst, pt1, pt2, Scalar(0, 0, 255));
	//}

	/** 方法二 **/
	int extLineLen = src.rows + src.cols;
	for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + extLineLen*(-b));
        pt1.y = cvRound(y0 + extLineLen*(a));
        pt2.x = cvRound(x0 - extLineLen*(-b));
        pt2.y = cvRound(y0 - extLineLen*(a));
        line( dst, pt1, pt2, Scalar(0,0,255));
    }
}

/**
 * 概率Hough变换检测直线
 * @param src 原始灰度图像(单通道、8位)
 * @param dst 检测结果图像
 * @param accumThreshold 累加数组元素阈值（只保留累加数组中大于该阈值的元素）
 * @param minLineLen 需要检测的最小线段长度
 * @param maxLineGap 同一线段上允许的最大点间距
 */
void pbHoughLine(const Mat& src, Mat& dst, int accumThreshold, int minLineLen = 50, int maxLineGap = 10)
{
	CV_Assert(src.depth() == CV_8U);
	cvtColor(src, dst, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(src, lines, 1, CV_PI/180, accumThreshold, minLineLen, maxLineGap);

	for(size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255));
	}
}


/**
 * Hough变换检测圆
 * @param src 原始灰度图像(单通道、8位)
 * @param dst 检测结果图像
 * @param accumThreshold 累加数组元素阈值（只保留累加数组中大于该阈值的元素）
 * @param minRadius 需要检测的最小圆半径
 * @param maxRadius 需要检测的最大圆半径
 */
void stHoughCircle(const Mat& src, Mat& dst, int minRadius, int maxRadius)
{
	CV_Assert(src.depth() == CV_8U);
	cvtColor(src, dst, CV_GRAY2BGR);

	vector<Vec3f> circles;
	HoughCircles(src, circles, CV_HOUGH_GRADIENT, 1, 10, 100, 30, minRadius, maxRadius);

	for(size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		circle(dst, Point(c[0], c[1]), c[2], Scalar(0, 0, 255));
	}
}

int main(int argc, char** argv)
{
	/// 输入原始图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load source image: " << srcFileName << endl;
		return -1;
	}

	/// Canny边缘检测
	Canny(srcImg, srcImg, 50, 100, 3);
	imshow("Canny edge", srcImg);

	Mat dstImg;

	// 标准Hough直线检测
	//stHoughLine(srcImg, dstImg, 100);

	// 概率Hough直线检测
	pbHoughLine(srcImg, dstImg, 50, 50, 10);

	// Hough圆检测
	stHoughCircle(srcImg, dstImg, 60, 80);

	/// 显示检测结果
	imshow("Detected Result", dstImg);

	waitKey(0);

	return 0;
}