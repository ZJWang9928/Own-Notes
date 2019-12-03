#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

using namespace cv;
using namespace std;

#define HORIZONTAL    1
#define VERTICAL    0
#define ANGLE 1
#define ANGLECOF 5
#define KERNLSIZE 5
#define CHARSIZE 36

Mat classes;
Mat trainingDataf1;
Mat trainingDataf2;
Mat trainingDataf3;
Mat trainingDataf4;
vector<int> trainingLabels;

void addData(Mat &data, int label);

Mat ProjectedHistogram(Mat img, int t);
Mat getVisualHistogram(Mat *hist, int type);
void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData);
Mat features(Mat in, int sizeData);
void rotateImage(Mat *img, Mat *img_rotate, int degree);
Mat preprocessChar(Mat in);

int main ( int argc, char** argv )
{
    stringstream ssFileName(stringstream::in | stringstream::out);

    int charIndex = 0;
    int sampleIndex = 0;
    for (int i = 0; i < 73; i++)
    {
        if (i == 18 || i == 24)
        {
            continue;
        }
        ssFileName.str("");
        ssFileName << "trainImgs/" << i << ".jpg";

        Mat in = imread(ssFileName.str(), CV_LOAD_IMAGE_GRAYSCALE);

        Mat inInv;
        Mat out;
        Mat tmp;

        threshold(in, inInv, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);

        inInv = preprocessChar(inInv);

        for (int angle = ANGLE; angle >= -ANGLE; angle--)
        {
            rotateImage(&inInv, &tmp, angle * ANGLECOF);
            tmp = preprocessChar(tmp);
            addData(tmp, charIndex);
            stringstream s(stringstream::in | stringstream::out);
            s << "samImgs/" << sampleIndex << ".png";
            imwrite(s.str(), tmp);
            sampleIndex++;
        }

        // 生成样本
        for (int j = 1; j <= KERNLSIZE; j++)
        {
            Mat element = getStructuringElement(MORPH_RECT, Size(j, j));
            dilate(inInv, out, element);
            for (int angle = ANGLE; angle >= -ANGLE; angle--)
            {
                rotateImage(&out, &tmp, angle * ANGLECOF);
                tmp = preprocessChar(tmp);
                addData(tmp, charIndex);
                stringstream s(stringstream::in | stringstream::out);
                s << "samImgs/" << sampleIndex << ".png";
                imwrite(s.str(), tmp);
                sampleIndex++;
            }
        }

        for (int j = 1; j <= KERNLSIZE; j++)
        {
            Mat element = getStructuringElement(MORPH_RECT, Size(j, j));
            erode(inInv, out, element);
            for (int angle = ANGLE; angle >= -ANGLE; angle--)
            {
                rotateImage(&out, &tmp, angle * ANGLECOF);
                tmp = preprocessChar(tmp);
                addData(tmp, charIndex);
                stringstream s(stringstream::in | stringstream::out);
                s << "samImgs/" << sampleIndex << ".png";
                imwrite(s.str(), tmp);
                sampleIndex++;
            }
        }

        charIndex++;
    }

    cout << charIndex << endl;

    trainingDataf1.convertTo(trainingDataf1, CV_32FC1);
    trainingDataf2.convertTo(trainingDataf2, CV_32FC1);
    trainingDataf3.convertTo(trainingDataf3, CV_32FC1);
    trainingDataf4.convertTo(trainingDataf4, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);

    stringstream ss(stringstream::in | stringstream::out);
    ss << "../indentifyPlate/fileXML/OCR" << CHARSIZE << ".XML";
    FileStorage fs(ss.str(), FileStorage::WRITE);

    ss.str("");
    ss << "TrainingDataF" << CHARSIZE / 4;
    fs << ss.str() << trainingDataf1;

    ss.str("");
    ss << "TrainingDataF" << CHARSIZE / 2;
    fs << ss.str() << trainingDataf2;

    ss.str("");
    ss << "TrainingDataF" << CHARSIZE * 3 / 4;
    fs << ss.str() << trainingDataf3;

    ss.str("");
    ss << "TrainingDataF" << CHARSIZE;
    fs << ss.str() << trainingDataf4;

    fs << "classes" << classes;
    fs.release();

    return 0;
}

