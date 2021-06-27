
/**
 * BIN - cellular automata - majority problem
 * 
 * @author Alena Tesařová (xtesar36)
 * @date 16.4.2021
 **/

using namespace std;

/**
 * @brief Generate random number in range [low, high]
 * @author (c) MICHAL BIDLO, 2011 (VZOROVA IMPLEMENTACE JEDNODUCHEHO GENETICKEHO ALGORITMU) 
 **/
UINT urandom(int low, int high)
{
    return rand() % (high - low + 1) + low;
}
/**
 * @brief Crossover 2 parents and get 2 crossed children
 * @author (c) MICHAL BIDLO, 2011 (VZOROVA IMPLEMENTACE JEDNODUCHEHO GENETICKEHO ALGORITMU) 
 **/
void crossover(GA_chromosome *parent1, GA_chromosome *parent2,
               GA_chromosome *child1, GA_chromosome *child2)
{
    // zde standardni jednobodove krizeni
    UINT cpoint = urandom(1, RULES_LENGTH - 1);

    for (UINT i = 0; i < RULES_LENGTH; i++)
    {
        if (i < cpoint)
        {
            child1->chromosome[i] = parent1->chromosome[i];
            child2->chromosome[i] = parent2->chromosome[i];
        }
        else
        {
            child1->chromosome[i] = parent2->chromosome[i];
            child2->chromosome[i] = parent1->chromosome[i];
        }
    }
}

/**
 * @brief Mutation with _pmut probability
 * @author (c) MICHAL BIDLO, 2011 (VZOROVA IMPLEMENTACE JEDNODUCHEHO GENETICKEHO ALGORITMU) 
 **/
bool mutator(GA_chromosome *genome, UINT _pmut)
{
    if (urandom(0, unit) <= _pmut) // mutace s pravdepodobnosti _pmut
    {
        for (UINT i = 0; i < MUTAGENES; i++)
        {
            UINT g = urandom(0, RULES_LENGTH - 1);
            genome->chromosome[g] = 1 - genome->chromosome[g]; // gene inversion
        }

        return 1; // probehla-li mutace, vratim true...
    }

    return 0; // ...jinak vracim false
}
/**
 * @brief Initialize array with zeros and ones
 **/
void initialize(GA_chromosome *genome)
{
    for (int i = 0; i < RULES_LENGTH; i++)
    {
        genome->chromosome[i] = urandom(0, 1);
    }
    genome->fitness = 0;
}

/**
 * @brief Print rules in JSON format to web browser application
 * @param out output
 * @param rules rules to be printed in data field of json
 **/
void printRules(ostream &out, int *rules, int length)
{
    out << "JSON data: " << endl
        << "{"
        << "\"neighborhood\":" << NEIGHBORHOOD << "; "
        << "\"cellular_length\":" << CONFIG_LENGTH << "; "
        << "\"steps\":" << STEPS << "; ";
    out << "\""
        << "data"
        << "\": [";

    for (int i = 0; i < length; i++)
    {
        out << rules[i];
        if (i != length - 1)
        {
            out << "; ";
        }
    }
    out << "]}" << endl;
}

/**
 * @brief Print one row of specific length
 * @param arr array that we want to print
 * @param lengthArr the length of array
 **/
void printRow(int *arr, int lengthArr)
{
    cout << "[";
    for (int i = 0; i < lengthArr; i++)
    {
        cout << arr[i];
        if (i != lengthArr - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}
/**
 * @brief Compute major value in config array
 * @return Return either one or zero
 **/
int computeMajorValue(int *configArr)
{
    int counterOnes = 0;
    int counterZeros = 0;
    for (int i = 0; i < CONFIG_LENGTH; i++)
    {
        if (configArr[i])
        {
            counterOnes++;
        }
        else
        {
            counterZeros++;
        }
    }
    if (counterOnes > counterZeros)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Compare 2 arrays if they are the same
 * @param in first array
 * @param out second array
 * @param length the length of arrays (must be the same size)
 **/
bool isSameArray(int *in, int *out, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (in[i] != out[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Swap pointers
 **/
void swapPointers(GA_chromosome **first, GA_chromosome **second)
{
    GA_chromosome *temp = *first;
    *first = *second;
    *second = temp;
}

void save_statistics(ostream &StatsFile, GA_chromosome best_chromosome, UINT best_fitness_generation)
{
    StatsFile << NEIGHBORHOOD << ";"
              << NUM_CONFIG << ";"
              << CONFIG_LENGTH << ";"
              << STEPS << ";"
              << GENERATIONS << ";"
              << POPSIZE << ";"
              << best_chromosome.fitness << ";"
              << MAX_FITNESS << ";"
              << ((float)best_chromosome.fitness / (float)MAX_FITNESS) * 100 << ";"
              << best_chromosome.best_step << ";"
              << best_fitness_generation << ";";
    printRules(StatsFile, best_chromosome.chromosome, RULES_LENGTH);
}
