#include <cstdlib>
#include <iostream>

#include <getopt.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <time.h>
#include "contrast_enhancement.hpp"

using namespace cv;
using namespace std;

// Print help message
void print_help() {
    cout << "Usage: ./stress <algorithm> [options] <input>" << endl;
    cout << "Algorithms:" << endl;
    cout << "  hdr              High Dynamic Range Rendering" << endl;
    cout << "  contrast         Local Contrast Enhancement" << endl;
    cout << "Input:" << endl;
    cout << "  <input>          Input image/folder path. Depends on the chosen algorithm." << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help       Show this help message" << endl;
    cout << "  -N <int>         Set N (default: 50)" << endl;
    cout << "  -R <int>         Set R (default: 20)" << endl;
    cout << "  -M <int>         Set M (default: 8)" << endl;
    cout << "  -w, --show       Show the output image" << endl;
    cout << "  -e, --eval       Evaluate the algorithm" << endl;
    cout << "  -s, --save       Save the output at the given path" << endl;
}

int main(int argc, char** argv) {

    // Command line arguments initialization
    string algorithm;
    string input_path;
    string ouptut_path;
    string ground_truth_path;
    bool show_output = false;
    bool eval = false;
    bool save = false;
    int N = 100;
    int M = 3;
    int R = 300;

    // Output image
    Mat output_image;

    // Command line arguments options
    static struct option long_options[] = {
        {"eval", required_argument, 0, 'e'},
        {"show", no_argument, 0, 'w'},
        {"save", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    // Parse first argument specifying the algorithm
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
            algorithm = "hdr";
        } else if (first_arg == "contrast") {
            algorithm = "contrast";
        } else if (first_arg == "clahe") {
            algorithm = "clahe";
        } 
        else {
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
        int c = getopt_long(argc, argv, "N:R:M:e:s:w", long_options, &option_index);
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
    Mat image = imread(input_path, IMREAD_UNCHANGED);
    if (image.empty()) {
        cout << "Error: Could not open or find the input image" << endl;
        return -1;
    } else {

    // Compute the chosen algorithm on the input image(s)
    if (algorithm == "hdr") {
        cout << "HDR rendering algorithm not implemented yet." << endl;
        if (eval) {
            cout << "Evaluation for HDR rendering not implemented yet" << endl;
        }
    } else if (algorithm == "contrast") {
        // Read input image and compute contrast enhancement algorithm 
        if (eval) {
            // Evaluate contrast enhancement algorithm
            if (ground_truth_path.empty()) {
                cout << "Error: No ground truth path specified." << endl;
                print_help();
                return 1;
            } else {
                Mat ground_truth_image = imread(ground_truth_path, IMREAD_UNCHANGED);
                if (ground_truth_image.empty()) {
                    cout << "Error: Could not open or find the ground truth image" << endl;
                    return -1;
                } else {
                    contrast_evaluation(image, ground_truth_image, N, M, R);
                }
            }
        } else {
            // Compute contrast enhancement algorithm
            time_t t = time(nullptr);
            output_image = contrast_enhancement(image , N, M, R);
            t = time(nullptr) - t;
            printf("Temps pris :");
            printf("%f secondes\n", (double) t);
        }
    } 
    else if (algorithm == "clahe"){
            applyCLAHE(image, output_image);
        } 
    else {
        cout << "Error: Unknown algorithm: " << algorithm << endl;
        print_help();
        return 1;
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
    } 
    return 0;
}
