/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code03-07-imageNoise.cpp
  作  者: 龙满生
  版  本: 1.0
  日  期: 2014年7月11日
  描  述: 向图像中添加加性椒盐噪声或高斯噪声。
  其  它:
  函数列表:
    1. void SaltAndPepperNoise() 添加椒盐噪声
    2. void GaussianNoise() 添加高斯噪声
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      龙满生  2014/07/11    1.0     建立该文件
***********************************************************/
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <ctime>

using namespace std;
using namespace cv;

/// 添加椒盐噪声
void SaltAndPepperNoise(Mat& ioImg, double noiseDensity = 0.05);

/// 添加高斯噪声
void GaussianNoise(Mat& ioImg, double mean = 0.0, double sigma = 5.0);

int main()
{
	/// 指定图像文件
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}
	//cvtColor(srcImg, srcImg, CV_RGB2GRAY);

	/// 显示原始图像
	imshow("Original Image", srcImg);

	/// 产生椒盐噪声
	Mat saltpepperImg = srcImg.clone();
	SaltAndPepperNoise(saltpepperImg, 0.02);

	/// 显示椒盐噪声图像
	imshow("Salt & Pepper Noise Image", saltpepperImg);

	/// 产生高斯噪声
	Mat gaussianImg = srcImg.clone();
	GaussianNoise(gaussianImg, 0, 25.0);

	/// 显示高斯噪声图像
	imshow("Gaussian Noise Image", gaussianImg);

	waitKey(0);

	return 0;
}

/**
* 向图像添加椒盐噪声
* @param ioImg 处理前为原始图像，处理后为噪声图像
* @param noiseDensity 噪声密度（噪声点占像素数的比例，默认为0.05）
*/
void SaltAndPepperNoise(Mat& ioImg, double noiseDensity)
{
	if(ioImg.empty())
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// 创建浮点型均匀噪声图像[0, 1.0)
	Mat noiseImg = Mat::zeros(ioImg.size(), CV_32F);
	randu(noiseImg, 0.0f, 1.0f);

	/// 根据噪声密度分别创建黑点模板和白点模板
	Mat blackMask = noiseImg < (noiseDensity / 2.0f);
	Mat whiteMask = noiseImg >= (1.0f - noiseDensity / 2.0f);

	/// 根据黑点模板和白点模板分别将对应像素置为黑色（胡椒噪声）和白色（盐噪声）
	ioImg.setTo(0, blackMask);
	ioImg.setTo(255, whiteMask);

	/**
	可以使用下面的代码近似代替上面的代码，用RNG::uniform来产生均匀噪声。
	*/

/*
	/// 为了简化代码，暂时只处理无符号字符型图像
	if(ioImg.empty() || ioImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return;
	}

	/// 初始化随机数
	RNG rng(time(NULL));

	/// 添加椒盐噪声（脉冲噪声）
	int r, c;
	double randVal, halfVal = noiseDensity / 2.0;
	switch (ioImg.channels())
	{
	case 1:
		for(r = 0; r < ioImg.rows; r++)
		{
			for(c = 0; c < ioImg.cols; c++)
			{
				// 产生[0,1)之间的均匀噪声
				randVal = rng.uniform(0.0f, 1.0f);

				if(randVal < halfVal)
				{
					// 胡椒噪声
					ioImg.at<uchar>(r, c) = 0;
				}
				else if(randVal >= 1.0f - halfVal)
				{
					// 盐噪声
					ioImg.at<uchar>(r, c) = 255;
				}
			}
		}
		break;
	case 3:
		for(r = 0; r < ioImg.rows; r++)
		{
			for(c = 0; c < ioImg.cols; c++)
			{
				// 产生[0,1)之间的均匀噪声
				randVal = rng.uniform(0.0, 1.0);

				if(randVal < halfVal)
				{
					// 胡椒噪声
					ioImg.at<cv::Vec3b>(r, c) = cv::Vec3b(0, 0, 0);
				}
				else if(randVal < noiseDensity)
				{
					// 盐噪声
					ioImg.at<cv::Vec3b>(r, c) = cv::Vec3b(255, 255, 255);
				}
			}
		}
		break;
	default:
		break;
	}
*/
}

/**
* 向图像添加高斯噪声
* @param ioImg 处理前为原始图像，处理后为噪声图像
* @param mean 噪声均值
* @param sigma 噪声标准差
*/
void GaussianNoise(Mat& ioImg, double mean, double sigma)
{
	if(ioImg.empty())
	{
		cerr << "Invalid image format." << endl;
		return;
	}

	/// 根据原始图像创建浮点型图像
	Mat dstImg;
	ioImg.convertTo(dstImg, CV_MAKE_TYPE(CV_64F, ioImg.channels()));

/*
	// 下面这段代码为各通道分别产生高斯噪声，因而对于多通道其噪声是彩色的
	/// 指定噪声各通道的均值和标准差
	Mat meanMat = Mat::zeros(ioImg.channels(), 1, CV_64FC1);
	Mat sigmaMat = Mat::ones(ioImg.channels(), 1, CV_64FC1);
	meanMat += mean;
	sigmaMat *= sigma;

	/// 生成高斯噪声
	Mat noiseImg(dstImg.size(), dstImg.type());
	randn(noiseImg, meanMat, sigmaMat);

	/// 叠加噪声到图像
	dstImg += noiseImg;
*/

	//==============================================
	// 下面这段代码为各通道产生相同的高斯噪声，因而对于多通道其噪声是灰色的
	/// 生成高斯噪声
	Mat noiseImg(dstImg.size(), CV_64F);
	randn(noiseImg, mean, sigma);

	/// 叠加噪声到图像各通道
	std::vector<Mat> noiseVec;
	cv::split(dstImg, noiseVec);
	for(int i = 0; i < dstImg.channels(); i++)
	{
		noiseVec[i] += noiseImg;
	}
	cv::merge(noiseVec, dstImg);
	//----------------------------------------------

	/// 转换到目标图像
	dstImg.convertTo(ioImg, ioImg.type());

	/**
	可以使用下面的代码近似代替上面的代码，用RNG::gaussian来产生高斯噪声。
	*/

/*
	/// 根据原始图像创建浮点型图像
	Mat dstImg;
	ioImg.convertTo(dstImg, CV_MAKE_TYPE(CV_64F, ioImg.channels()));

	/// 初始化随机数
	RNG rng(time(NULL));

	/// 添加高斯噪声
	int y, x;
	double randVal;
	double* pData = NULL;
	switch (dstImg.channels())
	{
	case 1:
		for(y = 0; y < dstImg.rows; y++)
		{
			pData = dstImg.ptr<double>(y);
			for(x = 0; x < dstImg.cols; x++)
			{
				/// 生成高斯噪声
				randVal = rng.gaussian(sigma);

				/// 叠加高斯噪声
				pData[x] += mean + randVal;
			}
		}
		break;
	case 3:
		for(y = 0; y < dstImg.rows; y++)
		{
			pData = dstImg.ptr<double>(y);
			for(x = 0; x < dstImg.cols; x++)
			{
				/// 生成高斯噪声
				randVal = rng.gaussian(sigma);

				/// 叠加高斯噪声
				randVal += mean;
				pData[0] += randVal;
				pData[1] += randVal;
				pData[2] += randVal;

				pData += 3;
			}
		}
		break;
	default:
		break;
	}

	/// 转换到目标图像
	dstImg.convertTo(ioImg, ioImg.type());
*/
}