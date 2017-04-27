/**
* @file proj2.c
* @date 15.4.2017
* @author Alena Tesarova (xtesar36)
* @version 1.0
* @TODO:
* @brief
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


#define FILENAME	"proj2.out"
#define NOT_VALID_INPUT "Not valit input, input must be in this format: proj2 -A -C -AGT -CGT -AWT -CWT\n"


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
	sem_t	*sem_pom;
	sem_t	*sem_finished;
} shared;

typedef struct {
	FILE	*file;
	shared	*sh;
} Tshared;

int inicialize( Tshared *x );

void clean_memory(Tshared *x);

int adult_go(int id, Tshared *x, int max_waiting_time );


int child_go(int id, Tshared *x, int max_waiting_time);

int make_proces( Tshared *x, int total, char init, int c_fork_max, int a_fork_max, int c_wait_max, int a_wait_max);


