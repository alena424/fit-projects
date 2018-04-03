/**
* dhcp.c functions to work with DHCP server
* @date 30.3.2018
* @author Alena Tesarova
*/

#include "headers.h"
#include "lease.h"
#include "udp.h"
#include "dhcp.h"


/* Options */
// option cookies - to differ beetween BOOTP and a DHCP message
const uint8_t magic_cookies[] = { 0x63, 0x82, 0x53, 0x63  };
// option codes 128 - 256 are reserved for site-specific options
// end option is 255 and its length is 1 octet


const unsigned char broadcast_mac[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

/**
* Function dhcp_make_discover
* function creates dhcp packet
* @param dhcp data structure where we keep store info about dhcp
* @param mac client mac address
*/
void dhcp_make_discover(struct dhcp_packet* dhcp, const unsigned char *mac)
{
    uint8_t type = MSG_DISCOVER;

    memset(dhcp, 0, sizeof(struct dhcp_packet));

    dhcp->op = BOOTREQUEST;
    dhcp->htype = HTYPE_ETHER;
    dhcp->hlen = HLEN_ETHER;
    dhcp->xid = rand();

    dhcp->flags |= BROADCAST ;//htons(BROADCAST);
    memcpy(&dhcp->chaddr, mac, sizeof(mac));
    memcpy(dhcp->options, magic_cookies, sizeof(magic_cookies));

    size_t len = sizeof(MSG_DISCOVER);
    int i = 4;
    dhcp->options[i++] = DHCP_MESSAGE_TYPE;
    dhcp->options[i++] = '\x01';
    dhcp->options[i] = MSG_DISCOVER;
    dhcp->options[i + len] = 0xff;
}

/**
* Function resuest_lease
* function request for new lease from DHCP server
* @param socket_send socket that we will send to server
* @param mac actual mac address
* @param dstmac destination mac address
* @param retries max tries of sending dhcp requests
*/
int request_lease(int sock_send,int sock_recv, const unsigned char* mac, //int timeout,
                    int retries,
                  int interface_index )
{

    int timeout = 1;
    struct dhcp_packet dhcp;
    size_t dhcplen;
    unsigned char dstmac[] = {0xff, 0xff, 0xff, 0xff,0xff, 0xff};

    struct sockaddr_ll socket_addr;
    memset( &socket_addr, 0, sizeof( struct sockaddr_ll ) );
    socket_addr.sll_family = AF_PACKET;
    socket_addr.sll_halen = ETH_ALEN;
    socket_addr.sll_hatype = 1;
    socket_addr.sll_ifindex = interface_index;
    socket_addr.sll_pkttype = PACKET_BROADCAST;
    char packet_udp[1024];
    size_t bufflen;
    int sent_b;

    //printf("request: \n");
    //printf("Copy mac1: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );

    /* DHCPDISCOVER */
    dhcp_make_discover(&dhcp, mac );

    dhcplen = sizeof(dhcp);
    struct sockaddr *addrr = (struct sockaddr*) &socket_addr;

    bufflen = init_udp_packet(packet_udp, sizeof(packet_udp),&dhcp,dhcplen,ifmac, 0, htons(68), broadcast_mac, 0xffffffff, htons(67));


    if (bufflen == -1)
    {
        //log_err("can not initialize packet to send");
        printf("nothing in packet_udp\n");
        return -1;
    }
    //printf("sending\n");
    sent_b = sendto(sock_send, packet_udp, bufflen, 0, (struct sockaddr *)addrr, sizeof( struct sockaddr_ll ) );
    if (sent_b <= 0)
    {
        //log_err("can not send DHCP packet: %s\n", strerror(errno));
        perror("error: \n");
        return -1;
    }
    printf("SUCCESS\n");

}

void print_options(uint32_t options[])
{
    for ( int i = 0; i < VARIABLE_L; i ++ )
    {
        if ( options[i] == 0 || options[i] == 0xff )
        {
            break;
        }
        printf( "%d ", options[i] );
    }
    printf("END\n\n");
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




/*** end of dhcp.c ***/
