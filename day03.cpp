#include <iostream>
#include <vector>
#include <cctype>
#include <utility>
#include <numeric>
#include <unordered_set>

#include "tools.hpp"

struct PartNumber {
    size_t row;
    size_t colStart;
    size_t colEnd;
    int value;

    PartNumber(const PartNumber&) = default;
    bool operator==(const PartNumber& partNum) const {
        return (row == partNum.row && colStart == partNum.colStart);
    }
};


std::vector<Coord> adjacent_coords_of (const PartNumber& partNumber, const size_t colSize, const size_t rowSize) {
    const size_t
        row         {partNumber.row},
        colStart    {partNumber.colStart},
        colEnd      {partNumber.colEnd};

    std::vector<Coord> adjacentCoords;
    /*
     * emplace_back order:
     * -- 0 --
     * 1     2
     * -- 3 --
     */

    if (row != 0) {
        if (colStart != 0) {
            adjacentCoords.emplace_back(row - 1, colStart - 1);
        }
        for (auto jTmp {colStart}; jTmp != colEnd; ++jTmp) {
            adjacentCoords.emplace_back(row - 1, jTmp);
        }
        if (colEnd < colSize) {
            adjacentCoords.emplace_back(row - 1, colEnd);
        }
    }
    if (colStart != 0) {
        adjacentCoords.emplace_back(row, colStart - 1);
    }
    if (colEnd < colSize) {
        adjacentCoords.emplace_back(row, colEnd);
    }
    if (row + 1 < rowSize) {
        if (colStart != 0) {
            adjacentCoords.emplace_back(row + 1, colStart - 1);
        }
        for (auto jTmp {colStart}; jTmp != colEnd; ++jTmp) {
            adjacentCoords.emplace_back(row + 1, jTmp);
        }
        if (colEnd < colSize) {
            adjacentCoords.emplace_back(row + 1, colEnd);
        }
    }
    return adjacentCoords;
}

bool adjacent_to_symbol (const Matrix<char>& inputMatrix, const PartNumber& candidate) {
    bool res {false};
    std::vector<Coord> adjacentCoords {adjacent_coords_of(
        candidate,
        inputMatrix.size(),
        inputMatrix.at(0).size()
    )};

    for (const auto& coord : adjacentCoords) {
        const auto ch {inputMatrix.at(coord)};
        if (ch != '.' && !std::isdigit(ch)) {
            res = true;
            break;
        }
    }
    return res;
}

std::vector<PartNumber> parse_part_number(const Matrix<char>& inputMatrix) {
    std::vector<PartNumber> partNums;
    const auto ROW_SIZE {inputMatrix.at(0).size()};

    for (size_t i {0}; i != inputMatrix.size(); ++i) {
        size_t j {0};
        do {
            // Find the next number in this row
            while (j < ROW_SIZE && !std::isdigit(inputMatrix.at({i, j}))) {
                // Find the next digit
                ++j;
            }
            if (j < ROW_SIZE) {
                const auto numStart {j};
                do {
                    ++j;
                } while (j < ROW_SIZE && std::isdigit(inputMatrix.at({i, j})));
                const auto numEnd {j}; // [numStart, numEnd)

                PartNumber candidate {i, numStart, numEnd, 0};
                if (adjacent_to_symbol(inputMatrix, candidate)) {
                    for (auto jTmp {numStart}; jTmp != numEnd; ++jTmp) {
                        candidate.value *= 10;
                        candidate.value += inputMatrix.at({i, jTmp}) - '0';
                    }
                    partNums.push_back(candidate);
                }

                j = numEnd;
            } // else j is row end
        } while (j < ROW_SIZE);
    }
    return partNums;
}

PartNumber get_part_number_at(const Matrix<char>& inputMatrix, size_t i, size_t j) {
    auto start {j}, end {j};
    while (start && std::isdigit(inputMatrix.at(i).at(start - 1))) {
        --start;
    }
    while (end < inputMatrix.at(0).size() && std::isdigit(inputMatrix.at(i).at(end))) {
        ++end;
    }
    int value {0};
    for (auto k {start}; k != end; ++k) {
        value *= 10;
        value += inputMatrix.at(i).at(k) - '0';
    }
    return {i, start, end, value};
}

struct HashPartNumber {
    size_t operator()(const PartNumber& partNum) const {
        return static_cast<size_t>(pow(2, partNum.row)*pow(3, partNum.colStart));
    }
};

// return 0 if not a gear
int gear_ratio(const Matrix<char>& inputMatrix, size_t i, size_t j) {
    int ratio {0};
    if (inputMatrix.at(i).at(j) == '*') {
        std::unordered_set<PartNumber, HashPartNumber> partNumsAdjacent;
        const auto ajacent_insert {[&inputMatrix, &partNumsAdjacent](size_t i, size_t j) -> void {
            if (std::isdigit(inputMatrix.at(i).at(j))) {
                partNumsAdjacent.insert(get_part_number_at(inputMatrix, i, j));
            }
        }};

        const auto ROW_SIZE {inputMatrix.at(0).size()};
        if (i) {
            if (j) {
                ajacent_insert(i - 1, j - 1);
            }
            ajacent_insert(i - 1, j);
            if (j + 1 < ROW_SIZE) {
                ajacent_insert(i - 1, j + 1);
            }
        }
        if (j) {
            ajacent_insert(i, j - 1);
        }
        if (j + 1 < ROW_SIZE) {
            ajacent_insert(i, j + 1);
        }
        if (i + 1 < inputMatrix.size()) {
            if (j) {
                ajacent_insert(i + 1, j - 1);
            }
            ajacent_insert(i + 1, j);
            if (j + 1 < ROW_SIZE) {
                ajacent_insert(i + 1, j + 1);
            }
        }
        if (partNumsAdjacent.size() == 2) {
            ratio = 1;
            for (const auto& partNum : partNumsAdjacent) {
                ratio *= partNum.value;
            }
        }
    }
    return ratio;
}

int main() {
    auto inputMatrix {get_input_matrix()};
    const auto partNumbers {parse_part_number(inputMatrix)};

    std::cout << "Problem 1: " << std::transform_reduce(
        partNumbers.cbegin(),
        partNumbers.cend(),
        0,
        std::plus<>(),
        [](const PartNumber& n) -> int {return n.value;}
    ) << '\n';

    int sumGearRatio {0};
    for (size_t i {0}; i < inputMatrix.size(); ++i) {
        for (size_t j {0}; j < inputMatrix.at(0).size(); ++j) {
            if (inputMatrix.at(i).at(j) == '*') {
                sumGearRatio += gear_ratio(inputMatrix, i, j);
            }
        }
    }

    std::cout << "Problem 2: " << sumGearRatio << '\n';

    return 0;
}
