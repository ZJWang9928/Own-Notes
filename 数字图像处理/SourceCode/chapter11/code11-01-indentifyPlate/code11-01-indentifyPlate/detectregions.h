/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: detectregions.h
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: DetectRegions类定义，用于定位、分割车牌区域
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/

#ifndef DETECTREGIONS_H
#define DETECTREGIONS_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "plate.h"

using namespace cv;
using namespace std;

class DetectRegions
{
public:
    DetectRegions();
    ~DetectRegions();
// Get和Set函数，读取和设置私有成员变量
public:
    bool getSaveRegions();
    void setSaveRegions(bool isSaveRegions);
    bool getShowSteps();
    void setShowSteps(bool isShowSteps);
    string getStrFilename();
    void setStrFilename(string filename);
    Size getPlateSize();
    void setPlateSize(Size inSize);

public:
    vector<Plate> run(Mat input);

private:
    vector<Plate> segment(Mat input);
    bool verifySizes(RotatedRect mr);
    Mat histeq(Mat in);

protected:
private:
    bool saveRegions;
    bool showSteps;
    Size plateSize;
    string strFilename;
};

#endif // DETECTREGIONS_H
