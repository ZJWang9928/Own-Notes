/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: ocr.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014-03-11
  描  述: OCR类实现文件，实现车牌字符分割及其识别
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
       耿楠   2014-03-11     1.0     建立该文件
***********************************************************/

#include "ocr.h"

// 识别结果查找表
const int OCR::numCharacters = 71;
const string OCR::strCharacters[] = {"0","1","2","3","4","5","6","7","8","9",
                                     "A","B","C","D","E","F","G","H","J","K",
                                     "L","M","N","P","Q","R","S","T","U","V",
                                     "W","X","Y","Z",
                                     "京","津","冀","晋","蒙","辽","吉","黑",
                                     "沪","苏","浙","皖","闽","赣","鲁","豫",
                                     "鄂","湘","粤","桂","琼","渝","川","贵",
                                     "云","藏","陕","甘","青","宁","新","港",
                                     "澳","使","领","学","警"
                                    };
OCR::OCR()
{
    isDebug = false;
    isTrained = false;
    isSaveSegments = false;
    charSize = 20;
    shinkSize = 3;
}

OCR::OCR(string trainFile, int charS, int shinkS)
{
    isDebug = false;
    isTrained = false;
    isSaveSegments = false;
    charSize = charS;
    shinkSize = shinkS;

    trainSamFilename = trainFile;

    // 构建样本XML文件节点名称
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // 读入训练样本XML文件
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // 训练人工神经网络分类器
    //trainAnn(TrainingData, Classes, 10);
    // 训练k-最近邻分类器
    trainKnn(TrainingData, Classes, 10);
}
OCR::~OCR()
{
    //dtor
}

bool OCR::getDEBUG()
{
    return isDebug;
}
void OCR::setDEBUG(bool Debug)
{
    isDebug = Debug;
}
bool OCR::getSaveSegments()
{
    return isSaveSegments;
}
void OCR::setSaveSegments(bool isSavedSeg)
{
    isSaveSegments = isSavedSeg;
}
string OCR::getStrFilename()
{
    return strFilename;
}
void OCR::setStrFilename(string filename)
{
    strFilename = filename;
}
int OCR::getCharSize()
{
    return charSize;
}
void OCR::setCharSize(int charSizeVal)
{
    charSize = charSizeVal;

    // 构建样本XML文件节点名称
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // 读入训练样本XML文件
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // 训练人工神经网络分类器
    //trainAnn(TrainingData, Classes, 10);
    // 训练k-最近邻分类器
    trainKnn(TrainingData, Classes, 10);
}
int OCR::getShinkSize()
{
    return shinkSize;
}
void OCR::setShinkSize(int shinkSizeVal)
{
    shinkSize = shinkSize;

    // 构建样本XML文件节点名称
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // 读入训练样本XML文件
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // 训练人工神经网络分类器
    //trainAnn(TrainingData, Classes, 10);
    // 训练k-最近邻分类器
    trainKnn(TrainingData, Classes, 10);
}
bool OCR::getIsTrained()
{
    return isTrained;
}
void OCR::setIsTrained(bool isTrained)
{
    isTrained = isTrained;
}
CvANN_MLP OCR::getAnnClassifier()
{
    return annClassifier;
}
void OCR::setAnnClassifier(CvANN_MLP annVal)
{
    annClassifier = annVal;
}
CvKNearest OCR::getKnnClassifier()
{
    return knnClassifier;
}
void OCR::setKnnClassifier(CvKNearest knnVal)
{
    knnClassifier = knnVal;
}

int OCR::getknnKVal()
{
    return knnK;
}
void OCR::setknnKVal(int kval)
{
    knnK = kval;
}

