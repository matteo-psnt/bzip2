#include "utils.h"
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

std::vector<unsigned char> bitStreamToBinary(const std::string &bitStream) {
    size_t numBytes = (bitStream.size() + 7) / 8;
    std::vector<unsigned char> byteData(numBytes, 0);

    for (size_t i = 0; i < bitStream.size(); ++i) {
        if (bitStream[i] == '1') {
            byteData[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    return byteData;
}

std::string binaryToBitStream(const std::vector<unsigned char> &binaryData) {
    std::string bitStream;
    for (unsigned char byte : binaryData) {
        for (int j = 7; j >= 0; --j) {
            bitStream += (byte & (1 << j)) ? '1' : '0';
        }
    }
    return bitStream;
}

void writeBinaryToFile(const std::string &filename, const std::vector<unsigned char> &mapBinary, const std::vector<unsigned char> &dataBinary) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    // Write the size of the Huffman map in bytes (as a 32-bit integer)
    uint32_t mapSize = mapBinary.size();
    outfile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

    // Write the Huffman map binary data
    outfile.write(reinterpret_cast<const char*>(mapBinary.data()), mapBinary.size());

    // Write the Huffman encoded data binary
    outfile.write(reinterpret_cast<const char*>(dataBinary.data()), dataBinary.size());

    outfile.close();
}

void readBinaryFromFile(const std::string &filename, std::vector<unsigned char> &mapBinary, std::vector<unsigned char> &dataBinary) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    // Read the size of the Huffman map in bytes (as a 32-bit integer)
    uint32_t mapSize;
    infile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    // Read the Huffman map binary data
    mapBinary.resize(mapSize);
    infile.read(reinterpret_cast<char*>(mapBinary.data()), mapSize);

    // Read the remaining data as the Huffman encoded data binary
    dataBinary.assign((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

    infile.close();
}
