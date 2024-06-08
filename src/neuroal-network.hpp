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
};

struct Link {
  size_t from;
  size_t to;
  double weight;
};


constexpr size_t InputSize = 4, OutputSize = 2;
// template <size_t InputSize, size_t OutputSize>

class NeuronalNetwork {
private:

  /**
   * Size = HiddenSize + OutputSize (WITHOUT InputSize)
   */
  std::vector<Neuron> neurons;

  /**
   * Links are in topological order.
   * Indices of neurons are shifted by InputSize.
   */
  std::vector<Link> links;

  /**
   * Example Network State
   * 
   *   0   1   2   3
   *   |   |\  |  /
   *   |   | \ | /
   *   |   |  \|/
   *   7   |   5
   *    \  |  /|
   *     \ | / |
   *      \|/  |
   *       6   |
   *        \  |
   *         \ |
   *          \|
   *           4
   * 
   * InputSize = 4
   * OutputSize = 1
   * 
   * Links History
   * [ (0, 4), (1, 4), (2, 4), (3, 4) ]  / Add neuron #5
   * [ (0, 4), (1, 4), (2, 4), (3, 4) ]  /
   */

public:
  NeuronalNetwork() {
    neurons.reserve(OutputSize);

    for (size_t j = 0; j < OutputSize; j++) {
      neurons.push_back(Neuron { .bias = randomBetweenMinusOneAndPlusOne() });
    }
    
    links.reserve(InputSize * OutputSize);

    for (size_t out = 0; out < OutputSize; out++) {
      for (size_t in = 0; in < InputSize; in++) {
        links.push_back(Link { .from = in, .to = InputSize + out, .weight = randomBetweenMinusOneAndPlusOne() });
      }
    }
  }

  // std::array<double, OutputSize> forward(std::span<double, InputSize> input) {

  // }

  void addNeuron() {
    size_t newNeuronIdx = neurons.size();
    neurons.push_back(Neuron { .bias = randomBetweenMinusOneAndPlusOne() });

    size_t linkIdx = randomIndex(links.size());
    Link newLink = { .from = newNeuronIdx, .to = links[linkIdx].to, .weight = links[linkIdx].weight };

    links[linkIdx].to = newNeuronIdx;
    links.insert(links.begin() + linkIdx + 1, newLink);
  }

  void addLink() {

  }
};

/**
 *   0   1   2   3
 *   |   |\  |  /
 *   |   | \ | /
 *   |   |  \|/
 *   7   |   5
 *    \  |  /|
 *     \ | / |
 *      \|/  |
 *       6   |
 *        \  |
 *         \ |
 *          \|
 *           4
 */