// 几何变换，统一字符图像大小 便于后续识别
Mat OCR::preprocessChar(Mat in)
{
    int h = in.rows;
    int w = in.cols;

    // 构建变换矩阵
    Mat transformMat = Mat::eye(2, 3, CV_32F);
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    Mat warpImage(m, m, in.type());

    // 仿射变换
    warpAffine(in, warpImage, transformMat,
               warpImage.size(),
               INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    Mat out;
    // 缩放大小
    resize(warpImage, out, Size(charSize, charSize));

    return out;
}

// 字符确认，是字符，返回真，否则返回假
bool OCR::verifySizes(Mat r)
{
    // 字符宽高比（1/2）
    float aspect = 45.0f / 90.0f;
    float charAspect = (float)r.cols / (float)r.rows;
    float error = 0.35;
    float minHeight = 15;
    float maxHeight = 46;
    // 字符1的宽高比与其它不一致，此处设为0.2
    float minAspect = 0.2;
    // 最大宽高比
    float maxAspect = aspect + aspect * error;

    // 区域像素面积（含有非零像素的个数）
    float area = countNonZero(r);
    // 区域面积（宽高相乘）
    float bbArea = r.cols * r.rows;
    // 像素面积占的比例
    float percPixels = area / bbArea;

    if (isDebug)
    {
        cout << "Aspect: "<< aspect << " ["<< minAspect << "," << maxAspect << "] "  << "Area "<< percPixels <<" Char aspect " << charAspect  << " Height char "<< r.rows << "\n";
    }

    // 判断是否满足条件
    if (percPixels < 0.8
            && charAspect > minAspect && charAspect < maxAspect
            && r.rows >= minHeight && r.rows < maxHeight)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 清除上、下边界
// 根据一行中像素值黑白变化的次数判断这一行是不是噪声
// 阈值设为10次
/**
其具体的算法为： 从上至下逐行扫描各个水平线上的像素断点数，
如果该行像素跳变数目小于某个先验值阈，则将该行全部清零,否则保留。
如果连续三行的像素跳变次数大于某一固定阈值，说明扫描到车牌的字符位置，
那么就将之前的小于某一固定阀值行以上区域裁切掉。 同理，可以将下边框裁掉。
同理， 利用类似的方法，可以将存在左右边框也去除掉。
"基于OPENCV的车辆牌照识别系统研究_西安电子科技大学_杨思源"硕士论文第5章P33
2.0版后实现
**/
Mat OCR::delMargin(Mat binPlate)
{
    Mat cropMarg;
    binPlate.copyTo(cropMarg);

    for (int i = 0; i < binPlate.rows; i++)
    {
        uchar pixelTmp = binPlate.at<uchar>(i, 0);
        int flipCounter = 0;
        for (int j = 1; j < binPlate.cols; j++)
        {
            if (binPlate.at<uchar>(i, j) != pixelTmp)
            {
                flipCounter++;
                pixelTmp = binPlate.at<uchar>(i, j);
            }
        }
        // 1行内像素值变化少于10次，应删除
        /// 可检测少于10次变化的连续3行，再删除
        if (flipCounter < 10)
        {
            for (int k = 0; k < binPlate.cols; k++)
            {
                cropMarg.at<uchar>(i, k) = 0;
            }
        }
    }
    return cropMarg;
}

// 删除面积和比例不满足要求的区域
void OCR::delNoCharArea(Mat &in)
{
    // 删除不满足要求的区域
    Mat imgContours;
    in.copyTo(imgContours);

    // 查找可能的区域
    vector< vector< Point > > contours;
    findContours(imgContours,
                 contours, // 轮廓向量
                 CV_RETR_EXTERNAL, // 外部轮廓
                 CV_CHAIN_APPROX_NONE); // 记录所有点

    /// 建议处理之前通过计算轮廓顶点夹角cos值，处理异常点（毛刺）
    /// 2.0版本后一定要改进

    // 处理每个轮廓
    vector<vector<Point> >::iterator itc = contours.begin();

    // 删除面积和比例不满足要求的区域
    while (itc != contours.end())
    {
        // 创建轮廓的外接矩形
        Rect mr = boundingRect(Mat(*itc));
        Mat auxRoi(in, mr);

        // 计算宽高比（长边 / 窄边）
        float r = 0.0;
        if( mr.width > mr.height)
        {
            r = (float)mr.width / (float)mr.height;
        }
        else
        {
            r = (float)mr.height / (float)mr.width;
        }

        if (r > 10.0)
        {
            // 该区域赋0
            auxRoi = Scalar(0);
            ++itc;
            continue;
        }

        // 轮廓中面积
        float area = contourArea(*itc);
        // 外接矩形面积
        float bbArea = mr.height * mr.width;
        // 所点百分比
        float percPixels = area / bbArea;
        if (percPixels >= 0.8)
        {
            auxRoi = Scalar(0);
            ++itc;
            continue;
        }
        ++itc;
    }
}

// 计算字符提取区域
Rect OCR::getCharsROI(Mat &in)
{
    // 利用水平积分投影计算图像上下边界
    Mat ProjectedHistH = ProjectedHistogram(in, CV_HORIZONTAL, false);

    // 取中间值的1/3作为阈值
    int midPos = ProjectedHistH.cols / 2;
    int midHistH = ProjectedHistH.at<int>(midPos) / 3;

    int minH = 0;
    int maxH = ProjectedHistH.cols;

    // 自顶向下处理
    for (int i = 0; i < ProjectedHistH.cols; i++)
    {
        if (ProjectedHistH.at<int>(i) > midHistH)
        {
            minH = i;
            break;
        }
    }
    // 自下向上处理
    for (int i = ProjectedHistH.cols - 1; i > minH; i--)
    {
        if (ProjectedHistH.at<int>(i) > midHistH)
        {
            maxH = i;
            break;
        }
    }
    // 设置上下边界处理后的感兴趣区域
    Rect rectH(0, minH, in.cols, maxH - minH);
    Mat cropedH(in, rectH);

    // 利用垂直积分投影计算取得水平边界
    Mat ProjectedHistV = ProjectedHistogram(cropedH, CV_VERTICAL, false);

    int minV = 0;
    int maxV = ProjectedHistV.cols;
    vector< int > xPos;
    int flipFlag = 1;

    // 统计各区域投影边界水平坐标，压入xPos向量
    for (int i = 0; i < ProjectedHistV.cols; i++)
    {
        if(flipFlag)
        {
            // 上跳变（子区域左边界）
            if (ProjectedHistV.at<int>(i) > 5)
            {
                xPos.push_back(i);
                flipFlag = 0;
                continue;
            }
        }
        else
        {
            // 下跳变（子区域右边界）
            if (ProjectedHistV.at<int>(i) < 5)
            {
                xPos.push_back(i);
                flipFlag = 1;
                continue;
            }
        }
    }

    // 左右边界初值
    minV = xPos.empty() ? 0 : xPos.at(0);
    maxV = ProjectedHistV.cols;
    int xEnd = xPos.size();

    // 重直积分投影跳变次数少于7个字符，按未能检测出字符处理
    if (xEnd >= 14)
    {
        // 从左边开始处理，查找左边界
        for (int i = 1; i < xEnd; i += 2)
        {
            if ((xPos.at(i) - xPos.at(i - 1)) > 3)
            {
                minV = xPos.at(i - 1);
                break;
            }
        }
        // 从右边开始处理，查找右边界
        for (int i = xEnd - 2 ; i > minV; i -= 2)
        {
            if ((xPos.at(i + 1) - xPos.at(i)) > 3)
            {
                maxV = xPos.at(i + 1);
                break;
            }
        }

        // 条件成立，则右边可能是字符区域，调用右边界
        // 主要因为最右边无下跳变时，其位置无法压入xPos
        if ((ProjectedHistV.cols - maxV) > 5)
        {
            maxV = in.cols;
        }
    }

    // 判断裁剪区域合法否
    if (minV >= maxV || minH >= maxH)
    {
        // 构建裁剪区域
        Rect rectROI(0, 0, 0, 0);
        return rectROI;
    }
    else
    {
        // 构建裁剪区域
        Rect rectROI(minV, minH, maxV - minV, maxH - minH);
        return rectROI;
    }
}

// 利用垂直积分投影分割字符
void OCR::createCharSegments(vector<CharSegment> &outputPlate, Mat &in)
{
    // 生成垂直积分投影
    Mat projV = ProjectedHistogram(in, CV_VERTICAL, false);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_CropedPlate" << ".jpg";
        imwrite(ss.str(), in);
        imshow("Croped Plate", in);
        //waitKey(0);
    }

    // 生成字符水平分割位置向量
    vector< int > charSegPosX;
    int flipFlag = 1;
    for (int i = 0; i < projV.cols; i++)
    {
        if(flipFlag)
        {
            if (projV.at<int>(i) > 3)
            {
                charSegPosX.push_back(i);
                flipFlag = 0;
                continue;
            }
        }
        else
        {
            if (projV.at<int>(i) < 3)
            {
                charSegPosX.push_back(i);
                flipFlag = 1;
                continue;
            }
        }
    }

    // 分割字符区域
    Rect charRect(0, 0, in.cols, in.rows);

    // 显示结果
    Mat result;
    if (isDebug)
    {
        in.copyTo(result);
        cvtColor(result, result, CV_GRAY2RGB);
    }

    // 计算每个字符占有的像素数
    float scalePerPixe = 409 / in.cols;
    int pixelsPerChar = 45 / scalePerPixe;
    int minPPC = pixelsPerChar - 6;
    int maxPPC = pixelsPerChar + 6;

    if (charSegPosX.size() >= 14)
    {
        for (int i = 1; i < (int)charSegPosX.size(); i += 2)
        {
            int xPixels = charSegPosX.at(i) - charSegPosX.at(i - 1);
            if (xPixels > minPPC && xPixels < maxPPC)
            {
                charRect.x = charSegPosX.at(i - 1);
                charRect.width = xPixels;
                Mat auxRoi(in, charRect);
                //auxRoi = 255 - auxRoi;
                auxRoi = preprocessChar(auxRoi);
                outputPlate.push_back(CharSegment(auxRoi, charRect));
                if (isDebug)
                {
                    rectangle(result, charRect, Scalar(0, 255, 0));
                }
            }
        }
        // 计数为奇数，则右边界可能存在字符区域，特殊处理
        if (charSegPosX.size() % 2 == 1)
        {
            int xPixels = in.cols - charSegPosX.at(charSegPosX.size() - 1);
            if (xPixels > minPPC && xPixels < maxPPC)
            {
                charRect.x = charSegPosX.at(charSegPosX.size() - 1);
                charRect.width = xPixels;
                Mat auxRoi(in, charRect);
                //auxRoi = 255 - auxRoi;
                auxRoi = preprocessChar(auxRoi);
                outputPlate.push_back(CharSegment(auxRoi, charRect));
                if (isDebug)
                {
                    rectangle(result, charRect, Scalar(0, 255, 0));
                }
            }
        }
    }

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_SegmentedChars" << ".jpg";
        imwrite(ss.str(), result);
        imshow("Segmented Chars", result);
    }
}

