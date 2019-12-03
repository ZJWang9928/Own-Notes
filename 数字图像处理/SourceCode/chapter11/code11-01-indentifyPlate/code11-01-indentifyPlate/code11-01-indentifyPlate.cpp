/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code11-01-indentifyPlate.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: main函数文件，实现系统集成调用与测试
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/
#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

#include "DetectRegions.h"
#include "OCR.h"

using namespace cv;
using namespace std;

#define CV_CHARSIZE 36
#define CV_SHINKSIZE 4

string getFilename(string s);
static bool readStringList( const string& filename, vector<string>& l );

int main( int argc, char** argv )
{
    // 训练SVM(支持向量机)
    FileStorage fs;
    fs.open("fileXML\\SVM.xml", FileStorage::READ);
    Mat svmTrainingData;
    Mat svmClasses;
    fs["TrainingData"] >> svmTrainingData;
    fs["classes"] >> svmClasses;

    // 设置SVM(支持向量机)分类器参数
    CvSVMParams svmParams;
    svmParams.svm_type = CvSVM::C_SVC;
    svmParams.kernel_type = CvSVM::LINEAR;
    svmParams.degree = 0;
    svmParams.gamma = 1;
    svmParams.coef0 = 0;
    svmParams.C = 1;
    svmParams.nu = 0;
    svmParams.p = 0;
    svmParams.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
    //Train SVM
    CvSVM svmClassifier(svmTrainingData, svmClasses, Mat(), Mat(), svmParams);

    // 设置车牌字符识别参数
    stringstream ss(stringstream::in | stringstream::out);
    ss << "fileXML\\OCR" << CV_CHARSIZE << ".XML";
    OCR ocr(ss.str(), CV_CHARSIZE, CV_SHINKSIZE);
    ocr.setSaveSegments(true);
    ocr.setDEBUG(true);

    vector<string> imageList;
    readStringList("fileXML\\imageList.xml", imageList);
    imageList.resize(0);
    imageList.push_back("imgs\\41453.png");
    //imageList.push_back("testImgs\\test02.jpg");
    //imageList.push_back("testImgs\\test5.png");

    for ( int i = 0; i < (int)imageList.size(); i++ )
    {
        string filename = imageList.at(i);

        Mat inputImage = imread(filename, 1);
        if (inputImage.empty() || inputImage.data == NULL)
        {
            continue;
        }

        string filenameWhithoutExt = getFilename(filename);
        cout << "working with file: " << filenameWhithoutExt << "\n";

        // 车牌定位及车牌区域截取
        DetectRegions detectRegions;
        detectRegions.setStrFilename(filenameWhithoutExt);
        //detectRegions.setSaveRegions(false);
        detectRegions.setSaveRegions(true);
        detectRegions.setShowSteps(true);
        vector<Plate> posibleRegions = detectRegions.run( inputImage );

        // 车牌区域识别
        // 对于第个可能的区域进行分析识别
        vector<Plate> plates;
        for (int i = 0; i < (int)posibleRegions.size(); i++)
        {
            Mat img = posibleRegions[i].getPlateImg();
            Mat p = img.reshape(1, 1);
            p.convertTo(p, CV_32FC1);

            int response = (int)svmClassifier.predict( p );
            if (response == 1)
            {
                plates.push_back(posibleRegions[i]);
            }
        }
        cout << "Num plates detected: " << plates.size() << "\n";

        // 识别车牌字符
        ocr.setStrFilename(filenameWhithoutExt);

        //if (plates.size() >= 1)
        for (int i=0; i< (int)plates.size(); i++)
        {
            Plate plate = plates[i];

            bool isPlateNumber = ocr.run(&plate);
            if (isPlateNumber)
            {
                string licensePlate = plate.str();
                cout << "================================================\n";
                cout << "License plate number: "<< licensePlate << "\n";
                cout << "================================================\n";
                // 构建图像文件名称
                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmp/" << licensePlate << ".jpg";
                imwrite(ss.str(), inputImage);
            }
            else
            {
                cout << "================================================\n";
                cout << "Can't Recognize License plate number." << "\n";
                cout << "================================================\n";
            }
        }

        //waitKey(0);
    }

    return 0;
}

string getFilename(string s)
{
    char sep = '\\';
    char sepExt='.';

    size_t i = s.rfind(sep, s.length( ));
    if (i != string::npos)
    {
        string fn= (s.substr(i + 1, s.length( ) - i));
        size_t j = fn.rfind(sepExt, fn.length( ));
        if (i != string::npos)
        {
            return fn.substr(0, j);
        }
        else
        {
            return fn;
        }
    }
    else
    {
        size_t j = s.rfind(sepExt, s.length( ));
        if (j != string::npos)
        {
            return s.substr(0,j);
        }
        else
        {
            return s;
        }
    }
}

static bool readStringList( const string& filename, vector<string>& imgList )
{
    imgList.resize(0);

    FileStorage fs(filename, FileStorage::READ);

    if ( !fs.isOpened() )
    {
        return false;
    }

    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
    {
        return false;
    }

    FileNodeIterator it = n.begin(), it_end = n.end();

    for( ; it != it_end; ++it )
    {
        imgList.push_back((string)*it);
    }

    return true;
}
