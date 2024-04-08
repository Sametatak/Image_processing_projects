#include "readply.h"
#include "readply.cpp"
#include <iostream>
#include <fstream>
#include <vector>

const float sigmaSpatial = 1.0f; 
const float sigmaRange = 1.0f; 

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2 * sigma * sigma));
}

float intensityDistance(float int1, float int2) {
    return std::abs(int1 - int2);
}

float spatialDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

std::vector<std::vector<std::vector<float>>> applyBilateralFilter(
    const std::vector<std::vector<std::vector<float>>>& organized_point_cloud,
    float sigmaSpatial, float sigmaRange) {

    int height = organized_point_cloud.size();
    int width = organized_point_cloud[0].size();
    std::vector<std::vector<std::vector<float>>> filteredPointCloud = organized_point_cloud; 

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            
            if (std::isnan(organized_point_cloud[y][x][0])) {
                continue;
            }

            float sumWeights = 0.0f;
            std::vector<float> sumFilteredPoint(6, 0.0f); 

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int nx = x + i;
                    int ny = y + j;

                    
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && (i != 0 || j != 0)) {
                        
                        if (std::isnan(organized_point_cloud[ny][nx][0])) {
                            continue;
                        }

                        float spatialDistance = std::sqrt(i * i + j * j); 
                        float intensityDistance = std::sqrt(
                            std::pow(organized_point_cloud[y][x][0] - organized_point_cloud[ny][nx][0], 2) + 
                            std::pow(organized_point_cloud[y][x][1] - organized_point_cloud[ny][nx][1], 2) + 
                            std::pow(organized_point_cloud[y][x][2] - organized_point_cloud[ny][nx][2], 2));

                        float weight = gaussian(spatialDistance, sigmaSpatial) * gaussian(intensityDistance, sigmaRange);
                        sumWeights += weight;

                        for (int k = 0; k < 6; ++k) {
                            sumFilteredPoint[k] += weight * organized_point_cloud[ny][nx][k];
                        }
                    }
                }
            }

            if (sumWeights > 0) {
                for (int k = 0; k < 6; ++k) {
                    filteredPointCloud[y][x][k] = sumFilteredPoint[k] / sumWeights;
                }
            }
        }
    }

    return filteredPointCloud;
}

void exportPointCloudToCSV(const std::string& outputPath, const std::vector<std::vector<std::vector<float>>>& pointCloud) {
    std::ofstream outFile(outputPath);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open " << outputPath << " for writing." << std::endl;
        return;
    }

    
    outFile << "x,y,z,nx,ny,nz/n";

    for (const auto& row : pointCloud) {
        for (const auto& point : row) {
            
            if (!std::isnan(point[0])) {
                outFile << point[0] << "," << point[1] << "," << point[2] << ","
                        << point[3] << "," << point[4] << "," << point[5] << std::endl;
            }
        }
    }
}

void printPointCloud(const std::vector<std::vector<std::vector<float>>>& pointCloud) {
    for (size_t i = 0; i < pointCloud.size(); ++i) {
        for (size_t j = 0; j < pointCloud[i].size(); ++j) {
            std::cout << "Point[" << i << "][" << j << "]: ";
            for (size_t k = 0; k < pointCloud[i][j].size(); ++k) {
                std::cout << pointCloud[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
    }
}


std::vector<std::vector<std::vector<float>>> readPLYFileWithNormals(const std::string& filename, int width, int height);

int main() {
    
    std::string filename = "./OriginalMesh.ply";
    int width = 2592; 
    int height = 1944; 
    
    
    auto organized_point_cloud = readPLYFileWithNormals(filename, width, height);
    
    
    auto filteredPointCloud = applyBilateralFilter(organized_point_cloud,width,height);
    const std::string outputFilename = "filtered_point_cloud.ply";
    writePLYFile(outputFilename, filteredPointCloud, true);
    
    return 0;
}