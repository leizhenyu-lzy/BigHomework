//C++
// #include <cmath>
// #include <chrono>
// #include <vector>
// #include <cstdlib>
#include <iostream>
// opencv
#include <opencv4/opencv2/opencv.hpp>
// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
// Eigen3
// #include <eigen3/Eigen/Core>
// #include <eigen3/Eigen/Dense>

using namespace std;
// using namespace cv;

int main(int argc, char** argv)
{
    std::string path="./Pics/testImg.png";
    cv::Mat src = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if(src.empty())
    {
        cout<<"fuck\n"<<endl;
        return -1;
    }
    cv::imshow("testImg", src);
    cv::Mat srcc(src, cv::Rect(10,10,100,100));
    cv::imshow("tt",srcc);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}

// g++ testOpenCV.cpp -o test -lopencv_core -lopencv_imgcodecs -lopencv_highgui