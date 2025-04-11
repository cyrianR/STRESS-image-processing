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

    imshow("Original Image", im_gray);

    Mat Emin;
    Mat Emax;
    stressGray(im_gray, Emin, Emax, 50, 8, 20);

    Mat im_gray_contrast_enhanced;
    im_gray_contrast_enhanced.create(im_gray.size(), im_gray.type());
    for (int i = 0; i < im_gray.rows; i++) {
        for (int j = 0; j < im_gray.cols; j++) {
            double normalized_value = (im_gray.at<uchar>(i, j) - Emin.at<double>(i, j)) / 
                                       (Emax.at<double>(i, j) - Emin.at<double>(i, j));
            im_gray_contrast_enhanced.at<uchar>(i, j) = saturate_cast<uchar>(normalized_value * 255.0);
        }
    }

    imshow("Local contrast enhancement", im_gray_contrast_enhanced);

    while(cv::waitKey(1) != 27);
}