#define NEIGHBORHOOD 2   // number of cells from left and right of the main cell that will be computed with current cell
#define NUM_CONFIG 2000  // number of random configuration that will be used to somlue fitness, space 2^cellular_length
#define GENERATIONS 30   // number of generations
#define POPSIZE 1000     // size of population
#define STEPS 20         // number of steps for cullular automata to count fitness
#define CONFIG_LENGTH 11 // length of input configuration
#define MUTAGENES 2      // number of mutations
#define PCROSS 80        // probability of crossover
#define PMUT 2           // probability of mutation

#define TOUR 4 // number of individuals in turnament selection

typedef unsigned int UINT;

const unsigned int unit = 100; // unit
UINT MAX_FITNESS = CONFIG_LENGTH * NUM_CONFIG;
const int RULES_LENGTH = pow(2, 2 * NEIGHBORHOOD + 1); // number of rules (combination of zeros and ones in neighborhood))
int statistics[2];                                     // count major black and white for statistics

typedef struct
{
    int chromosome[RULES_LENGTH]; // chromosone content = rules in our case
    int best_step;                // the step where the fitness was calculated
    UINT fitness;                 // fitness of individual
    bool evaluate;                // if it s necessary to evaluate fitness again
} GA_chromosome;
