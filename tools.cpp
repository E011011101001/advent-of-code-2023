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
