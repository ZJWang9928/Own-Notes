/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-10-autoMarkerWatershed.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月8日
  描  述: 结合阈值分割、距离变换和形态运算来自动获取标记（Marker），
          从而实现基于标记的分水岭分割（主要用于粘连物体的分割）。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/8     1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main( int argc, char** argv )
{
	/// 输入原始图像（转换为灰度图像）
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName <<endl;
		return -1;
	}
	imshow("Original image", srcImg);

	/// 自动阈值分割（Otsu阈值分割）
	Mat bwImg;
	threshold(srcImg, bwImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("Threshold segmentation", bwImg);

	/// 开运算去除噪声，或闭运算填充孔洞
	Mat structElem = getStructuringElement(MORPH_RECT, Size(3, 3));				// 结构元素
	Mat openImg;
	cv::morphologyEx(bwImg, openImg, MORPH_OPEN, structElem, Point(-1,-1), 2);	// 形态开运算
	imshow("Morpholoy open", openImg);

	/// 形态膨胀运算（找出确切属于背景的区域，以便生成背景标记）
	Mat backImg;
	dilate(openImg, backImg, structElem, Point(-1,-1), 5);	// 黑色部分为确切的背景
	imshow("Sure background region", backImg);

	/// 距离变换与阈值分割（找出确切属于目标的区域，以便生成目标标记）
	Mat distImg, foreImg;
	distanceTransform(openImg, distImg, CV_DIST_L2, 5);
	double maxDist;	// 最大距离
	minMaxLoc(distImg, 0, &maxDist);
	threshold(distImg, foreImg, 0.68*maxDist, 255, CV_THRESH_BINARY);
	distImg.convertTo(distImg, CV_8U, 255 / maxDist);
	imshow("Distance image", distImg);
	foreImg.convertTo(foreImg, CV_8U);
	imshow("Sure object region", foreImg);

	/// 获取不能明确判断为目标或背景的未知区域（在标记图像将其置为0）
	Mat unknownImg;
	unknownImg = backImg - foreImg;

	/// 制作标记图像（图像类型要求为CV_32S，正值表示确切目标/背景区域的标记，零值表示未知区域）
	// 提取标记（marker）的轮廓
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(foreImg, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	// 创建标记图像（确切的背景区域置为1，确切的目标区域置为2以上的整数，未知区域置为0）
	Mat markerImg = Mat::ones(foreImg.size(), CV_32S);				
	int compCount = 1;
	for(int idx = 0; idx >= 0; idx = hierarchy[idx][0], compCount++)
	{
		drawContours(markerImg, contours, idx, Scalar::all(compCount+1), CV_FILLED, 8, hierarchy);
	}
	markerImg.setTo(Scalar(0), unknownImg == 255);


	// 创建标记颜色表（用于给与标记相关的连通成分赋予一种随机颜色）
	vector<Vec3b> colorTab;
	colorTab.reserve(compCount);
	for(int i = 0; i < compCount; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// 执行基于标记的分水岭分割运算（输入图像要求CV_8UC3，标记图像要求为CV_32S）
	cvtColor(srcImg, srcImg, CV_GRAY2BGR);
	watershed(srcImg, markerImg);

	// 创建输出图像并显示分割结果
	Mat wsImg(markerImg.size(), CV_8UC3);
	for(int i = 0; i < markerImg.rows; i++)
	{
		for(int j = 0; j < markerImg.cols; j++)
		{
			int idx = markerImg.at<int>(i,j);
			if(idx == -1)
			{
				// 在输出图像中，分水岭像素（标记为-1）置为白色
				wsImg.at<Vec3b>(i,j) = Vec3b(255,255,255);
			}
			else if(idx >= 1 && idx <= compCount)
			{
				// 在输出图像中，每个连通成分（标记为1－compCount）置为某种颜色
				wsImg.at<Vec3b>(i,j) = colorTab[idx-1];
			}
			else
			{
				// 其余未知像素置为黑色（应该不会出现这种情况）
				wsImg.at<Vec3b>(i,j) = Vec3b(0,0,0);
			}
		}
	}

	// 混合分割结果与原始灰度图像（可选）
	wsImg = wsImg * 0.5 + srcImg * 0.5;

	// 显示分割结果
	imshow( "Watershed segmentation", wsImg);

	waitKey(0);

	return 0;
}
