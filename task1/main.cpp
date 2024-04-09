#include <cmath>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <numbers>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int PERIOD = 10;
const double M_PI =  3.14159265358979323846;
const double PHASE_SHIFT = M_PI / 3.0 ; 

cv::Mat generateSinusoidalPattern(double phaseShift) {
    cv::Mat pattern(HEIGHT, WIDTH, CV_64F);
    phaseShift = std::fmod(phaseShift, 360.0);
    for (int x = 0; x < WIDTH; ++x) {
        
        double value = std::sin(2 * M_PI / PERIOD * x + phaseShift);
        for (int y = 0; y < HEIGHT; ++y) {
            pattern.at<double>(y, x) = value;
        }
    }
    return pattern;
}


cv::Mat computePhaseMap(const cv::Mat& I1, const cv::Mat& I2, const cv::Mat& I3) {
    cv::Mat phaseMap(HEIGHT, WIDTH, CV_64F);

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            
            double I1_val = I1.at<double>(y, x);
            double I2_val = I2.at<double>(y, x);
            double I3_val = I3.at<double>(y, x);
            
            
            double numerator = sqrt(3.0) * (I1_val - I3_val);
            double denominator = 2.0 * I2_val - I1_val - I3_val;
            double phase = std::atan2(numerator, denominator);
            
            
            if (phase < 0) phase += 2 * M_PI;
            
            phaseMap.at<double>(y, x) = phase;
        }
    }
    return phaseMap;
}

int main() {
    
    cv::Mat I1_first = generateSinusoidalPattern(0);
    cv::Mat I2_first = generateSinusoidalPattern(PHASE_SHIFT);
    cv::Mat I3_first = generateSinusoidalPattern(2 * PHASE_SHIFT);
    
    
    cv::Mat phaseMap_first = computePhaseMap(I1_first, I2_first, I3_first);
    
    
    cv::Mat I1_second = generateSinusoidalPattern(3 * PHASE_SHIFT);
    cv::Mat I2_second = generateSinusoidalPattern(4 * PHASE_SHIFT);
    cv::Mat I3_second = generateSinusoidalPattern(5 * PHASE_SHIFT);
    
    
    cv::Mat phaseMap_second = computePhaseMap(I1_second, I2_second, I3_second);
    
    
    cv::Mat phaseMap_avg = (phaseMap_first + phaseMap_second) / 2.0;

    
    cv::Mat displayPhaseMap2;
    cv::Mat displayPhaseMap;
    phaseMap_avg.convertTo(displayPhaseMap, CV_8UC1, 255 / (2 * M_PI), 255/2);
    phaseMap_first.convertTo(displayPhaseMap2, CV_8UC1, 255 / (2 * M_PI), 255/2);
    
    
    cv::imwrite("phase_map_avg.png", displayPhaseMap);
    cv::imwrite("phase_map_2.png", displayPhaseMap2);
    cv::imshow("phase_map_avg.png", displayPhaseMap);
    cv::waitKey(0);
    std::cout << "Average phase map has been saved." << std::endl;
    
    return 0;
}
