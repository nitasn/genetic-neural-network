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

#define debug(expression) print("debug:", #expression, "==", (expression))