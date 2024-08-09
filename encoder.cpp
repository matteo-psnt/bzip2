#include "inc/huffman.h"
#include "inc/burrows_wheeler.h"
#include "inc/move_to_front.h"
#include "inc/run_length_encoding.h"
#include "inc/utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

void encodeFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::string input((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();

    size_t originalSize = input.size();

    // Step 1: Perform Burrows-Wheeler Transform
    std::string bwt = burrowsWheelerTransform(input);

    // Step 2: Perform Move-to-Front Transform
    std::vector<int> mtf = moveToFrontTransform(bwt);

    // Step 3: Perform Run-Length Encoding
    std::vector<std::pair<int, int> > rle = runLengthEncode(mtf);

    // Convert RLE data to string
    std::string encodedData;
    for (const auto &p : rle) {
        encodedData += std::to_string(p.first) + ' ' + std::to_string(p.second) + ' ';
    }

    // Step 4: Build Huffman Tree and Encode Data
    std::unordered_map<char, std::string> huffmanCode;
    std::string huffmanEncodedData = huffmanEncode(encodedData, huffmanCode);

    // Step 5: Convert Huffman-encoded data and Huffman code map to binary
    std::vector<unsigned char> mapBinary = encodeHuffmanCodeMap(huffmanCode);
    std::vector<unsigned char> dataBinary = bitStreamToBinary(huffmanEncodedData);

    // Step 6: Write binary data to output file
    std::string outputFilename = filename + "._bz2";
    writeBinaryToFile(outputFilename, mapBinary, dataBinary);

    // Step 7: Delete the original input file
    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Error deleting file: " << filename << std::endl;
    } else {
        std::cout << "File successfully encoded: " << filename << std::endl;
    }

    size_t encodedSize = mapBinary.size() + dataBinary.size();  // Encoded size in bytes

    // Step 7: Calculate space savings
    double savings = ((double)(originalSize - encodedSize) / originalSize) * 100;
    // std::cout << "Original size: " << originalSize << " bytes" << std::endl;
    // std::cout << "Encoded size: " << encodedSize << " bytes" << std::endl;
    // std::cout << "Space savings: " << savings << "%" << std::endl;

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    encodeFile(filename);

    return 0;
}
