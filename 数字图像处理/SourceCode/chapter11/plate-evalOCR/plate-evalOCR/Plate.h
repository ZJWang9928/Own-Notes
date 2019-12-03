#ifndef PLATE_H
#define PLATE_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Plate
{
public:
    Plate();
    Plate(Mat img, Rect pos);
    virtual ~Plate();

public:
    string str();
    Rect getPosition();
    Mat getPlateImg();
//    vector< char > getChars();
//    vector< Rect > getCharsPos();

public:
    vector<char> chars;
    vector<Rect> charsPos;

protected:

private:
    Rect position;
    Mat plateImg;

};

#endif // PLATE_H
