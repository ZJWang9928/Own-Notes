/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code05-02-dctTransform.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月4日
  描  述: 实现离散余弦变换，并显示其频谱。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      耿楠    2014/03/04     1.0     创建文件
***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    // 读入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    // 图像宽和高必须是2的整数倍，若不是则进行调整
    Mat padded;
    int w = img.cols;
    int h = img.rows;
    int w2,h2;
    if (w % 2 == 0)
        w2 = w;
    else
        w2 = w + 1;
    if (h % 2 == 0)
        h2 = h;
    else
        h2 = h + 1;

    // 扩展图像，并将扩展的像素用0填充
    copyMakeBorder(img, padded, 0, h2 - h, 0, w2 - w, BORDER_CONSTANT, Scalar::all(0));

    // dct()函数要求数据是浮点数
    Mat imgFloat = Mat(padded.rows, padded.cols, CV_64FC1);
    padded.convertTo(imgFloat, CV_64FC1);

    // 执行DCT变换
    Mat imgFreq;
    dct(imgFloat, imgFreq);

    // 转换为8位灰度
    Mat imgdisp = Mat(imgFloat.rows, imgFloat.cols, CV_8UC1);
    imgFloat.convertTo(imgdisp, CV_8UC1);

    Mat freqDisp = Mat(imgFreq.rows, imgFreq.cols, CV_8UC1);
    imgFreq.convertTo(freqDisp, CV_8UC1);

    // 显示结果
    imshow("Input Image", imgdisp);
    imshow("spectrum magnitude", freqDisp);

    waitKey(0);

    return 0;
}
