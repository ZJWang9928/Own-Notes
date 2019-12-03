/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: detectregions.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: DetectRegions类实现，用于定位、分割车牌区域
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/

#include <ctime>

#include "detectregions.h"

DetectRegions::DetectRegions() : saveRegions(false),
    showSteps(false)
{
    plateSize = Size(160, 51);
}

DetectRegions::~DetectRegions()
{
    //dtor
}

bool DetectRegions::getSaveRegions()
{
    return saveRegions;
}
void DetectRegions::setSaveRegions(bool isSaveRegions)
{
    saveRegions = isSaveRegions;
}
bool DetectRegions::getShowSteps()
{
    return showSteps;
}
void DetectRegions::setShowSteps(bool isShowSteps)
{
    showSteps = isShowSteps;
}
string DetectRegions::getStrFilename()
{
    return strFilename;
}
void DetectRegions::setStrFilename(string filename)
{
    strFilename = filename;
}

Size DetectRegions::getPlateSize()
{
    return plateSize;
}
void DetectRegions::setPlateSize(Size inSize)
{
    plateSize = inSize;
}

// 用宽高比及面积检测区域是否符合车牌特征
bool DetectRegions::verifySizes(RotatedRect mr)
{
    /// 建议这些参数改为成员变量，以便于设置，提高通用性
    /// 2.0版本后一定要改进
    // 容差系数
    float error = 0.4;
    // 车牌尺寸: 440x140 宽高比：3.4
    float aspect = 440.0 / 140.0;

    // 面积范围最小最大值，不在这一范围的区域都将被丢弃
    int min = 35 * aspect * 35; // 最小面积
    int max = 155 * aspect * 155; // 最大面积

    // 车牌宽高比容差范围的最小最大值
    float rmin = aspect - aspect * error;
    float rmax = aspect + aspect * error;

    // 计算面积
    int area = mr.size.height * mr.size.width;
    // 计算宽高比：宽/高
    float r = (float)mr.size.width / (float)mr.size.height;

    // r < 1重新计算宽高比：高/宽
    if (r < 1)
    {
        r = (float)mr.size.height / (float)mr.size.width;
    }

    // 是车牌区域返回true，否则返回false
    if (( area < min || area > max ) || ( r < rmin || r > rmax ))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// 直方图均衡化
Mat DetectRegions::histeq(Mat in)
{
    Mat out(in.size(), in.type());

    if (in.channels() == 3)
    {
        // 3通道彩色图像，转换为hsv彩色空间
        // 对H分量通道进行均衡化
        // 再合并并转换为RGB图像
        Mat hsv;
        vector<Mat> hsvSplit;

        cvtColor(in, hsv, CV_BGR2HSV);

        split(hsv, hsvSplit);

        // 对H分量通道进行均衡化
        equalizeHist(hsvSplit[2], hsvSplit[2]);

        merge(hsvSplit, hsv);

        cvtColor(hsv, out, CV_HSV2BGR);
    }
    else if (in.channels() == 1)
    {
        equalizeHist(in, out);
    }

    return out;
}

// 启动检测
vector<Plate> DetectRegions::run(Mat input)
{
    // 检测车牌区域，并获得分割后的可能区域图像及其在原位置
    vector<Plate> detectedPlates = segment(input);

    // 返回检测到的可能是车牌的区域
    return detectedPlates;
}

// 分割图像
vector<Plate> DetectRegions::segment(Mat input)
{
    // 输出结果
    vector<Plate> output;

    // 将图像转换为灰度图像
    Mat imgGray;
    cvtColor(input, imgGray, CV_BGR2GRAY);
    // 平滑去噪（5*5高斯模板）
    blur(imgGray, imgGray, Size(5, 5));

    // 利用的是车牌区域存在大量垂直边缘进行车牌分割
    Mat imgSobel;
    // 利用Sobel滤波器求一阶水平方向导数，查找垂直边缘
    Sobel(imgGray, imgSobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    //Canny( imgGray, imgSobel, 50, 180, 3);
    // 显示中间处理结果
    if (showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Canny" << ".jpg";
        imwrite(ss.str(), imgSobel);
        imshow("Canny", imgSobel);
    }

    // 图像二值化
    Mat imgThreshold;
    // Otsu自适应阈值算法获得图像二值化的阈值
    threshold(imgSobel, imgThreshold,
              0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

    // 显示中间处理结果
    if (showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Threshold" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold", imgThreshold);
    }

    // 形态学闭操作
    // 采用闭操作，去除每个垂直边缘线之间的空白，
    // 并连接所有包含大量边缘的区域
    /// 应将形态学操作核尺寸Size(17, 1)设置为类成员变量，
    /// 以便于根据不同情况进行参数设置，提高通用性
    /// 2.0版本后一定要改进
    Mat elementMorph = getStructuringElement(MORPH_RECT, Size(17, 1));
    morphologyEx(imgThreshold, imgThreshold, CV_MOP_CLOSE, elementMorph);

    // 显示中间处理结果
    if(showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Close" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Close", imgThreshold);
    }

    // 检测外部轮廓
    vector< vector< Point > > contours;
    findContours(imgThreshold,
                 contours, // 轮廓存储向量
                 CV_RETR_EXTERNAL, // 仅检索外层轮廓
                 CV_CHAIN_APPROX_NONE); // 记录并存储所有轮廓点

    /// 建议处理之前通过计算轮廓顶点夹角cos值，处理异常点（毛刺）
    /// 2.0版本后一定要改进

    // 利用迭代器处理检测到的每一个轮廓
    vector< vector<Point> >::iterator itc = contours.begin();
    vector< RotatedRect > rects;

    // 利用轮廓外接矩形的宽高比和区域面积，删除多余区域
    while (itc != contours.end())
    {
        // 获得轮廓最小外接矩形
        RotatedRect mr = minAreaRect(Mat(*itc));

        // 符合要求保留，否则剔除该轮廓
        if ( !verifySizes(mr))
        {
            itc = contours.erase(itc);
        }
        else
        {
            ++itc;
            rects.push_back(mr);
        }
    }

    // 绘制轮廓
    Mat result;
    if (showSteps)
    {
        input.copyTo(result);
        drawContours(result, contours,
                     -1,                // 绘制所有轮廓
                     Scalar(255,0,0),   // 用蓝色绘制
                     1);                // 绘制线条宽度为1
    }

    // 可以使用floodfill算法【用指定颜色填充某一密闭区域】
    // 漫水算法来提取车牌区域
    for (int i = 0; i < (int)rects.size(); i++)
    {
        if(showSteps)
        {
            // 以轮廓外接矩形中心为圆心，半径取3，绘制绿色圆
            circle(result, rects[i].center, 3, Scalar(0, 255, 0), -1);
        }

        // 轮廓外接矩形宽高最小值
        float minSize = (rects[i].size.width < rects[i].size.height) ?
                        rects[i].size.width : rects[i].size.height;
        //minSize = minSize - minSize * 0.5;
        minSize *= 0.5;

        // 随机取中心点附近的5个点（floodfill算法需求）
        // 设置随机数种子
        srand ( time(NULL) );

        // 初始化floodfill算法参数和变量
        Mat mask;
        mask.create(input.rows + 2, input.cols + 2, CV_8UC1);
        mask = Scalar::all(0);

        /// 设置为类成员变量，
        /// 以便于根据不同情况进行参数设置，提高通用性
        /// 2.0版本后一定要改进
        int loDiff = 40;
        int upDiff = 40;
        int connectivity = 4;
        int newMaskVal = 255;
        int NumSeeds = 10;

        Rect ccomp;

        int flags = connectivity + (newMaskVal << 8 )
                    + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;

        for (int j = 0; j < NumSeeds; j++)
        {
            // 随机生成种子
            Point seed;

            seed.x = rects[i].center.x
                     + rand() % (int)minSize - (minSize / 2);
            seed.y = rects[i].center.y
                     + rand() % (int)minSize - (minSize / 2);

            // 小于0，赋值为0
            if (seed.x < 0)
            {
                seed.x = 0;
            }
            if (seed.y < 0)
            {
                seed.y = 0;
            }

            if(showSteps)
            {
                circle(result, seed, 1, Scalar(0, 255, 255), -1);
            }

            floodFill(input, mask, seed,
                      Scalar(255, 0, 0),
                      &ccomp,
                      Scalar(loDiff, loDiff, loDiff),
                      Scalar(upDiff, upDiff, upDiff),
                      flags);
        }

        // 显示中间处理结果
        if (showSteps)
        {
            stringstream ss(stringstream::in | stringstream::out);
            ss << "tmp/" << strFilename << "_MASK" << ".jpg";
            imwrite(ss.str(), mask);
            imshow("MASK", mask);
        }

        // 对这些裁剪区域，再次用纵横比和区域面积进行验证，
        // 再去除图像的旋转，并裁剪图像到统一尺寸，均衡化图像的灰度

        // 记录floodfill算法得到的mask图像中的区域坐标
        vector< Point > pointsInterest;
        Mat_< uchar >::iterator itMask = mask.begin<uchar>();
        Mat_< uchar >::iterator end = mask.end<uchar>();

        for ( ; itMask != end; ++itMask)
        {
            if (*itMask == 255)
            {
                pointsInterest.push_back(itMask.pos());
            }
        }

        // 获得轮廓最小外接矩形
        RotatedRect minRect = minAreaRect(pointsInterest);

        // 符合要求(宽高比和面积)
        if (verifySizes(minRect))
        {
            // 绘制结果
            if(showSteps)
            {
                // 取得矩形四个顶点坐标，并绘制矩形
                Point2f rectPoints[4];
                minRect.points(rectPoints);
                // 依次循环，当j=3时，(j + 1) % 4 = 0，
                // 回到起始点，很好玩的方法哟
                for (int j = 0; j < 4; j++)
                {
                    line(result,
                         rectPoints[j],
                         rectPoints[(j + 1) % 4],
                         Scalar(0, 0, 255), 1, 8 );
                }
            }

            // 计算旋转矩阵
            float r = (float)minRect.size.width
                      / (float)minRect.size.height;
            float angle = minRect.angle;

            if (r < 1)
            {
                angle = 90 + angle;
            }
            Mat rotmat = getRotationMatrix2D(minRect.center, angle, 1);

            // 校正图像
            Mat imgRotated;
            warpAffine(input, imgRotated,
                       rotmat, input.size(),
                       CV_INTER_CUBIC);

            // 裁减图像
            Size rectSize = minRect.size;
            if (r < 1)
            {
                swap(rectSize.width, rectSize.height);
            }
            Mat imgCrop;
            getRectSubPix(imgRotated, rectSize,
                          minRect.center, imgCrop);

            Mat resultResized;
            // 注意调用产对plateSize进行设置，默认为Size(160, 51)
            resultResized.create(plateSize, CV_8UC3);
            resize(imgCrop, resultResized,
                   resultResized.size(), 0, 0, INTER_CUBIC);

            // 直方图均衡化
            Mat grayResult;
            cvtColor(resultResized, grayResult, CV_BGR2GRAY);
            blur(grayResult, grayResult, Size(3, 3));
            grayResult = histeq(grayResult);

            // 将提取的区域存储为图像，作为SVM训练样本
            if (saveRegions)
            {
                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmp/" << strFilename << "_" << i << ".jpg";
                imwrite(ss.str(), grayResult);
            }
            // 输出结果
            output.push_back(Plate(grayResult, minRect.boundingRect()));
        }
    }

    // 显示中间处理结果
    if(showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Contours" << ".jpg";
        imwrite(ss.str(), result);
        imshow("Contours", result);
    }

    return output;
}

