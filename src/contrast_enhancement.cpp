#include "contrast_enhancement.hpp"
#include "stress.hpp"

#include <iostream>

#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

cv::Mat contrast_enhancement(const cv::Mat input_im, const int N, const int M, const int R) {

    if (input_im.channels() == 1) {
        cv::Mat Emin, Emax;
        stressGray(input_im, Emin, Emax, N, M, R);
        cv::Mat output_im = input_im.clone();
        for (int i = 0; i < input_im.rows; i++) {
            for (int j = 0; j < input_im.cols; j++) {
                double x = input_im.at<uchar>(i, j);
                double e_min = Emin.at<double>(i, j);
                double e_max = Emax.at<double>(i, j);
                output_im.at<uchar>(i, j) = cv::saturate_cast<uchar>((x - e_min) / (e_max - e_min) * 255);
            }
        }
        return output_im;
    } else {
        Mat Emin,Emax;
        stressRGB(input_im,Emin,Emax,N,M,R);
        Mat output_im = input_im.clone();
        for (int i = 0; i < input_im.rows; i++) {
            for (int j = 0; j < input_im.cols; j++) {
                Vec3b x = input_im.at<Vec3b>(i, j);
                Vec3d e_min = Emin.at<Vec3d>(i, j);
                Vec3d e_max = Emax.at<Vec3d>(i, j);
                Vec3b result;
                for (int c = 0; c < input_im.channels(); c++){ 
                    if(e_max[c] != e_min[c]){ 
                        result[c] = saturate_cast<uchar>((x[c] - e_min[c])/(e_max[c]  - e_min[c])*255);
                    }else{result[c] = saturate_cast<uchar>(x[c]); }   
                } 
                output_im.at<Vec3b>(i, j)  = result;
            }
        } 
        return output_im;
    }

}