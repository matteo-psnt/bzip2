#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

std::vector<unsigned char> bitStreamToBinary(const std::string &bitStream);
std::string binaryToBitStream(const std::vector<unsigned char> &binaryData);
void writeBinaryToFile(const std::string &filename, const std::vector<unsigned char> &mapBinary, const std::vector<unsigned char> &dataBinary);
void readBinaryFromFile(const std::string &filename, std::vector<unsigned char> &mapBinary, std::vector<unsigned char> &dataBinary);

#endif
