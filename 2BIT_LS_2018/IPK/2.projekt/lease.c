/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

#include "headers.h"
#include "lease.h"

int request_release(){

}

void inicialized_release( struct dhcp_lease *lease ){
    lease = NULL; // points to null
}

void new_lease( dhcp_lease *lease ){
    // allocate new lease
    struct dhcp_lease new_lease = malloc( sizeof(struct dhcp_lease));
    new_lease->xid = rand(); // random number
    new_lease->next = NULL;
    if ( lease == NULL ){
        // first element
        lease = new_lease;
    } else {
        while( lease != NULL ){
            lease = lease->next;
        }
        lease->next = new_lease;
    }
}

/** Function free_leases
* @param lease - list of leases
*/
void free_leases( struct dhcp_lease *lease  ){
    struct dhcp_lease *tmp;
    while( lease != NULL ){
        tmp = lease;
        lease = lease->next;
        free(tmp);
    }
    lease = NULL;
}

/*** end of lease.c ***/
