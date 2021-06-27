/**
 * @file    parallel_heat_solver.h
 * @author  xlogin00 <xlogin00@stud.fit.vutbr.cz>
 *
 * @brief   Course: PPP 2020/2021 - Project 1
 *
 * @date    2021-MM-DD
 */

#ifndef PARALLEL_HEAT_SOLVER_H
#define PARALLEL_HEAT_SOLVER_H

#include "base_heat_solver.h"

/**
 * @brief The ParallelHeatSolver class implements parallel MPI based heat
 *        equation solver in 2D using 1D and 2D block grid decomposition.
 */
class ParallelHeatSolver : public BaseHeatSolver
{
    //============================================================================//
    //                            *** BEGIN: NOTE ***
    //
    // Modify this class declaration as needed.
    // This class needs to provide at least:
    // - Constructor which passes SimulationProperties and MaterialProperties
    //   to the base class. (see below)
    // - Implementation of RunSolver method. (see below)
    //
    // It is strongly encouraged to define methods and member variables to improve
    // readability of your code!
    //
    //                             *** END: NOTE ***
    //============================================================================//

public:
    /**
     * @brief Constructor - Initializes the solver. This should include things like:
     *        - Construct 1D or 2D grid of tiles.
     *        - Create MPI datatypes used in the simulation.
     *        - Open SEQUENTIAL or PARALLEL HDF5 file.
     *        - Allocate data for local tile.
     *        - Initialize persistent communications?
     *
     * @param simulationProps Parameters of simulation - passed into base class.
     * @param materialProps   Parameters of material - passed into base class.
     */
    ParallelHeatSolver(SimulationProperties &simulationProps, MaterialProperties &materialProps);
    virtual ~ParallelHeatSolver();

    /**
     * @brief Run main simulation loop.
     * @param outResult Output array which is to be filled with computed temperature values.
     *                  The vector is pre-allocated and its size is given by dimensions
     *                  of the input file (edgeSize*edgeSize).
     *                  NOTE: The vector is allocated (and should be used) *ONLY*
     *                        by master process (rank 0 in MPI_COMM_WORLD)!
     */
    virtual void RunSolver(std::vector<float, AlignedAllocator<float>> &outResult);

    void printMatrix(float *matrix, int nRows, int nCols, int rank);

    void mpiPrintf(int who, const char *__restrict__ format, ...);
    void mpiFlush();

protected:
    int m_rank;                     ///< Process rank in global (MPI_COMM_WORLD) communicator.
    int m_size;                     ///< Total number of processes in MPI_COMM_WORLD.
    int globalCols;                 ///< Domain decomposition in X axis.
    int globalRows;                 ///< Domain decomposition in Y axis.
    const unsigned int padding = 2; //< Size of surrounding that we need to count

    unsigned int n;         // m_materialProperties.GetEdgeSize()
    unsigned int tempN;     // m_materialProperties.GetEdgeSize() with padding from left and right
    unsigned int tileCols;  // tile cols size without padding, columns length = tile size X
    unsigned int tileRows;  // tile rows size without padding, rows length = tile size Y
    unsigned int blockCols; // tile cols with padding from both sides
    unsigned int blockRows; // tile rows with padding from both sides

    //hid_t dset_id; // dataset id
    hid_t filespace;
    hid_t memspace;

    float *tile;
    float *newTile;
    int *domainMap;
    float *domainParams;

    bool isModeRMA;       // = m_simulationProperties.IsRunParallelRMA()
    bool isRunSequential; // = m_simulationProperties.IsRunSequential()

    std::vector<float, AlignedAllocator<float>> m_tempArray;
    std::vector<float, AlignedAllocator<float>> m_domainParams;
    std::vector<int, AlignedAllocator<int>> m_domainMap;
    struct RankProfile
    {
        bool isLeftRank;   // is the rank on the left edge
        bool isRightRank;  // is the rank on the right edge
        bool isTopRank;    // is the rank on the top edge
        bool isBottomRank; // is the rank on the bottom edge
    };

    struct RankOffsets
    {
        int startLF;
        int endLF;
        int startTB;
        int endTB;
    };

