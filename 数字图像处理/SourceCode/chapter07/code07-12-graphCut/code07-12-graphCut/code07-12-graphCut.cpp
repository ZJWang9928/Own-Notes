/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code07-12-graphCut.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年8月10日
  描  述: 基于图割（Graph Cut）的图像分割，首先采用高斯混合模型GMM对图像目标与背景的灰度概率分布进行建模，
          再通过EM方法求解，接着构建s-t图，最后利用Min-Cut/Max-Flow方法求解得到能量最优化后的分割结果。
  其  它:
  参考文献:
    1. "An Experimental Comparison of Min-Cut/Max-Flow Algorithms for Energy Minimization in Vision."
	    Yuri Boykov and Vladimir Kolmogorov.
	    In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), September 2004
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/08/10    1.0     建立该文件
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>
#include <iostream>

// 引用Yuri Boykov and Vladimir Kolmogorov的最小图割代码
#include "gc\GCoptimization.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	/// 加载原始图像
	string fileName;
	cout << "Enter the source image file name: ";
	cin >> fileName;
	Mat srcImg = imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << fileName << endl;
		return -1;
	}

	///==================================================================================
	//               利用高斯混合模型（GMM）对图像目标与背景进行建模
	// 通过高斯混合模型获得目标与背景的灰度概率分布，将用于构建s-t图时计算t-links的权值
	// 注：读者也可以试着用K-均值聚类等方法估算目标与背景的灰度概率分布。
	// 注：对于同一幅图像，可以将建模结果保存起来供后面的图割多次使用，以节省训练时间。
	///----------------------------------------------------------------------------------
	// 创建高斯混合模型
	int labelNum = 2;	// 聚类数目（本例只实现前景与背景的分割，故只需2类，用户可根据实际图像内容设定）
	cv::EM gmmModel(labelNum);

	// 指定训练样本（单通道，每行作为一个样本）
	Mat sampleImg;
	srcImg.reshape(1, srcImg.rows * srcImg.cols).convertTo(sampleImg, CV_32FC1, 1.0/255.0);
	
	// 样本训练，获取训练结果
	Mat likeImg;	// 似然概率（尺度：样本数目*1，类型：CV_32FC1）
	Mat labelImg;	// 标记结果（尺度：样本数目*1，类型：CV_32SC1）
	Mat probImg;	// 后验概率（尺度：样本数目*聚类数目，类型：CV_64FC1）
	gmmModel.set("maxIters", 300);
	gmmModel.set("epsilon", 0.001);
	cout << "Starting EM training" << endl; 
	gmmModel.train(sampleImg, likeImg, labelImg, probImg); // 不需要的输出矩阵可用noArray()代替
	cout << "Finished training EM" << endl;

	///==================================================================================
	//                 基于图像数据构建s-t图
	// 图像的每个像素都成为s-t图中的一个普通顶点，每个像素与上下左右四个像素都有一条
	// n-links边，每个像素与终端顶点S和T各有一条t-links边。n-links边的权值根据像素对的灰度
	// 差异来计算，t-links边的权值根据像素的后验概率（本例由GMM估算得到）来计算。
	//
	// 注：当n-links边的权值需要随像素对的空间位置而变化时，可以使用GCoptimizationGeneralGraph
	// 来创建s-t图，通过setNeighbors成员函数来自定义邻域，否则使用GCoptimizationGridGraph即可。
	///----------------------------------------------------------------------------------
	// 创建s-t图
	//GCoptimizationGridGraph gc(srcImg.cols, srcImg.rows, labelNum);
	GCoptimizationGeneralGraph gc(srcImg.cols * srcImg.cols, labelNum);

	// 设置t-links边的权值（对应区域能量项）
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			int pos = r * srcImg.cols + c;
			double *pp = probImg.ptr<double>(pos);
			for(int l = 0; l < labelNum; l++)
			{
				// 根据像素的后验概率设置普通顶点与源点S和汇点T的连接权值
				// 注：用户可以根据需要设计不同的权值计算方法，如依赖于图像梯度等。
				gc.setDataCost(pos, l, (float)-log(pp[l]));
			}
		}
	}

	// 设置n-links边的权值（对应边界平滑项）
	//float beta = 20;
	//for(int l1 = 0; l1 < labelNum; l1++)
	//{
	//	for(int l2 = 0; l2 < labelNum; l2++)
	//	{
	//		// 注：用户可以根据需要设计不同的权值计算方法，如依赖于图像梯度等。
	//		float cost = beta * (1 - (int)(l1 == l2));
	//		gc.setSmoothCost(l1, l2, cost);
	//	}
	//}

	//===================================================================================
	// 下面一段代码根据像素对<p,q>的灰度差异来计算其连接权值， 即：
	// cost(p,q,Lp,Lq) = exp(-(Ip-Iq)^2/(2*sigma^2)) * (1-(Lp==Lq)
	// 式中，Lp和Lq分别为分配给像素p和q的标记号，Ip和Iq分别为像素p和q的灰度值
	// 注意：需要用GCoptimizationGeneralGraph来创建s-t图，通过setNeighbors来设置邻域像素及其权值
	
	// 边界平滑项的代价
	float beta = 20;
	for(int lp = 0; lp < labelNum; lp++)
	{
		for(int lq = 0; lq < labelNum; lq++)
		{
			float pqDelta = beta * (1 - (int)(lp == lq));
			gc.setSmoothCost(lp, lq, (float)pqDelta);
		}
	}

	// 设置水平邻域
	float sigma = 50;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 1; c < srcImg.cols; c++)
		{
			int q = r * srcImg.cols + c;
			int p = q - 1;

			// 对邻域像素p和q之间的不连续性进行惩罚
			int pqDiff = srcImg.at<uchar>(r, c-1) - srcImg.at<uchar>(r, c);
			float pqCost = exp(-(pqDiff * pqDiff) / (2 * sigma * sigma));
			gc.setNeighbors(p, q, pqCost);
		}
	}
	//-----------------------------------------------------------------------------------


	// 设置垂直邻域
	for(int r = 1; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			int q = r * srcImg.cols + c;
			int p = q - srcImg.cols;

			// 对邻域像素p和q之间的不连续性进行惩罚
			int pqDiff = srcImg.at<uchar>(r-1, c) - srcImg.at<uchar>(r, c);
			float pqCost = exp(-(pqDiff * pqDiff) / (2 * sigma * sigma));
			gc.setNeighbors(p, q, pqCost);
		}
	}

	///==================================================================================
	//                 求解最小割（最大流）
	///----------------------------------------------------------------------------------
	cout << "Before optimization energy is " << gc.compute_energy() << endl; 
	gc.expansion(1); 
	cout << "After optimization energy is " << gc.compute_energy() << endl;

	///==================================================================================
	//                 输出图割结果
	///----------------------------------------------------------------------------------
	// 创建标记颜色表（用于给每一种区域标记赋予一种随机颜色）
	vector<Vec3b> colorTab;
	colorTab.reserve(labelNum);
	for(int i = 0; i < labelNum; i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);

		colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// 根据图割结果创建输出图像
	Mat dstImg;
	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	int i = 0;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			dstImg.at<Vec3b>(r, c) = colorTab[gc.whatLabel(i++)];
		}
	}

	///==================================================================================
	//                 显示分割结果
	///----------------------------------------------------------------------------------
	// 显示原始图像
	imshow("Original image", srcImg);

	// 显示标记结果
	Mat colorLabel(srcImg.size(), CV_8UC3);
	int j = 0;
	for(int r = 0; r < srcImg.rows; r++)
	{
		for(int c = 0; c < srcImg.cols; c++)
		{
			colorLabel.at<Vec3b>(r, c) = colorTab[labelImg.at<int>(j++)];
		}
	}
	imshow("Labeled image", colorLabel);

	// 显示似然概率
	likeImg *= -1;
	likeImg = likeImg.reshape(1, srcImg.rows);
	imshow("Minus log likelihood image", likeImg);

	// 显示分割结果
	imshow("Segmentation result", dstImg);
	waitKey(0);

	return 0;
}

