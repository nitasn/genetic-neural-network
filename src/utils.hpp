#include <span>
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

inline auto indexOf(const auto& vec, const auto& x) {
  return std::find(vec.begin(), vec.end(), x);
}

inline auto findIf(const auto& vec, const auto& predicate) {
  return std::find_if(vec.begin(), vec.end(), predicate);
}