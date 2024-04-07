#ifndef READPLY_H
#define READPLY_H

#include <string>
#include <vector>

void writePLYFile(const std::string& filename, 
                  const std::vector<std::vector<std::vector<float>>>& organized_point_cloud, 
                  bool organized );

std::vector<std::vector<std::vector<float>>> readPLYFileWithNormals(const std::string& filename, 
                                                                    int width, int height);

#endif // READPLY_H
