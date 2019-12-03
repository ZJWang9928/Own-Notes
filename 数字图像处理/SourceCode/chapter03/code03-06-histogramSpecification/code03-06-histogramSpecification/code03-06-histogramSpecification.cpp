/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-06-histogramSpecification.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月3日
  描  述: 实现直方图规定化。
  其  它: 单映射和组映射规则应用示例
 	Mat srcHist = Mat::zeros(8, 1, CV_32FC1);
	Mat dstHist = Mat::zeros(8, 1, CV_32FC1);

	// 原始直方图
	srcHist.at<float>(0) = 0.19;
	srcHist.at<float>(1) = 0.25;
	srcHist.at<float>(2) = 0.21;
	srcHist.at<float>(3) = 0.16;
	srcHist.at<float>(4) = 0.08;
	srcHist.at<float>(5) = 0.06;
	srcHist.at<float>(6) = 0.03;
	srcHist.at<float>(7) = 0.02;

	// 规定直方图
	dstHist.at<float>(3) = 0.2;
	dstHist.at<float>(5) = 0.5;
	dstHist.at<float>(7) = 0.3;

	vector<uchar> grayMap;
	
	// 单映射规则
	singleMapping(srcHist, dstHist, grayMap);
	// 组映射规则
	groupMapping(srcHist, dstHist, grayMap);
  函数列表:
    1. void HistogramSpecification() 直方图规定化
    2. void createGrayHistogram() 创建灰度直方图
    3. void drawHistogram() 绘制灰度直方图
    4. void singleMapping() 单映射规则
	5. void groupMapping() 组映射规则
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/03    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// 直方图规定化
void HistogramSpecification(Mat& grayImg, const Mat& dstHist, int mapRule = 0);

/// 创建灰度直方图
void createGrayHistogram(Mat srcImg, Mat& grayHist);

/// 绘制灰度直方图
void drawHistogram(const string &histWinName, Mat &grayHist);

/// 单映射规则
void singleMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap);

/// 组映射规则
void groupMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap);

int main(int argc, char** argv)
{
	/// 加载原始图像
	string srcFileName;
	cout << "Enter the image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 转换为灰度图像
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// 规定直方图
	Mat dstHist = Mat::zeros(256, 1, CV_32FC1);
	for(int i = 0; i < 256; i++)
	{
		dstHist.at<float>(i) = float(i);
	}

	/// 直方图规定化
	Mat dstImg = srcImg.clone();
	HistogramSpecification(dstImg, dstHist);

	/// 显示图像
	const string srcWinName = "Source image";
	const string dstWinName = "Result image";
	namedWindow(srcWinName);
	imshow(srcWinName, srcImg);
	namedWindow(dstWinName);
	imshow(dstWinName, dstImg);

	/// 显示直方图
	drawHistogram("Desired histogram", dstHist);
	Mat rstHist;
	createGrayHistogram(dstImg, rstHist);
	drawHistogram("Resulted histogram", rstHist);

	waitKey(0);

	return 0;
}


/**
 * 直方图规定化
 * @param grayImg 规定化前为原始灰度图像，规定化后为增强灰度图像（单通道灰度图像）
 * @param dstHist 指定的直方图，要求与原始图像的灰度级数一致
 * @param mapRule 映射规则：0-单映射规则；1-组映射规则
 */
void HistogramSpecification(Mat& grayImg, const Mat& dstHist, int mapRule)
{
	if(grayImg.empty() || grayImg.channels() != 1)
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// 计算原始图像的灰度直方图
	Mat srcHist;
	int histSize = 256;			// 直方图的箱格数
	float range[] = {0, 255};
	const float* histRange = {range};	// 直方图各维的箱格的数值范围
	calcHist(&grayImg, 1, 0, Mat(), srcHist, 1, &histSize, &histRange, true, false);

	/// 计算灰度的对应关系
	vector<uchar> grayMap;
	switch (mapRule)
	{
	case 0:
		singleMapping(srcHist, dstHist, grayMap);
		break;
	case 1:
		groupMapping(srcHist, dstHist, grayMap);
		break;
	default:
		break;
	}

	/// 进行灰度映射
	uchar *pGray = NULL;
	for(int r = 0; r < grayImg.rows; r++)
	{
		pGray = grayImg.ptr<uchar>(r);
		for(int c = 0; c < grayImg.cols; c++)
		{
			pGray[c] = grayMap[pGray[c]];
		}
	}
}


/**
 * 创建灰度直方图
 * @param srcImg 原始图像
 * @param grayHist 灰度直方图
 */
void createGrayHistogram(Mat srcImg, Mat& grayHist)
{
	///// 转换为灰度图像
	//cvtColor(srcImg, srcImg, CV_BGR2GRAY);

	/// 计算灰度直方图
	int histSize = 256;			// 直方图的箱格数
	float range[] = {0, 255};
	const float* histRange = {range};	// 直方图各维的箱格的数值范围
	calcHist(&srcImg, 1, 0, Mat(), grayHist, 1, &histSize, &histRange, true, false);
}

