#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <sstream>

#include "tools.hpp"

int main() {
    auto cards {get_input_lines()};

    int pointsTotal {0};
    std::vector<int> copyCnt(cards.size(), 1);

    size_t
        colonPos {cards.front().find(':')},
        barPos   {cards.front().find('|')};

    for (size_t i {0}; i < cards.size(); ++i) {
        std::istringstream
            winNumIn {cards.at(i).substr(colonPos + 1, barPos - colonPos)},
            cardNumIn {cards.at(i).substr(barPos + 1)};
        std::vector<int> winNums, cardNums;
        int winNum, cardNum;
        while (winNumIn >> winNum) {
            winNums.push_back(winNum);
        }
        while (cardNumIn >> cardNum) {
            cardNums.push_back(cardNum);
        }

        std::vector<int> matches;
        std::sort(winNums.begin(), winNums.end());
        std::sort(cardNums.begin(), cardNums.end());
        std::set_intersection(winNums.cbegin(), winNums.cend(), cardNums.cbegin(), cardNums.cend(), std::back_inserter(matches));

        if (matches.size()) {
            pointsTotal += (1 << (matches.size() - 1));
        }
        for (size_t j {0}; j < matches.size(); ++j) {
            copyCnt.at(i + j + 1) += copyCnt.at(i);
        }
    }

    std::cout << "Problem 1: " << pointsTotal << '\n';
    std::cout << "Problem 2: " << std::accumulate(copyCnt.cbegin(), copyCnt.cend(), 0) << '\n';

    return 0;
}
