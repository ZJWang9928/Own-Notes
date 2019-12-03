#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main()
{
    stringstream ssFileName(stringstream::in | stringstream::out);

    for (int i = 0; i < 73; i++)
    {
        ssFileName.str("");
        ssFileName << "imgs/" << i << ".jpg";
        Mat in = imread(ssFileName.str(), CV_LOAD_IMAGE_GRAYSCALE);

        ssFileName.str("");

        string strTemp("");
        ssFileName << "imgs/" << i << "/";
        strTemp = ssFileName.str();

        Mat inInv = 255 - in;
        Mat out;


        ssFileName.str("");
        ssFileName << strTemp;
        ssFileName << "0.jpg";
        imwrite(ssFileName.str(), inInv);

        /// ¸¯Ê´ÅòÕÍ±ä»»´¦Àí
        for (int j = 1; j <= 4; j++)
        {
            ssFileName.str("");
            ssFileName << strTemp << j << ".jpg";

            Mat element = getStructuringElement(MORPH_CROSS, Size(j, j));
            dilate(inInv, out, element);
            imwrite(ssFileName.str(), out);
            //imshow("dilate", in);
            //waitKey(0);
        }

        for (int j = 1; j <= 4; j++)
        {
            ssFileName.str("");
            ssFileName << strTemp << j + 4 << ".jpg";

            Mat element = getStructuringElement(MORPH_CROSS, Size(j, j));
            erode(inInv, out, element);
            imwrite(ssFileName.str(), out);
        }

    }

    return 0;
}