/**
 * 绘制灰度直方图
 * @param histWinName 用于显示灰度直方图的窗口名称
 * @param grayHist 灰度直方图数据
 */
void drawHistogram(const string &histWinName, Mat &grayHist)
{
	/// 创建直方图图像
	int histWidth = 512;	// 直方图宽度
	int histHeight = 400;	// 直方图高度
	int binWidth = cvRound((double)histWidth / grayHist.rows);	// 箱格宽度
	Mat histImg(histHeight, histWidth, CV_8UC3, Scalar(0,0,0));

	/// 规范化直方图
	normalize(grayHist, grayHist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

	/// 绘制直方图
	//for(int i = 1; i < grayHist.rows; i++)
	//{
	//	line(histImg, Point(binWidth*(i-1), histHeight-cvRound(grayHist.at<float>(i-1))),
	//		Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
	//		Scalar(128, 128, 128), 2, 8, 0);
	//}
	for(int i = 0; i < grayHist.rows; i++)
	{
		line(histImg, Point(binWidth*(i), histHeight),
			Point(binWidth*i, histHeight-cvRound(grayHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	/// 显示直方图
	imshow(histWinName, histImg);
}

/**
 * 直方图规定化中的单映射规则
 * @param srcHist 原始直方图
 * @param dstHist 规定直方图
 * @param grayMap 灰度映射表（映射结果）
 * @note 要求原始直方图与规定直方图具有相同的灰度级，默认为256。
 */
void singleMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap)
{
	assert(srcHist.rows == dstHist.rows);

	/// 直方图的箱格数（默认为256）
	int histSize = srcHist.rows;

	/// 规范化直方图
	Scalar srcHistTot, dstHistTot;
	srcHistTot = cv::sum(srcHist);
	srcHist /= srcHistTot[0];
	dstHistTot = cv::sum(dstHist);
	dstHist /=  dstHistTot[0];
	
	/// 计算累积直方图
	vector<double> srcCumHist(histSize, 0);
	vector<double> dstCumHist(histSize, 0);
	double srcTotVal = 0, dstTotVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		srcTotVal += srcHist.at<float>(i);
		dstTotVal += dstHist.at<float>(i);

		srcCumHist[i] = srcTotVal;
		dstCumHist[i] = dstTotVal;
	}

	/// 计算灰度的对应关系
	grayMap.assign(histSize, 0);
	double minVal = 0, absDiff = 0;
	int mapVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		minVal = 1.0;
		for(int j = mapVal; j < histSize; j++)
		{
			absDiff = std::abs(dstCumHist[j] - srcCumHist[i]);
			if(absDiff < minVal)
			{
				minVal = std::abs(dstCumHist[j] - srcCumHist[i]);
				mapVal = j;
			}
			else if(absDiff > minVal)
			{
				break;
			}
		}

		grayMap[i] = mapVal;
	}
}

/**
 * 直方图规定化中的组映射规则
 * @param srcHist 原始直方图
 * @param dstHist 规定直方图
 * @param grayMap 灰度映射表（映射结果）
 * @note 要求原始直方图与规定直方图具有相同的灰度级，默认为256。
 */
void groupMapping(const Mat &srcHist, const Mat &dstHist, vector<uchar> &grayMap)
{
	assert(srcHist.rows == dstHist.rows);

	/// 直方图的箱格数（默认为256）
	int histSize = srcHist.rows;

	/// 规范化直方图
	Scalar srcHistTot, dstHistTot;
	srcHistTot = cv::sum(srcHist);
	srcHist /= srcHistTot[0];
	dstHistTot = cv::sum(dstHist);
	dstHist /=  dstHistTot[0];
	
	/// 计算累积直方图
	vector<double> srcCumHist(histSize, 0);
	vector<double> dstCumHist(histSize, 0);
	double srcTotVal = 0, dstTotVal = 0;
	for(int i = 0; i < histSize; i++)
	{
		srcTotVal += srcHist.at<float>(i);
		dstTotVal += dstHist.at<float>(i);

		srcCumHist[i] = srcTotVal;
		dstCumHist[i] = dstTotVal;
	}

	/// 计算灰度的对应关系
	grayMap.assign(histSize, 0);
	double minVal = 0, absDiff = 0;
	int lastIdx = -1, minIdx = 0;
	for(int j = 0; j < histSize; j++)
	{
		if(dstHist.at<float>(j) <= 0)
		{
			continue;
		}

		/// 找出满足条件的最小I(l)
		minVal = 1.0;
		for(int i = minIdx; i < histSize; i++)
		{
			absDiff = std::abs(dstCumHist[j] - srcCumHist[i]);
			if(absDiff < minVal)
			{
				minVal = std::abs(dstCumHist[j] - srcCumHist[i]);
				minIdx = i;
			}
			else if(absDiff > minVal)
			{
				break;
			}
		}

		/// 确立映射关系：[I(l-1), I(l)] --> zl
		for(int k = lastIdx + 1; k <= minIdx; k++)
		{
			grayMap[k] = j;
		}

		/// 更新I(l-1)
		lastIdx = minIdx;
	}
}