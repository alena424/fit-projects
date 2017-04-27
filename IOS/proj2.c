
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


/**
* @brief inicializace struktury
* @param x struktura obsahujici potrebna data
*/
int inicialize( Tshared *x )
{

	if ( (x->file = fopen(FILENAME,"w")) == NULL ) {
		perror("error");
		exit(1);
	} 
	
	//_IONBF - unbuffered, hned se zapise na vystup
	if ( setvbuf ( x->file , NULL , _IONBF , 0 )) {
		perror("error setvbuf()");
		fclose(x->file);
		exit(1);		
	}
	
	int id;
	if (( id = shmget(IPC_PRIVATE, sizeof (shared), IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
	{
		perror("can t get memory");
		fclose(x->file);
		exit(1);
	}
	
	//adresa pameti
	if ((x->sh = (shared *) shmat(id, NULL, 0)) == (void *) -1)
	{
		perror("Can't get shared memory");
		fclose(x->file);
		exit(1);
	}
	
	// at nam nevisi semafory
	if ( shmctl(id, IPC_RMID, NULL) == -1 ) {
		perror("...\n");
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
	}

	x->sh->id = 1;
	x->sh->childrenInCenter = 0;
	x->sh->adultsInCenter = 0;
	x->sh->wait_child = 0;
	x->sh->wait_adult = 0;
 
	if ((x->sh->sem_childin = sem_open("/xtesar36_semforchild", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		perror( "Can not create semaphor /xtesar36_semforchild" );
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
	}
	if ((x->sh->sem_parout = sem_open("/xtesar36_semforpar", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		perror( "Can not create semaphor /xtesar36_semforpar" );
		sem_close(x->sh->sem_childin);
		sem_unlink("/xtesar36_semforchild");	
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
	}
	if ((x->sh->sem_workmem = sem_open("/xtesar36_semformem", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)	// 1 = open 
	{
		perror( "Can not create semaphor /xtesar36_semformem" );
		sem_close(x->sh->sem_parout);
		sem_unlink("/xtesar36_semforpar");
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
		
	}
	if ((x->sh->sem_finished = sem_open("/xtesar36_semforfinish", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		perror( "Can not create semaphor /xtesar36_semforfinish" );
		sem_close(x->sh->sem_workmem);
		sem_unlink("/xtesar36_semformem");
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
	} 
	if ((x->sh->sem_pom = sem_open("/xtesar36_sempom", O_CREAT | O_EXCL, 0666, 1) ) == SEM_FAILED)
	{
		perror( "Can not create semaphor /xtesar36_sempom" );
		shmdt(x->sh);
		fclose(x->file);
		exit(1);
	}
	return EXIT_SUCCESS;
						
	
}


void clean_memory(Tshared *x)
{
	if ( x->file != NULL ) {
		fclose(x->file);
		x->file = NULL;
	}
	
	sem_close(x->sh->sem_finished);
	sem_close(x->sh->sem_workmem);
	sem_close(x->sh->sem_parout);
	sem_close(x->sh->sem_childin);
	sem_close(x->sh->sem_pom);

	sem_unlink("/xtesar36_semforfinish");
	sem_unlink("/xtesar36_semformem");
	sem_unlink("/xtesar36_semforpar");
	sem_unlink("/xtesar36_semforchild");
	sem_unlink("/xtesar36_sempom");
	
	shmdt(x->sh);
}

int adult_go(int id, Tshared *x, int max_waiting_time ){
	//dospeli vznikl a chce vstoupit
	
	sem_wait( x->sh->sem_workmem );
	fprintf( x->file, "%d\t: A %d \t: started\n", x->sh->id++, id );	
	fprintf( x->file, "%d\t: A %d \t: enter\n", x->sh->id++, id );	
	x->sh->adultsInCenter++;
	//sem_post(x->sh->sem_workmem);
	
	if ( x->sh->wait_adult ) {
		sem_trywait( x->sh->sem_parout );
		sem_post(x->sh->sem_parout);
		}
	else if ( x->sh->wait_child ) {
		int repeat = x->sh->wait_child <= 3 ? ( x->sh->wait_child ) : ( 3 );
		for ( int i = 0; i < repeat; i++ ){
		//printf("%d uvolnilo sem_childin %dx\n", id,i);
		sem_post(x->sh->sem_childin);
		}
	}
	sem_post(x->sh->sem_workmem);
	
	int wait = max_waiting_time  ? ( rand() % max_waiting_time ) : ( 0 ) ;
	usleep(wait * 1000);
	
	sem_wait( x->sh->sem_workmem );
	fprintf( x->file,"%d\t: A %d \t: trying to leave\n", x->sh->id++, id );
	if ( ( x->sh->adultsInCenter-1 )*3 < x->sh->childrenInCenter  ){
		//nemuze odejit
		fprintf( x->file,"%d\t: A %d \t: waiting : %d : %d\n", x->sh->id++, id, x->sh->adultsInCenter ,  x->sh->childrenInCenter);
		//printf("Children: %d Adults: %d (%d)\n", x->sh->childrenInCenter, x->sh->adultsInCenter, id);
		x->sh->wait_adult++;
		sem_post(  x->sh->sem_workmem  );
		
		sem_wait( x->sh->sem_parout );
		
		//fprintf( x->file,"%d\t: A %d \t: stopped waiting\n", x->sh->id++, id );
		sem_wait( x->sh->sem_workmem );
		x->sh->wait_adult--;
		//sem_wait( x->sh->sem_parout ); // hned ho zavru

	}
	
	fprintf( x->file,"%d\t: A %d \t: leave\n", x->sh->id++, id );
	
	x->sh->adultsInCenter--;
	x->sh->adultsrun--;
	x->sh->run--;
	//printf("Children: %d Adults: %d (%d)\n", x->sh->childrenInCenter, x->sh->adultsInCenter, id);
	if ( x->sh->adultsrun == 0 ){
		//printf("No running adults, waiting children %d\n", x->sh->wait_child );
		sem_post( x->sh->sem_childin ); //never should happend
		//printf("%d uvolnilo sem_childin\n", id);
		
	}
	sem_post( x->sh->sem_workmem );
	if ( x->sh->run == 0 ){
		sem_post(x->sh->sem_finished);
	}
	
	sem_wait(x->sh->sem_finished);
	fprintf( x->file,"%d\t: A %d\t: finished\n", x->sh->id++, id);
	sem_post(x->sh->sem_finished);
	
	return 0;

}

int child_go(int id, Tshared *x, int max_waiting_time){
	//dite vzniklo a chce vstoupit
	sem_wait( x->sh->sem_workmem );
	
	fprintf( x->file,"%d\t: C %d \t: started\n", x->sh->id++, id );
	sem_post( x->sh->sem_workmem );
	
	//dite muze vstoupit az bude dost rodicu

	sem_wait( x->sh->sem_workmem ); //chci pouze ja psat
	
	if ( x->sh->adultsrun && x->sh->adultsInCenter*3 <= x->sh->childrenInCenter  ){
		
		
		fprintf( x->file,"%d\t: C %d \t: waiting : %d : %d\n", x->sh->id++, id, x->sh->adultsInCenter ,  x->sh->childrenInCenter);
		//printf("Children: %d Adults: %d (%d)\n", x->sh->childrenInCenter, x->sh->adultsInCenter, id);
		x->sh->wait_child++; //dite ceka
		sem_post( x->sh->sem_workmem );
		
		sem_wait( x->sh->sem_childin );
		
		sem_wait( x->sh->sem_workmem );
		x->sh->wait_child--; //uz dite neceka
		//fprintf( x->file,"%d\t: C %d \t: stopped waiting\n", x->sh->id++, id );
		
			
	}
	
	fprintf( x->file,"%d\t: C %d \t: enter\n", x->sh->id++, id );
	x->sh->childrenInCenter++;
	//printf("Children: %d Adults: %d (%d)\n", x->sh->childrenInCenter, x->sh->adultsInCenter, id);
	/*if ( x->sh->adultsrun == 0 || x->sh->adultsInCenter*3 > x->sh->childrenInCenter  ){
		if ( x->sh->wait_child ){
		//pokud je dite ve fronte, uvolnime ho
		//printf("%d uvolnilo sem_childin\n", id);
		//sem_post( x->sh->sem_childin ); //dalsi dite muze vstoupit
		}
	}else{
	
		if ( x->sh->wait_child ){
		//printf("%d zablokovalo sem_childin\n", id);
		//sem_wait( x->sh->sem_childin ); //zablokujeme pro dalsi dite
		}
	}*/
	sem_post( x->sh->sem_workmem );
	
	int wait = max_waiting_time  ? ( rand() % max_waiting_time ) : ( 0 ) ;
	usleep(wait * 1000); //milisekundy
	
	sem_wait( x->sh->sem_workmem );
	fprintf( x->file,"%d\t: C %d \t: trying to leave\n", x->sh->id++, id );
	fprintf( x->file,"%d\t: C %d \t: leave\n", x->sh->id++, id );
	x->sh->childrenInCenter--;
	x->sh->run--;
	 if ( x->sh->wait_child ) {
	 	//printf("%d uvolnilo sem_childin\n", id);
		sem_post(x->sh->sem_childin);
	} //odejde a uvolni misto
	if ( x->sh->wait_child == 0 && x->sh->wait_adult && ( ( x->sh->adultsInCenter-1 )*3 >= x->sh->childrenInCenter ) ) {
	 	//printf("%d uvolnilo sem_adult\n", id);
	 	sem_trywait( x->sh->sem_parout );
		sem_post( x->sh->sem_parout );
	} //odejde a uvolni misto
	sem_post( x->sh->sem_workmem );
	if ( x->sh->run == 0 ){
		sem_post(x->sh->sem_finished);
	}
	
	sem_wait(x->sh->sem_finished);
	fprintf(x->file,"%d\t: C %d\t: finished\n", x->sh->id++, id);
	sem_post(x->sh->sem_finished);
	
			
	return 0;
}


int make_proces( Tshared *x, int total, char init, int c_fork_max, int a_fork_max, int c_wait_max, int a_wait_max){
	
	//budu si pamatovat vsechny hlavni pid
	int array_pid[total];
	int wait = 0;
	for ( int i = 1; i <= total; i++ ){
		if ( init == 'A' ){
			wait = a_fork_max ? ( rand() % a_fork_max) : ( 0 ) ;	
		}else{
			wait = a_fork_max ? ( rand() % c_fork_max) : ( 0 ) ;	
		}
		//printf("wait %c %d\n", init, wait);
		usleep(wait * 1000);
		int pid = fork();
		if ( pid < 0 ){
			perror("fork failed\n");
			clean_memory(x);
			exit(1);
		}
		if ( pid == 0 ) //child
		{
			if ( init == 'A' ){
				//printf("Jsem dospeli %d cislo %d\n",getpid(),i);
				adult_go( i , x, a_wait_max );
				return 0;
			}else{
				//printf("Jsem dite %d cislo %d\n", getpid(), i);
				child_go( i , x, c_wait_max );
				return 0;
			}
		}
		array_pid[i] = pid;
		//printf("%d \n", pid);
		
	}
	
	for ( int j = total; j > 0; j-- ){
		//printf("proces %d, ceka na %d\n",getpid(),array_pid[j]);
		waitpid( array_pid[j], NULL, 0 );
		//printf("proces: %d, konci %d\n",getpid(),array_pid[j]);
	}
	
	return 0;

}


int main(int argc, char *argv[])
{
    
    //get arguments
    unsigned int number_of_adults = 1; //A
    unsigned int number_of_children = 4; //C
    int max_time_for_adult_to_getnew = 0; //AGT; AGT >= 0 && AGT < 5001.
    int max_time_for_child_to_getnew = 0; //CGT; CGT >= 0 && CGT < 5001.
    int max_time_for_adult_incenter = 0; //AWT >= 0 && AWT < 5001.
    int max_time_for_child_incenter = 0; //CWT >= 0 && CWT < 5001.
    
    Tshared x;
    

    char *zbytek = NULL;
    char *zb1 = NULL;
    char *zb2;
    char *zb3;
    char *zb4;
    char *zb5;
   
    if (argc != 7){
        fprintf(stderr, NOT_VALID_INPUT);
        exit(1);
    }else{
        number_of_adults 			= (int) strtol ( argv[1], &zbytek, 10);
        number_of_children			= (int) strtol ( argv[2], &zb1, 10);
        
        if ( number_of_adults <= 0 || number_of_children <= 0  ){
        	fprintf(stderr, "Number of children and number of adults must be greater than zero\n");
        	exit(1);
        }
        
        max_time_for_adult_to_getnew= (int) strtol ( argv[3], &zb2, 10);
        max_time_for_child_to_getnew= (int) strtol ( argv[4], &zb3, 10);
        max_time_for_adult_incenter = (int) strtol ( argv[5], &zb4, 10);
        max_time_for_child_incenter = (int) strtol ( argv[6], &zb5, 10);
        
        if ( max_time_for_adult_to_getnew < 0 || max_time_for_adult_to_getnew >= 5001 || 
        	 max_time_for_child_to_getnew < 0 || max_time_for_child_to_getnew >= 5001 || 
        	 max_time_for_adult_incenter < 0 || max_time_for_adult_incenter >= 5001 || 
        	 max_time_for_adult_incenter < 0 || max_time_for_adult_incenter >= 5001 ){
        	 	fprintf(stderr, "Number of times must be greater than zero\n");
        		exit(1);       	 	
        	 }
    }
    

	srand(time(NULL));   // initialize random number generator
	setbuf(stdout, NULL);  // for valid printing
	setbuf(stderr, NULL);  // for valid printing

	//inicialujeme pamet a semafor
	if ( inicialize(&x) == EXIT_FAILURE){
		exit(1);
	}
	
	pid_t ID_makeA;
	pid_t ID_makeC;
	x.sh->adultsrun = number_of_adults;
	x.sh->run = number_of_adults + number_of_children;
    
	ID_makeA = fork();
	if (ID_makeA == 0) // child
	{
		
		return make_proces(&x, number_of_adults, 'A', max_time_for_child_to_getnew, max_time_for_adult_to_getnew, max_time_for_child_incenter, max_time_for_adult_incenter );
	}
	if ( ID_makeA == -1 ) //
	
	{
		perror("Fork failed");
		clean_memory(&x);
		exit(1);
	}
    
    
	ID_makeC = fork();
	if (ID_makeC == 0) // child
	{
		
		return make_proces(&x, number_of_children, 'C', max_time_for_child_to_getnew, max_time_for_adult_to_getnew, max_time_for_child_incenter, max_time_for_adult_incenter);
	}
	if ( ID_makeC == -1 ) //
	{
		perror("Fork failed");
		/* ..musi se osetrit uz probihajici adult..*/
		clean_memory(&x);
		exit(1);
	}
	
	//hlavni proces pocka az se ukonci deti
	
	int status;
	waitpid( ID_makeA, &status, 0 );
	waitpid( ID_makeC, &status, 0 );
	
	
	clean_memory(&x);
	return EXIT_SUCCESS;
}


