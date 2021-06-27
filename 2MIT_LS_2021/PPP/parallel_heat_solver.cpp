/**
 * @file    parallel_heat_solver.cpp
 * @author  xtesar36 <xtesar36@stud.fit.vutbr.cz>
 *
 * @brief   Course: PPP 2020/2021 - Project 1
 *
 * @date    2021-04-23
 */

#include "parallel_heat_solver.h"

#include <unistd.h>
#include <math.h>

using namespace std;

//============================================================================//
//                            *** BEGIN: NOTE ***
//
// Implement methods of your ParallelHeatSolver class here.
// Freely modify any existing code in ***THIS FILE*** as only stubs are provided
// to allow code to compile.
//
//                             *** END: NOTE ***
//============================================================================//

void ParallelHeatSolver::InitTileVariables()
{
    n = m_materialProperties.GetEdgeSize();
    tempN = n + 2 * padding;            // padding from left and right
    tileCols = n / globalCols;          // columns length = tile size X
    tileRows = n / globalRows;          // rows length = tile size Y
    blockCols = tileCols + 2 * padding; // tile cols with padding from both sides
    blockRows = tileRows + 2 * padding; // tile rows with padding from both sides
    isModeRMA = m_simulationProperties.IsRunParallelRMA();
    isRunSequential = !m_simulationProperties.IsUseParallelIO();

    tile = new float[blockRows * blockCols]();    // old tile
    newTile = new float[blockRows * blockCols](); // updated tile used in computation

    // We will add borders of padding size to all parameters (it will be easier job to work with extended arrays)
    domainMap = new int[blockRows * blockCols];
    domainParams = new float[blockRows * blockCols];
}
ParallelHeatSolver::~ParallelHeatSolver()
{
    MPI_Type_free(&MPI_ROW_BLOCK);
    if (m_rank == 0)
    {
        MPI_Type_free(&MPI_COL_MAT_RES);
    }
    if (isModeRMA)
    {
        MPI_Win_free(&winNewTile);
        MPI_Win_free(&winTile);
    }

    //Free block of rows
    delete[] tile;
    delete[] newTile;
}

void ParallelHeatSolver::AddPaddingToArray(float *data, int size, int padding, float *newData)
{
    int newRowSize = size + (padding * 2); // padding is from left and right
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            newData[(i + padding) * newRowSize + (j + padding)] = data[(size * i) + j];
        }
    }
}

void ParallelHeatSolver::AddPaddingToIntArray(int *data, int size, int padding, int *newData)
{
    int newRowSize = size + (padding * 2); // padding is from left and right
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            newData[(i + padding) * newRowSize + (j + padding)] = data[(size * i) + j];
        }
    }
}

//-----------------------------

/**
 * Get size of the communicator.
 * @param [in] comm - actual communicator.
 * @return number of ranks within the comm.
 */
int ParallelHeatSolver::mpiGetCommSize(const MPI_Comm &comm)
{
    int size = -1;
    MPI_Comm_size(comm, &size);
    return size;
} // end of mpiGetCommSize

/**
     * @brief Creates and commits new vector type.
     * @param count Size of vector (number fo elements).
     * @param stride Vector stride.
     * @param oldtype Type from which the new type will be created.
     * @param newtype New desired type vector.
     * @param size Resized size.
     */
void ParallelHeatSolver::CreateResVector(int count, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype, unsigned long size)
{
    MPI_Datatype tempType;
    MPI_Type_vector(count, 1, stride, oldtype, &tempType);
    MPI_Type_commit(&tempType);
    MPI_Type_create_resized(tempType, 0, 1 * size, newtype);
    MPI_Type_commit(newtype);
} // end of CreateResVector

/**
 * @brief Creates array of m_size filled with offset number at each position (used at Scatter_v).
 * @param offset Number that will be put at each position.
 */
