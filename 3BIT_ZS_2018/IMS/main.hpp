/**
 * @project IMS - Simulace pivovaru
 *
 * @file main.hpp
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 *
 */

#include <iostream>
#include <simlib.h>
#include <string>

// casove konstanty
#define DEN 86400
#define HODINA 3600

// pocet tanku
#define TANKY 23

// globalni promenne pro zjisteni, jaky je den v tydnu
extern unsigned int denVTydnu;
// globalni promenna pro pocet volnych tanku
extern unsigned int volneTanky;
// globalni promenna pro znaceni pocet varek od posledniho cisteni
extern unsigned int varekOdCisteni;
// globalni promenna pro zjisteni, zda mame nejaky tank poloplny
extern bool rozdelanyTank;

extern unsigned int tankyKeStaceni;

extern Facility stacirna;

extern unsigned int stacirnaObs;

// Statistika udaje
extern unsigned int dobaStatistika;
// pocet tanku, ktere se povedlo stocit
unsigned int pocetStocenychTanku = 0;

// kolikrat byl umyty tanky
unsigned int pocetUmytychTanku = 0;

// nejsou volne tanky (pivo se kvasi), neni co umyvat, nemuzeme varit ani stacet
unsigned int pocetSmenNemaCoDelat = 0;

unsigned int pocetStacSmenNemaCoDelat = 0;

// pocet smen pres vsedni dny
unsigned int pocetSmenNepracDny = 0;

//pocet smen pres nepracovni dny
unsigned int pocetSmenVikendy = 0;

// pocet 10°
unsigned int pocet10 = 0;

// pocet 11°
unsigned int pocet11 = 0;

// pocet 12°
unsigned int pocet12 = 0;

// pocet 16° piv
unsigned int pocet16 = 0;

// pomocne udaje u statistik

// priznak, ze smena muze varit
bool muzemeVarit = true;

// priznak, ze smena muze stacet
bool muzemeStacet = true;

// zapina debugovaci mod
bool debug = 1;

void info( const char *retezec ) {
   // pokud debugujeme, tak vypisovat, jinak ne
   if ( debug ) {
      std::cerr << static_cast<int>(Time) << ": " << retezec << std::endl;
   }
}

/**
 * Proces zajistujici staceni piva.
 */
class Staceni: public Process {
   void Behavior() {
      if ( tankyKeStaceni ) {
         info( "Jdeme stacet tank." );
         tankyKeStaceni--;
         Wait( 6 * HODINA );
         info( "Umyjeme tank." );
         Wait( 4 * HODINA );
         info( "Vracime tank mezi volne." );
         volneTanky++;
         pocetStocenychTanku++;
      } else {
         muzemeStacet = false;
      }
   }
};

/**
 * Proces simulujici kvaseni piva.
 */
class Kvaseni: public Process {
   void Behavior() {
      double r = Random();
      int dobaKvaseni;
      if ( r <= 0.7 ) {
         info( "Kvasime 11°." );
         dobaKvaseni = 40;
         pocet11++;
      } else if ( r > 0.7 && r <= 0.86 ) {
         info( "Kvasime 12°." );
         dobaKvaseni = 50;
         pocet12++;
      } else if ( r > 0.86 && r <= 0.93 ) {
         info( "Kvasime 10°." );
         dobaKvaseni = 30;
         pocet10++;
      } else {
         info( "Kvasime 16°." );
         dobaKvaseni = 90;
         pocet16++;
      }
      Wait( dobaKvaseni * DEN );
      tankyKeStaceni++; 
   }
};

/**
 * Proces simulujici proces vareni piva.
 */
class Vareni: public Process {
   void Behavior() {
      if ( varekOdCisteni == 10 ) {
         info( "Jdeme cistit tanky na vareni." );
         varekOdCisteni = 0;
      } else {
         if ( rozdelanyTank ) {
            info( "Jdeme dodelat varku do rozdelaneho tanku" );
            varekOdCisteni++;
            rozdelanyTank = false;
            ( new Kvaseni )->Activate( Time + 12 * HODINA );
         } else if ( volneTanky > 0 ) {
            info( "Jdeme vytvorit varku do noveho volneho tanku" );
            volneTanky--;
            varekOdCisteni++;
            rozdelanyTank = true;
         } else {
            muzemeVarit = false;  
         }
      }
   }
};

