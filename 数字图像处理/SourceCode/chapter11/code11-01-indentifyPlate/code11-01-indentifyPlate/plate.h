/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: plate.h
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: Plate类定义，记录车牌区域图像和及其在原图中的位置
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/
#ifndef PLATE_H
#define PLATE_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class Plate
{
public:
    Plate();
    Plate(Mat img, Rect pos);
    ~Plate();

// Get和Set函数，读取和设置私有成员变量
public:
    Rect getPlatePos();
    void setPlatePos(Rect rectPos);
    Mat getPlateImg();
    void setPlateImg(Mat img);
    vector< string > getStrChars();
    void pushStrChars(string strChar);
    vector< Rect > getCharsPos();
    void pushCharsPos(Rect rectPos);

public:
    string str();
protected:
private:
    Rect platePos;
    Mat plateImg;
    vector< string > strChars;
    vector< Rect > charsPos;
};

#endif // PLATE_H
