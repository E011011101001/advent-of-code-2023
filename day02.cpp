#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <numeric>
#include <utility>
#include <sstream>

#include "tools.hpp"

int main() {
    auto lines {get_input_lines()};
    int sumPossibleGames {0};
    int sumOfPower {0};

    const int
        R_MAX {12},
        G_MAX {13},
        B_MAX {14};

    for (int i {1}; i <= lines.size(); ++i) {
        std::stringstream in {lines.at(i-1)};
        char ch;
        do {
            in.get(ch);
        } while (ch != ':');

        int rAmount {0}, gAmount {0}, bAmount {0};

        int amount;
        std::string colorStr;
        bool endLoop {false};
        do {
            in >> amount >> colorStr;
            ch = colorStr.at(colorStr.length() - 1);
            if (ch == ',' || ch == ';') {
                colorStr = colorStr.substr(0, colorStr.size() - 1);
            } else {
                endLoop = true;
            }

            if (colorStr == "red" && amount > rAmount) {
                rAmount = amount;
            } else if (colorStr == "green" && amount > gAmount) {
                gAmount = amount;
            } else if (colorStr == "blue" && amount > bAmount) {
                bAmount = amount;
            }
        } while (!endLoop);
        if (rAmount <= R_MAX && gAmount <= G_MAX && bAmount <= B_MAX) {
            sumPossibleGames += i;
        }
        sumOfPower += rAmount * gAmount * bAmount;
    }

    std::cout << "Problem 1: " << sumPossibleGames << '\n';
    std::cout << "Problem 2: " << sumOfPower << '\n';


    return 0;
}
