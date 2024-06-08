#include "utils.hpp"
#include "neuroal-network.hpp"

int main() {
  NeuronalNetwork nn;

  std::array<double, 4> input = { 0.1, -0.2, 0.3, -0.4 };
  printArrayLike(nn.forward(input));

  for (size_t i = 0; i < 1000; i++) {
    nn.applyRandomMutation();
    printArrayLike(nn.forward(input));
  }
}