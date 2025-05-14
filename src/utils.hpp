#include <opencv2/core.hpp>

double PSNR(const cv::Mat& I1, const cv::Mat& I2);

cv::Scalar SSIM(const cv::Mat& I1, const cv::Mat& I2);