/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-04-regionGrow.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月31日
  描  述: 利用OpenCV的floodfill函数实现区域生长，通过鼠标选择
          区域生长的种子点，通过滑动条调整生长规则（灰度差）。
  参  考：OpenCV示例代码：samples\cpp\ffilldemo.cpp
  函数列表:
    1. void onMouseEvent() 鼠标事件回调函数
    2. void help() 输出帮助
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/31     1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


const string winName = "RegionGrow"; // 窗口名称
Mat srcImg;	// 原始图像
Mat dstImg;	// 填充图像

/// 区域生长的相关参数
int loDiff = 2;		// 当前像素与邻域像素（生长区域）的灰度差值下限的绝对值
int upDiff = 2;		// 当前像素与邻域像素（生长区域）的灰度差值上限的绝对值
int conType = 4;	// 区域连通性（4或8）
int growMode = 0;	// 区域生长准则（0：根据当前像素与相邻像素之间的灰度差值判断；1：根据当前像素与种子像素之间的灰度差值判断）

/// 鼠标回调函数
static void onMouseEvent(int event, int x, int y, int flags, void* userdata);

/// 打印帮助
static void help();

int main( int argc, char** argv )
{
	/// 打印帮助
	help();

	/// 加载原始图像
	string srcFileName;
	cout << "Enter the source file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load the source image." << endl;
		return -1;
	}
	srcImg.copyTo(dstImg);

	/// 命名图像窗口
	namedWindow(winName);

	/// 创建控制loDiff与upDiff的滑块条
	createTrackbar("loDiff", winName, &loDiff, 128, 0, 0);
	createTrackbar("upDiff", winName, &upDiff, 128, 0, 0);

	/// 设置鼠标回调函数
	setMouseCallback(winName, onMouseEvent, &srcImg);

	/// 显示图像
	bool bLoop = true;
	while(bLoop)
	{
		imshow(winName, dstImg);

		int ch = waitKey(0);
		switch (ch)
		{
		case 27:
			bLoop = false;
			break;
		case '4':
			cout << "4-connectivity mode is set\n";
			conType = 4;
			break;
		case '8':
			cout << "8-connectivity mode is set\n";
			conType = 8;
			break;
		case 'f':
			if(growMode == 0)
			{
				growMode = 1;
				cout << "Fixed range floodfill mode is set\n";
			}
			else
			{
				growMode = 0;
				cout << "Floating range floodfill mode is set\n";
			}
			break;
		default:
			break;
		}
	}

    return 0;
}

/**
 * 打印帮助
 */
static void help()
{
    cout << "Hot keys: \n"
            "\tESC - quit the program\n"
            "\tr - restore the original image\n"
            "\tf - switch floodfill mode between floating range and fixed range\n"
            "\t4 - use 4-connectivity mode\n"
            "\t8 - use 8-connectivity mode\n" << endl;
}

/**
 * 鼠标回调函数：先通过鼠标单击获取区域生长的种子点，随后进行区域生长
 * @param event 鼠标事件类型
 * @param x 鼠标事件发生时的水平坐标分量
 * @param y 鼠标事件发生时的垂直坐标分量
 * @param flags 鼠标事件标志
 * @param userdata 附加的用户数据
 */
static void onMouseEvent(int event, int x, int y, int flags, void* userdata)
{
	if(dstImg.empty())
	{
		return;
	}
	
	/// 单击左键选择区域生长的种子点
	if(event == CV_EVENT_FLAG_LBUTTON)
	{
		// 指定区域生长种子点
		Point seedPoint(x, y);

		// 指定生长区域的颜色
		int r = (unsigned char)theRNG() & 255;
		int g = (unsigned char)theRNG() & 255;
		int b = (unsigned char)theRNG() & 255;
		Scalar regionColor = (dstImg.channels() == 1) ? Scalar(r * 0.299 + g * 0.587 + b * 0.114) : Scalar(r, g, b);
	
		/// 实现区域生长
		// floodFill函数的填充标志（高8位为填充模式，低8位为连通性类型）
		int fillFlags = conType + 0xFF00 + (growMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
		int regionArea = floodFill(dstImg, seedPoint, regionColor, 0, 
			Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff), fillFlags);

		// 显示图像
		imshow(winName, dstImg);
		cout << "Region area: " << regionArea << endl;
	}

	/// 单击右键重置填充图像
	if(event == CV_EVENT_FLAG_RBUTTON)
	{
		srcImg.copyTo(dstImg);

		// 显示图像
		imshow(winName, dstImg);
	}
}