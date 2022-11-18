#include "HarrisCorner.h"


int main()
{
    Mat asuka = imread("./Pics/asuka_small.png",IMREAD_GRAYSCALE);
    GaussianBlur(asuka,asuka,Size(9,9),3.0f,3.0f,BORDER_REFLECT);

    // Mat asuka = Mat::zeros(Size(100,60),CV_8UC1);
    // asuka.at<unsigned char>(50,50)=100;
    // asuka.at<unsigned char>(20,51)=100;
    // imshow("asuka",asuka);

    Mat dstMat;

    float threshold = myHarrisCornerDetectionManualThreshold(asuka,dstMat,11,MY_GAUSSIAN_WINDOW_TYPE);

    // // myHarrisCornerDetection(asuka,dstMat,11,MY_GAUSSIAN_WINDOW_TYPE,280.0f);
    // // imshow("dstMat", dstMat);
    
    // // float thF = 0.0f;
    // myHarrisCornerDetection(asuka,dstMat,9,MY_GAUSSIAN_WINDOW_TYPE,500.0f);

    // vector<pair<int,int>> points;
    // myConvertHarrisMatToVector(dstMat,points);

    
    // vector<pair<int,int>>::iterator pb = points.begin();
    // vector<pair<int,int>>::iterator pe = points.end();

    // while(pb!=pe)
    // {
    //     pair<int,int>temp = *pb;
    //     pb++;
    //     cout<<"------"<<endl;
    //     cout<<temp.first<<"  "<<temp.second<<endl;
    // }
    
    imshow("dstMat",dstMat);
    waitKey(0);
    destroyAllWindows();

    return 0;
}



/*
CV_8UC1     0
CV_8UC3     16
CV_16FC1    7
CV_16FC3    23
CV_32FC1    5
CV_32FC3    21
*/



// vector<pair<pair<float, float>, pair<float, float>>>
// g++ HarrisCorner.cpp -o harris -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