int *ParallelHeatSolver::GetSendCounts(int offset)
{
    int *sendCounts = new int[m_size];
    for (int i = 0; i < m_size; i++)
    {
        sendCounts[i] = offset;
    }
    return sendCounts;
} // end of GetSendCounts

/**
     * @brief Counts displacement for each tile taking into account global displacement.
     * @param n Tile row size.
     */
int *ParallelHeatSolver::GetDisplacementCounts(int n)
{
    int *displacements = new int[m_size];
    displacements[0] = 0;
    int poc = 0;
    for (int i = 0; i < globalRows; i++)
    {
        for (int j = 0; j < globalCols; j++)
        {

            displacements[poc] = (j * tileCols) + (i * tileRows * n);
            poc++;
        }
    }
    return displacements;
} // end of GetDisplacementCounts

/**
 * @brief Init beggining and end of tile computation according to rank position (left, rigt, top).
 */
void ParallelHeatSolver::InitRankOffsets()
{

    rankOffsets.startLF = 2;
    rankOffsets.endLF = blockRows - 2;
    rankOffsets.startTB = 2;
    rankOffsets.endTB = blockCols - 2;
    if (rankProfile.isBottomRank)
    {
        rankOffsets.endLF = blockRows - 4;
    }
    if (rankProfile.isTopRank)
    {
        rankOffsets.startLF = 4;
    }
    // mozna bude potreba prehodit
    if (rankProfile.isLeftRank)
    {
        rankOffsets.startTB = 4; // starting computation from fourth row
    }
    if (rankProfile.isRightRank)
    {
        rankOffsets.endTB = blockCols - 4;
    }
    if (!rankProfile.isLeftRank && !rankProfile.isRightRank && !rankProfile.isTopRank && !rankProfile.isBottomRank)
    {
        // Avoid duplicating calculation
        rankOffsets.startTB = 4;
        rankOffsets.endTB = blockCols - 4;
    }
}

/**
 * @brief Init information about rank position in the global tile.
 */
void ParallelHeatSolver::InitRankProfile()
{
    rankProfile.isLeftRank = m_rank % globalCols == 0;
    rankProfile.isRightRank = m_rank % globalCols == globalCols - 1;
    rankProfile.isTopRank = m_rank < globalCols;
    rankProfile.isBottomRank = m_rank >= m_size - globalCols;
}

/**
 * @brief Init values of material arrays (domainParams, domainMap, tempArray) - add padding to them.
 */
void ParallelHeatSolver::InitWorkingArrays()
{
    AddPaddingToArray(m_materialProperties.GetInitTemp().data(), n, padding, m_tempArray.data());
    AddPaddingToArray(m_materialProperties.GetDomainParams().data(), n, padding, m_domainParams.data());
    AddPaddingToIntArray(m_materialProperties.GetDomainMap().data(), n, padding, m_domainMap.data());
}

/**
 * @brief Init new types for root rank.
 */
void ParallelHeatSolver::InitRootRankTypes()
{
    CreateResVector(blockRows, tempN, MPI_FLOAT, &MPI_COL_MAT_RES, sizeof(float)); //Create type for one column in matrix (with padding)  of type float
    CreateResVector(blockRows, tempN, MPI_INT, &MPI_COL_MAP_RES, sizeof(int));     // Create type for one column in matrix (with padding) of type int
    CreateResVector(tileRows, n, MPI_FLOAT, &MPI_COL_TILE_N_RES, sizeof(float));
}
void ParallelHeatSolver::InitRankTypes()
{

    MPI_Type_vector(blockCols, 2, blockRows, MPI_FLOAT, &MPI_COL_TILE);
    MPI_Type_commit(&MPI_COL_TILE);

    int *sizeTile = new int[2];
    sizeTile[0] = tileRows;

    int *size = new int[2];
    size[0] = blockRows;

    int *start = new int[2];
    start[0] = padding;
    // Create 1d subarray that extracts inner part of matrix (part without borders of padding size)
    MPI_Type_create_subarray(1, size, sizeTile, start, MPI_ORDER_C, MPI_FLOAT, &MPI_TILE);
    MPI_Type_commit(&MPI_TILE);

    // Create type for one row in tile
    MPI_Type_contiguous(blockRows, MPI_FLOAT, &MPI_ROW_BLOCK);
    MPI_Type_contiguous(blockRows, MPI_INT, &MPI_ROW_MAP);

    MPI_Type_commit(&MPI_ROW_BLOCK);
    MPI_Type_commit(&MPI_ROW_MAP);
} // end of InitRankTypes

