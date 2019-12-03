/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code04-03-flip.cpp
  作  者: 宋怀波
  版  本: 1.0
  日  期: 2014-08-11
  描  述: 进行图像的镜像变换并显示镜像图像
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宋怀波   2014-08-11     1.0     建立该文件
***********************************************************/

#include <iostream>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/// 输入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat src = imread(srcFileName, 1);
	if(src.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// 镜像变换
	Mat dst;
	flip(src, dst, 1);

	/// 显示原图像
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);

	/// 显示镜像图像
	namedWindow("dst", CV_WINDOW_AUTOSIZE);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}
