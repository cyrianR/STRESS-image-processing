#include "contrast_enhancement.hpp"
#include "stress.hpp"

#include <iostream>

#include <opencv2/core.hpp>

using namespace std;

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
        cout << "RGB images not supported yet." << endl;
        return cv::Mat();
    }

}