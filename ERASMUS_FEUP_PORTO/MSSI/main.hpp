/**
 * @project MSSI - brewery simulation
 * 
 * @file main.cpp
 * @author Alena Tesarova (xtesar36)
 *
 */

#include <iostream>
#include <simlib.h>
#include <string>

// time constant
#define DAY 86400
#define HOUR 3600

// number of tanks
#define TANKS 23


// global variable to get what day of the week is
extern unsigned int dayOfTheWeek;

// global variable to get number of free tanks
extern unsigned int freeTanks;

// global variable to mark number of batches from last cleaning
extern unsigned int batchesFromLastCleaning;

// global variable to find out of we have an unfinished tank
extern bool unfinishedTank;

extern unsigned int tanksToBeTap;

extern Facility bottling;

extern unsigned int bottlingObs;

// Statistics data
extern unsigned int timeForStatistics;

// number of tanks that we managed to tap
unsigned int numberOfTappedTanks = 0;

// how many time a tank was washed
unsigned int numberOfWashedTanks = 0;

// there are no free tanks (fermentation process), there is nothing to wash so the shift can not cook nor tap
unsigned int numberOfShiftsHavingNothingToDo = 0;

// @note tap = stacet
unsigned int numberOfTappingShiftsHavingNothingToDo = 0;

// number of shifts during week days (vsedni)
unsigned int numberOfShiftsWeekday = 0;

// number of shifts during weekend
unsigned int numberOfShiftsWeekend = 0;

// numberOf 10°
unsigned int numberOf10 = 0;

// numberOf 11°
unsigned int numberOf11 = 0;

// numberOf 12°
unsigned int numberOf12 = 0;

// numberOf 16° beer
unsigned int numberOf16 = 0;


// flag that we can cook
bool canWeCook = true;

// flag that shift can tap
bool canWeTap = true;

//  debug mode
bool debug = 0;

void info( const char *str ) {
   // debugging info
   if ( debug ) {
      std::cerr << static_cast<int>(Time) << ": " << str << std::endl;
   }
}

/**
 * Process of tapping the beer
 */
class Tapping: public Process {
   void Behavior() {
      if ( tanksToBeTap ) {
         info( "We are tapping the tank" );
         tanksToBeTap--;
         Wait( 6 * HOUR );
         info( "We are washing the tank." );
         Wait( 4 * HOUR );
         info( "We are giving the tank back." );
         freeTanks++;
         numberOfTappedTanks++;
      } else {
         canWeTap = false;
      }
   }
};

/**
 * Process simulating beer fermentation
 */
class Fermentation: public Process {
   void Behavior() {
      double r = Random();
      int fermentationTime;
      if ( r <= 0.7 ) {
         info( "Fermenting 11°." );
         fermentationTime = 40;
         numberOf11++;
      } else if ( r > 0.7 && r <= 0.86 ) {
         info( "Fermenting 12°." );
         fermentationTime = 50;
         numberOf12++;
      } else if ( r > 0.86 && r <= 0.93 ) {
         info( "Fermenting 10°." );
         fermentationTime = 30;
         numberOf10++;
      } else {
         info( "Fermenting 16°." );
         fermentationTime = 90;
         numberOf16++;
      }
      Wait( fermentationTime * DAY );
      tanksToBeTap++; 
   }
};

/**
 * Process simulating cooking of the beer
 */
class Cooking: public Process {
   void Behavior() {
      if ( batchesFromLastCleaning == 10 ) {
         info( "We are going to clean the tanks for cooking." );
         batchesFromLastCleaning = 0;
      } else {
         if ( unfinishedTank ) {
            info( "We are going to make a batch to unfinished tank." );
            batchesFromLastCleaning++;
            unfinishedTank = false;
            ( new Fermentation )->Activate( Time + 12 * HOUR );
         } else if ( freeTanks > 0 ) {
            info( "We are going to make a batch to new free tank." );
            freeTanks--;
            batchesFromLastCleaning++;
            unfinishedTank = true;
         } else {
            canWeCook = false;  
         }
      }
   }
};

