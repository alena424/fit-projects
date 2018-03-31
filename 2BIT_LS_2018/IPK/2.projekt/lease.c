/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

#include "headers.h"
#include "lease.h"


/*int request_release(){

}
*/


void inicialized_lease( LeaseList *lease ){
    lease->first = NULL; // points to null
}

int new_lease( LeaseList *lease ){
    // allocate new lease

    LeaseElem *new_lease = malloc( sizeof( LeaseElem ));
    if( new_lease == NULL ){
        fprintf( stderr, "Error in malloc\n");
        return EXIT_FAILURE;
    }
    new_lease->xid = rand() ; // random number
    new_lease->next = NULL;
    if ( lease->first == NULL ){
        // first element
        printf("First elemetns");
        lease->first = new_lease;
    } else {
        LeaseElem *tmp = lease->first;
        while( tmp != NULL ){
            tmp = tmp->next;
        }
        tmp->next = new_lease;
    }

    return EXIT_SUCCESS;
}


LeaseElem *get_last_lease( LeaseList *lease ){
    LeaseElem *tmp = lease->first;
    LeaseElem *retlease = lease->first;
    while( tmp != NULL ){
        retlease = tmp;
        tmp = tmp->next;
    }
    return retlease;
}


/** Function free_leases
* @param lease - list of leases
*/
void free_leases( LeaseList *lease  ){
    LeaseElem *tmp = lease->first;
    LeaseElem *tmpdel = lease->first;
    if ( lease->first != NULL ){

        while( tmp != NULL ){
            tmpdel = tmp;
            tmp = tmp->next;
            free(tmpdel);
        }
    }

    lease = NULL;
}

void print_lease( LeaseList *lease ){
     LeaseElem *tmp = lease->first;;
    while( tmp != NULL ){
        printf( "HMAC: %d, CLIENT_ADDR: %d, Server Id: %d, Xid: %d => ", tmp->hmac, tmp->client_addr, tmp->server_id, tmp->xid );
        tmp = tmp->next;
    }
    printf("NULL\n\n");
}

/*** end of lease.c ***/
