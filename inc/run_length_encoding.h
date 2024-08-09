#ifndef RUN_LENGTH_ENCODING_H
#define RUN_LENGTH_ENCODING_H

#include <vector>
#include <utility>

std::vector<std::pair<int, int> > runLengthEncode(const std::vector<int> &input);
std::vector<int> runLengthDecode(const std::vector<std::pair<int, int> > &encoded);


#endif
