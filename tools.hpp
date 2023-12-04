#ifndef EOL_AOC_TOOLS
#define EOL_AOC_TOOLS

#include <vector>
#include <string>
#include <utility>

std::vector<std::string> get_input_lines();

using Coord = std::pair<size_t, size_t>;
template <class T>
class Matrix {
    using Matrix_ = std::vector<std::vector<T>>;
    Matrix_ _mat;
public:
    Matrix() : _mat{} {}

    using Row = std::vector<T>;
    const T& at(const Coord&) const;
    const Row& at(size_t) const;
    [[nodiscard]] size_t size() const {
        return _mat.size();
    }
    void push_back(const Row&);
    void push_back(Row&&);
};

template <class T>
const typename Matrix<T>::Row& Matrix<T>::at(const size_t i) const {
    return _mat.at(i);
}

template <class T>
const T& Matrix<T>::at(const Coord& coord) const {
    return _mat.at(coord.first).at(coord.second);
}

template <class T>
void Matrix<T>::push_back(const Row& row) {
    _mat.push_back(row);
}

template <class T>
void Matrix<T>::push_back(Row&& row) {
    _mat.emplace_back(std::move(row));
}

Matrix<char> get_input_matrix();

#endif // EOL_AOC_TOOLS
