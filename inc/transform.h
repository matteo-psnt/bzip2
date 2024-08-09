#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <string>

std::string burrowsWheelerTransform(const std::string &input);
std::vector<int> moveToFrontTransform(const std::string &input);
std::vector<std::pair<int, int>> runLengthEncode(const std::vector<int> &input);

#endif
