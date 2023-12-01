#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <numeric>
#include <utility>

std::vector<std::string> get_input_lines () {
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }

    return lines;
}

int parse_first_digit(const std::vector<std::pair<std::string, int>>& DICT, const std::string& str) {
    int res {0};
    std::list<int> ALL_CANDIDATES;
    for (auto i {0}; i < DICT.size(); ++i) {
        ALL_CANDIDATES.push_back(i);
    }
    std::list<int> candidates(ALL_CANDIDATES);
    size_t candidateCheckingPos {0};

    for (auto iCh {0}; iCh < str.length(); ++iCh) {
        const auto ch {str.at(iCh)};
        if (std::isdigit(ch)) {
            res = ch - '0';
            break;
        }
        auto it {candidates.cbegin()};
        bool found {false};
        while (it != candidates.cend()) {
            if (ch != DICT.at(*it).first.at(candidateCheckingPos)) {
                candidates.erase(it++);
            } else {
                if (candidateCheckingPos + 1 == DICT.at(*it).first.size()) {
                    res = DICT.at(*it).second;
                    found = true;
                    break;
                }
                ++it;
            }
        }
        if (found) {
            break;
        }
        if (!candidates.size()) {
            candidates = ALL_CANDIDATES;
            iCh -= candidateCheckingPos;
            candidateCheckingPos = 0;
        } else {
            ++candidateCheckingPos;
        }
    }

    return res;
}

int main() {
    auto lines {get_input_lines()};

    // Problem 1
    std::vector<int> firstDigits, lastDigits;
    for (const auto& line : lines) {
        for (const auto ch : line) {
            if (std::isdigit(ch)) {
                firstDigits.push_back(ch - '0');
                break;
            }
        }
        for (auto ri {line.crbegin()}; ri != line.crend(); ++ri) {
            const auto ch {*ri};
            if (std::isdigit(ch)) {
                lastDigits.push_back(ch - '0');
                break;
            }
        }
    }

    std::cout << "Problem 1: ";
    std::cout << std::accumulate(firstDigits.cbegin(), firstDigits.cend(), 0) * 10 +
        std::accumulate(lastDigits.cbegin(), lastDigits.cend(), 0) << '\n';

    // Problem 2
    firstDigits.clear();
    lastDigits.clear();

    const auto DICT {std::vector<std::pair<std::string, int>>{
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}
    }};
    const auto R_DICT {std::vector<std::pair<std::string, int>>{
        {"eno", 1},
        {"owt", 2},
        {"eerht", 3},
        {"ruof", 4},
        {"evif", 5},
        {"xis", 6},
        {"neves", 7},
        {"thgie", 8},
        {"enin", 9}
    }};

    for (const auto& line : lines) {
        firstDigits.push_back(parse_first_digit(DICT, line));
        std::string rline(line.size(), '\0');
        std::reverse_copy(line.cbegin(), line.cend(), rline.begin());
        lastDigits.push_back(parse_first_digit(R_DICT, rline));
    }

    std::cout << "Problem 2: ";
    std::cout << std::accumulate(firstDigits.cbegin(), firstDigits.cend(), 0) * 10 +
        std::accumulate(lastDigits.cbegin(), lastDigits.cend(), 0) << '\n';

    return 0;
}
