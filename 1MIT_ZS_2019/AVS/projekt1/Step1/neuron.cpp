/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xtesar36
 */

#include <cstdlib>
#include "neuron.h"

float evalNeuron(
        size_t inputSize,
        size_t neuronCount,
        const float* input,
        const float* weights,
        float bias,
        size_t neuronId
)
{

  float suma = bias;
  int weightIndex = 0;

  for (unsigned int i = 0; i < inputSize; i++) {
    weightIndex = i* neuronCount + neuronId;	
    suma += input[i] * weights[weightIndex]; // hodnota jednoho neuronu
  }

  if ( suma >= 0.0 ){
    return suma;
  }
  return 0.0;
}
