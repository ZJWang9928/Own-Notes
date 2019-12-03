#include "Plate.h"

Plate::Plate()
{
    //ctor
}

Plate::Plate(Mat img, Rect pos)
{
    plateImg = img;
    position = pos;
}

Plate::~Plate()
{
    //dtor
}

string Plate::str()
{
    string result = "";

    //Order numbers
    vector<int> orderIndex;
    vector<int> xpositions;

    for (int i = 0; i < (int)charsPos.size(); i++)
    {
        orderIndex.push_back(i);
        xpositions.push_back(charsPos[i].x);
    }

    float min=xpositions[0];
    int minIdx=0;

    for (int i = 0; i < (int)xpositions.size(); i++)
    {
        min = xpositions[i];
        minIdx = i;
        for (int j = i; j < (int)xpositions.size(); j++)
        {
            if (xpositions[j] < min)
            {
                min = xpositions[j];
                minIdx = j;
            }
        }

        int aux_i = orderIndex[i];
        int aux_min = orderIndex[minIdx];
        orderIndex[i] = aux_min;
        orderIndex[minIdx] = aux_i;

        float aux_xi = xpositions[i];
        float aux_xmin = xpositions[minIdx];
        xpositions[i] = aux_xmin;
        xpositions[minIdx] = aux_xi;
    }

    for (int i = 0; i < (int)orderIndex.size(); i++)
    {
        result = result + chars[orderIndex[i]];
    }

    return result;
}

Rect Plate::getPosition()
{
    return position;
}

Mat Plate::getPlateImg()
{
    return plateImg;
}

//vector< char > Plate::getChars()
//{
//    return chars;
//}
//
//vector< Rect > Plate::getCharsPos()
//{
//    return charsPos;
//}