// 分割字符
vector<CharSegment> OCR::segment(Plate plate)
{
    Mat input = plate.getPlateImg();

    vector<CharSegment> output;

    // 二值化
    Mat imgThreshold;
    threshold(input, imgThreshold, 161, 255, CV_THRESH_BINARY);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_BinaryPalate" << ".jpg";
        imwrite(ss.str(), imgThreshold);
    }

    // 删除面积和比例不满足要求的区域
    delNoCharArea(imgThreshold);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_BinaryPalate_delNoChar" << ".jpg";
        imwrite(ss.str(), imgThreshold);
    }

    // 去除上下边缘（用每行黑白变化数去除，<10则去除这一行）
    imgThreshold = delMargin(imgThreshold);

    if (isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Thresholdplate_delMargin" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold plate", imgThreshold);
    }

    // 利用垂直和水平积分投影查找字符区域
    Rect ROI = getCharsROI(imgThreshold);
    // 未能找到字符区域，退出分割
    if (ROI.size().width == 0 || ROI.size().height == 0)
    {
        return output;
    }

    /// 2.0版拟加入“感叹号”去粘连
    /// （可参考厦门大学硕士论文“基于OpenCV2.x的车牌识别系统设计与实现”第6章 P47）

    imgThreshold = imgThreshold(ROI);

    if (isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Thresholdplate_ROI" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold plate", imgThreshold);
    }

    // 字符分割
    createCharSegments(output, imgThreshold);

    if(isDebug)
    {
        cout << "Num chars: " << output.size() << "\n";
    }

    return output;
}

