/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: charsegment.cpp
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
#include "charsegment.h"

CharSegment::CharSegment()
{
    //ctor
}
CharSegment::CharSegment(Mat matImg, Rect rectPos)
{
    charImg = matImg;
    charPos = rectPos;
}
CharSegment::~CharSegment()
{
    //dtor
}

Mat CharSegment::getCharImg()
{
    return charImg;
}
void CharSegment::setCharImg(Mat img)
{
    charImg = img;
}
Rect CharSegment::getCharPos()
{
    return charPos;
}
void CharSegment::setCharPos(Rect rectPos)
{
    charPos = rectPos;
}
