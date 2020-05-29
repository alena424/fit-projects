/**
 * @project MSSI - brewery simulation
 * 
 * @file main.cpp
 * @author Alena Tesarova (xtesar36)
 *
 */

#include "main.hpp"

Facility bottling;
unsigned int bottlingObs = 1;


// we are starting to simulate from Monday daily shifts
unsigned int dayOfTheWeek = 1;

// we start with 23 empty tanks
unsigned int freeTanks = 23;

// we start with 0 number of batch from last cleaning process
unsigned int batchesFromLastCleaning = 0;

// at the beginning there is no unfinished tank
bool unfinishedTank = false;

unsigned int tanksToBeTap = 0;

// if we want to measure in days, months, years etc.
unsigned int timeForStatistics =10 * 365 * DAY;

int main(int argc, char **argv) {

   unsigned totalTime = 10 * 365 * DAY;

   if (argc > 1){
		   char *pEnd;
   		freeTanks = (int) strtol (argv[1], &pEnd, 10);
		//printf("Tanks were set to %d\n", freeTanks);
   }
   info( "Beginning of the simulation" );
   Init( 0, totalTime );
   ( new Week )->Activate();
   ( new Statistics )->Activate( );
   Run();
   info( "End of simulation" );
   return 0;
}