// 计算积分投影
Mat OCR::ProjectedHistogram(Mat img, int t, bool nomerlized)
{
    // 水平垂直方向判断
    int sz=(t) ? img.rows : img.cols;
    Mat mhist = Mat::zeros(1, sz, CV_32F);

    // 计算积分投影
    for (int j = 0; j < sz; j++)
    {
        Mat data = (t) ? img.row(j) : img.col(j);
        mhist.at<float>(j) = countNonZero(data);
    }

    if (nomerlized)
    {
        // 归一化
        double min, max;
        minMaxLoc(mhist, &min, &max);

        if (max > 0)
        {
            mhist.convertTo(mhist, -1 , 1.0f / max, 0);
        }
    }
    else
    {
        // 不进行归一化，但转换为int类型数据
        mhist.convertTo(mhist, CV_32SC1);
    }

    return mhist;
}

// 显示积分投影
// 根据积分投影值，用线条绘制
Mat OCR::getVisualHistogram(Mat *hist, int type)
{
    /// 建议改为成员变量，并提供设置函数，以提高通用性
    /// 2.0版后改
    int size = 100;
    Mat imHist;

    if (type == CV_HORIZONTAL)
    {
        imHist.create(Size(size, hist->cols), CV_8UC3);
    }
    else
    {
        imHist.create(Size(hist->cols, size), CV_8UC3);
    }

    imHist = Scalar(55, 55, 55);

    for (int i = 0; i < hist->cols; i++)
    {
        float value = hist->at<float>(i);
        int maxval = (int)(value * size);

        Point pt1;
        Point pt2, pt3, pt4;

        if(type == CV_HORIZONTAL)
        {
            pt1.x = pt3.x = 0;
            pt2.x = pt4.x = maxval;
            pt1.y = pt2.y = i;
            pt3.y = pt4.y = i + 1;

            line(imHist, pt1, pt2, CV_RGB(220, 220, 220), 1, 8, 0);
            line(imHist, pt3, pt4, CV_RGB(34, 34, 34), 1, 8, 0);

            pt3.y = pt4.y = i + 2;
            line(imHist, pt3, pt4, CV_RGB(44, 44, 44), 1, 8, 0);
            pt3.y = pt4.y = i + 3;
            line(imHist, pt3, pt4, CV_RGB(50, 50, 50), 1, 8, 0);
        }
        else
        {
            pt1.x = pt2.x = i;
            pt3.x = pt4.x = i + 1;
            pt1.y = pt3.y = size;
            pt2.y = pt4.y = size - maxval;

            line(imHist, pt1, pt2, CV_RGB(220, 220, 220), 1, 8, 0);
            line(imHist, pt3, pt4, CV_RGB(34, 34, 34), 1, 8, 0);

            pt3.x = pt4.x = i + 2;
            line(imHist, pt3, pt4, CV_RGB(44, 44, 44), 1, 8, 0);
            pt3.x = pt4.x = i + 3;
            line(imHist, pt3, pt4, CV_RGB(50, 50, 50), 1, 8, 0);
        }
    }

    return imHist ;
}

