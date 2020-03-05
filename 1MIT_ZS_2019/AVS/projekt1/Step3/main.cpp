/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xtesar36
 */

#include <iostream>
#include <hdf5.h>
#include "neuron.h"

//PAPI is used for trivial measurements but has impact on optimalisation report clarity.
//It can be removed with cmake options -DWITH_PAPI=0
#ifdef WITH_PAPI
#include "papi_cntr.h"
#endif

/**
 * @brief Calculate output of each neuron in the layer
 * @param inputSize     - number of layer's inputs
 * @param neuronCount   - number of neurons
 * @param input         - pointer to neuron inputs (inputSize)
 * @param weight        - weights, a vector for each neuron (neuronCount * inputSize)
 * @param bias          - bias for each neuron (inputSize)
 * @param output        - pointer to store the results
 */
void evaluateLayer(
  size_t inputSize,
  size_t neuronCount,
  const float* input,
  const float* weight,
  const float* bias,
  float* output
);

/**
 * @brief Reads float data from the HDF5 dataset
 * @param datasetName   - dataset name
 * @param file          - descriptor to a file opened for reading
 * @param buffer        - pointer to read buffer
 */
void simpleReadDataset(const char* datasetName, hid_t file, void* buffer);

/**
 * @brief Creates new matrix and fill it with data transposed to the input data.
 *        New/modified pointer is returned and the old one is freed.
 *
 * @param data    - pointer to data which should be transposed
 * @param dimX    - size of the data in the x dimension before the transpose
 * @param dimY    - size of the data in the y dimension before the transpose
 */
void transpose2D(float*& data, size_t dimX, size_t dimY);

/**
 * @brief Allocates memory as array of floats for given number of elements
 * @param numberOfElements  - number of elements
 * @return pointer to allocated memory
 */
//TODO:Step3 - modify allocation
float* allocateMemory(size_t numberOfElements)
{
  return (float*) malloc(numberOfElements * sizeof(float));
}