/**
 * Process indicating that is weekend or not
 */
class Week: public Process {
   void Behavior() {
      // this process is cyclic and goes over and over again
      while ( 1 ) {
         if ( dayOfTheWeek < 8 ) {
            if ( dayOfTheWeek == 1 ){
               info( "Another week" );
            }
            info( "Daily shift came for tapping and cooking" );
            numberOfShiftsWeekday++;
            canWeTap = true;
            canWeCook = true;
            ( new Cooking )->Activate();
            ( new Tapping )->Activate();
            Wait( 12 * HOUR );
            if ( ! canWeCook ){
               numberOfShiftsHavingNothingToDo++;
            }
            if ( ! canWeTap ) {
               numberOfTappingShiftsHavingNothingToDo++;
            }
            info( "Night shift came for cooking" );
            canWeTap = true;
            canWeCook = true;
            numberOfShiftsWeekday++;
            ( new Cooking )->Activate();
            Wait( 12 * HOUR );
            if ( ! canWeCook ){
               numberOfShiftsHavingNothingToDo++;
            }
            dayOfTheWeek++;
			 

				if (dayOfTheWeek == 7){
					dayOfTheWeek = 1; // Monday again
				} 
			

         } else {
            info( "Saturday shift came for cooking" );
            numberOfShiftsWeekend++;
            canWeCook = true;
            ( new Cooking )->Activate();
            //( new Tapping )->Activate();
            Wait( DAY );
            if ( !canWeCook ){
                numberOfShiftsHavingNothingToDo++;
            }
            dayOfTheWeek++;
            info( "Sunday shift came for cooking" );
            numberOfShiftsWeekend++;
            canWeCook = true;
            ( new Cooking )->Activate();
            Wait( DAY );
            if ( !canWeCook ){
               numberOfShiftsHavingNothingToDo++;
            }
            dayOfTheWeek = 1;
         }
      }
   }
};

/**
 * Process generating the statistics to std out. Output can be used to generate the graphs.
 */
class Statistics: public Process {
   void Behavior() {
       //std::cout << "Time,Total amount of shifts,Number of cooking shifts having nothing to do, Number of tapping shifts having nothing to do, Total amount of tapped tanks,Number of tanks with 10° beer,Number of tanks with 11° beer, Number of tanks with 12° beer,Number of tanks with special beer,Tanks" << std::endl;
      while (1) {
//         Wait( timeForStatistics / 7 * 365 * 10  );
         //std::cout << static_cast<int>(Time) << ",";
         //std::cout << numberOfShiftsWeekday + numberOfShiftsWeekend << ",";
         std::cout << numberOfShiftsHavingNothingToDo << ",";
         std::cout << numberOfTappingShiftsHavingNothingToDo << ",";
         std::cout << numberOfTappedTanks << ",";
         //std::cout << numberOf10 << ",";
         //std::cout << numberOf11 << ",";
         //std::cout << numberOf12 << ",";
         //std::cout << numberOf16 << ",";
	     //std::cout << freeTanks << std::endl;
         
         numberOfShiftsWeekday = 0;
         numberOfShiftsWeekend = 0;
         numberOfShiftsHavingNothingToDo = 0;
         numberOfTappingShiftsHavingNothingToDo = 0;
         numberOfTappedTanks = 0;
         numberOf10 = 0;
         numberOf11 = 0;
         numberOf12 = 0;
         numberOf16 = 0;

         Wait( timeForStatistics );
      }
   }
};

class ClearStat: public Process {
   void Behavior() {
      numberOfShiftsWeekday = 0;
      numberOfShiftsWeekend = 0;
      numberOfTappingShiftsHavingNothingToDo = 0;
      numberOfTappedTanks = 0;
      numberOf10 = 0;
      numberOf11 = 0;
      numberOf12 = 0;
      numberOf16 = 0;
   }
};

