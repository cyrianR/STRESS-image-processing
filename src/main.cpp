#include <cstdlib>
#include <iostream>

#include <getopt.h>
#include <omp.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "contrast_enhancement.hpp"
#include "utils.hpp"


using namespace cv;
using namespace std;

// Print help message
void print_help() {
    cout << "Usage: stress <algorithm> [options] <input>" << endl;
    cout << "Algorithms:" << endl;
    cout << "  hdr                      High Dynamic Range Rendering with STRESS" << endl;
    cout << "  contrast                 Local Contrast Enhancement with STRESS" << endl;
    cout << "  clahe                    Contrast Limited Adaptive Histogram Equalization" << endl;
    cout << "Input:" << endl;
    cout << "  <input>                  Input image/folder path. Depends on the chosen algorithm." << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help               Show this help message" << endl;
    cout << "  -N <int>                 Set N (default: 100)" << endl;
    cout << "  -M <int>                 Set M (default: 20)" << endl;
    cout << "  -R <int>                 Set R (default: 300)" << endl;
    cout << "  -w, --show               Show the output image" << endl;
    cout << "  -e, --eval <str>         Evaluate the algorithm by comparing to the given groundtruth" << endl;
    cout << "  -t, --time               Show the time taken by the algorithm" << endl;
    cout << "  -p, --parallel           Use parallel processing" << endl;
    cout << "  -s, --save <str>         Save the output at the given path" << endl;
}

int main(int argc, char** argv) {

    // Command line arguments initialization
    string algorithm;
    string input_path;
    string ouptut_path;
    string ground_truth_path;
    bool show_output = false;
    bool eval = false;
    bool parallel = false;
    bool show_time = false;
    bool save = false;
    int N = 100;
    int M = 20;
    int R = 300;

    // Output image
    Mat output_image;

    // Elapsed time
    double elapsed_time;

    // Command line arguments options
    static struct option long_options[] = {
        {"eval", required_argument, 0, 'e'},
        {"save", required_argument, 0, 's'},
        {"show", no_argument, 0, 'w'},
        {"parallel", no_argument, 0, 'p'},
        {"time", no_argument, 0, 't'}
    };

    // Parse first argument specifying the algorithm or help
    if (argc < 2) {
        cout << "Error: No algorithm specified." << endl;
        print_help();
        return 1;
    } else {
        string first_arg = argv[1];
        if (first_arg == "--help" || first_arg == "-h") {
            print_help();
            return 0;
        } else if (first_arg == "hdr") {
            algorithm = first_arg;
        } else if (first_arg == "contrast") {
            algorithm = first_arg;
        } else if (first_arg == "clahe") {
            algorithm = first_arg;
        } else {
            cout << "Error: Unknown algorithm: " << first_arg << endl;
            print_help();
            return 1;
        }
    }

    // Skip the first argument
    argc--;
    argv++;

    // Parse optionnal arguments
    while (true) {
        int option_index = 0;
        int c = getopt_long(
            argc,
            argv,
            "N:R:M:e:s:t:w:p",
            long_options,
            &option_index);
        if (c == -1) break;
        switch (c) {
            case 'N':
                N = atoi(optarg);
                break;
            case 'R':
                R = atoi(optarg);
                break;
            case 'M':
                M = atoi(optarg);
                break;
            case 'e':
                ground_truth_path = optarg;
                eval = true;
                break;
            case 's':
                ouptut_path = optarg;
                save = true;
                break;
            case 'w':
                show_output = true;
                break;
            case 't':
                show_time = true;
                break;
            case 'p':
                parallel = true;
                break;
            default:
                break;
        }
    }

    // Parse input path
    if (optind + 1 == argc) {
        input_path = argv[optind];
    } else {
        cout << "Error: No input path specified." << endl;
        print_help();
        return 1;
    }

    // Read input image
    Mat image = imread(input_path, IMREAD_UNCHANGED);
    if (image.empty()) {
        cout << "Error: Could not open or find the input image" << endl;
        return 1;
    }

    // Enable or disable OpenMP parallel processing
    if (parallel) {
        omp_set_num_threads(omp_get_max_threads());
        cout << "Parallel processing enabled with " << omp_get_max_threads() << " threads." << endl;
    } else {
        omp_set_num_threads(1);
        cout << "Parallel processing disabled." << endl;
    }

    // Compute the chosen algorithm on the input image(s)
    if (algorithm == "hdr") {
        if (eval) {
            // Evaluate HDR rendering algorithm with STRESS
            cout << "Evaluation for HDR rendering not implemented yet" << endl;
        } else {
            // Compute HDR rendering algorithm with STRESS
            cout << "HDR rendering algorithm not implemented yet." << endl;
        }
    } else if (algorithm == "contrast") {
        if (eval) {
            // Evaluate contrast enhancement algorithm with STRESS
            if (ground_truth_path.empty()) {
                cout << "Error: No ground truth path specified." << endl;
                print_help();
                return 1;
            } else {
                Mat ground_truth_image = imread(ground_truth_path, IMREAD_UNCHANGED);
                if (ground_truth_image.empty()) {
                    cout << "Error: Could not open or find the ground truth image" << endl;
                    return 1;
                } else {
                    elapsed_time = measure_execution_time([&]() {
                        contrast_evaluation(image, ground_truth_image, N, M, R);
                    }, show_time);
                }
            }
        } else {
            // Compute contrast enhancement algorithm with STRESS
            elapsed_time = measure_execution_time([&]() {
                output_image = contrast_enhancement(image, N, M, R);
            }, show_time);
        }
    } else if (algorithm == "clahe") {
        // Compute CLAHE algorithm
        elapsed_time = measure_execution_time([&]() {
            compute_CLAHE(image, output_image);
        }, show_time);
    } else {
        // Unknown algorithm
        cout << "Error: Unknown algorithm: " << algorithm << endl;
        print_help();
        return 1;
    }

    // Show execution time if requested
    if (show_time) {
        cout << "Execution time: " << elapsed_time << " seconds" << endl;
    }

    // Save output image
    if (save) {
        if (ouptut_path.empty()) {
            cout << "Error: No output path specified." << endl;
            print_help();
            return 1;
        } else {
            if (!imwrite(ouptut_path, output_image)) {
                cout << "Error : Failed to write the output image to the specified path.";
            }
        }
    }

    // Show output image
    if (show_output) {
        imshow("Source Image", image);
        imshow("Resulting Image", output_image);
        // Wait until esc pressed
        while(cv::waitKey(1) != 27);
    } 
    return 0;
}
