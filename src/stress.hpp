#include <opencv2/core.hpp>

void stressGray(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R);

void stressRGB(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R);