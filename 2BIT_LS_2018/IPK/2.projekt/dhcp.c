/*
* functions to work with DHCP server
* @date 30.3.2018
* @author Alena Tesarova
*/

#include "headers.h"
#include "lease.h"
#include "udp.h"
#include "dhcp.h"



/* Options */
// option cookies - to differ beetween BOOTP and a DHCP message
int magic_cookies[] = { 99, 130, 83, 99 };

// option codes 128 - 256 are reserved for site-specific options
// end option is 255 and its length is 1 octet

void print_options(uint32_t options[]){
    for ( int i = 0; i < VARIABLE_L; i ++ ){
        if ( options[i] == 0 || options[i] == 0xff ) {
            break;
        }
        printf( "%d ", options[i] );
    }
    printf("END\n\n");
}

/** DHCP discover
* creating DHCP DISCOVER packer according to RFC 2131
*/

int dhcp_discover( dhcp_packet* dhcp, LeaseList *leaselist  ){

		// set parameters from page 36, RFC 2131
		memset( dhcp, 0, sizeof( dhcp_packet ) );
		LeaseElem *lease = leaselist->first;

		dhcp->op = BOOTREQUEST;
		dhcp->htype = HTYPE_ETHER;
		dhcp->hlen = HLEN_ETHER;
		dhcp->htops = 0;
		dhcp->xid = lease->xid; // random numebr, needs to remember
		//printf("flags\n");

		//dhcp->flags |= BROADCAST;
		//dhcp->ciaddr = 0;
		//dhcp->yiaddr = 0;
		//dhcp->siaddr = 0;
		//dhcp->giaddr = 0;
		//printf("memcopy: \n");

		memcpy ( &dhcp->chaddr, &lease->hmac, sizeof( lease->hmac ) );
		int counter = 4; // index of free space in dhcp->options, first 4 bytes
		memcpy( dhcp->options, magic_cookies, sizeof( magic_cookies )) ;
		// we need to specified type of message in options
		//msg->options

		dhcp->options[ counter++ ] = DHCP_MESSAGE_TYPE;
		dhcp->options[ counter++ ] = sizeof(uint8_t);
		// needs to store LEN bytes to options
		//printf("options: \n");
		int value = MSG_DISCOVER;
		memcpy( dhcp->options + counter, &value, DHCP_MESSAGE_LEN);
		// need to add end
		dhcp->options[ counter + DHCP_MESSAGE_LEN ] = 0xff;
		//MSG_DISCOVER;
		//print_options(dhcp->options);

		dhcp->sec = 0;
		//printf("end \n");

		return sizeof(dhcp);
}

/** DHCP request - choosing ip address
* creating DHCP DISCOVER packer according to RFC 2131
*/
/*
int dhcp_request( dhcp_packet* dhcp, struct dhcp_lease *lease  ){

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
		//MSG_DISCOVER;

		dhcp->sec = 0;

		return 1;
}*/

/**
* init_udp_packet
* @param output contains of UDP packet
* @param dhcp data in dhcp structure
* @param srcmac source mac address (mac address we generated)
* @param dstmac destination address ( address of server or broadcast address )
* @param srcport source port (for server: 67, for client: 68)
* @param dscport destination port
* @param srchost ip source
* @param dsthost ip destination
*/
/*
int init_udp_packet(char *output, dhcp_packet dhcp, unsigned char *srcmac, unsigned char *dstmac, int srcport, int dscport, int srchost, int dsthost ){

    struct dhcp


    struct udphdr *udp = malloc( sizeof( struct udphdr ) );
    udp->source = srcport;
    udp->dest =descport;
    udp->len = htons()

}*/

/**
* Function resuest_lease
* function request for new lease from DHCP server
* @param socket_send socket that we will send to server
* @param mac actual mac address
* @param dstmac destination mac address
* @param max_tries max tries of sending dhcp requests
*/
int request_lease(int socket_send, int socket_recv, unsigned char * mac, unsigned char *dstmac, int max_tries, unsigned char *interface_mac, int interface_index ){
    // need to send dhcp request until we get reply (max_tries)
    //printf("request lease: \n");
    LeaseList * lease = malloc( sizeof( lease ) );

    if ( lease == NULL){
        fprintf(stderr, "Error in malloc\n ");
        return EXIT_FAILURE;
    }


    inicialized_lease( lease );
    new_lease(lease);
    print_lease(lease);
    dhcp_packet dhcp;
    unsigned char brd_mac[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    struct sockaddr_ll socket_addres;
    //printf("socket_address1\n");
    // https://illumos.org/man/3socket/sockaddr_ll
    memset( &socket_addres, 0, sizeof( struct sockaddr_ll ) );
    socket_addres.sll_family = AF_PACKET;
    socket_addres.sll_halen = ETH_ALEN;
    socket_addres.sll_hatype = 1;
    socket_addres.sll_ifindex = interface_index;
    socket_addres.sll_pkttype = PACKET_BROADCAST;
    //printf("socket_address\n");
    memcpy( &socket_addres.sll_addr, brd_mac, sizeof(brd_mac));


    int send_packet_b = 0;
    unsigned char udp_packet[1024];
    // unsigned char buffer[1024];


    //for ( int i = 0; i < max_tries; i ++){
       //printf("dhcp discover: \n");
        size_t dhcp_len = dhcp_discover( &dhcp, lease );

        // init UDP packet
        //printf("init packet: \n");
        size_t udppacket_len = init_udp_packet( udp_packet, sizeof(udp_packet), &dhcp, dhcp_len, interface_mac, 0, htons(68),  brd_mac, 0xffffffff, htons(67));
        //printf("UDP packet: %s, Length: %d\n", udp_packet, udppacket_len );
        //size_t udppacket_len = init_udp_packet( buffer, sizeof(buffer), &dhcp, dhcp_len, interface_mac, 0, htons(68), brd_mac  , 0xffffffff, htons(67) );

        //printf("send packet: \n");
        struct sockaddr *addrr = (struct sockaddr*) &socket_addres;
        send_packet_b = sendto( socket_send, udp_packet, udppacket_len, 0, (struct sockaddr *)addrr, sizeof( struct sockaddr_ll ) );
        if ( send_packet_b <= 0 ){
            perror ("Error in send\n");
            return EXIT_FAILURE;
        }
        printf("SUCCESS\n");

    //}
    return 1;
}


 /*** end of dhcp.c ***/
