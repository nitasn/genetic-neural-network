#pragma once

#include <span>
#include <array>
#include <cmath>
#include <vector>

#include "random.hpp"


double relu(double x) {
  return std::max(0.0, x);
}

double sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}

double identity(double x) {
  return x;
}


struct Link {
  size_t toIndex;
  double weight;
};

struct Neuron {
  double bias, value;
  double (*activation)(double);
  std::vector<Link> outGoingLinks;
};


/**
 * Numbers represent neurons in topological order.
 * 
 *   0   1   2   3
 *   |   |\  |  /
 *   |   | \ | /
 *   |   |  \|/
 *   4   |   5
 *    \  |  /|\
 *     \ | / | \
 *      \|/  |  \
 *       6   7   )
 *        \  |  /
 *         \ | /
 *          \|/
 *           8
 */

// constexpr size_t InputSize = 4, OutputSize = 2;
template <size_t InputSize, size_t OutputSize>
class NeuronalNetwork {
private:
  std::vector<Neuron> neurons;
  // new neurons appended at the end, i.e. 
  // neurons[: InputSize] are the inputs (in their original order),
  // neurons[InputSize: InputSize + OutputSize] are the outputs (in their original order),
  // neurons[InputSize + OutputSize: ] are new neurons.

  std::vector<size_t> neuronsTopologicalOrder;
  // in neuronsTopologicalOrder, 
  // the first InputSize elements are inputs, and the last OutputSize elements are the outputs.

  std::vector<std::pair<size_t, size_t>> cachedLinks;

  std::pair<size_t, size_t>* randomLink() {
    size_t randIdx = randomIndex(cachedLinks.size());
    return &cachedLinks[randIdx];
  }

public:
  NeuronalNetwork() {
    neurons.reserve(InputSize + OutputSize);
    neuronsTopologicalOrder.reserve(InputSize + OutputSize);
    cachedLinks.reserve(InputSize * OutputSize);

    for (size_t in = 0; in < InputSize; in++) {
      neurons.push_back(Neuron{ .bias = randomSigned(), .activation = identity });
      neuronsTopologicalOrder.push_back(in);
    }

    for (size_t out = 0; out < OutputSize; out++) {
      neurons.push_back(Neuron{ .bias = randomSigned(), .activation = sigmoid });
      neuronsTopologicalOrder.push_back(InputSize + out);
    }

    for (size_t in = 0; in < InputSize; in++) {
      for (size_t out = 0; out < OutputSize; out++) {
        neurons[in].outGoingLinks.push_back(Link { .toIndex = InputSize + out, .weight = randomSigned() });
        cachedLinks.push_back({ in, InputSize + out });
      }
    }
  }

  // Copy constructor (deep copy)
  NeuronalNetwork(const NeuronalNetwork& other)
    : neurons(other.neurons),
      neuronsTopologicalOrder(other.neuronsTopologicalOrder),
      cachedLinks(other.cachedLinks) { }

  // Move Constructor
  NeuronalNetwork(NeuronalNetwork&& other) noexcept
    : neurons(std::move(other.neurons)),
      neuronsTopologicalOrder(std::move(other.neuronsTopologicalOrder)),
      cachedLinks(std::move(other.cachedLinks)) { }

  // Move assignment operator
  NeuronalNetwork& operator=(NeuronalNetwork&& other) noexcept {
    if (this != &other) {
      neurons = std::move(other.neurons);
      neuronsTopologicalOrder = std::move(other.neuronsTopologicalOrder);
      cachedLinks = std::move(other.cachedLinks);
    }
    return *this;
  }

  // Copy assignment operator (deep copy)
  NeuronalNetwork& operator=(const NeuronalNetwork& other) {
    if (this != &other) {
      neurons = other.neurons;
      neuronsTopologicalOrder = other.neuronsTopologicalOrder;
      cachedLinks = other.cachedLinks;
    }
    return *this;
  }

  std::array<double, OutputSize> forward(std::span<double, InputSize> input) {
    for (size_t i = 0; i < InputSize; i++) {
      neurons[i].value = input[i];
    }

    for (size_t i = InputSize; i < neurons.size(); i++) {
      neurons[i].value = 0;
    }
    
    for (size_t idx : neuronsTopologicalOrder) {
      Neuron& neuron = neurons[idx];
      neuron.value = neuron.activation(neuron.value);

      for (Link& link : neuron.outGoingLinks) {
        neurons[link.toIndex].value += link.weight * neuron.value;
      }
    }

    std::array<double, OutputSize> output;
    for (size_t out = 0; out < OutputSize; out++) {
      output[out] = neurons[InputSize + out].value;
    }
    return output;
  }

  void addRandomNeuron() {
    Neuron newOne = { .bias = randomSigned(), .activation = relu };

    auto* cachedLink = randomLink();
    auto [oldFrom, oldTo] = *cachedLink;

    auto oldLink = std::find_if(
      neurons[oldFrom].outGoingLinks.begin(),
      neurons[oldFrom].outGoingLinks.end(),
      [=](Link& link) { return link.toIndex == oldTo; }
    );

    newOne.outGoingLinks.push_back(Link { .toIndex = oldLink->toIndex, .weight = oldLink->weight });
    oldLink->toIndex = neurons.size();

    auto neuronPos = indexOf(neuronsTopologicalOrder, oldLink->toIndex);
    neuronsTopologicalOrder.insert(neuronPos, neurons.size());

    cachedLink->second = neurons.size();
    cachedLinks.push_back({ neurons.size(), oldTo });

    neurons.push_back(newOne);
  }

  void addRandomLink() {
    size_t fromTopo = randomIndex(neurons.size() - OutputSize);
    size_t toTopo = randomIndex(neurons.size() - InputSize) + InputSize;
    
    if (fromTopo > toTopo) {
      std::swap(fromTopo, toTopo);
    }
    else if (fromTopo == toTopo) {
      (randomBits() & 1) ? (--fromTopo) : (--toTopo);
    }

    size_t from = neuronsTopologicalOrder[fromTopo];
    size_t to = neuronsTopologicalOrder[toTopo];

    neurons[from].outGoingLinks.push_back(Link { .toIndex = to, .weight = randomSigned() });
    cachedLinks.push_back({ from, to });
  }

  void mutateRandomNeuron() {
    size_t randIdx = randomIndex(neurons.size());
    neurons[randIdx].bias += randomNormal() * 0.1;
  }

  void mutateRandomLink() {
    auto* cachedLink = randomLink();
    auto [oldFrom, oldTo] = *cachedLink;

    auto oldLink = std::find_if(
      neurons[oldFrom].outGoingLinks.begin(),
      neurons[oldFrom].outGoingLinks.end(),
      [=](Link& link) { return link.toIndex == oldTo; }
    );
    
    oldLink->weight += randomNormal() * 0.1;
  }

  void applyRandomMutation() {
    constexpr std::array<double, 4> weights = { 0.3, 0.1, 0.4, 0.2 };
    constexpr std::array<double, 4> thresholds = cumsum(weights);

    double random = randomUniform();

    return
      random < thresholds[0] ? addRandomLink() :
      random < thresholds[1] ? addRandomNeuron() :
      random < thresholds[2] ? mutateRandomLink() : 
                               mutateRandomNeuron() ;
  }
};
