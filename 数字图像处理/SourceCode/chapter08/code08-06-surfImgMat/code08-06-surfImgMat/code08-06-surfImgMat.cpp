/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-06-surfImgMat.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现surf图像匹配算法。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宁纪锋  2014/04/20    1.0     建立该文件
***********************************************************/

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	string firstFile, secondFile;
	cout << "Enter the first image file name: ";
	cin >> firstFile;
	cout << "Enter the second image file name: ";
	cin >> secondFile;
	
	Mat img1=imread(firstFile);
	if(img1.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	Mat img2=imread(secondFile);
	if(img2.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}


	// 转换为灰度图像
	if (img1.channels()>=3)
		cvtColor(img1,img1,CV_RGB2GRAY);

	if (img2.channels()>=3)
		cvtColor(img2,img2,CV_RGB2GRAY);

  int minHessian = 8000;

  SurfFeatureDetector detector( minHessian );

  vector<KeyPoint> keyPnt1, keyPnt2;

  // 检测surf关键点
  detector.detect( img1, keyPnt1 );
  detector.detect( img2, keyPnt2 );

  SurfDescriptorExtractor extractor;

  Mat desc1, desc2;

  // 计算surf特征向量
  extractor.compute( img1, keyPnt1, desc1 );
  extractor.compute( img2, keyPnt2, desc2 );

  // 匹配surf特征点
  BFMatcher matcher(NORM_L2);
  vector< DMatch > matches;
  matcher.match( desc1, desc2, matches );

  // 画匹配结果
  Mat img_matches;
  drawMatches(img1, keyPnt1, img2, keyPnt2, matches, img_matches,CV_RGB(0,0,0),CV_RGB(0,0,0),Mat(),4);
  // 显示匹配结果
  imshow("Matches", img_matches );

  waitKey(0);

  return 0;
  }