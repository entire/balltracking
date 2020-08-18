#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

cv::RNG rng(12345);

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

        // set low, high for hsv threshold
        int low_H = 16, low_S = 0, low_V = 174;
        int high_H = 93, high_S = 249, high_V = 251;

        // add gaussian blur
        cv::Mat frameBlurred;
        cv::GaussianBlur(frame, frameBlurred, cv::Size(3, 3), 0);
        
        // convert to hsv
        cv::Mat frameHSV;
        cv::cvtColor(frameBlurred, frameHSV, CV_BGR2HSV);
        
        // hsv threshold
        cv::Mat thresMask;
        cv::inRange(frameHSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), thresMask);
        cv::Mat erodeMask;
        cv::erode(thresMask, erodeMask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::Mat dilateMask;
        cv::dilate(erodeMask, dilateMask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(dilateMask.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

        cv::Point2f center;
        float radius;
        std::vector<cv::Point> contours_poly;
        int contourSize = contours[0].size();
        cv::Mat drawing = cv::Mat::zeros(dilateMask.size(), CV_8UC3 );
        
        std::cout << contours[0] << std::endl;
        if (contourSize > 0) {
            // TODO: 
        }

        std::cout << "Frames per seconds : " << fps << std::endl;
        cv::imshow("tracker", frame);

        // press 'q' to quit
        if (cv::waitKey(10) >= 0) {
            break;
        }
    }
    return 0;
}