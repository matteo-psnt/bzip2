#include "huffman.h"
#include "utils.h"
#include <unordered_map>
#include <bitset>
#include <queue>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

struct HuffmanNode {
    char ch;
    int freq;
    std::unique_ptr<HuffmanNode> left, right;

    HuffmanNode(char ch, int freq) : ch(ch), freq(freq) {}
    HuffmanNode(std::unique_ptr<HuffmanNode> left, std::unique_ptr<HuffmanNode> right) : left(std::move(left)), right(std::move(right)) {
        freq = this->left->freq + this->right->freq;
        ch = '\0';
    }
};

std::unique_ptr<HuffmanNode> buildHuffmanTree(const std::string &input) {
    std::unordered_map<char, int> freq;
    for (char c : input) {
        ++freq[c];
    }

    auto compare = [](HuffmanNode* lhs, HuffmanNode* rhs) {
        return lhs->freq > rhs->freq;
    };

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(compare)> pq(compare);
    for (const auto &p : freq) {
        pq.push(new HuffmanNode(p.first, p.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        pq.push(new HuffmanNode(std::unique_ptr<HuffmanNode>(left), std::unique_ptr<HuffmanNode>(right)));
    }

    std::unique_ptr<HuffmanNode> root(pq.top());
    pq.pop();
    return root;
}

void createHuffmanCode(const HuffmanNode* node, const std::string& prefix, std::unordered_map<char, std::string>& huffmanCode) {
    if (!node) return;

    if (!node->left && !node->right) {
        huffmanCode[node->ch] = prefix;
    }

    createHuffmanCode(node->left.get(), prefix + "0", huffmanCode);
    createHuffmanCode(node->right.get(), prefix + "1", huffmanCode);
}

std::string huffmanEncode(const std::string &input, std::unordered_map<char, std::string> &huffmanCode) {
    std::unique_ptr<HuffmanNode> root = buildHuffmanTree(input);
    createHuffmanCode(root.get(), "", huffmanCode);

    std::string encodedString;
    for (char ch : input) {
        encodedString += huffmanCode.at(ch);
    }
    return encodedString;
}

std::string huffmanDecode(const std::string &encodedString, const std::unordered_map<char, std::string> &huffmanCode) {
    std::string decodedString;
    std::string currentCode;
    std::unordered_map<std::string, char> reverseHuffmanCode;

    for (const auto &pair : huffmanCode) {
        reverseHuffmanCode[pair.second] = pair.first;
    }

    for (char bit : encodedString) {
        currentCode += bit;
        if (reverseHuffmanCode.find(currentCode) != reverseHuffmanCode.end()) {
            decodedString += reverseHuffmanCode[currentCode];
            currentCode.clear();
        }
    }

    return decodedString;
}

std::vector<unsigned char> encodeHuffmanCodeMap(const std::unordered_map<char, std::string> &huffmanCode) {
    std::string binaryMap;
    for (const auto &p : huffmanCode) {
        std::bitset<8> charBits(p.first);
        binaryMap += charBits.to_string();

        std::bitset<8> lengthBits(p.second.size());
        binaryMap += lengthBits.to_string();

        binaryMap += p.second;
    }
    return bitStreamToBinary(binaryMap);
}

std::unordered_map<char, std::string> decodeHuffmanCodeMap(const std::vector<unsigned char> &binaryMap) {
    std::string binaryMapStr = binaryToBitStream(binaryMap);
    std::unordered_map<char, std::string> huffmanCode;
    size_t i = 0;

    while (i + 8 <= binaryMapStr.size()) {
        
        char ch = static_cast<char>(std::bitset<8>(binaryMapStr.substr(i, 8)).to_ulong());
        i += 8;

        if (i + 8 > binaryMapStr.size()) {
            std::cerr << "Error: Out of range while reading length bits at index " << i << std::endl;
            break;
        }
        int length = static_cast<int>(std::bitset<8>(binaryMapStr.substr(i, 8)).to_ulong());
        i += 8;

        if (i + length > binaryMapStr.size()) {
            std::cerr << "Error: Out of range while reading code bits at index " << i << std::endl;
            break;
        }
        std::string code = binaryMapStr.substr(i, length);
        i += length;

        huffmanCode[ch] = code;
    }
    return huffmanCode;
}
