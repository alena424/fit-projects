/**
 * @project IMS - Simulace pivovaru
 * 
 * @file main.cpp
 * @author Jan Sorm (xsormj00), Alena Tesarova (xtesar36)
 *
 */

#include "main.hpp"

Facility stacirna;
unsigned int stacirnaObs = 1;

// zaciname modelovat od pondelni denni smeny
unsigned int denVTydnu = 1;
// zaciname s 23 volnymi tanky
unsigned int volneTanky = 23;
// zaciname s nulovym poctem varek od posledniho cisteni
unsigned int varekOdCisteni = 0;
// na zacatku neni zadny tank rozdelany
bool rozdelanyTank = false;

unsigned int tankyKeStaceni = 0;

unsigned int dobaStatistika = 365 * DEN;

int main() {

   unsigned celkovaDoba = 10 * 365 * DEN;

   info( "Zacatek simulace" );
   Init( 0, celkovaDoba );
   ( new Tyden )->Activate();
   ( new Statistika )->Activate( );
   Run();
   info( "Konec simulace" );
   return 0;
}
