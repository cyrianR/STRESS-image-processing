#include "stress.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <opencv2/core.hpp>

using namespace std;

void polarToCartesian(const int r, const int teta, int& x, int& y) {
    x = r * cos(teta);
    y = r * sin(teta);
}

void stressGray(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R) {

    // TODO Check if the input image is grayscale    

    int n_rows = input.rows;
    int n_cols = input.cols;

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            int x = input.at<uchar>(i, j);
            double range_mean = 0.0;
            double relative_value_mean = 0.0;
            for (int it = 0; it < N; it++) {
                vector<int> rand_neighbors(M,0); 
                for (int k = 0; k < M; k++) {
                    bool valid_rand_pixel = false;
                    while (!valid_rand_pixel) {
                        int d = rand() % R;
                        int teta = (rand() % 2*CV_PI) - CV_PI;
                        int abs, ord;
                        polarToCartesian(d, teta, abs, ord);
                        abs = i + round(abs);
                        ord = j + round(ord);
                        if (abs >= 0 && abs < n_rows && ord >= 0 && ord < n_cols) {
                            rand_neighbors[k] = input.at<uchar>(abs, ord);
                            valid_rand_pixel = true;
                        }
                    }
                }
                int min = *min_element(rand_neighbors.begin(), rand_neighbors.end());
                int max = *max_element(rand_neighbors.begin(), rand_neighbors.end());
                int r = max - min;
                range_mean += r;
                if (r == 0) {
                    relative_value_mean += 1.0/2.0;
                } else {
                    relative_value_mean += (double)(x - min) / (double)r;
                }
            }
            range_mean /= N;
            relative_value_mean /= N;
            Emin.at<double>(i, j) = (double)x - range_mean * relative_value_mean;
            Emax.at<double>(i, j) = (double)x + range_mean * (1 - relative_value_mean);
        }
    }

}


