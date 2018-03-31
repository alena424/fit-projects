/*
* functions to work with DHCP server
* @date 30.3.2018
* @author Alena Tesarova
*/

#include "headers.h"
#include "dhcp.h"

/* DHCP discover
* creating DHCP DISCOVER packer according to RFC 2131
*/
int dhcp_discover( dhcp_packet* dhcp, dhcp_lease *lease  ){

		// set parameters from page 36, RFC 2131
		memset( dhcp, 0, sizeof( struct dhcp_packet ) );

		dhcp->op = BOOTREQUEST;
		dhcp->htype = HTYPE_ETHER;
		dhcp->hlen = HLEN_ETHER;
		dhcp->htops = 0;
		dhcp->xid = lease->xid; // random numebr, needs to remember

		dhcp->flags |= BROADCAST;
		dhcp->ciaddr = 0;
		dhcp->yiaddr = 0;
		dhcp->siaddr = 0;
		dhcp->giaddr = 0;
		memcpy ( dhcp->chaddr, lease->hmac, sizeof( lease->hmac ) );
		int counter = 4; // index of free space in dhcp->options, first 4 bytes
		memcpy( dhcp->options, magic_cookies, sizeof( magic_cookies )) ;
		// we need to specified type of message in options
		//msg->options
		dhcp->options[ counter++ ] = DHCP_MESSAGE_TYPE;
		dhcp->options[ counter++ ] = DHCP_MESSAGE_LEN;
		// needs to store LEN bytes to options
		memcpy( dhcp->options + counter, MSG_DISCOVER, DHCP_MESSAGE_LEN);
		// need to add end
		dhcp->options[ counter + DHCP_MESSAGE_LEN ] = 0xff;
		MSG_DISCOVER;

		dhcp->sec = 0;

		return 1;
}

/* DHCP request - choosing ip address
* creating DHCP DISCOVER packer according to RFC 2131
*/
int dhcp_request( dhcp_packet* dhcp, dhcp_lease *lease  ){

		// set parameters from page 36, RFC 2131
		memset( dhcp, 0, sizeof( struct dhcp_packet ) );

		dhcp->op = BOOTREQUEST; // from client to server
		dhcp->htype = HTYPE_ETHER;
		dhcp->hlen = HLEN_ETHER;
		dhcp->htops = 0;
		dhcp->xid = lease->xid; // random numebr, needs to remember

		dhcp->flags |= BROADCAST;
		dhcp->ciaddr = lease->client_addr;
		memcpy ( dhcp->chaddr, lease->hmac, sizeof( lease->hmac ) );
		int counter = 4; // index of free space in dhcp->options, first 4 bytes
		memcpy( dhcp->options, magic_cookies, sizeof( magic_cookies )) ;
		// we need to specified type of message in options
		//msg->options
		dhcp->options[ counter++ ] = DHCP_MESSAGE_TYPE;
		dhcp->options[ counter++ ] = DHCP_MESSAGE_LEN;
		// needs to store LEN bytes to options
		memcpy( dhcp->options + counter, MSG_REQUEST, DHCP_MESSAGE_LEN);
		// need to add end
		dhcp->options[ counter + DHCP_MESSAGE_LEN ] = 0xff;
		MSG_DISCOVER;

		dhcp->sec = 0;

		return 1;
}



 /*** end of dhcp.c ***/