void addData(Mat &data, int label)
{
    Mat f1 = features(data, CHARSIZE / 4);
    Mat f2 = features(data, CHARSIZE / 2);
    Mat f3 = features(data, CHARSIZE * 3 / 4);
    Mat f4 = features(data, CHARSIZE);

    trainingDataf1.push_back(f1);
    trainingDataf2.push_back(f2);
    trainingDataf3.push_back(f3);
    trainingDataf4.push_back(f4);
    trainingLabels.push_back(label);
}


Mat ProjectedHistogram(Mat img, int t)
{
    int sz=(t)?img.rows:img.cols;
    Mat mhist=Mat::zeros(1,sz,CV_32F);

    for(int j=0; j<sz; j++)
    {
        Mat data=(t)?img.row(j):img.col(j);
        mhist.at<float>(j)=countNonZero(data);
    }

    //Normalize histogram
    double min, max;
    minMaxLoc(mhist, &min, &max);

    if(max>0)
        mhist.convertTo(mhist,-1 , 1.0f/max, 0);

    return mhist;
}

Mat getVisualHistogram(Mat *hist, int type)
{

    int size=100;
    Mat imHist;


    if(type == HORIZONTAL)
    {
        imHist.create(Size(size,hist->cols), CV_8UC3);
    }
    else
    {
        imHist.create(Size(hist->cols, size), CV_8UC3);
    }

    imHist=Scalar(55,55,55);

    for(int i=0; i<hist->cols; i++)
    {
        float value=hist->at<float>(i);
        int maxval=(int)(value*size);

        Point pt1;
        Point pt2, pt3, pt4;

        if(type==HORIZONTAL)
        {
            pt1.x=pt3.x=0;
            pt2.x=pt4.x=maxval;
            pt1.y=pt2.y=i;
            pt3.y=pt4.y=i+1;

            line(imHist, pt1, pt2, CV_RGB(220,220,220),1,8,0);
            line(imHist, pt3, pt4, CV_RGB(34,34,34),1,8,0);

            pt3.y=pt4.y=i+2;
            line(imHist, pt3, pt4, CV_RGB(44,44,44),1,8,0);
            pt3.y=pt4.y=i+3;
            line(imHist, pt3, pt4, CV_RGB(50,50,50),1,8,0);
        }
        else
        {

            pt1.x=pt2.x=i;
            pt3.x=pt4.x=i+1;
            pt1.y=pt3.y=100;
            pt2.y=pt4.y=100-maxval;


            line(imHist, pt1, pt2, CV_RGB(220,220,220),1,8,0);
            line(imHist, pt3, pt4, CV_RGB(34,34,34),1,8,0);

            pt3.x=pt4.x=i+2;
            line(imHist, pt3, pt4, CV_RGB(44,44,44),1,8,0);
            pt3.x=pt4.x=i+3;
            line(imHist, pt3, pt4, CV_RGB(50,50,50),1,8,0);

        }
    }

    return imHist ;
}

