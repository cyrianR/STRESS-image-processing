#include <opencv2/core.hpp>

// Function to perform local contrast enhancement using STRESS algorithm
cv::Mat contrast_enhancement(const cv::Mat input_im, const int N, const int M, const int R);

// Function to evaluate the contrast enhancement algorithm
void contrast_evaluation(const cv::Mat result_im, const cv::Mat ground_truth_im, const int N, const int M, const int R);

// Function to compute Contrast Limited Adaptive Histogram Equalization (CLAHE)
void compute_CLAHE(const cv::Mat& input, cv::Mat& output);