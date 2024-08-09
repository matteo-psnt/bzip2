#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <vector>
#include <string>

std::string huffmanEncode(const std::string &input, std::unordered_map<char, std::string> &huffmanCode);
std::string huffmanDecode(const std::string &encodedString, const std::unordered_map<char, std::string> &huffmanCode);
std::vector<unsigned char> encodeHuffmanCodeMap(const std::unordered_map<char, std::string> &huffmanCode);
std::unordered_map<char, std::string> decodeHuffmanCodeMap(const std::vector<unsigned char> &binaryMap);

#endif
