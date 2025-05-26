#include <opencv2/core.hpp>

// Computes STRESS algorithm for gray images
void stressGray(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R);

// Computes STRESS algorithm for RGB images
void stressRGB(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R);