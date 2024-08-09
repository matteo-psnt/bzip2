#include "burrows_wheeler.h"
#include <algorithm>
#include <string>
#include <vector>


std::vector<int> buildSuffixArray(const std::string &input) {
    int n = input.size();
    std::vector<int> suffixArray(n);
    std::vector<int> rank(n);
    std::vector<int> tempRank(n);

    for (int i = 0; i < n; ++i) {
        suffixArray[i] = i;
        rank[i] = input[i];
    }

    for (int k = 1; k < n; k *= 2) {
        auto compare = [&](int a, int b) {
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            int rankA = (a + k < n) ? rank[a + k] : -1;
            int rankB = (b + k < n) ? rank[b + k] : -1;
            return rankA < rankB;
        };
        std::sort(suffixArray.begin(), suffixArray.end(), compare);

        tempRank[suffixArray[0]] = 0;
        for (int i = 1; i < n; ++i) {
            tempRank[suffixArray[i]] = tempRank[suffixArray[i - 1]] + compare(suffixArray[i - 1], suffixArray[i]);
        }
        rank = tempRank;
    }

    return suffixArray;
}

std::string burrowsWheelerTransform(const std::string &input) {
    std::string s = input + '\x1E';
    std::vector<int> suffixArray = buildSuffixArray(s);
    std::string bwt;
    
    for (int i = 0; i < suffixArray.size(); i++) {
        bwt += s[(suffixArray[i] + s.size() - 1) % s.size()];
    }
    
    return bwt;
}

std::string inverseBurrowsWheelerTransform(const std::string &bwt) {
    int n = bwt.size();

    std::vector<int> count(256, 0);
    for (char c : bwt) {
        count[c]++;
    }

    std::vector<int> startPos(256, 0);
    for (int i = 1; i < 256; ++i) {
        startPos[i] = startPos[i - 1] + count[i - 1];
    }

    std::vector<int> next(n);
    for (int i = 0; i < n; ++i) {
        next[startPos[bwt[i]]] = i;
        startPos[bwt[i]]++;
    }

    std::string original;
    original.reserve(n);
    int pos = next[0];
    for (int i = 1; i < n; ++i) {
        original += bwt[pos];
        pos = next[pos];
    }

    return original;
}
