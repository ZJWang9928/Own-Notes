#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

static bool readStringList( const string& filename,
                            vector<string>& l );

int main ( int argc, char** argv )
{
    cout << "OpenCV Training SVM Automatic Number Plate Recognition\n";
    cout << "\n";

    Mat classes;//(numPlates+numNoPlates, 1, CV_32FC1);
    Mat trainingData;//(numPlates+numNoPlates, imageWidth*imageHeight, CV_32FC1 );

    Mat trainingImages;
    vector<int> trainingLabels;

    vector<string> imagePlatesList;
    readStringList("imageListPlates.xml", imagePlatesList);

    for ( int i = 0; i < (int)imagePlatesList.size(); i++ )
    {
        Mat img = imread(imagePlatesList.at(i), 0);

        img = img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(1);
    }

    vector<string> imageNoPlatesList;
    readStringList("imageListNoPlates.xml", imageNoPlatesList);

    for ( int i = 0; i < (int)imageNoPlatesList.size(); i++ )
    {
        Mat img = imread(imageNoPlatesList.at(i), 0);

        img = img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(0);
    }

    Mat(trainingImages).copyTo(trainingData);
    trainingData.convertTo(trainingData, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);

    FileStorage fs("SVM.xml", FileStorage::WRITE);
    fs << "TrainingData" << trainingData;
    fs << "classes" << classes;
    fs.release();

    return 0;
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