int main(int argc, char* argv[])
{
  // Check arguments
  if(argc != 4)
  {
    throw std::runtime_error("Expecting three arguments. Path to the network file, "
                             "to the data file and to the output file.");
  }

  std::string networkDataFile(argv[1]);
  std::string testingDataFile(argv[2]);
  std::string outputDataFile(argv[3]);


  #ifdef WITH_PAPI
  printf("Compiled with PAPI\n");
  PapiCounterList papi_routines;
  papi_routines.AddRoutine("network");
  #else
  printf("Compiled without PAPI\n");
  #endif


  // The input of the network is an image, which is processed by three fully connected
  // neuron layers.
  // The first layer has 784 inputs corresponding to image pixels and 512 outputs,
  // the second layer has both 512 inputs and outputs, and the last layer has 512 inputs
  // and 10 classification outputs correspoding to the numbers 0 - 9.
  // Each output describes a "confidence" of the network that the input picture contains
  // the respective number.
  const size_t imagePixels = 784;
  const size_t layerSize = 512;
  const size_t outputSize = 10;

  // Allocate memory for network data
  float* weight1 = allocateMemory(imagePixels * layerSize);
  float* weight2 = allocateMemory(layerSize * layerSize);
  float* weight3 = allocateMemory(layerSize * outputSize);
  float* bias1 = allocateMemory(layerSize);
  float* bias2 = allocateMemory(layerSize);
  float* bias3 = allocateMemory(outputSize);

  // Read the network from file
  hid_t networkFile = H5Fopen(networkDataFile.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  simpleReadDataset("/model_weights/dense_1/dense_1/kernel:0", networkFile, weight1);
  simpleReadDataset("/model_weights/dense_2/dense_2/kernel:0", networkFile, weight2);
  simpleReadDataset("/model_weights/dense_3/dense_3/kernel:0", networkFile, weight3);
  simpleReadDataset("/model_weights/dense_1/dense_1/bias:0", networkFile, bias1);
  simpleReadDataset("/model_weights/dense_2/dense_2/bias:0", networkFile, bias2);
  simpleReadDataset("/model_weights/dense_3/dense_3/bias:0", networkFile, bias3);

  // We got all the data, file is no longer needed
  H5Fclose(networkFile);

  // Read the input, we take only one/N pictures instead all dataset, so we need to specify memspace and dataspace

  // Get size of the tested dataset
  size_t imageCount;
  hid_t dataFile = H5Fopen(testingDataFile.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  // Open dataset
  hid_t dataset = H5Dopen(dataFile, "/image_count", H5P_DEFAULT);

  hid_t status = H5Dread(dataset, H5T_NATIVE_ULONG, H5S_ALL,
                         H5S_ALL, H5P_DEFAULT, &imageCount);

  printf("%zu\n", imageCount);

  if (status < 0)
  {
    throw std::runtime_error("Could not read the image count.");
  }
  H5Dclose(dataset);


  // Prepare space for images
  float* input = allocateMemory(imageCount * imagePixels);

  simpleReadDataset("/x_test", dataFile, input);

  H5Fclose(dataFile);

//####################################################################################################################//
//                                     !!! Do not edit main() above !!!                                               //
//####################################################################################################################//

  // Memory for the outputs from each layer. First two are reused for each input picture.
  // The last output holds the classification results of all input pictures.
  float* output1 = allocateMemory(layerSize*imageCount);
  float* output2 = allocateMemory(layerSize*imageCount);
  float* output3 = allocateMemory(imageCount * outputSize);

  #ifdef WITH_PAPI
  papi_routines["network"].Start();
  #endif

  transpose2D(weight1, layerSize, imagePixels);
  transpose2D(weight2, layerSize, layerSize);
  transpose2D(weight3, outputSize, layerSize);

  for (size_t i = 0; i < imageCount; i++)
  {
    // The fist layer 784 -> 512
    evaluateLayer(imagePixels, layerSize, &input[i * imagePixels], weight1, bias1, &output1[i*layerSize]);
  }
 for (size_t i = 0; i < imageCount; i++)
  {
    // The second layer 512 -> 512
    evaluateLayer(layerSize, layerSize, &output1[i*layerSize], weight2, bias2, &output2[layerSize*i]);
  }
 
 for (size_t i = 0; i < imageCount; i++){
    // The third layer 512 -> 10
    evaluateLayer(layerSize, outputSize, &output2[i*layerSize], weight3, bias3, &output3[i*outputSize]);
 }
  #ifdef WITH_PAPI
  papi_routines["network"].Stop();
  papi_routines.PrintScreen();
  #endif

  // Print some of the results for a quick functionality check
  for (size_t image = 0; image < 5; image++)
  {
    printf("Image %zu: ", image);

    for (size_t i = 0; i < outputSize; i++)
    {
      printf("%zu: %f  ", i, output3[image*outputSize+i]);
    }
    std::cout << std::endl;
  }

//####################################################################################################################//
//                                     !!! Do not edit main() below !!!                                               //
//####################################################################################################################//

  // Store results for testing

  // Output dataset size
  hsize_t     dims[2] = {imageCount, outputSize};

  // Create a new file using the default properties. If file already exist it will be erased and rewritten.
  hid_t file = H5Fcreate(outputDataFile.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Create dataspace. Maximal dimensions are same as the current.
  hid_t space = H5Screate_simple (2, dims, dims);

  hid_t dset = H5Dcreate (file, "/output_data", H5T_NATIVE_FLOAT, space, H5P_DEFAULT,
                    H5P_DEFAULT, H5P_DEFAULT);

  // Write the data to the dataset.
  status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, output3);
  if (status < 0)
  {
    throw std::runtime_error("Could not store the output dataset.");
  }

  H5Dclose(dset);
  H5Sclose(space);
  H5Fclose(file);

  free(output1);
  free(output2);
  free(output3);

  free(input);

  free(bias1);
  free(bias2);
  free(bias3);

  free(weight1);
  free(weight2);
  free(weight3);

  return 0;
}

/**
 * @brief Reads float data from the HDF5 dataset
 * @param datasetName   - dataset name
 * @param file          - descriptor to file opened for read
 * @param buffer        - pointer to read buffer
 */
void simpleReadDataset(const char* datasetName, hid_t file, void* buffer)
{

  hid_t dataset = H5Dopen(file, datasetName, H5P_DEFAULT);
  hid_t status = H5Dread(dataset, H5T_NATIVE_FLOAT, H5S_ALL,
                         H5S_ALL, H5P_DEFAULT, buffer);

  if (status < 0)
  {
    throw std::runtime_error("Could not read the dataset");
  }
  H5Dclose(dataset);
}

/**
 * @brief Calculate output of each neuron in the layer
 * @param inputSize     - number of layer's inputs
 * @param neuronCount   - number of neurons
 * @param input         - pointer to inputs
 * @param weight        - weights, a vector for each neuron
 * @param bias          - bias for each neuron
 * @param output        - pointer to store results
 */
void evaluateLayer(
  const size_t inputSize,
  const size_t neuronCount,
  const float* input,
  const float* weight,
  const float* bias,
  float* output
)
{
  // For each neuron in the layer calculate its output
  //TODO: Step1 - enforce vectorization of this loop

  for (size_t i = 0; i < neuronCount; i++)
  {
    //TODO: Step2 - Modify "evalNeuron" to work with transposed weights
    //output[i] = evalNeuron(inputSize, neuronCount, input, weight, bias[i], i);
    output[i] = evalNeuron(inputSize, input, &weight[i*inputSize], bias[i]);
  }
}

/**
 * @brief Creates new matrix and fill it with data transposed to the input data.
 *        New/modified pointer is returned and the old one is freed.
 *
 * @param data    Pointer to data which should be transposed
 * @param dimX    Size of the x dimension
 * @param dimY    Size pf the y dimension
 */
// TODO:Step2 - Use this function to provide more effective memory access patern
void transpose2D(float*& data, const size_t dimX, const size_t dimY)
{
  // store the original pointer
  float* tmp = data;
  // create new matrix with same size
  data = allocateMemory(dimX * dimY);

  // copy data with swapped x and y indices
  for (size_t x = 0; x < dimX; x++)
  {
    for (size_t y = 0; y < dimY; y++)
    {
      data[x * dimY + y] = tmp[y * dimX + x];
    }
  }
  // release the original matrix
  free(tmp);
}

