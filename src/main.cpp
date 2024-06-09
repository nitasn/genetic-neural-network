#include "utils.hpp"
#include "neuroal-network.hpp"

int main() {
  // for (size_t i = 0; i < 1000000; i++) {

  //   NeuronalNetwork<4, 2> nn1;

  //   std::array<double, 4> input = { 0.1, -0.2, 0.3, -0.4 };
  //   auto x = nn1.forward(input);
    
  //   NeuronalNetwork<4, 2> nn2 = nn1;
  //   nn1.applyRandomMutation();
    
  //   auto z = nn2.forward(input);

  //   if (!arraysEqual(x, z)) throw std::invalid_argument("oh no !arraysEqual(x, z)");
  // }

  NeuronalNetwork<4, 2> nn1;

  nn1.neurons[0].outGoingLinks[0].toIndex = 999;

  NeuronalNetwork<4, 2> nn2 = nn1;

  nn2.neurons[0].outGoingLinks[0].toIndex = 55;

  debug(nn1.neurons[0].outGoingLinks[0].toIndex);
  debug(nn2.neurons[0].outGoingLinks[0].toIndex);
}