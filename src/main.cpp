#include "utils.hpp"
#include "neuroal-network.hpp"
#include <tournament.hpp>

std::array<double, 4> input = { .1, .2, -.3, .4 };

double fitness(NeuronalNetwork<4, 2>& nn) {
  auto output = nn.forward(input);
  return exp(output[0] - output[1]);
}

int main() {
  Tournament tour(15, 0.30, fitness);
  for (size_t i = 0; i < 100; i++) {
    tour.advanceOneGeneration();
  }
  
}