/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: charsegment.h
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: CharSegment类定义，记录字符图像及其在车牌中的位置
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/
#ifndef CHARSEGMENT_H
#define CHARSEGMENT_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class CharSegment
{
public:
    CharSegment();
    CharSegment(Mat matImg, Rect rectPos);
    ~CharSegment();

    // Get和Set函数，读取和设置私有成员变量
public:
    Mat getCharImg();
    void setCharImg(Mat img);
    Rect getCharPos();
    void setCharPos(Rect rectPos);

public:

protected:
private:
    Mat charImg;
    Rect charPos;
};

#endif // CHARSEGMENT_H
