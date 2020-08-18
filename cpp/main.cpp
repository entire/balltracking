#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

int main() {

    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Error! couldn't open camera" << std::endl;
        return 1;
    }

    // make a window
    cv::namedWindow("ball tracker", CV_WINDOW_AUTOSIZE);

    cv::Mat frame;
    while(1) {
        double fps = camera.get(cv::CAP_PROP_FPS); 
        bool bSuccess = camera.read(frame);
        if (bSuccess == false) {
            std::cout << "this is the end of the video" << std::endl;
            break;
        }

        int low_H = 16, low_S = 0, low_V = 174;
        int high_H = 93, high_S = 249, high_V = 251;

        // add gaussian blur, convert to hsv
        cv::Mat frameBlurred;
        cv::GaussianBlur(frame, frameBlurred, cv::Size(3, 3), 0);
        cv::Mat frameHSV;
        cv::cvtColor(frameBlurred, frameHSV, CV_BGR2HSV);
        cv::Mat frameThreshold;
        cv::inRange(frameHSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), frameThreshold);

        std::cout << "Frames per seconds : " << fps << std::endl;
        cv::imshow("tracker", frameThreshold);

        

        // press 'q' to quit
        if (cv::waitKey(10) >= 0) {
            break;
        }
    }
    return 0;
}