// 显示原图、水平和垂直积分投影及放大图像
void OCR::drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData, int num)
{
    // 6倍于原图像大小，中间画分割线，多1个像素
    int width = charSize * 6 + 1;
    int height = charSize * 6 + 1;
    Mat img(width, height, CV_8UC3, Scalar(0,0,0));
    Mat ch;
    Mat ld;

    cvtColor(character, ch, CV_GRAY2RGB);

    // 放大原图5倍
    resize(lowData, ld,
           Size(width - charSize - 1, height - charSize - 1),
           0, 0, INTER_NEAREST );

    cvtColor(ld, ld, CV_GRAY2RGB);

    Mat hh = getVisualHistogram(&hhist, CV_HORIZONTAL);
    Mat hv = getVisualHistogram(&vhist, CV_VERTICAL);

    Mat subImg = img(Rect(0, height - charSize, charSize, charSize));
    ch.copyTo(subImg);

    subImg = img(Rect(charSize + 1,
                      height - charSize,
                      width - charSize - 1,
                      charSize));
    resize(hh, hh, Size(width - charSize - 1, charSize));
    hh.copyTo(subImg);

    subImg = img(Rect(0, 0, charSize, height - charSize -1));
    resize(hv, hv, Size(charSize,  height - charSize -1));
    hv.copyTo(subImg);

    subImg = img(Rect(charSize + 1, 0,
                      width - charSize -1,
                      height - charSize -1));
    ld.copyTo(subImg);

    line(img,
         Point(0, height - charSize -1),
         Point(width, height - charSize -1),
         Scalar(0,0,255));
    line(img,
         Point(charSize, 0),
         Point(charSize, height),
         Scalar(0,0,255));

    stringstream ss(stringstream::in | stringstream::out);
    ss << "tmp/" << strFilename << "_VisualFeatures" << num << ".jpg";
    imwrite(ss.str(), img);
    imshow("Visual Features", img);

    //waitKey(0);
}

