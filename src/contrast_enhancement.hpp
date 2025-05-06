#include <opencv2/core.hpp>

cv::Mat contrast_enhancement(const cv::Mat input_im, const int N, const int M, const int R);

void contrast_evaluation(const cv::Mat result_im, const cv::Mat ground_truth_im, const int N, const int M, const int R);