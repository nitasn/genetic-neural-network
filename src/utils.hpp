#include <span>
#include <numeric>  // for std::partial_sum
#include <concepts> // for std::ranges::range
#include <iostream>

void print() {
  std::cout << '\n';
}

template <typename T, typename... Rest>
void print(const T& first, const Rest&... rest) {
  std::cout << first;
  if (sizeof...(rest)) std::cout << ' ';
  print(rest...);
}

template <typename ArrayLike>
void printArrayLike(const ArrayLike& input) {
  std::cout << "[ ";
  for (size_t i = 0; i < input.size();) {
    std::cout << input[i];
    if (++i < input.size()) std::cout << ',';
    std::cout << ' ';
  }
  std::cout << "]\n";
}

#define debug(expression) print("debug:", #expression, "==", (expression))

inline auto indexOf(auto& vec, auto& x) {
  return std::find(vec.begin(), vec.end(), x);
}

/**
 * input must not be empty, otherwise it's undefined behavior.
 */
template<std::ranges::range ArrLike>
constexpr ArrLike cumsum(const ArrLike& input) {
  ArrLike result;

  result[0] = input[0];
  for (std::size_t i = 1; i < input.size(); ++i) {
    result[i] = result[i - 1] + input[i];
  }

  return result;
}