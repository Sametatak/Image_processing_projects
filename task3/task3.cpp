#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <numeric>




cv::Mat csvToMat(const std::string &filename) {
    std::ifstream inputFile(filename);
    std::string line;

    
    std::vector<float> tempArray;
    int cols = 0;
    int rows = 0;

    
    while (std::getline(inputFile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        rows++; 

        if (rows == 1) { 
            while (std::getline(lineStream, cell, ',')) {
                cols++;
                tempArray.push_back(std::stof(cell)); 
            }
        } else {
            while (std::getline(lineStream, cell, ',')) {
                tempArray.push_back(std::stof(cell)); 
            }
        }
    }

    
    cv::Mat dataMat(rows, cols, CV_32F); 
    
    std::memcpy(dataMat.data, tempArray.data(), tempArray.size() * sizeof(float));

    return dataMat;
}

// Computes disparity by finding the minimum difference 
//in intensity for each pixel in the left image against 
//all pixels in the same row in the right image.
// The resulting map shows horizontal displacement (disparity) values.
cv::Mat computeDisparity(const cv::Mat &leftMap, const cv::Mat &rightMap) {
    cv::Mat disparityMap(leftMap.rows, leftMap.cols, CV_32F);

    for (int y = 0; y < leftMap.rows; ++y) {
        for (int x = 0; x < leftMap.cols; ++x) {
            float minDifference = std::numeric_limits<float>::max();
            int bestMatchIndex = -1;

            for (int xRight = 0; xRight < rightMap.cols; ++xRight) {
                float difference = std::abs(leftMap.at<float>(y, x) - rightMap.at<float>(y, xRight));
                if (difference < minDifference) {
                    minDifference = difference;
                    bestMatchIndex = xRight;
                }
            }
            disparityMap.at<float>(y, x) = static_cast<float>(x - bestMatchIndex);
        }
    }

    return disparityMap;
}

cv::Mat medianfilter(const cv::Mat& Map) {
    // Assuming Map is a single-channel, floating-point matrix.
    cv::Mat processedMap = Map.clone();  // Clone the input matrix to preserve original data.

    for (int y = 0; y < processedMap.rows; ++y) {
        for (int x = 0; x < processedMap.cols; ++x) {
            std::vector<float> elements;

            // Collect up to 5 elements from the current position.
            for (int xRight = 0; xRight < 5 && (x + xRight) < processedMap.cols; ++xRight) {
                elements.push_back(processedMap.at<float>(y, x + xRight));
            }

            // Calculate the average of the collected elements.
            float average = std::accumulate(elements.begin(), elements.end(), 0.0f) / elements.size();

            // Find the element furthest from the average.
            auto furthestIt = std::max_element(elements.begin(), elements.end(),
                [average](float a, float b) {
                    return std::abs(a - average) < std::abs(b - average);
                });

            // Update the furthest element to the average value.
            int furthestIndex = std::distance(elements.begin(), furthestIt);
            if (furthestIndex + x < processedMap.cols) { // Check to avoid accessing out of bounds
                processedMap.at<float>(y, x + furthestIndex) = std::round(average - processedMap.at<float>(y, x + furthestIndex)/4);
            }
        }
    }

    return processedMap;
}



void writeMatToCSV(const std::string &filename, const cv::Mat &mat) {
    std::ofstream outputFile(filename);

    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            outputFile << mat.at<float>(i, j);
            if (j < mat.cols - 1) outputFile << ",";
        }
        outputFile << "\n";
    }
}

int main() {
    
    cv::Mat leftMap = csvToMat("C:/Users/samet/Desktop/opencvtest/task3/left_uwp_map.csv");
    cv::Mat rightMap = csvToMat("C:/Users/samet/Desktop/opencvtest/task3/right_uwp_map.csv");
    
    
    leftMap.convertTo(leftMap, CV_32F);
    rightMap.convertTo(rightMap, CV_32F);
    std::cout << "leftMap.converted\n";
    //gauss bluring filter

    //median bluring filter to clear the impulse noise

    cv::GaussianBlur(leftMap, leftMap, cv::Size(5, 1), 0);
    cv::GaussianBlur(rightMap, rightMap, cv::Size(5, 1), 0);
    cv::Mat temp;
    //Generate the disparity map from left and right images for stereo depth estimation.
    cv::Mat disparityMap = computeDisparity(leftMap, rightMap);
    disparityMap = medianfilter(disparityMap);
    
    //Laplacian filter


    

    
    
    
    writeMatToCSV("C:/Users/samet/Desktop/opencvtest/task3/disparity_map.csv",disparityMap);

    std::cout << "Disparity mp ahas been written to disparity_map.csv\n";

    return 0;
}