/**
 * @brief Scatter values of working material arrays to all process.
 * @param sendCountsTempN Integer array specifying the number of elements to send to each processor 
 * @param displacementsTempN Integer array. Entry i specifies the displacement (relative to sendbuf from which to take the outgoing data to process i).
 */
void ParallelHeatSolver::ScatterValues(int *sendCountsTempN, int *displacementsTempN)
{

    MPI_Scatterv(m_tempArray.data(), sendCountsTempN, displacementsTempN, MPI_COL_MAT_RES, tile, blockCols, MPI_ROW_BLOCK,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(m_tempArray.data(), sendCountsTempN, displacementsTempN, MPI_COL_MAT_RES, newTile, blockCols, MPI_ROW_BLOCK,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(m_domainParams.data(), sendCountsTempN, displacementsTempN, MPI_COL_MAT_RES, domainParams, blockCols, MPI_ROW_BLOCK,
                 0, MPI_COMM_WORLD);

    MPI_Scatterv(m_domainMap.data(), sendCountsTempN, displacementsTempN, MPI_COL_MAP_RES, domainMap, blockCols, MPI_ROW_BLOCK,
                 0, MPI_COMM_WORLD);
} // end of ScatterValues

/**
 * @brief Init dataset, memspace, filespace, hd5file for paralel IO.
 */
void ParallelHeatSolver::InitParallelIO()
{
    // Declare an HDF5 file.
    hid_t plist;
    hid_t propertyList = H5Pcreate(H5P_FILE_ACCESS);
    const char *datasetname = "Dataset-1";

    // Create a property list to open the file using MPI-IO in the MPI_COMM_WORLD communicator.
    H5Pset_fapl_mpio(propertyList, MPI_COMM_WORLD, MPI_INFO_NULL);

    m_fileHandle.Set(H5Fcreate(m_simulationProperties.GetOutputFileName("par").c_str(),
                               H5F_ACC_TRUNC, H5P_DEFAULT, propertyList),
                     H5Fclose);
    // Close file access list
    H5Pclose(propertyList);

    //  Create file space - a 2D matrix [n][n]
    hsize_t dimsf[] = {hsize_t(n), hsize_t(n)};
    //  Create mem space  - a 2D matrix [blockCols][blockRows] mapped on 1D array tile/newTile.
    hsize_t mem[] = {hsize_t(blockCols), hsize_t(blockRows)};
    hsize_t datasetRank = 2; // 2d
    filespace = H5Screate_simple(datasetRank, dimsf, nullptr);
    memspace = H5Screate_simple(datasetRank, mem, nullptr);
}

/**
 * @brief Writes tile data from all ranks to file.
 */
void ParallelHeatSolver::H5WriteTileToFile(int iteration)
{
    // Create XFER property list and set Collective IO.
    hid_t propertyListXfer = H5Pcreate(H5P_DATASET_XFER);

    // Write data into the dataset
    H5Pset_dxpl_mpio(propertyListXfer, H5FD_MPIO_COLLECTIVE);

    int startFileCurrent = (m_rank % globalCols) * tileCols;
    hsize_t startFile[] = {hsize_t((m_rank / globalCols) * tileRows), hsize_t(startFileCurrent)};
    hsize_t countFile[] = {hsize_t(tileRows), hsize_t(1)};

    int startMemCurrent = padding;
    hsize_t startMem[] = {hsize_t(startMemCurrent), hsize_t(padding)};
    hsize_t countMem[] = {hsize_t(1), hsize_t(tileRows)};

    hsize_t gridSize[] = {n, n};

    // Create new HDF5 file group named as "Timestep_N", where "N" is number
    //    of current snapshot. The group is placed into root of the file "/Timestep_N".
    std::string groupName = "Timestep_" + std::to_string(static_cast<unsigned long long>(iteration / m_simulationProperties.GetDiskWriteIntensity()));

    AutoHandle<hid_t> groupHandle(H5Gcreate(m_fileHandle, groupName.c_str(),
                                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                                  H5Gclose);

    {
        // Create new dataset "/Timestep_N/Temperature" which is simulation-domain
        //    sized 2D array of "float"s.
        std::string dataSetName("Temperature");
        // Define shape of the dataset (2D edgeSize x edgeSize array).
        AutoHandle<hid_t> dataSpaceHandle(H5Screate_simple(2, gridSize, NULL), H5Sclose);
        // Create datased with specified shape.
        AutoHandle<hid_t> dataSetHandle(H5Dcreate(groupHandle, dataSetName.c_str(),
                                                  H5T_NATIVE_FLOAT, dataSpaceHandle,
                                                  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                                        H5Dclose);

        // Write the data from memory pointed by "data" into new datased.
        // Note that we are filling whole dataset and therefore we can specify
        // "H5S_ALL" for both memory and dataset spaces.
        //H5Dwrite(dataSetHandle, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

        for (int i = 0; i < tileCols; i++)
        {
            H5Sselect_hyperslab(
                filespace,
                H5S_SELECT_SET,
                startFile, // kam zapisu
                nullptr,
                countFile, // moje cast
                nullptr);

            H5Sselect_hyperslab(
                memspace,
                H5S_SELECT_SET,
                startMem,
                nullptr,
                countMem,
                nullptr);

            H5Dwrite(
                dataSetHandle,
                H5T_NATIVE_FLOAT,
                memspace,
                filespace,
                propertyListXfer,
                newTile);
            startMemCurrent++; // Get to next row
            startMem[0] = hsize_t(startMemCurrent);
            startFileCurrent++;
            startFile[1] = hsize_t(startFileCurrent);
        }
    }
    {
        // Create Integer attribute in the same group "/Timestep_N/Time"
        //    in which we store number of current simulation iteration.
        std::string attributeName("Time");

        //Dataspace is single value/scalar.
        AutoHandle<hid_t> dataSpaceHandle(H5Screate(H5S_SCALAR), H5Sclose);

        // Create the attribute in the group as double.
        AutoHandle<hid_t> attributeHandle(H5Acreate2(groupHandle, attributeName.c_str(),
                                                     H5T_IEEE_F64LE, dataSpaceHandle,
                                                     H5P_DEFAULT, H5P_DEFAULT),
                                          H5Aclose);

        //Write value into the attribute.
        double snapshotTime = double(iteration);
        H5Awrite(attributeHandle, H5T_IEEE_F64LE, &snapshotTime);
    }

    // Close XREF property list.
    H5Pclose(propertyListXfer);
}
ParallelHeatSolver::ParallelHeatSolver(SimulationProperties &simulationProps,
                                       MaterialProperties &materialProps) : BaseHeatSolver(simulationProps, materialProps),
                                                                            m_fileHandle(H5I_INVALID_HID, static_cast<void (*)(hid_t)>(nullptr))
{
    MPI_Comm_size(MPI_COMM_WORLD, &m_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);

    // std::cout << "Number of process is: " << m_size << std::endl;
    // std::cout << "I am rank: " << m_rank << std::endl;

    int matrixSize = (materialProps.GetEdgeSize() + padding * 2) * (materialProps.GetEdgeSize() + padding * 2);

    m_tempArray.resize(matrixSize);
    m_domainParams.resize(matrixSize);
    m_domainMap.resize(matrixSize);

    //AutoHandle<hid_t> myHandle(H5I_INVALID_HID, static_cast<void (*)(hid_t)>(nullptr));
    m_simulationProperties.GetDecompGrid(globalCols, globalRows);
    InitTileVariables();
    InitRankProfile();
    InitRankOffsets();
    InitRankTypes();

    if (!m_simulationProperties.GetOutputFileName().empty())
    {
        // open root rank opens the file in sequential mode
        if (isRunSequential && m_rank == 0)
        {
            // Open output file if its name was specified.
            m_fileHandle.Set(H5Fcreate(simulationProps.GetOutputFileName("par").c_str(),
                                       H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT),
                             H5Fclose);
        }
        if (!isRunSequential)
        {
            // init dataset, file, memspace, filespace for all ranks
            InitParallelIO();
        }
    }

    if (isModeRMA)
    {
        MPI_Win_create(newTile, blockRows * blockCols * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &winNewTile); // size of tile
        MPI_Win_create(tile, blockRows * blockCols * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &winTile);       // size of tile
    }

} // end of ParallelHeatSolver constructor

void ParallelHeatSolver::RunSolver(std::vector<float, AlignedAllocator<float>> &outResult)
{
    float middleColAvgTemp = 0.0f;
    std::vector<float, AlignedAllocator<float>> matrixFinal(n * n); // computed final matrix
    double startTime = 0.0;

    MPI_Comm MPI_COL_COMM; // Comunicator for counting middle average temperature.
    MPI_Win win;           // local window that will be used as winNewTile or winTile depending on number of iteration

    // Create a col communicator using split
    MPI_Comm_split(MPI_COMM_WORLD,
                   m_rank % globalCols,
                   m_rank / globalCols,
                   &MPI_COL_COMM);

    if (m_rank == 0)
    {
        startTime = MPI_Wtime();
        InitWorkingArrays();
        InitRootRankTypes();
    }
    int *sendCountsN = GetSendCounts(tileCols);
    int *displacementsN = GetDisplacementCounts(n);

    // Allocate arrays for sendCounts and displacements
    int *sendCountsTempN = GetSendCounts(blockCols);
    int *displacementsTempN = GetDisplacementCounts(tempN);

    ScatterValues(sendCountsTempN, displacementsTempN);

    bool temperature_computed = false; // flag if in this iteration temperature was computed
    for (size_t iter = 0; iter < m_simulationProperties.GetNumIterations(); ++iter)
    {
        temperature_computed = false;
        MPI_Request request[4];
        MPI_Status status[4];
        // Compute only borders
        // Compute left border
        if (!rankProfile.isLeftRank)
        {
            UpdateTile(tile, newTile, domainParams, domainMap, rankOffsets.startLF, 2, rankOffsets.endLF - rankOffsets.startLF, 2, blockRows,
                       m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
        }
        // Compute right border
        if (!rankProfile.isRightRank)
        {
            //Count right border
            UpdateTile(tile, newTile, domainParams, domainMap, rankOffsets.startLF, blockCols - 4, rankOffsets.endLF - rankOffsets.startLF, 2, blockRows,
                       m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
        }
        // Compute bottom border
        if (!rankProfile.isBottomRank)
        {
            UpdateTile(tile, newTile, domainParams, domainMap, blockRows - 4, rankOffsets.startTB, 2, rankOffsets.endTB - rankOffsets.startTB, blockRows,
                       m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
        }
        // Compute top border
        if (!rankProfile.isTopRank)
        {
            //Count top = right border
            UpdateTile(tile, newTile, domainParams, domainMap, 2, rankOffsets.startTB, 2, rankOffsets.endTB - rankOffsets.startTB, blockRows,
                       m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
        }
        if (isModeRMA)
        {
            MPI_Win_fence(0, winNewTile); // Open window
            MPI_Win_fence(0, winTile);    // Open window

            if (iter % 2 == 0)
            {
                win = winNewTile; // even iterations pointer is on winNewTile
            }
            else
            {
                win = winTile; // odd iterations pointer is on winTile
            }

            // Same logic as MPI mode except we read the data from tiles of neighbors directly
            if (!rankProfile.isLeftRank)
            {
                MPI_Put(&newTile[2 * blockRows], 2, MPI_ROW_BLOCK, m_rank - 1, (blockCols - 2) * blockRows, 2, MPI_ROW_BLOCK, win); // put 2 rows to 0 index to rank on the left
            }
            if (!rankProfile.isRightRank)
            {
                MPI_Put(&newTile[(blockCols - 4) * blockRows], 2, MPI_ROW_BLOCK, m_rank + 1, 0, 2, MPI_ROW_BLOCK, win);
            }
            if (!rankProfile.isBottomRank)
            {
                MPI_Put(&newTile[blockRows - 4], 1, MPI_COL_TILE, m_rank + globalCols, 0, 1, MPI_COL_TILE, win);
            }
            if (!rankProfile.isTopRank)
            {
                MPI_Put(&newTile[2], 1, MPI_COL_TILE, m_rank - globalCols, blockRows - 2, 1, MPI_COL_TILE, win);
            }
        }
        else
        {
            // Send counted values
            if (!rankProfile.isLeftRank)
            {
                // Send left border
                MPI_Isend(&newTile[2 * blockRows], 2, MPI_ROW_BLOCK, m_rank - 1, 0, MPI_COMM_WORLD, &request[0]);
                MPI_Recv(newTile, 2, MPI_ROW_BLOCK, m_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                request[0] = MPI_REQUEST_NULL;
            }
            if (!rankProfile.isRightRank)
            {
                // Get right border
                MPI_Isend(&newTile[(blockCols - 4) * blockRows], 2, MPI_ROW_BLOCK, m_rank + 1, 0, MPI_COMM_WORLD, &request[1]);
                MPI_Recv(&newTile[(blockCols - 2) * blockRows], 2, MPI_ROW_BLOCK, m_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                request[1] = MPI_REQUEST_NULL;
            }
            if (!rankProfile.isBottomRank)
            {
                MPI_Isend(&newTile[blockRows - 4], 1, MPI_COL_TILE, m_rank + globalCols, 0, MPI_COMM_WORLD, &request[2]);
                MPI_Recv(&newTile[blockRows - 2], 1, MPI_COL_TILE, m_rank + globalCols, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                request[2] = MPI_REQUEST_NULL;
            }
            if (!rankProfile.isTopRank)
            {
                MPI_Isend(&newTile[2], 1, MPI_COL_TILE, m_rank - globalCols, 0, MPI_COMM_WORLD, &request[3]);
                MPI_Recv(newTile, 1, MPI_COL_TILE, m_rank - globalCols, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                request[3] = MPI_REQUEST_NULL;
            }
        }

        // // Compute inner block
        UpdateTile(tile, newTile, domainParams, domainMap, 4, 4, blockRows - 8, blockCols - 8, blockRows,
                   m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
        if (isModeRMA)
        {
            MPI_Win_fence(0, winNewTile); // closing window winNewTile
            MPI_Win_fence(0, winTile);    // closing window winTile
        }
        else
        {
            MPI_Waitall(4, request, status);
        }

        if (ShouldPrintProgress(iter))
        {
            ComputeMiddleColAvgTemp(&middleColAvgTemp, MPI_COL_COMM);
            temperature_computed = true;
        }
        if ((iter % m_simulationProperties.GetDiskWriteIntensity()) == 0)
        {
            if (isRunSequential)
            {
                MPI_Gatherv(&newTile[2 * blockRows], tileCols, MPI_TILE, matrixFinal.data(), sendCountsN, displacementsN, MPI_COL_TILE_N_RES, 0, MPI_COMM_WORLD);
                if (m_rank == 0 && m_fileHandle != H5I_INVALID_HID)
                {
                    // sequential version
                    StoreDataIntoFile(m_fileHandle, iter, matrixFinal.data());
                }
            }
            if (!isRunSequential && m_fileHandle != H5I_INVALID_HID)
            {
                H5WriteTileToFile(iter);
            }
        }

        if (m_rank == 0)
        {
            PrintProgressReport(iter, middleColAvgTemp);
        }
        // Swap source and destination buffers
        std::swap(tile, newTile);
    }

    MPI_Gatherv(&tile[2 * blockRows], tileCols, MPI_TILE, matrixFinal.data(), sendCountsN, displacementsN, MPI_COL_TILE_N_RES, 0, MPI_COMM_WORLD);

    if (!temperature_computed)
    {
        ComputeMiddleColAvgTemp(&middleColAvgTemp, MPI_COL_COMM);
    }
    if (m_rank == 0)
    {
        // Measure total execution time and report
        double elapsedTime = MPI_Wtime() - startTime;
        PrintFinalReport(elapsedTime, middleColAvgTemp, "par");
        std::copy(matrixFinal.begin(), matrixFinal.end(), outResult.begin());
    }

} // end of RunSolver

/**
 * Print block of the matrix
 * @param block     - block to print out
 * @param blockCols - number of rows in the block
 * @param nCols     - number of colls.
 */
void ParallelHeatSolver::printMatrix(float *matrix, int nRows, int nCols, int rank)
{
    std::string str;
    char val[40];

    for (int i = 0; i < nRows; i++)
    {
        str = "";
        sprintf(val, " - Rank %d = (row: %d)[", rank, i);
        str += val;

        for (int j = 0; j < nCols - 1; j++)
        {
            sprintf(val, "%.2f, ", matrix[i * nCols + j]);
            str += val;
        }
        sprintf(val, " %.2f]\n", matrix[i * nCols + nCols - 1]);
        str += val;
        printf("%s", str.c_str());
    }
} // end of printBlock

/**
 * C printf routine with selection which rank prints
 * @param who    - which rank should print. If -1 then all prints.
 * @param format - format string
 * @param ...    - other parameters
 */
void ParallelHeatSolver::mpiPrintf(int who,
                                   const char *__restrict__ format, ...)
{
    if ((who == -1) || (who == m_rank))
    {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }
} // end of mpiPrintf

/**
 * @brief Compute middle temperature.
 * @param middleColAvgTemp [OUT] Output middle temperature.
 * @param comm Comunicator that will be used to compute the middle temperature.
 */
void ParallelHeatSolver::ComputeMiddleColAvgTemp(float *middleColAvgTemp, const MPI_Comm &comm)
{
    float localSum = 0.0f;
    float temperatureSum = 0.0f;
    bool evenColumns = globalCols % 2 == 0;

    for (int i = 2; i < tileRows + 2; i++)
    {
        if (evenColumns)
        {
            // Compute the first row
            localSum += newTile[2 * blockRows + i];
        }
        else
        {
            // Compute the middle row
            int middleRow = std::ceil(tileCols / 2.0) - 1;
            localSum += newTile[2 * blockRows + (middleRow * blockRows) + i];
        }
    }
    if (comm != MPI_COMM_NULL)
    {
        MPI_Reduce(&localSum, &temperatureSum, 1, MPI_FLOAT, MPI_SUM, 0, comm);
    }
    int middleRank = globalCols / 2;

    if (middleRank == m_rank)
    {
        //printMatrix(newTile, blockCols, blockRows, m_rank);
        *middleColAvgTemp = temperatureSum / (tileRows * mpiGetCommSize(comm));
        MPI_Send(middleColAvgTemp, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    if (m_rank == 0)
    {
        MPI_Recv(middleColAvgTemp, 1, MPI_FLOAT, middleRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
} // end of ComputeMiddleColAvgTemp

/**
 * Flush stdout and call barrier to prevent message mixture.
 */
void ParallelHeatSolver::mpiFlush()
{
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    // A known hack to correctly order writes
    usleep(100);
} // end of mpiFlush
