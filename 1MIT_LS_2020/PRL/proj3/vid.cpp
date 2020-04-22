/*
 * Algorithm: Visibility PRAM algorithm
 * @author Alena Tesarova (xtesar36)
 * @date April 2020
 */

#include <fstream>
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>       /* atan */
#include <algorithm>

using namespace std;

#define TAG 0
#define SMALLEST_ATAN -2.0 // default number for angle

/**
 * @function power_ceil
 * @describe Function rounds up number x to power of 2 
 * @param x - number to be rounded
 * @return power of 2 of the number
 * @url https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
 **/
unsigned power_ceil(unsigned x) {
    if (x <= 1) return 1;
    int power = 2;
    x--;
    while (x >>= 1) power <<= 1;
    return power;
}

/** main start **/
int main(int argc, char *argv[]) {
    int numprocess, myid;                 // number of process
    
    MPI_Init(&argc, &argv);         // inicialization of MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numprocess);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Status stat;

    int success_buf = 1;
    unsigned int size = numprocess * 2;
    unsigned int realsize = 0; // just root will know
    
    unsigned power_size = power_ceil(size); // size rounded to power of 2
    
    double result_angle[power_size] = {SMALLEST_ATAN}; // with size size
    double max_prev_angle[power_size];
    double matrix_alts[size] = {0};        // altitudes on input
    char result[size] = {'_'};
   /* std::ofstream outfile;
    outfile.open("time.txt", std::ios_base::app);*/
    
    for (size_t i = 0; i < power_size; i++){
        max_prev_angle[i] = SMALLEST_ATAN; // init
        result_angle[i] = SMALLEST_ATAN;
    }

    // Master reads the input
    if (myid == 0){
        if (argc != 2){
            cerr << "There should be one argument - a string with altitutes separated by comma." << endl;
            success_buf = 0;
            MPI_Bcast( &success_buf,1,MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Finalize();
            return 0;
        }

        stringstream input(argv[1]); // input string
        int poc = 0;
        for (double i; input >> i; ) {  
            if (i == 0 ){
                cerr  << "Numbers of altitude are only natural numbers (1,2...N)." << endl;
                success_buf = 0;
                MPI_Bcast( &success_buf,1,MPI_INT, 0, MPI_COMM_WORLD);
                MPI_Finalize();
                return 0;
            }
            matrix_alts[poc] = i;
            poc++;
            if (input.peek() == ','){
                input.ignore();
            }
        }
        realsize = poc; // first process knows
        MPI_Bcast( &realsize,1,MPI_INT, 0, MPI_COMM_WORLD); // everything was correct and we can continue
        MPI_Bcast( &(matrix_alts[0]),1,MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        for ( size_t i = 0; i < numprocess; i++){
            MPI_Send(&(matrix_alts[i*2]), 2, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
        }   
    }
    // is the input correct?
    MPI_Bcast( &realsize,1,MPI_INT, 0, MPI_COMM_WORLD);
    if (! realsize){
         cerr << "The process" << myid<< " saying Error" << std::endl;
         MPI_Finalize();
         return 0;
    }
    MPI_Bcast( &(matrix_alts[0]),1,MPI_DOUBLE, 0, MPI_COMM_WORLD); //initial altitude
    
    double start = MPI_Wtime();

    int index = myid*2; // every process takes 2 values
   
    MPI_Recv(&(matrix_alts[index]), 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD,&stat);
    
    if (myid != 0){ // no division by zero!
        result_angle[index] = atan ( ((double) matrix_alts[index] - (double) matrix_alts[0]) / (double)index );
    }
    result_angle[index+1] = atan ( ((double) matrix_alts[index + 1] - (double) matrix_alts[0]) / (double)(index+1) );
    max_prev_angle[index] = result_angle[index]; // making a copy
    max_prev_angle[index+1] = max(result_angle[index], result_angle[index+1]); // counting max on their index
    
    double log_size = log2(numprocess);
    // UpSweep - every node remembers sum
    for (int d = 0; d < log_size; d++) {
        int pom_pow = pow(2,d); // 2^d
        int pom_pow_1 = pom_pow * 2; //2^(d+1)
        
        if ((myid + 1) % pom_pow_1 == 0) {
            double value = 0.0;
            MPI_Recv(&(value), 1, MPI_DOUBLE, (myid - pom_pow), TAG, MPI_COMM_WORLD,&stat);
             
            max_prev_angle[index+1] = max(max_prev_angle[index+1], value);
           /* cout << myid <<": " << "2. max_prev_angle[" << index << "] = " << max_prev_angle[index] <<"rad ="<< (max_prev_angle[index] / (3.14/180) )<< endl;
            cout << myid <<": " << "2. max_prev_angle[" << index + 1 <<"] = " <<  max_prev_angle[index+1] <<"rad ="<< ( max_prev_angle[index+1] / (3.14/180) )<< endl;*/
            
        } else if ((myid + 1) % pom_pow == 0) { // passing upper value to process that need it
           // cout << myid <<": sending to " << myid + pom_pow << " value: " << max_prev_angle[index+1] / (3.14/180)<< endl;
             MPI_Send(&(max_prev_angle[index+1]), 1, MPI_DOUBLE, myid + pom_pow, TAG, MPI_COMM_WORLD); //sending result
        } 
    }
    
      // DownSpeep - root will get a neutral element, we go down and copying left node and summing root with a left of UP tree
    max_prev_angle[size - 1] = SMALLEST_ATAN; // clear
    max_prev_angle[power_size - 1] = SMALLEST_ATAN; // clear
    
    double tmp = 0.0;
    
    for (int d = log_size - 1; d >= 0; d--) {
        int pom_pow = pow(2,d); // 2^d
        int pom_pow_1 = pom_pow * 2; //2^(d+1)
        
        if ((myid + 1) % pom_pow_1== 0) {
            
            MPI_Recv(&(tmp), 1, MPI_DOUBLE, myid - pom_pow, TAG, MPI_COMM_WORLD,&stat);
            MPI_Send(&(max_prev_angle[index+1]), 1, MPI_DOUBLE, myid - pom_pow, TAG, MPI_COMM_WORLD); //sending result

            max_prev_angle[index+1] = max(max_prev_angle[index+1], tmp);
           /* cout << myid <<": " << "2. max_prev_angle[" << index << "] = " << max_prev_angle[index] <<"rad ="<< (max_prev_angle[index] / (3.14/180) )<< endl;
            cout << myid <<": " << "2. max_prev_angle[" << index + 1 <<"] = " <<  max_prev_angle[index+1] <<"rad ="<< ( max_prev_angle[index+1] / (3.14/180) )<< endl;*/
            
        } else if ((myid + 1) % pom_pow == 0) {
            MPI_Send(&(max_prev_angle[index+1]), 1, MPI_DOUBLE, (myid + pom_pow), TAG, MPI_COMM_WORLD); //sending result

            MPI_Recv(&(max_prev_angle[index+1]), 1, MPI_DOUBLE, (myid + pom_pow), TAG, MPI_COMM_WORLD,&stat);
        }
    }
    // now, it s necessary to extend max value do index position
    tmp = max_prev_angle[index];
    
    max_prev_angle[index] = max_prev_angle[index+1];
    max_prev_angle[index+1] = max(max_prev_angle[index+1], tmp);
    
    if (result_angle[index] > max_prev_angle[index]){
         result[index] = 'v'; 
    } else {
         result[index] = 'u'; 
    }
    if (result_angle[index+1] > max_prev_angle[index+1]){
         result[index+1] = 'v'; 
    } else {
        result[index+1] = 'u'; 
    }
    
    MPI_Send(&(result[index]), 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD); // we will send 2 recounted values
    
    if (myid == 0){          
        for ( size_t i = 0; i < numprocess; i++){
            index = i*2;
            MPI_Recv(&(result[index]), 2, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD,&stat);   
        }
    }
    double end = MPI_Wtime();
    
     if (myid == 0){
        result[0] = '_';
        for (size_t i = 0; i < realsize; i++){
                cout << result[i];
                if (i != (realsize - 1) ){
                    cout << ",";
                }
            }
        cout << endl;
     }
    
    MPI_Finalize();
    //outfile << end - start << " ";
    return 0;

}
//end of main

