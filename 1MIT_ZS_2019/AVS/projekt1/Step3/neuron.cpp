/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xtesar36
 */

#include <cstdlib>
#include "neuron.h"

float evalNeuron(
        size_t inputSize,
        const float* input,
        const float* weights,
        float bias
)
{
  float suma = bias;
  
  #pragma omp simd reduction(+:suma)
  for (unsigned int i = 0; i < inputSize; i++) {
    suma += input[i] * weights[i]; // hodnota jednoho neuronu
  }

  if ( suma >= 0.0 ){
    return suma;
  }
  return 0.0;
}
