#include "stress.hpp"

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

int main() {

    const string img_path = "../images/camera.jpg";

    Mat im;
    im = imread(img_path, cv::IMREAD_COLOR);

    Mat im_gray;
    cvtColor(im, im_gray, COLOR_BGR2GRAY);

    im_gray.convertTo(im_gray, CV_8U);
    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original Image", im_gray);

    Mat Emin;
    Mat Emax;
    stressGray(im_gray, Emin, Emax, 50, 8, 20);


    //namedWindow("Local contrast enhancement", WINDOW_AUTOSIZE);
    //imshow("Local contrast enhancement", (im_gray - Emin) / (Emax - Emin));


}