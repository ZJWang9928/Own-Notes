/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: plate.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: Plate类实现文件，用于记录车牌图像及其信息
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/
#include "plate.h"

Plate::Plate()
{
    //ctor
}

Plate::Plate(Mat img, Rect pos)
{
    plateImg = img;
    platePos = pos;
}

Plate::~Plate()
{
    //dtor
}

Rect Plate::getPlatePos()
{
    return platePos;
}
void Plate::setPlatePos(Rect rectPos)
{
    platePos = rectPos;
}
Mat Plate::getPlateImg()
{
    return plateImg;
}
void Plate::setPlateImg(Mat img)
{
    plateImg = img;
}
vector< string > Plate::getStrChars()
{
    return strChars;
}
void Plate::pushStrChars(string strChar)
{
    strChars.push_back(strChar);
}
vector< Rect > Plate::getCharsPos()
{
    return charsPos;
}
void Plate::pushCharsPos(Rect rectPos)
{
    charsPos.push_back(rectPos);
}

/*************************************************
  名    称: string Plate::str()
  描    述: 按从左到右的顺序生成车牌字符串
  输入参数: 无
  输出参数: 无
  返    回: str-->车牌字符串
  其    它: 无
  作    者：耿楠
  日    期：2014年3月11日
*************************************************/
string Plate::str()
{
    string result = "";

    // 字符顺序
    vector< int > orderIndex;
    vector< int > xPositions;

    for (int i = 0; i < (int)charsPos.size(); i++)
    {
        orderIndex.push_back(i);
        xPositions.push_back(charsPos[i].x);
    }

    float min = xPositions[0];
    int minIdx = 0;

    for (int i = 0; i < (int)xPositions.size(); i++)
    {
        min = xPositions[i];
        minIdx = i;
        for (int j = i; j < (int)xPositions.size(); j++)
        {
            if (xPositions[j] < min)
            {
                min = xPositions[j];
                minIdx = j;
            }
        }

        int auxI = orderIndex[i];
        int auxMin = orderIndex[minIdx];
        orderIndex[i] = auxMin;
        orderIndex[minIdx] = auxI;

        float auxXi = xPositions[i];
        float auxXmin = xPositions[minIdx];
        xPositions[i] = auxXmin;
        xPositions[minIdx] = auxXi;
    }

    for (int i = 0; i < (int)orderIndex.size(); i++)
    {
        result = result + strChars[orderIndex[i]];
    }

    return result;
}
