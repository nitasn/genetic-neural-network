#pragma once

#include "neuroal-network.hpp"
#include "utils.hpp"
#include <limits> // for NaN

constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

constexpr size_t InputSize = 4, OutputSize = 2;

typedef NeuronalNetwork<InputSize, OutputSize> NN;

/**
 * fitness must be strictly positive!
 * todo: or it it? (vsauce music)
 */
using FitnessFunction = double (*)(NeuronalNetwork<InputSize, OutputSize>&);

class Tournament {
private:
  std::vector<NN> population;
  FitnessFunction fitnessFn;

  void logGenerationFitness(const std::vector<std::pair<size_t, double>>& idxAndScore) {
    std::vector<double> scores = mapVector<std::pair<size_t, double>, double>
      (idxAndScore, [](const auto& p) { return p.second; } );

    printArrayLike(scores);
  }

public:
  const size_t PopulationSize;
  const size_t EliteSize;

  Tournament(int popSize, double eliteRatio, FitnessFunction fitnessFn) 
    : population(popSize),
      fitnessFn(fitnessFn),
      PopulationSize(popSize),
      EliteSize(std::ceil(eliteRatio * popSize)) { }
  
  void advanceOneGeneration() {
    std::vector<std::pair<size_t, double>> idxAndScore;
    idxAndScore.reserve(PopulationSize);

    for (size_t idx = 0; idx < PopulationSize; idx++) {
      idxAndScore.emplace_back(idx, NaN);
    }

    // this should be parallelized
    for (size_t idx = 0; idx < PopulationSize; idx++) {
      NN& nn = population[idx];
      idxAndScore[idx].second = fitnessFn(nn);
    }

    // sort by the (nn_idx, nn_fitness) pairs by fitness, best to worst
    std::sort(idxAndScore.begin(), idxAndScore.end(), [](const auto& a, const auto& b) {
      return a.second > b.second;
    });

    logGenerationFitness(idxAndScore);

    std::vector<NN> nextGeneration;
    nextGeneration.reserve(PopulationSize);

    // add elite to next generation
    for (size_t i = 0; i < EliteSize; i++) {
      size_t idx = idxAndScore[i].first;
      nextGeneration.push_back(population[idx]);
    }

    double total_scores = 0;
    for (const auto& [idx, score] : idxAndScore) {
      total_scores += score;
    }

    std::vector<double> cumsumWeights;
    cumsumWeights.reserve(PopulationSize);

    cumsumWeights.push_back(idxAndScore[0].second / total_scores);
    for (size_t i = 1; i < PopulationSize; i++) {
      double normalizedScore = idxAndScore[i].second / total_scores;
      cumsumWeights.push_back(cumsumWeights[i - 1] + normalizedScore);
    }

    while (nextGeneration.size() < PopulationSize) {
      double random = randomUniform();
      auto it = std::lower_bound(cumsumWeights.begin(), cumsumWeights.end(), random);
      size_t sortedIdx = it - cumsumWeights.begin();
      size_t nnIndex = idxAndScore[sortedIdx].first;

      NN clone = population[nnIndex];
      clone.applyRandomMutation();
      nextGeneration.push_back(clone);
    }

    population = nextGeneration;
  }
};