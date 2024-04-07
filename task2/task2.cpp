#include "readply.h"
#include "readply.cpp"
#include <iostream>
#include <fstream>
#include <vector>

const float sigmaSpatial = 1.0f; // Example spatial standard deviation
const float sigmaRange = 1.0f; // Example range standard deviation

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
    std::vector<std::vector<std::vector<float>>> filteredPointCloud = organized_point_cloud; // Copy to maintain structure

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // Skip invalid points (assuming the first coordinate being NaN means the point is invalid)
            if (std::isnan(organized_point_cloud[y][x][0])) {
                continue;
            }

            float sumWeights = 0.0f;
            std::vector<float> sumFilteredPoint(6, 0.0f); // XYZ, NX, NY, NZ

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int nx = x + i;
                    int ny = y + j;

                    // Check bounds and skip the center point
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && (i != 0 || j != 0)) {
                        // Skip invalid neighbor points
                        if (std::isnan(organized_point_cloud[ny][nx][0])) {
                            continue;
                        }

                        float spatialDistance = std::sqrt(i * i + j * j); // Euclidean distance in grid space
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

    // Optional: Write header line
    outFile << "x,y,z,nx,ny,nz/n";

    for (const auto& row : pointCloud) {
        for (const auto& point : row) {
            // Check if the point is valid (not NaN)
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
    // The filename should be the path to your PLY file
    std::string filename = "C:/Users/samet/Desktop/opencvtest/task2/OriginalMesh.ply";
    int width = 2592; // Width of your point cloud
    int height = 1944; // Height of your point cloud
    
    // Read the point cloud data
    auto organized_point_cloud = readPLYFileWithNormals(filename, width, height);
    
    // Apply bilateral filter to the point cloud
    auto filteredPointCloud = applyBilateralFilter(organized_point_cloud,width,height);
    const std::string outputFilename = "filtered_point_cloud.ply";
    writePLYFile(outputFilename, filteredPointCloud, true);
    
    return 0;
}