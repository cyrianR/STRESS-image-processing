#include <iostream>
#include <opencv2/core.hpp>
#include <chrono>

// Function to compute Peak Signal-to-Noise Ratio (PSNR)
double PSNR(const cv::Mat& I1, const cv::Mat& I2);

// Function to compute the Structural Similarity Index (SSIM)
cv::Scalar SSIM(const cv::Mat& I1, const cv::Mat& I2);

// Function to compute the luminance component of SSIM
double SSIM_lum(const cv::Mat& I1, const cv::Mat& I2);

// Template function to measure execution time
template <typename Func>
double measure_execution_time(Func func, bool show_time) {
    if (!show_time) {
        func();
        return 0.0;
    }
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}