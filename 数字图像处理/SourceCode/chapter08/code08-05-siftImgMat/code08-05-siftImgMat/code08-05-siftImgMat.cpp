/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code08-05-siftImgMat.cpp
  作  者: 宁纪锋
  版  本: 1.0
  日  期: 2014年4月20日
  描  述: 实现sift图像匹配算法。
  其  它:
  函数列表:
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宁纪锋  2014/04/20    1.0     建立该文件
***********************************************************/

#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/nonfree/features2d.hpp"  

#include <iostream>  
#include <stdio.h>  
#include <stdlib.h>  
  
using namespace cv;  
using namespace std;  
  
int main(int argc, char** argv)  
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

	int thresh = 125;
	int max_thresh = 255;

    initModule_nonfree();//初始化模块

    Ptr<FeatureDetector> detector = FeatureDetector::create( "SIFT" );//创建SIFT特征检测器  
    Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create( "SIFT" );//创建特征向量生成器  
    Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create( "BruteForce" );//创建特征匹配器  
    if( detector.empty() || descriptor_extractor.empty() )  
        cout<<"fail to create detector!";  
  
  
    //特征点检测  
    vector<KeyPoint> keyPnt1,keyPnt2;  
    detector->detect( img1, keyPnt1 );//检测img1中的SIFT特征点，存储到keyPnt1中  
    detector->detect( img2, keyPnt2 );  
    cout<<"图像1特征点个数:"<<keyPnt1.size()<<endl;  
    cout<<"图像2特征点个数:"<<keyPnt2.size()<<endl;  
  
    //根据特征点计算特征描述子矩阵，即特征向量矩阵  
    Mat desc1,desc2;  
    descriptor_extractor->compute( img1, keyPnt1, desc1 );  
    descriptor_extractor->compute( img2, keyPnt2, desc2 );  

    cout<<"图像1特征描述矩阵大小："<<desc1.size()  
        <<"，特征向量个数："<<desc1.rows<<"，维数："<<desc1.cols<<endl;  
    cout<<"图像2特征描述矩阵大小："<<desc2.size()  
        <<"，特征向量个数："<<desc2.rows<<"，维数："<<desc2.cols<<endl;  
  
    //画出特征点  
    Mat img_keyPnt1,img_keyPnt2;  
    drawKeypoints(img1,keyPnt1,img_keyPnt1,Scalar::all(-1),0);  
    drawKeypoints(img2,keyPnt2,img_keyPnt2,Scalar::all(-1),0);   
  
    //特征匹配  
    vector<DMatch> matches;//匹配结果  
    descriptor_matcher->match( desc1, desc2, matches );//匹配两个图像的特征矩阵  
    cout<<"Match个数："<<matches.size()<<endl;  
  
    //计算匹配结果中距离的最大和最小值  
    //距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近  
    double max_dist = 0;  
    double min_dist = 100;  
    for(int i=0; i<matches.size(); i++)  
    {  
        double dist = matches[i].distance;  
        if(dist < min_dist) min_dist = dist;  
        if(dist > max_dist) max_dist = dist;  
    }  
    cout<<"最大距离："<<max_dist<<endl;  
    cout<<"最小距离："<<min_dist<<endl;  
  
    //筛选出较好的匹配点  
    vector<DMatch> goodMatches;  
    for(int i=0; i<matches.size(); i++)  
    {  
        if(matches[i].distance < 0.31 * max_dist)  
        {  
            goodMatches.push_back(matches[i]);  
        }  
    }  
    cout<<"goodMatch个数："<<goodMatches.size()<<endl;  
  
    //画出匹配结果  
    Mat img_matches;  
    drawMatches(img1,keyPnt1,img2,keyPnt2,goodMatches,img_matches,  
                CV_RGB(0,0,0),CV_RGB(0,255,0),Mat(),4);  
  
    imshow("MatchSIFT",img_matches);  
    waitKey(0);  
    return 0;  
}  