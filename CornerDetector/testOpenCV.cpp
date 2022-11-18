//C++
// #include <cmath>
// #include <chrono>
#include <vector>
// #include <cstdlib>
#include <iostream>
// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
// Eigen3
// #include <eigen3/Eigen/Core>
// #include <eigen3/Eigen/Dense>

using namespace std;
using namespace cv;

void trackingBarWindow();
void MixChannels();
void InRange();
void minMaxMeanStddev();

// Mat src = imread("./Pics/testImg.png", IMREAD_COLOR);

int main(int argc, char** argv)
{
    // trackingBarWindow();
    minMaxMeanStddev();
    return 0;
}


void minMaxMeanStddev()
{
    Mat src = imread("./Pics/testImg.png");

    vector<Mat>channels;
    split(src,channels);

    // imshow("src",src);
    // waitKey(0);
    // destroyAllWindows();

    double minv =0.0 ,maxv=0.0;
    Point minLoc,maxLoc;
    int minIdx,maxIdx;
    minMaxIdx(channels[0],&minv,&maxv,&minIdx,&maxIdx,Mat());
    cout<<minv<<"  "<<maxv<<endl;
    // cout<<min<<"  "<<max<<"  "<<minLoc<<"  "<<maxLoc<<endl;

    Mat mean,stddev;

    vector<Mat>::iterator vectorptr = channels.begin();
    vector<Mat>::iterator vectorend = channels.end();
    
    while(vectorptr!=vectorend)
    {
        Mat Mean,StdDev;
        Mat tempChannel(*vectorptr);
        meanStdDev(tempChannel,Mean,StdDev);
        cout<<"Mean\n"<<Mean<<endl;
        cout<<"StdDev\n"<<StdDev<<endl;
        vectorptr++;
    }
}


void InRange()
{
    Mat src = imread("./Pics/test.png");
    Mat red(src.size(),CV_8UC3,Scalar(0,0,234));
    Mat dst;
    // Mat hsv;
    // cvtColor(src, hsv, COLOR_BGR2HSV);
    Mat mask = Mat::ones(src.size(),CV_8UC1);

    inRange(src,Scalar(23,23,23),Scalar(155,155,155),mask);
    src.copyTo(red,mask);

    imshow("red",red);
    imshow("src",src);
    imshow("mask",mask);



    waitKey(0);
    destroyAllWindows();
}


void MixChannels()
{
    Mat src(500,500,CV_8UC3,Scalar(200,0,0));
    Mat dst(500, 500, CV_8UC3);
    vector<Mat>channels;
    split(src, channels);
    // imshow("channel0", channels[0]);
    // imshow("channel1", channels[1]);
    // imshow("channel2", channels[2]);
    
    vector<int>fromTo = {0,2,1,1,2,0};
    mixChannels(src,dst,fromTo);
    
    imshow("src", src);
    imshow("dst", dst);
    waitKey(0);
    destroyAllWindows();
}


void onTrack(int value, void* src)
{
    Mat dst;
    add(*(Mat*)src,Scalar(value-100,value-100,value-100),dst);
    imshow("trackingBar", dst);
    // waitKey(0);
    // destroyAllWindows();
}

void trackingBarWindow()
{
    Mat src = imread("./Pics/testImg.png", IMREAD_COLOR);
    if(src.empty())
    {
        cout<<"读取失败"<<endl;
        return;
    }
    else
    {
        cout<<"读取成功"<<endl;
    }

    namedWindow("trackingBar", WINDOW_AUTOSIZE);
    imshow("trackingBar", src);
    int maxValue = 200;
    int initValue = 0;
    createTrackbar("trackBar","trackingBar",&initValue,maxValue,onTrack,static_cast<void*>(&src));
    // onTrack(initValue, (void*)&src);
    waitKey(0);
    destroyAllWindows();
    return;
}

// g++ testOpenCV.cpp -o test -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc