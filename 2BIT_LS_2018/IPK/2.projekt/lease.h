/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

// need to store leases and information
struct dhcp_lease{
	uint32_t hmac; // matches to chaddr
	uint32_t client_addr; // matches to ciadd
	uint32_t server_id; // need to identify server, siaddr
	uint32_t xid;
	struct dhcp_lease *next;

}

