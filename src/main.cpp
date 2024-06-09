#include "utils.hpp"
#include "neuroal-network.hpp"
#include <tournament.hpp>

namespace nitsan {
  std::array<double, 2> x0 = { 0, 0 };
  std::array<double, 2> x1 = { 0, 1 };
  std::array<double, 2> x2 = { 1, 0 };
  std::array<double, 2> x3 = { 1, 1 };

  std::array<std::array<double, 2>, 4> X = { x0, x1, x2, x3 };

  std::array<double, 1> y0 = { 0 };
  std::array<double, 1> y1 = { 1 };
  std::array<double, 1> y2 = { 1 };
  std::array<double, 1> y3 = { 0 };

  std::array<std::array<double, 1>, 4> Y = { y0, y1, y2, y3 };
}


double fitness(NeuronalNetwork<2, 1>& nn) {
  double result = 0;

  for (int i = 0; i < 4; i++) {
    auto x = nitsan::X[i];
    auto y = nitsan::Y[i];
    auto y_hat = nn.forward(x);
    double diff = y[0] - y_hat[0];
    result -= diff * diff;
  }

  return exp(result);
}

int main() {
  Tournament tour(99, 0.30, fitness);
  for (size_t i = 0; i < 1000; i++) {
    tour.advanceOneGeneration();
  }

  auto nn = tour.populationBest();

  for (int i = 0; i < 4; i++) {
    auto x = nitsan::X[i];
    auto y_hat = nn.forward(x);
    printArrayLike(x);
    print("network says", y_hat[0]);
    print();
  }
}