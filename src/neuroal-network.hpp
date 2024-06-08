#pragma once

#include <span>
#include <array>
#include <vector>

#include "random.hpp"

double relu(double x) {
  return std::max(0.0, x);
}

double sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}


struct Neuron {
  double bias;
  double accum;
  int distFromInput;
};

struct Link {
  int from;
  int to;
  double weight;
};


constexpr int InputSize = 4, OutputSize = 2;
// template <int InputSize, int OutputSize>

class NeuronalNetwork {
private:
  std::vector<Neuron> neurons; // size: HiddenSize + OutputSize (WITHOUT InputSize)
  std::vector<Link> links;

public:
  NeuronalNetwork() {
    neurons.reserve(OutputSize);

    for (int j = 0; j < OutputSize; j++) {
      neurons.push_back(Neuron { .bias = randomBetweenMinusOneAndPlusOne(), .distFromInput = 1 });
    }
    
    links.reserve(InputSize * OutputSize);

    for (int i = 0; i < InputSize; i++) {
      for (int j = 0; j < OutputSize; j++) {
        links.emplace_back(Link { .from = i, .to = j, .weight = randomBetweenMinusOneAndPlusOne() });
      }
    }
  }

  std::array<double, OutputSize> forward(std::span<double, InputSize> input) {

  }

  void addNeuron() {

  }
};

/*

topological order of node indices: [1, 2, 3, 4, 5, 6, 7]

1   2   3   4
|   |\  |  /
|   | \ | /
|   |  \|/
X   |   5
 \  |  /|
  \ | / |
   \|/  |
    6   |
     \  |
      \ |
       \|
        7

we want to insert node X on the link (1, 6)
X could be spliced before either 5 or 6, i.e. [1, 2, 3, 4, X, 5, 6, 7] or [1, 2, 3, 4, 5, X, 6, 7]
because dist(6, input) == dist(5, input)
and hence a future link could be either (5, 6) or (6, 5) depending on where we choose to splice X.

*/