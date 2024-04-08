#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream> 
const int WIDTH = 1280;
const int HEIGHT = 720;
const int PERIOD = 10;
const double M_PI =  3.14159265358979323846;
const double PHASE_SHIFT = M_PI / 3.0 ; 

unsigned grayToBinary(unsigned num) {
    unsigned mask;
    for (mask = num >> 1; mask != 0; mask = mask >> 1) {
        num = num ^ mask;
    }
    return num;
}


cv::Mat decodeGrayCodeImages(const std::vector<cv::Mat>& grayCodeImages) {
    const int height = grayCodeImages[0].rows;
    const int width = grayCodeImages[0].cols;
    cv::Mat fringeOrders(height, width, CV_32S);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned grayValue = 0;
            for (int k = 0; k < grayCodeImages.size(); ++k) {
                
                grayValue |= (grayCodeImages[k].at<uchar>(y, x) / 255) << k;
            }
            
            unsigned binaryCode = grayToBinary(grayValue);
            fringeOrders.at<int>(y, x) = static_cast<int>(binaryCode);
        }
    }
    return fringeOrders;
}


cv::Mat unwrapPhaseMap(const cv::Mat& wrappedPhaseMap, const cv::Mat& fringeOrders) {
    cv::Mat unwrappedPhaseMap(wrappedPhaseMap.size(), CV_64F);
    double maxPhase = 2 * M_PI;

    for (int y = 0; y < wrappedPhaseMap.rows; ++y) {
        for (int x = 0; x < wrappedPhaseMap.cols; ++x) {
            double wrappedPhase = wrappedPhaseMap.at<double>(y, x);
            int K = fringeOrders.at<int>(y, x);
            unwrappedPhaseMap.at<double>(y, x) = wrappedPhase + maxPhase * K;
        }
    }

    return unwrappedPhaseMap;
}

int main() {
    std::string directory = "./double-three-step"; 
    std::string wrappedPhaseMapFilename = "./phase_map_avg.png"; 

    
    cv::Mat wrappedPhaseMapImg = cv::imread(wrappedPhaseMapFilename, cv::IMREAD_UNCHANGED);
    if (wrappedPhaseMapImg.empty()) {
        std::cerr << "Failed to load the wrapped phase map: " << wrappedPhaseMapFilename << std::endl;
        return -1;
    }
    
    cv::Mat wrappedPhaseMap;
    wrappedPhaseMapImg.convertTo(wrappedPhaseMap, CV_64F, 2*M_PI/255.0, -M_PI); 

    
    std::vector<cv::Mat> grayCodeImages;
    for (int i = 0; i < 7; ++i) {
        std::string filename = directory + "/gray_pattern_" + std::to_string(i) + ".png";
        cv::Mat img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        if (img.empty()) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            return -1;
        }
        grayCodeImages.push_back(img);
    }

    
    cv::Mat fringeOrders = decodeGrayCodeImages(grayCodeImages);

    
    cv::Mat unwrappedPhaseMap = unwrapPhaseMap(wrappedPhaseMap, fringeOrders);

    
    int rowToPlot = unwrappedPhaseMap.rows / 2; 
    std::vector<double> phaseRow(unwrappedPhaseMap.cols);
    for (int x = 0; x < unwrappedPhaseMap.cols; ++x) {
        phaseRow[x] = unwrappedPhaseMap.at<double>(rowToPlot, x);
    }
    
    std::ofstream file("phaseRow.csv");

    if (file.is_open()) {
        for (size_t i = 0; i < phaseRow.size(); ++i) {
            
            file << phaseRow[i] << "/n";
        }
        file.close(); 
        std::cout << "Data has been written to phaseRow.csv" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
    std::cout << "mai2 executed" << std::endl;
    return 0;
}

 
