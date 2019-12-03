/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code02-01-readAndWriteImage.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月1日
  描  述: 利用OpenCV读写和显示图像。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      耿楠    2014/03/01     1.0     创建文件
***********************************************************/


#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat image, result;

	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	cout << "size: " << image.size().height << " , "<< image.size().width << endl;

    namedWindow("Original Image");
    imshow("Original Image", image);

    flip(image,result,1);

    namedWindow("Output Image");
    imshow("Output Image", result);

    imwrite("output.bmp", result);

    waitKey(0);

    return 0;
}
