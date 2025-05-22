#include "contrast_enhancement.hpp"
#include "stress.hpp"
#include "utils.hpp"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

cv::Mat contrast_enhancement(const cv::Mat input_im, const int N, const int M, const int R) {

    if (input_im.channels() == 1) {
        cv::Mat Emin, Emax;
        stressGray(input_im, Emin, Emax, N, M, R);
        cv::Mat output_im = input_im.clone();
        #pragma omp parallel
        {
        #pragma omp for collapse(2)
        for (int i = 0; i < input_im.rows; i++) {
            for (int j = 0; j < input_im.cols; j++) {
                double x = input_im.at<uchar>(i, j);
                double e_min = Emin.at<double>(i, j);
                double e_max = Emax.at<double>(i, j);
                output_im.at<uchar>(i, j) = cv::saturate_cast<uchar>((x - e_min) / (e_max - e_min) * 255);
            }
        }
        }
        return output_im;
    } else {
        Mat Emin,Emax;
        stressRGB(input_im,Emin,Emax,N,M,R);
        Mat output_im = input_im.clone();
        #pragma omp parallel
        {
        #pragma omp for collapse(2)
        for (int i = 0; i < input_im.rows; i++) {
            for (int j = 0; j < input_im.cols; j++) {
                Vec3b x = input_im.at<Vec3b>(i, j);
                Vec3d e_min = Emin.at<Vec3d>(i, j);
                Vec3d e_max = Emax.at<Vec3d>(i, j);
                Vec3b result;
                for (int c = 0; c < input_im.channels(); c++){ 
                    result[c] = saturate_cast<uchar>((x[c] - e_min[c])/(e_max[c] - e_min[c])*255);
                } 
                output_im.at<Vec3b>(i, j) = result;
            }
        } 
        }
        return output_im;
    }

}


void contrast_evaluation(const cv::Mat result_im, const cv::Mat ground_truth_im, const int N, const int M, const int R) {

    cout << "---------------------------------------------" << endl;
    cout << "Evaluating contrast enhancement algorithm :" << endl;

    cout << " " << endl ;

    double psnr = PSNR(result_im, ground_truth_im);
    cout << "PSNR: " << psnr << " dB" << endl;

    cv::Scalar ssim = SSIM(result_im, ground_truth_im);
    cout << "SSIM: " << ssim << endl;

}


void applyCLAHE(const cv::Mat& input, cv::Mat& output) {
    cv::Mat labImage;
    cv::cvtColor(input, labImage, cv::COLOR_BGR2Lab);

    cv::Mat labChannels_0;
    cv::extractChannel(labImage, labChannels_0,0);
    //Appliquer CLAHE uniquement sur le canal L
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(4.0);
    clahe->apply(labChannels_0, labChannels_0);
    

    cv::insertChannel(labChannels_0, labImage,0);
    cv::cvtColor(labImage, output, cv::COLOR_Lab2BGR);
    
}
