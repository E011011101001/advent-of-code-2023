#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <functional>

#include "tools.hpp"

int main() {
    auto lines {get_input_lines()};

    using UnsignedMap_f = std::function<unsigned int(unsigned int)>;

    std::istringstream seedsLine {lines.front()};
    seedsLine.seekg(7);
    std::vector<unsigned int> seeds;
    unsigned int seed;
    while (seedsLine >> seed) {
        seeds.push_back(seed);
    }

    std::vector<UnsignedMap_f> functions;

    auto it {lines.cbegin() + 1};
    decltype(it) jt;
    do {
        it += 2;
        jt = it;
        do {
            ++jt;
        } while (jt != lines.cend() && std::isdigit(jt->front()));

        unsigned int dst, start, length;
        std::vector<unsigned int> dsts, starts, lengths;
        for (auto pLine {it}; pLine != jt; ++pLine) {
            std::istringstream line {*pLine};
            line >> dst >> start >> length;
            dsts.push_back(dst);
            starts.push_back(start);
            lengths.push_back(length);
        }

        functions.emplace_back([dsts, starts, lengths](unsigned int n) -> unsigned int {
            unsigned int res {n};
            for (size_t i {0}; i != dsts.size(); ++i) {
                if (n >= starts.at(i) && n < starts.at(i) + lengths.at(i)) {
                    res = n - starts.at(i) + dsts.at(i);
                    break;
                }
            }
            return res;
        });

        it = jt;
    } while (jt != lines.cend());

    UnsignedMap_f total_map {std::reduce(
        functions.cbegin(),
        functions.cend(),
        UnsignedMap_f{[](unsigned int n) -> unsigned int {
            return n;
        }},
        [](const UnsignedMap_f reduced_f, const UnsignedMap_f f) -> UnsignedMap_f {
            return [reduced_f, f](unsigned int n) -> unsigned int {
                return f(reduced_f(n));
            };
        }
    )};

    std::vector<unsigned int> locations;
   // locations.reserve(seeds.size());
    std::transform(
        seeds.cbegin(),
        seeds.cend(),
        std::back_inserter(locations),
        total_map
    );
    std::cout << "Problem 1: " << *std::min_element(locations.cbegin(), locations.cend()) << '\n';

    return 0;
}
