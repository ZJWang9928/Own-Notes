/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code02-06-reduceColorWithPtrData.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月1日
  描  述: 利用data成员指针访问图像数据，实现颜色下采样。
  其  它:
  函数列表:
      1. void colorReduce：颜色下采样
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      耿楠    2014/03/01     1.0     创建文件
***********************************************************/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void colorReduce(Mat& inputImage, Mat& outputImage, int div);

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	Mat image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}


    imshow("源图像",image);
    Mat result;//用来保存结果
    result.create(image.rows,image.cols,image.type());//它的大小、类型与原图片相匹配
    double duration;
    duration = static_cast<double>(cv::getTickCount());

    colorReduce(image,result,64);

    duration = static_cast<double>(cv::getTickCount())-duration;
    duration /= cv::getTickFrequency(); // the elapsed time in m
    cout<<"time is"<<duration<<endl;

    imshow("显示结果",result);
    waitKey(0);
}

void colorReduce(Mat& inputImage, Mat& outputImage, int div)
{
    outputImage = inputImage.clone();
    int rows = outputImage.rows;
    int cols = outputImage.cols;

    uchar* dataout = outputImage.data;

    for(int i = 0; i < rows; i++)
    {
        //dataout = outputImage.data + i *  outputImage.step;
        for(int j = 0; j < cols; j++)
        {
            *dataout = *dataout/div*div + div/2;
//            dataout[1] = dataout[1]/div*div + div/2;
//            dataout[2] = dataout[2]/div*div + div/2;
            dataout++;// += outputImage.elemSize();
        }
    }
}
