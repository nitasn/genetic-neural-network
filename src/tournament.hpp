#include "neuroal-network.hpp"

constexpr size_t InputSize = 4, OutputSize = 2;

class Tournament {
private:
  const int PopulationSize;
  const float ElitismRatio;

  double (*fitnessFn)(const NeuronalNetwork<InputSize, OutputSize>&);

public:
  Tournament(int populationSize, float elitismRatio) 
    : PopulationSize(populationSize), ElitismRatio(elitismRatio) { }
  
  void advanceOneGeneration() {
    
  }
};