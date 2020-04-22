/*
 * Algorithm: odd-even transposition sort 
 * @author Alena Tesarova (xtesar36)
 */

#include <fstream>
#include <mpi.h>
#include <iostream>

using namespace std;

#define TAG 0

int main(int argc, char *argv[]) {
    int numprocess;               //pocet procesoru
    char input[] = "numbers";
    int myid;
    //MPI
    MPI_Init(&argc, &argv);                          // inicializace MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numprocess);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           // zjistíme id svého procesu
    
    /*std::ofstream outfile;
    outfile.open("time.txt", std::ios_base::app); */
   

    // MASTER vypise cisla a prideli je procesorum
    if (myid == 0) {
        fstream fin;
        fin.open(input, ios::in);
        int number = 0;
        int counterProcess = 0;
        while (fin.good()) {
            number = fin.get(); // nacteme prvni cislo
            if (!fin.good()) { // konec souboru
                break;
            }
            cout << number << " " ;
            MPI_Send(&number, 1, MPI_INT, counterProcess, TAG, MPI_COMM_WORLD);
            counterProcess++; // zvetsime pocitadlo pro dalsi proces
        }
        cout << endl;
    }


    MPI_Status stat;
    int actualNumber = 0;
    int neighnumber = 0;
    MPI_Recv(&actualNumber, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD,
             &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
   // cout << "i am:" << myid << " my number is:" << actualNumber << endl;

    //Limity pro indexy
    int oddlimit = 2 * (numprocess / 2) - 1;   //limity pro sude
    int evenlimit = 2 * ((numprocess - 1) / 2);  // limit pro liche
    int halfcycles = (numprocess/2) + (numprocess % 2); // zaokrouhlujeme nahoru

    //double start = MPI_Wtime();
    
    for (int i = 0; i < halfcycles; i++){

        if (myid < oddlimit && (myid % 2 == 0 || myid == 0)) {
            // sude cislo, posleme sousedovi napravo cislo
            MPI_Send(&actualNumber, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD);
            MPI_Recv(&actualNumber, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD, &stat);
        } else if (myid <= oddlimit) {
            // licha cisla
            MPI_Recv(&neighnumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat); // prijemem souseda
            if (neighnumber > actualNumber) {
                // vymenime si pozice
                MPI_Send(&actualNumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD);
                actualNumber = neighnumber;
            } else {
                // takto zustane poradi, poslu souseda
                MPI_Send(&neighnumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD);
            }
        }

        if (myid < evenlimit && (myid % 2 )) {
            // liche cislo, posleme sousedovi napravo cislo
            MPI_Send(&actualNumber, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD);
            MPI_Recv(&actualNumber, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD, &stat);
        } else if (myid <= evenlimit && myid != 0) {
            // licha cisla
            MPI_Recv(&neighnumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat); // prijemem souseda
            if (neighnumber > actualNumber) {
                // vymenime si pozice
                MPI_Send(&actualNumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD);
                actualNumber = neighnumber;
            } else {
                // takto zustane poradi, poslu souseda
                MPI_Send(&neighnumber, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD);
            }
        }
    }
    
    double end = MPI_Wtime();
    
    int *resultArray = new int[numprocess];

    for (int j = 0; j < numprocess;j++){
        if (myid == j){
            MPI_Send(&actualNumber, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        }
        if (myid == 0){
            MPI_Recv(&neighnumber, 1, MPI_INT, j, TAG, MPI_COMM_WORLD, &stat); //jsem 0 a prijimam
            resultArray[j] = neighnumber;
        }
    }
    if (myid == 0){
        resultArray[0] = actualNumber; // to prvni tam jese nemame
        for (int i = 0; i < numprocess; ++i) {
            cout << resultArray[i] << endl;
        }
    }

    // TIME
    MPI_Finalize();
    //std::cout << "The process" << myid<< " took " << end - start << " seconds to run." << std::endl;
    //outfile << end - start << " ";

    return 0;

}//main