/**
 * Proces znacici, zda je vikend nebo ne.
 */
class Tyden: public Process {
   void Behavior() {
      // tento proces bezi porad dokola
      while ( 1 ) {
         if ( denVTydnu < 6 ) {
            if ( denVTydnu == 1 ){
               info( "Novy tyden" );
            }
            info( "Prisla denni smena na staceni a vareni." );

            pocetSmenNepracDny++;
            muzemeStacet = true;
            muzemeVarit = true;
            ( new Vareni )->Activate();
            ( new Staceni )->Activate();
            Wait( 12 * HODINA );
            if ( ! muzemeVarit ){
               pocetSmenNemaCoDelat++;
            }
            if ( ! muzemeStacet ) {
               pocetStacSmenNemaCoDelat++;
            }

            info( "Prisla nocni smena na vareni." );
            muzemeStacet = true;
            muzemeVarit = true;
            pocetSmenNepracDny++;
            ( new Vareni )->Activate();
            Wait( 12 * HODINA );
            if ( ! muzemeVarit ){
               pocetSmenNemaCoDelat++;
            }
            denVTydnu++;

         } else {
            info( "Prisla sobotni smena na vareni." );
            pocetSmenVikendy++;
            muzemeVarit = true;
            ( new Vareni )->Activate();
            //( new Staceni )->Activate();
            Wait( DEN );
            if ( !muzemeVarit ){
                pocetSmenNemaCoDelat++;
            }
            denVTydnu++;
            info( "Prisla nedelni smena na vareni." );
            pocetSmenVikendy++;
            muzemeVarit = true;
            ( new Vareni )->Activate();
            Wait( DEN );
            if ( !muzemeVarit ){
               pocetSmenNemaCoDelat++;
            }
            denVTydnu = 1;
         }
      }
   }
};

/**
 * Proces, ktery generuje statistiky standartni vystup. Vystup tohoto procesu lze pouzit
 * na generovani grafu.
 */
class Statistika: public Process {
   void Behavior() {
      std::cout << "Cas,Pocet smen celkem,Pocet varicich smen nemajici co delat, Pocet stacicich smen nemajici co delat, Pocet stocenych tanku celkem,Pocet tanku 10° piva,Pocet tanku 11° piva, Pocet tanku 12° piva,Pocet tanku specialu" << std::endl;
      while (1) {
//         Wait( dobaStatistika / 7 * 365 * 10  );
         std::cout << static_cast<int>(Time) << ",";
         std::cout << pocetSmenNepracDny + pocetSmenVikendy << ",";
         std::cout << pocetSmenNemaCoDelat << ",";
         std::cout << pocetStacSmenNemaCoDelat << ",";
         std::cout << pocetStocenychTanku << ",";
         std::cout << pocet10 << ",";
         std::cout << pocet11 << ",";
         std::cout << pocet12 << ",";
         std::cout << pocet16 << std::endl;
         
         pocetSmenNepracDny = 0;
         pocetSmenVikendy = 0;
         pocetSmenNemaCoDelat = 0;
         pocetStacSmenNemaCoDelat = 0;
         pocetStocenychTanku = 0;
         pocet10 = 0;
         pocet11 = 0;
         pocet12 = 0;
         pocet16 = 0;

         Wait( dobaStatistika );
      }
   }
};

class ClearStat: public Process {
   void Behavior() {
      pocetSmenNepracDny = 0;
      pocetSmenVikendy = 0;
      pocetSmenNemaCoDelat = 0;
      pocetStacSmenNemaCoDelat = 0;
      pocetStocenychTanku = 0;
      pocet10 = 0;
      pocet11 = 0;
      pocet12 = 0;
      pocet16 = 0;
   }
};

