#include "stress.hpp"

#include <iostream>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <random>
#include <vector>

using namespace std;

void polarToCartesian(const int r, const int teta, int& x, int& y) {
    x = r * cos(teta);
    y = r * sin(teta);
}

void stressGray(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R, vector<cv::Vec2i> neighbors) {

    // TODO Check if the input image is grayscale   

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist_R(1, R);
    std::uniform_real_distribution<> uniform_dist_teta(-CV_PI, CV_PI);

    int n_rows = input.rows;
    int n_cols = input.cols;

    Emin.create(n_rows, n_cols, CV_64F);
    Emax.create(n_rows, n_cols, CV_64F);

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            int x = input.at<uchar>(i, j);
            double range_mean = 0.0;
            double relative_value_mean = 0.0;
            // liste de coordonnées de pixels voisins
            //vector<cv::Vec2i> neighbors;

            for (int it = 0; it < N; it++) {
                vector<int> rand_neighbors(M+1,0);
                for (int k = 0; k < M; k++) {
                    bool valid_rand_pixel = false;
                    while (!valid_rand_pixel) {
                        //int d = rand() % R;
                        int d = uniform_dist_R(e1);
                        //double teta = ((double)rand() / RAND_MAX) * 2 * CV_PI - CV_PI;
                        double teta = uniform_dist_teta(e1);
                        int abs, ord;
                        polarToCartesian(d, teta, abs, ord);
                        abs = i + round(abs);
                        ord = j + round(ord);
                        if (abs >= 0 && abs < n_rows && ord >= 0 && ord < n_cols && abs != i && ord != j) {
                            rand_neighbors[k] = input.at<uchar>(abs, ord);
                            valid_rand_pixel = true;
                            if (i == 200 &&  j == 150) {
                                neighbors.push_back(cv::Vec2i(abs, ord));
                                cout << "HERE : " + to_string(abs) + " " + to_string(ord) << endl;
                            }
                        }
                    }
                }
                rand_neighbors[M] = x; // add the current pixel to the list of neighbors
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

void stressRGB(const cv::Mat input, cv::Mat& Emin, cv::Mat& Emax, const int N, const int M, const int R){
    
    int n_rows = input.rows;
    int n_cols = input.cols;
    Emin.create(n_rows, n_cols, CV_64FC3);
    Emax.create(n_rows, n_cols, CV_64FC3);
    // Appliquer l'algorithme précédent sur chaque canal de l'image d'origine
    for (int c = 0; c < input.channels(); c++){
        cv::Mat input_c,E_min_c,E_max_c;
        cv::extractChannel(input,input_c,c);
        vector<cv::Vec2i> neighbors;
        stressGray(input_c,E_min_c,E_max_c,N,M,R,neighbors);
        cv::insertChannel(E_min_c,Emin,c);
        cv::insertChannel(E_max_c,Emax,c);
    } 
} 


