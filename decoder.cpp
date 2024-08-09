#include "inc/utils.h"
#include "inc/huffman.h"
#include "inc/burrows_wheeler.h"
#include "inc/move_to_front.h"
#include "inc/run_length_encoding.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

std::vector<std::pair<int, int> > parseRLE(const std::string &input) {
    std::vector<std::pair<int, int> > rle;
    std::istringstream iss(input);
    int value, count;

    while (iss >> value >> count) {
        rle.emplace_back(value, count);
    }

    return rle;
}

void decodeFile(const std::string &filename) {
    // Step 1: Check if the file has the correct extension (.bit)
    if (filename.size() < 5 || filename.substr(filename.size() - 5) != "._bz2") {
        std::cerr << "Error: File extension must be ._bz2" << std::endl;
        return;
    }

    // Step 2: Read binary data from file
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }
    
    std::vector<unsigned char> mapBinary;
    std::vector<unsigned char> dataBinary;
    readBinaryFromFile(filename, mapBinary, dataBinary);

    // Step 3: Decode the Huffman code map
    std::unordered_map<char, std::string> huffmanCode = decodeHuffmanCodeMap(mapBinary);

    // Step 4: Convert binary bitstream back to the original bitstream
    std::string bitStream = binaryToBitStream(dataBinary);

    // Step 5: Decode the Huffman-encoded bitstream
    std::string huffmanDecodedData = huffmanDecode(bitStream, huffmanCode);

    // Step 6: Reverse Run-Length Encoding
    std::vector<int> rleDecoded = runLengthDecode(parseRLE(huffmanDecodedData));

    // Step 7: Reverse Move-to-Front Transform
    std::string mtfDecoded = inverseMoveToFrontTransform(rleDecoded);

    // Step 8: Reverse Burrows-Wheeler Transform
    std::string originalData = inverseBurrowsWheelerTransform(mtfDecoded);

    // Step 9: Write the decoded data to a new file with the .bin extension removed
    std::string outputFilename = filename.substr(0, filename.size() - 5);
    std::ofstream outfile(outputFilename);
    if (!outfile) {
        std::cerr << "Error creating output file: " << outputFilename << std::endl;
        return;
    }

    outfile << originalData;
    outfile.close();

    // Step 10: Delete the original .bin file
    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Error deleting file: " << filename << std::endl;
    } else {
        std::cout << "File successfully decoded: " << filename << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    decodeFile(filename);

    return 0;
}