// 提取特征
Mat OCR::features(Mat in, int sizeData, int num)
{
    // 水平和垂直积分投影特征
    Mat vhist = ProjectedHistogram(in, CV_VERTICAL);
    Mat hhist = ProjectedHistogram(in, CV_HORIZONTAL);

    // 重采样
    Mat lowData;
    resize(in, lowData, Size(sizeData, sizeData) );

    if (isDebug)
    {
        drawVisualFeatures(in, hhist, vhist, lowData, num);
    }

    // 构建特征向量
    int numCols = vhist.cols + hhist.cols
                  + lowData.rows * lowData.cols;

    Mat out = Mat::zeros(1, numCols, CV_32F);

    // 为特殊向量赋值
    int j = 0;
    // 垂直积分投影值
    for (int i = 0; i < vhist.cols; i++)
    {
        out.at<float>(j)=vhist.at<float>(i);
        j++;
    }
    // 水平积分投影值
    for (int i=0; i<hhist.cols; i++)
    {
        out.at<float>(j) = hhist.at<float>(i);
        j++;
    }
    // 原图像像素值
    for (int x = 0; x < lowData.cols; x++)
    {
        for (int y = 0; y < lowData.rows; y++)
        {
            out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
            j++;
        }
    }

    /// 建议增加正负45度方向积分投影，以提高准确率
    /// 2.0版后改进

    if(isDebug)
    {
        //cout << out << "\n===========================================\n";
    }
    return out;
}

// 训练人工神经网络
void OCR::trainAnn(Mat TrainData, Mat classes, int nlayers)
{
    /// 一直未能调试通过，初步分析是numCharacters设置的原因
    /// 2.0版待查，以期获得正确结果

    Mat layers(1, 3, CV_32SC1);
    layers.at<int>(0) = TrainData.cols;
    layers.at<int>(1) = nlayers;
    layers.at<int>(2) = numCharacters;
    annClassifier.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);

    // 准备数据
    Mat trainClasses;
    trainClasses.create( TrainData.rows, numCharacters, CV_32FC1 );
    for ( int i = 0; i <  trainClasses.rows; i++ )
    {
        for ( int k = 0; k < trainClasses.cols; k++ )
        {
            if ( k == classes.at<int>(i) )
            {
                trainClasses.at<float>(i,k) = 1;
            }
            else
            {
                trainClasses.at<float>(i,k) = 0;
            }
        }
    }
    Mat weights( 1, TrainData.rows, CV_32FC1, Scalar::all(1) );

    // 训练学习
    annClassifier.train( TrainData, trainClasses, weights );

    isTrained = true;
}

// 人工神经网络分类
int OCR::classifyAnn(Mat f)
{
    Mat output(1, numCharacters, CV_32FC1);
    annClassifier.predict(f, output);
    Point maxLoc;
    double maxVal;
    minMaxLoc(output, 0, &maxVal, 0, &maxLoc);

    return maxLoc.x;
}

// k-最近邻分类器分类
int OCR::classifyKnn(Mat f)
{
    int response = (int)knnClassifier.find_nearest( f, knnK );
    return response;
}

// 训练k-最近邻分类器
void OCR::trainKnn(Mat trainSamples, Mat trainClasses, int k)
{
    knnK = k;

    knnClassifier.train( trainSamples, trainClasses, Mat(), false, knnK );
}

// 启动检测与识别
bool OCR::run(Plate *input)
{
    // 分割车牌中的字符
    vector<CharSegment> segments = segment(*input);

    if (segments.size() == 7)
    {
        for (int i = 0; i < (int)segments.size(); i++)
        {
            // 将每个字符大小变换为统一尺寸大小
            Mat ch = segments[i].getCharImg();
            if (isSaveSegments)
            {
                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmpChars/" << strFilename << "_" << i << ".jpg";
                imwrite(ss.str(), ch);
            }
            // 提取特征向量
            Mat f = features(ch, charSize * shinkSize / 4, i);
            // 用人工神经网络分类器分类识别得到结果数组索引值
            //int character = classifyAnn(f);
            // 用k-最近邻分类器分类识别得到结果数组索引值
            int character = classifyKnn(f);
            // 在结果数组取得字符串，并将该字符串及其位置区域写入识别结果
            input->pushStrChars(strCharacters[character]);
            input->pushCharsPos(segments[i].getCharPos());
        }
        return true;
    }
    return false;
}


