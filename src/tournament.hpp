#pragma once

#include "neuroal-network.hpp"
#include "utils.hpp"
#include <limits> // for NaN

constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

constexpr size_t InputSize = 4, OutputSize = 2;

typedef NeuronalNetwork<InputSize, OutputSize> NN;

using FitnessFunction = double (*)(NeuronalNetwork<InputSize, OutputSize>&);

class Tournament {
private:
  std::vector<NN> population;
  FitnessFunction fitnessFn;

public:
  const int PopulationSize;
  const int ElitismSize;

  Tournament(int popSize, double elitismRatio, FitnessFunction fitnessFn) 
    : population(popSize),
      fitnessFn(fitnessFn),
      PopulationSize(popSize),
      ElitismSize(std::ceil(elitismRatio * popSize)) { }
  
  void advanceOneGeneration() {
    std::vector<std::pair<size_t, double>> idx2score;
    idx2score.reserve(PopulationSize);

    for (size_t idx = 0; idx < PopulationSize; idx++) {
      idx2score.emplace_back(idx, NaN);
    }

    // this should be parallelized
    for (size_t idx = 0; idx < PopulationSize; idx++) {
      NN& nn = population[idx];
      idx2score[idx].second = fitnessFn(nn);
    }

    // sort by fitness in descending order
    std::sort(idx2score.begin(), idx2score.end(), [](const auto& a, const auto& b) {
      return a.second > b.second;
    });

    std::vector<std::pair<size_t, double>> elite(idx2score.begin(), idx2score.begin() + ElitismSize);

    for (auto& [idx, score] : elite) {
      print("index:", idx, '\t', "score:", score);
    }
  }
};