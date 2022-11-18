#include "FASTCorner.h"

int main()
{
    Mat asuka = imread("./Pics/asuka_small.png",IMREAD_GRAYSCALE);

    Mat dst;
    int threshold = 0;
    threshold = myFastCornerDetectionManualThreshold(asuka,dst);


    return 0;
}