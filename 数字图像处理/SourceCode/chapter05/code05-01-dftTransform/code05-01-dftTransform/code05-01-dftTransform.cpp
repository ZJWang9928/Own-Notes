/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code05-01-dftTransform.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月4日
  描  述: 实现离散傅立叶变换，并显示其频谱。
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

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    // 优化输入图像大小，以提高计算速度
    Mat padded;
    int m = getOptimalDFTSize( img.rows );
    int n = getOptimalDFTSize( img.cols );
    // 将扩展的像素用0填充
    copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    // 通道合并，将原图像扩展为2个通道，新通道用0填充
    merge(planes, 2, complexI);
    // DFT变换
    dft(complexI, complexI);

    // 计算幅值并转换为对数坐标
    /// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    /// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    split(complexI, planes);
    /// sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], planes[0]);
    /// planes[0] = magnitude
    Mat magI = planes[0];

    // 变换为对数坐标: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    // 频谱平移
    /// 1 | 4      3 | 2
    /// -----  ==> -----
    /// 2 | 3      4 | 1
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    // 创建每个象限ROI区域
    Mat q0(magI, Rect(0, 0, cx, cy));   // 左上角
    Mat q1(magI, Rect(cx, 0, cx, cy));  // 右上角
    Mat q2(magI, Rect(0, cy, cx, cy));  // 左下角
    Mat q3(magI, Rect(cx, cy, cx, cy)); // 右下角

    Mat tmp;
    // 数据交换(左上角与右下角)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // 数据交换（右上角与左下角）
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // 将矩阵变换为标准化数据（0到1）
    normalize(magI, magI, 0, 1, CV_MINMAX);

    // 显示结果
    imshow("Input Image", img);
    imshow("spectrum magnitude", magI);
    waitKey();

    return 0;
}