void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData)
{
    Mat img(121, 121, CV_8UC3, Scalar(0,0,0));
    Mat ch;
    Mat ld;

    cvtColor(character, ch, CV_GRAY2RGB);

    resize(lowData, ld, Size(100, 100), 0, 0, INTER_NEAREST );
    cvtColor(ld,ld,CV_GRAY2RGB);

    Mat hh=getVisualHistogram(&hhist, HORIZONTAL);
    Mat hv=getVisualHistogram(&vhist, VERTICAL);

    Mat subImg=img(Rect(0,101,20,20));
    ch.copyTo(subImg);

    subImg=img(Rect(21,101,100,20));
    hh.copyTo(subImg);

    subImg=img(Rect(0,0,20,100));
    hv.copyTo(subImg);

    subImg=img(Rect(21,0,100,100));
    ld.copyTo(subImg);

    line(img, Point(0,100), Point(121,100), Scalar(0,0,255));
    line(img, Point(20,0), Point(20,121), Scalar(0,0,255));

    imshow("Visual Features", img);

    cvWaitKey(0);
//    Mat img(145, 73, CV_8UC3, Scalar(0, 0, 0));
//    Mat ch;
//    Mat ld;
//
//    cvtColor(character, ch, CV_GRAY2RGB);
//
//    resize(lowData, ld, Size(48, 96), 0, 0, INTER_NEAREST);
//    cvtColor(ld, ld, CV_GRAY2RGB);
//
//    Mat hh = getVisualHistogram(&hhist, HORIZONTAL);
//    Mat hv = getVisualHistogram(&vhist, VERTICAL);
//
//    resize(hh, hh, Size(48, 48), 0, 0, INTER_NEAREST);
//    resize(hv, hv, Size(24, 96), 0, 0, INTER_NEAREST);
//
//    Mat subImg = img(Rect(0, 97, 24, 48));
//    ch.copyTo(subImg);
//
//    subImg = img(Rect(25, 97, 48, 48));
//    hh.copyTo(subImg);
//
//    subImg = img(Rect(0, 0, 24, 96));
//    hv.copyTo(subImg);
//
//    subImg = img(Rect(25, 0, 48, 96));
//    ld.copyTo(subImg);
//
//    line(img, Point(0, 96), Point(73, 96), Scalar(0,0,255));
//    line(img, Point(25, 0), Point(25, 145), Scalar(0,0,255));
//
//    imshow("Visual Features", img);
//
//    cvWaitKey(0);
}

Mat features(Mat in, int sizeData)
{
    //Histogram features
    Mat vhist = ProjectedHistogram(in, VERTICAL);
    Mat hhist = ProjectedHistogram(in, HORIZONTAL);

    //Low data feature
    Mat lowData;
    resize(in, lowData, Size(sizeData, sizeData) );

//    if (DEBUG)
//    {
//        drawVisualFeatures(in, hhist, vhist, lowData);
//    }

    //Last 10 is the number of moments components
    int numCols = vhist.cols + hhist.cols
                  + lowData.rows * lowData.cols;

    Mat out = Mat::zeros(1, numCols, CV_32F);
    //Asign values to feature
    int j = 0;
    for (int i = 0; i < vhist.cols; i++)
    {
        out.at<float>(j) = vhist.at<float>(i);
        j++;
    }
    for (int i = 0; i<hhist.cols; i++)
    {
        out.at<float>(j) = hhist.at<float>(i);
        j++;
    }
    for (int x = 0; x < lowData.cols; x++)
    {
        for (int y = 0; y < lowData.rows; y++)
        {
            out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
            j++;
        }
    }

//    if (DEBUG)
//        cout << out << "\n===========================================\n";

    return out;
}

// 逆时针旋转图像degree角度（原尺寸）
void rotateImage(Mat *img, Mat *img_rotate, int degree)
{
    //旋转中心为图像中心
    Point2f center;
    center.x = float(img->cols / 2.0 + 0.5);
    center.y = float(img->rows / 2.0 + 0.5);

    //计算二维旋转的仿射变换矩阵
    float m[6];
    Mat M( 2, 3, CV_32F, m);
    M = getRotationMatrix2D(center, degree, 1);
    //cv2DRotationMatrix( center, degree,1, &M);
    //变换图像，并用黑色填充其余值
    warpAffine(*img, *img_rotate, M,
               img->size(),
               INTER_LINEAR,
               BORDER_CONSTANT, Scalar(0));
}

Mat preprocessChar(Mat in)
{
    // Remap image
    int h = in.rows;
    int w = in.cols;

    Mat transformMat = Mat::eye(2, 3, CV_32F);
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    Mat warpImage(m, m, in.type());
    warpAffine(in, warpImage, transformMat,
               warpImage.size(),
               INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    Mat out;
    resize(warpImage, out, Size(CHARSIZE, CHARSIZE));

    return out;
}

