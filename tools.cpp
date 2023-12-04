#include "tools.hpp"

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> get_input_lines () {
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }

    return lines;
}

Matrix<char> get_input_matrix() {
    Matrix<char> matrix;
    auto lines {get_input_lines()};
    for (const auto& line : lines) {
        Matrix<char>::Row row;
        for (const auto& ch : line) {
            row.push_back(ch);
        }
        matrix.push_back(std::move(row));
    }
    return matrix;
}
