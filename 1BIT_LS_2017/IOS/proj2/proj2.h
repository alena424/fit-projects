/**
* @file proj2.h
* @date 27.4.2017
* @author Alena Tesarova (xtesar36)
* @version 1.0
* @brief Hlavictovy soubor pro proj2.c
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <time.h>


#define FILENAME	"proj2.out" //nazev slozky, do ktere budeme zapisovat
#define NOT_VALID_INPUT "Not valit input, input must be in this format: proj2 -A -C -AGT -CGT -AWT -CWT\n" //chybova hlaska

/**
* Strutura se sdilenymi promennymi: id, run, adultsrun, wait_child, wait_adult, childrenInCenter, adultsInCenter
* Semafory: sem_childin, sem_parout, sem_workmem, sem_pom, sem_finished
* Inicializace ve funkci inicializace()
* @param file slozka, do ktere budeme zapisovat
*/
typedef struct {
	int	id;
	int	run;
	int	adultsrun;
	int	wait_child;
	int	wait_adult;
	int	childrenInCenter;
	int	adultsInCenter;
	sem_t	*sem_childin;
	sem_t	*sem_parout;
	sem_t	*sem_workmem;
	sem_t	*sem_finished;
} shared;

typedef struct {
	FILE	*file;
	shared	*sh;
} Tshared;

/**
* @brief Inicializace struktury - sdilenych dat a semaforu 
* @param x struktura obsahujici potrebna data
*/
int inicialize( Tshared *x );

/**
* @brief Mazani sdilenych promennych, odlinkovani semaforu
* @param x struktura obsahujici potrebna data
*/
void clean_memory(Tshared *x);

/**
* @brief Hlavni proces rodice, akce: started, enter, trying to leave, waiting, leave, finish
* @param id id procesu
* @param x structura s daty sdilenych promennych, semaforu a slozky, do ktere zapisujeme
* @param max_waiting_time maximalni cas, ktery dospeli stravi v centru
*/
int adult_go(int id, Tshared *x, int max_waiting_time );

/**
* @brief Hlavni proces ditete, akce: started, waiting, enter, trying to leave, leave, finish
* @param id id procesu
* @param x structura s daty sdilenych promennych, semaforu a slozky, do ktere zapisujeme
* @param max_waiting_time maximalni cas, ktery dite stravi v centru
*/
int child_go(int id, Tshared *x, int max_waiting_time);


/**
* @brief Funkce na generovani procesu
* @param total Kolik budeme generovat procesu adult/child
* @param x structura s daty sdilenych promennych, semaforu a slozky, do ktere zapisujeme
* @param init znak, podle ktereho urcujeme jestli se jedna o proces adult 'A' anebo child 'C'
* @param c_fork_max je maximalni hodnota doby (v milisekundach), po ktere je generovan novy proces adult
* @param a_fork_max je maximální hodnota doby (v milisekundách), po které je generován nový proces child
* @param c_wait_max je maximální hodnota doby (v milisekundách), po kterou proces adult simuluje cinnost v centru
* @param a_wait_max je maximální hodnota doby (v milisekundách), po kterou proces child simuluje cinnost v centru
*/
int make_proces( Tshared *x, int total, char init, int c_fork_max, int a_fork_max, int c_wait_max, int a_wait_max);

/*** Konec souboru proj2.h ***/


