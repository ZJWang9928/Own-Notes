#ifndef OCR_H
#define OCR_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

#include "Plate.h"

using namespace cv;
using namespace std;

#define HORIZONTAL    1
#define VERTICAL    0

class CharSegment
{
public:
    CharSegment();
    CharSegment(Mat i, Rect p);
    Mat img;
    Rect pos;
};

class OCR
{
public:
    bool DEBUG;
    bool saveSegments;
    string filename;
    static const int numCharacters;
    static const char strCharacters[];
    OCR(string trainFile);
    OCR();
    string run(Plate *input);
    int charSize;
    Mat preprocessChar(Mat in);
    int classify(Mat f);
    void train(Mat trainData, Mat trainClasses, int nlayers);
    int classifyKnn(Mat f);
    void trainKnn(Mat trainSamples, Mat trainClasses, int k);
    Mat features(Mat input, int size);

private:
    bool trained;
    vector<CharSegment> segment(Plate input);
    Mat Preprocess(Mat in, int newSize);
    Mat getVisualHistogram(Mat *hist, int type);
    void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData);
    Mat ProjectedHistogram(Mat img, int t);
    bool verifySizes(Mat r);
    CvANN_MLP  ann;
    CvKNearest knnClassifier;
    int K;
};

#endif // OCR_H
