/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: // code06-07-thinning
  作  者: // 胡少军、何东健、耿楠等
  日  期: // 2014年8月4日
  描  述: // 快速细化算法的OpenCV实现
  函数列表:
    1. Mat thinning(const Mat &binaryImg); //对二值图像进行细化运算
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//*************************************************************************************
//函数名称：Mat thinning()
//基本功能：对二值图像进行细化运算
//参数说明：待细化二值图像，该图像中背景色为0，前景色（目标）为255
//返 回 值：返回细化后二值图像
//*************************************************************************************
Mat thinning(const Mat &binaryImg)
{
	int i, j, k;
	uchar p[11];
	int pos[9][2] = {{0,0}, {-1,0} , {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}};
	int cond1, cond2, cond3, cond4, counter=0;
	bool pointsDeleted = true;
	Mat mask, dstImg;

	//转化为0,1二值图像
	dstImg = binaryImg / 255;
	//若没有可删除像素点
	while (pointsDeleted)
	{
		mask = Mat::zeros(dstImg.size(), CV_8UC1);//初始化模板为全0
		pointsDeleted = false;
		for (i=1; i<dstImg.rows-1; i++)
		{
			for (j=1; j<dstImg.cols-1; j++)
			{
				//获取3*3结构元素p1~p9对应像素值, 其中p1为中心点
				for (k=1; k<10; k++)
					p[k] = dstImg.at<uchar>(i+pos[k-1][0], j+pos[k-1][1]);

				//若中心点为背景色，跳过
				if(p[1]==0) continue;

				//计算中心点周围所有像素值之和
				cond1 = 0;
				for (k=2; k<10; k++) cond1 += p[k];

				//计算p2~p9从0到1变化的次数
				cond2 = 0;
				p[10] = p[2]; //用于处理k=8时, p[k+2]越界情况
				for (k=2; k<10; k+=2)
					cond2 += ((p[k]==0 && p[k+1]==1) + (p[k+1]==0 && p[k+2]==1));

				if(counter%2==0)//偶数次迭代判断条件
				{
					cond3 = p[2] * p[4] * p[6];
					cond4 = p[4] * p[6] * p[8];
				}
				else//奇数次迭代判断条件
				{
					cond3 = p[2] * p[4] * p[8];
					cond4 = p[2] * p[6] * p[8];
				}
				//若同时满足条件1~条件4
				if ((2<=cond1 && cond1<= 6) && (cond2==1) && (cond3==0) && (cond4==0))
				{
					pointsDeleted = true;
					mask.at<uchar>(i, j) = 1; //写入待删除的像素点至模板
				}
			}
		}
		dstImg &= ~mask; //通过逻辑与操作删除目标像素点
		counter++;
	}
	//恢复为0, 255二值图像
	dstImg *= 255;

	return dstImg;
}


int main( int argc, char** argv )
{
	Mat srcImg, binaryImg, dstImg;

	/// 装载图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	Mat grayImg (srcImg.size(), CV_8U);

	//将源图像转化为灰度图像
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	//如果细化目标为黑色区域，则反转图像
	grayImg = 255 - grayImg;

	//二值化处理
	threshold(grayImg, binaryImg, 100, 255, THRESH_BINARY);

	//细化处理
	dstImg = thinning(binaryImg);

	dstImg = 255 - dstImg;
	imwrite("..\\figures\\figure6_21_e.bmp", dstImg);

	imshow( "Before thinning", srcImg );
	imshow( "After thinning", dstImg );

	waitKey(0);
	return 0;
}
