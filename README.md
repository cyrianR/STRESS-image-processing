# STRESS

This project implements the Spatio-Temporal Retinex-Inspired Envelope with Stochastic
Sampling algorithm in C++, an application for image contrast enhancement for grayscale and color images and some metrics in order to evaluate this algorithm and compare it with gimp implementation.

The LOw Light paired dataset (LOL) was used to test our implementation. It contains images with very low exposures and high exposures. Our goal is to process low exposed images into better looking images with higher exposure and nice contrast.

> Note : We first wanted to implement HDR images tonemapping but we ran out of time so we stayed with contrast enhancement implementation only. In consequence, there are multiple artefacts of this started application of STRESS.

## Project sctructure

```
STRESS-image-processing/
├── images/ : Images to test the implementation
    ├── ground_truths/ : Images with high exposures or low exposed images processed by gimp
    └── memorial_exposure/ : Images to test application on tonemapping of HDR images
├── results/ : Folder with some resulting images
├── src/ : Source files of the project
    ├── main.cpp : Entry point, parses command line arguments and launch algorithms accordingly
    ├── stress.* : Base STRESS implementation 
    ├── contrast_enhancement.* : Algorithm and evaluation of contrast enhancement with STRESS
    └── utils.* : Utility functions such as SSIM and PSNR
├── project.pdf : Project's subject
└── README.md
```

This project has been made to be as much flexible as possible, it is easy to add a new application and evaluation of the STRESS algorithm.

Images filename formats :
- Groundtruth images : \<base image name\>\_gt\_high\_exposure

- Gimp processed images : \<base image name\>\_gt\_gimp\_\<N\>\_\<M>\_\<R>

- Result images : result\_<base image name\>\_\<N\>\_\<M>\_\<R>

## Requirements

- make
- C compiler (tested on clang and gcc)
- OpenMP
- OpenCV

## Build

To build the project use the following :

```
git clone https://github.com/cyrianR/STRESS-image-processing.git
cd STRESS-image-processing
mkdir build
cd build
cmake ..
make
```

## Usage

After build, you can simply use the binary in *build/bin* folder as follow :
```
stress <algorithm> [options] <input>
```

Full description of usage is available with ```stress --help``` :

```
Usage: stress <algorithm> [options] <input>
Algorithms:
  hdr                      High Dynamic Range Rendering with STRESS
  contrast                 Local Contrast Enhancement with STRESS
  clahe                    Contrast Limited Adaptive Histogram Equalization
Input:
  <input>                  Input image/folder path. Depends on the chosen algorithm.
Options:
  -h, --help               Show this help message
  -N <int>                 Set N (default: 100)
  -M <int>                 Set M (default: 20)
  -R <int>                 Set R (default: 300)
  -w, --show               Show the output image
  -e, --eval <str>         Evaluate the algorithm by comparing to the given groundtruth
  -t, --time               Show the time taken by the algorithm
  -p, --parallel           Use parallel processing
  -s, --save <str>         Save the output at the given path
```

> **WARNING** : There is a strange bug in the parsing of the options, when using short options names such as -w, -p or -t, it sometimes miss one of the options. However, when using full option names such as --show, --parallel or --time, this bug seems to disappear like magic...

## Example

```
./bin/stress contrast -N 20 -M 20 -R 721 --show --time --parallel ./../images/toys.png
```


## Authors

- [Cyrian Ragot](https://github.com/cyrianR)
- [Yannis Rosseel](https://github.com/yrosseel21)

## References

*STRESS original paper*
Øyvind Kolås, Ivar Farup, and Alessandro Rizzi. “Spatio-Temporal Retinex-Inspired Envelope with Stochastic Sampling: A Framework for Spatial Color Algorithms”. In: Journal of Imaging Science and Technology 55.4 (July 2011), 40503–1–40503–10. issn: 1943-3522. doi: 10.2352/j.imagingsci.technol.2011.55.4.040503.
url: https://ivarfa.folk.ntnu.no/publications/journal/Kolaas_11_jist.pdf.

*LOw Light paired dataset*
Chen Wei et al. “Deep Retinex Decomposition for Low-Light Enhancement”. In: British Machine Vision Conference. 2018. 
url: https://daooshee.github.io/BMVC2018website/