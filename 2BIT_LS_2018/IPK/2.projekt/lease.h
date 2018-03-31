/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */


// need to store leases and information
typedef struct dhcp_lease_elem{
	uint32_t hmac; // matches to chaddr
	uint32_t client_addr; // matches to ciadd
	uint32_t server_id; // need to identify server, siaddr
	uint32_t xid;
	struct dhcp_lease_elem *next;

} LeaseElem;

typedef struct leaselist{
    LeaseElem *first;
} LeaseList;

void inicialized_lease( LeaseList *lease );
int new_lease( LeaseList *lease );
void print_lease( LeaseList *lease  );
void free_leases( LeaseList *lease  );
