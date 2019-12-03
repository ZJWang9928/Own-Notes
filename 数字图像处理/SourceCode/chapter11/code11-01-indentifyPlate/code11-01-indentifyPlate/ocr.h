/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: ocr.h
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: OCR类定义文件，实现车牌字符分割及其识别
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/
#ifndef OCR_H
#define OCR_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

#include "Plate.h"
#include "charsegment.h"

using namespace cv;
using namespace std;

#define CV_HORIZONTAL    1
#define CV_VERTICAL    0

class OCR
{
public:
    OCR();
    OCR(string trainFile, int charS = 20, int shinkS = 3);

    ~OCR();

// Get和Set函数，读取和设置私有成员变量
public:
    bool getDEBUG();
    void setDEBUG(bool Debug);
    bool getSaveSegments();
    void setSaveSegments(bool isSavedSeg);
    string getStrFilename();
    void setStrFilename(string filename);
    int getCharSize();
    void setCharSize(int charSizeVal);
    int getShinkSize();
    void setShinkSize(int shinkSizeVal);
    bool getIsTrained();
    void setIsTrained(bool trained);
    CvANN_MLP getAnnClassifier();
    void setAnnClassifier(CvANN_MLP annVal);
    CvKNearest getKnnClassifier();
    void setKnnClassifier(CvKNearest knnVal);
    int getknnKVal();
    void setknnKVal(int kval);

public:
    bool run(Plate *input);

private:
    Mat features(Mat input, int size, int num);
    Mat preprocessChar(Mat in);
    int classifyAnn(Mat f);
    void trainAnn(Mat trainData, Mat trainClasses, int nlayers);
    int classifyKnn(Mat f);
    void trainKnn(Mat trainSamples, Mat trainClasses, int k);
    vector< CharSegment > segment(Plate input);
    Mat Preprocess(Mat in, int newSize);
    Mat getVisualHistogram(Mat *hist, int type);
    void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData, int num);
    Mat ProjectedHistogram(Mat img, int t, bool nomerlized = true);
    Mat delMargin(Mat img);
    void delNoCharArea(Mat &in);
    Rect getCharsROI(Mat &in);
    void createCharSegments(vector<CharSegment> &outputPlate, Mat &in);
    bool verifySizes(Mat r);

public:
    static const int numCharacters;
    static const string strCharacters[];

private:
    bool isDebug;
    bool isSaveSegments;
    string strFilename;
    string trainSamFilename;
    int charSize;
    int shinkSize;
    bool isTrained;
    Size plateSize;
    CvANN_MLP annClassifier;
    CvKNearest knnClassifier;
    int knnK;
};

#endif // OCR_H
