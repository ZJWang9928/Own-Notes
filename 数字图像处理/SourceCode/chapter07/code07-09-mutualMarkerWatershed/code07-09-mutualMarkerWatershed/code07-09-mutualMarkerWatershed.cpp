/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-09-mutualMarkerWatershed.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月8日
  描  述: 通过鼠标交互绘制标记（Marker）来实现基于标记的分水岭分割。
  其  它:
  参考文献:
    1. OpenCV示例代码：samples/cpp/watershed.cpp
  函数列表:
    1. void onMouseEvent() 鼠标事件的回调函数
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/08    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// 鼠标事件回调函数所需的用户数据类型
struct UserData
{
	Mat* pSrcImg;		// 原始图像
	Mat* pMarkerMask;	// 标记模板
	Point ptPrev;		// 上一鼠标位置
};

/**
 * 鼠标事件的回调函数，用于交互绘制标记
 */
static void onMouseEvent( int event, int x, int y, int flags, void* userData)
{
	UserData *pUserData = (UserData *)userData;
	if(!pUserData)
		return;

	if(x < 0 || x >= pUserData->pSrcImg->cols || y < 0 || y >= pUserData->pSrcImg->rows)
		return;

	if(event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
	{
		pUserData->ptPrev = Point(-1,-1);
	}
	else if( event == CV_EVENT_LBUTTONDOWN)
	{
		pUserData->ptPrev = Point(x,y);
	}
	else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
	{
		/// 按下鼠标左键移动时，绘制用作标记的线条
		Point pt(x, y);
		if(pUserData->ptPrev.x < 0)
		{
			pUserData->ptPrev = pt;
		}

		// 绘制到标记模板（用于生成标记图像）
		line(*pUserData->pMarkerMask, pUserData->ptPrev, pt, Scalar::all(255), 5, 8);

		// 绘制到原始图像（用于显示标记过程）
		line(*pUserData->pSrcImg, pUserData->ptPrev, pt, Scalar::all(255), 5, 8);

		// 保存当前点
		pUserData->ptPrev = pt;

		imshow("Original image", *pUserData->pSrcImg);
	}
}

int main( int argc, char** argv )
{
	/// 打印帮助信息
	cout << "分割前请在原图上用鼠标粗略勾画出各个目标与背景的标记！\n"
		"\tESC - 退出应用程序\n"
		"\tr - 清除标记，恢复原图\n"
		"\tw or SPACE - 基于标记的分水岭分割\n";

	/// 输入原始图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName <<endl;
		return -1;
	}

	/// 创建图像副本（用于恢复原始图像）
	Mat orgImg;
	srcImg.copyTo(orgImg);

	/// 创建标记模板（用于记录用户勾画的标记）
	Mat markerMask;
	cvtColor(srcImg, markerMask, COLOR_BGR2GRAY);

	/// 创建灰度副本（用于混合原始图像与分割结果）
	Mat grayImg;
	cvtColor(markerMask, grayImg, COLOR_GRAY2BGR);
	markerMask = Scalar::all(0);

	/// 显示原始图像
	imshow("Original image", srcImg);

	/// 设置鼠标回调函数
	UserData userData;
	userData.pSrcImg = &srcImg;
	userData.pMarkerMask = &markerMask;
	userData.ptPrev = Point(-1, -1);
	setMouseCallback( "Original image", onMouseEvent, &userData);

	/// 处理键盘事件
	bool bLoop = true;
	while(bLoop)
	{
		int c = waitKey(0);
		switch (c)
		{
		case 27:	// 退出应用程序
			bLoop = false;
			break;
		case 'r':	// 恢复原始状态
			markerMask = Scalar::all(0);
			orgImg.copyTo(srcImg);
			imshow("Original image", srcImg);
			break;
		case 'w':
		case ' ':	// 应用基于标记的分水岭分割
			{
				// 提取标记（marker）的轮廓
				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;
				findContours(markerMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

				// 创建标记图像（每个标记赋以一个大于0的整数值，以该值填充标记轮廓）
				Mat markerImg = Mat::zeros(markerMask.size(), CV_32S);				
				int compCount = 0;
				for(int idx = 0; idx >= 0; idx = hierarchy[idx][0], compCount++)
				{
					drawContours(markerImg, contours, idx, Scalar::all(compCount+1), CV_FILLED, 8, hierarchy);
				}

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
				watershed(orgImg, markerImg);

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
							// 未知像素置为黑色（应该不会出现这种情况）
							wsImg.at<Vec3b>(i,j) = Vec3b(0,0,0);
						}
					}
				}

				// 混合分割结果与原始灰度图像（可选）
				wsImg = wsImg * 0.5 + grayImg * 0.5;

				// 显示分割结果
				imshow( "Watershed segmentation", wsImg);
			}
			break;
		default:
			break;
		}
	}

	return 0;
}
