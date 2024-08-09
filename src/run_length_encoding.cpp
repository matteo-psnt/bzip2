#include "run_length_encoding.h"
#include <vector>

std::vector<std::pair<int, int>> runLengthEncode(const std::vector<int> &input) {
    std::vector<std::pair<int, int>> rle;
    int n = input.size();

    for (int i = 0; i < n; ++i) {
        int count = 1;
        while (i < n - 1 && input[i] == input[i + 1]) {
            ++i;
            ++count;
        }
        rle.emplace_back(input[i], count);
    }

    return rle;
}

std::vector<int> runLengthDecode(const std::vector<std::pair<int, int>> &encoded) {
    std::vector<int> result;

    for (const auto &p : encoded) {
        result.insert(result.end(), p.second, p.first);
    }

    return result;
}