    RankProfile rankProfile;
    RankOffsets rankOffsets;

    // Root rank types
    MPI_Datatype MPI_COL_MAT_RES;
    MPI_Datatype MPI_COL_MAP_RES;
    MPI_Datatype MPI_COL_TILE_N_RES;

    // All rank types
    MPI_Datatype MPI_ROW_BLOCK;
    MPI_Datatype MPI_ROW_MAP;
    MPI_Datatype MPI_TILE;
    MPI_Datatype MPI_COL_TILE;
    MPI_Datatype MPI_GLOBAL_ROW_BLOCK;
    MPI_Datatype MPI_GLOBAL_ROW_BLOCK_INT;
    MPI_Datatype MPI_GLOBAL_COL_BLOCK_RES;
    MPI_Datatype MPI_GLOBAL_COL_BLOCK;

    // Windows for RMA mode
    MPI_Win winNewTile;
    MPI_Win winTile;

    AutoHandle<hid_t> m_fileHandle;

    /**
     * @brief Add padding to top, left, bottom, right of the array of float
     * @param data [IN] Array to which edge will be added 
     * @param size size row/col size
     * @param padding size of padding
     * @param [OUT] Output array with borders
     */

    void AddPaddingToArray(float *data, int size, int padding, float *newData);
    /**
     * @brief Add padding to top, left, bottom, right of the array of int.
     * @param data [IN] Array to which edge will be added.
     * @param size Row/col size.
     * @param padding Size of padding.
     * @param [OUT] Output array with borders.
     */
    void AddPaddingToIntArray(int *data, int size, int padding, int *newData);

    /**
     * @brief Get size of the communicator.
     * @param [in] comm - actual communicator.
     * @return number of ranks within the comm.
     */
    int mpiGetCommSize(const MPI_Comm &comm);

    /**
     * @brief Create and commits new vector type.
     * @param count Size of vector (number fo elements).
     * @param stride Vector stride.
     * @param oldtype Type from which the new type will be created.
     * @param newtype New desired type vector.
     * @param size Resized size.
     */
    void CreateResVector(int count, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype, unsigned long size);

    /**
     * @brief Create array of m_size filled with offset number at each position (used at Scatter_v).
     * @param offset Number that will be put at each position.
     */
    int *GetSendCounts(int offset);

    /**
     * @brief Count displacement for each tile taking into account global displacement.
     * @param n Tile row size.
     */
    int *GetDisplacementCounts(int n);

    /**
     * @brief Compute middle temperature.
     * @param middleColAvgTemp [OUT] Output middle temperature.
     * @param comm Comunicator that will be used to compute the middle temperature.
     */
    void ComputeMiddleColAvgTemp(float *middleColAvgTemp, const MPI_Comm &comm);

    /**
     * @brief Init information about rank position in the global tile.
     */
    void InitRankProfile();

    /**
     * @brief Init all class state variables (as tile, new tile, domainParams etc.).
     */
    void InitTileVariables();

    /**
     * @brief Init beggining and end of tile computation according to rank position (left, rigt, top).
     */
    void InitRankOffsets();

    /**
     * @brief Init values of material arrays (domainParams, domainMap, tempArray) - add padding to them.
     */
    void InitWorkingArrays();

    /**
     * @brief Init new types for root rank.
     */
    void InitRootRankTypes();

    /**
     * @brief Init types for all ranks.
     */
    void InitRankTypes();

    /**
     * @brief Init dataset, memspace, filespace, hd5file for paralel IO.
     */
    void InitParallelIO();

    /**
     * @brief Writes tile data from all ranks to file.
     */
    void H5WriteTileToFile(int iteration);

    /**
     * @brief Scatter values of working material arrays to all process.
     * @param sendCountsTempN Integer array specifying the number of elements to send to each processor 
     * @param displacementsTempN Integer array. Entry i specifies the displacement (relative to sendbuf from which to take the outgoing data to process i).
     */
    void
    ScatterValues(int *sendCountsTempN, int *displacementsTempN);
};

#endif // PARALLEL_HEAT_SOLVER_H
