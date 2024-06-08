#include "utils.hpp"
#include "neuroal-network.hpp"

int main() {
  NeuronalNetwork nn;

  std::array<double, 4> input = { 0.1, -0.2, 0.3, -0.4 };
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));

  nn.mutateLinks();
  printArrayLike(nn.forward(input));
}