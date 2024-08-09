#include "move_to_front.h"
#include <algorithm>

std::vector<int> moveToFrontTransform(const std::string &input) {
    std::vector<int> mtf;
    std::vector<char> alphabet(256);
    for (int i = 0; i < 256; ++i) {
        alphabet[i] = i;
    }

    for (char c : input) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        int index = std::distance(alphabet.begin(), it);
        mtf.push_back(index);
        alphabet.erase(it);
        alphabet.insert(alphabet.begin(), c);
    }

    return mtf;
}

std::string inverseMoveToFrontTransform(const std::vector<int> &mtf) {
    std::vector<char> alphabet(256);
    for (int i = 0; i < 256; ++i) {
        alphabet[i] = i;
    }

    std::string result;
    for (int index : mtf) {
        char c = alphabet[index];
        result += c;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), c);
    }

    return result;
}
