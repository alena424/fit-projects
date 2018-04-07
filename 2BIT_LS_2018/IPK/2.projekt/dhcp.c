/**
* dhcp.c functions to work with DHCP server
* @date 30.3.2018
* @author Alena Tesarova
*/

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>

//#include "lease.h"
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

    dhcp->flags |= htons(BROADCAST); //broadcast flag
    memcpy(&dhcp->chaddr, mac, sizeof(mac));
    memcpy(dhcp->options, magic_cookies, sizeof(magic_cookies));

    size_t len = sizeof(MSG_DISCOVER);
    int i = 4;
    dhcp->options[i++] = DHCP_MESSAGE_TYPE;
    dhcp->options[i++] = 1; //1 bit
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
int request_lease(int sock_send,const unsigned char* mac, int interface_index )
{
    struct dhcp_packet dhcp;
    size_t dhcplen;
    unsigned char dstmac[] = { 0xff, 0xff, 0xff, 0xff,0xff, 0xff };

    struct sockaddr_ll socket_addr;
    memset( &socket_addr, 0, sizeof( struct sockaddr_ll ) );
    socket_addr.sll_family = AF_PACKET;
    socket_addr.sll_halen = ETH_ALEN;
    socket_addr.sll_hatype = 1;
    socket_addr.sll_ifindex = interface_index;
    socket_addr.sll_pkttype = PACKET_BROADCAST;
    char packet_udp[1024];
    size_t bufflen;

    /* DHCPDISCOVER */
    dhcp_make_discover(&dhcp, mac );

    dhcplen = sizeof(dhcp);
    struct sockaddr *addrr = (struct sockaddr*) &socket_addr;

    bufflen = init_udp_packet(packet_udp, sizeof(packet_udp),&dhcp,dhcplen,ifmac, 0, htons(68), broadcast_mac, 0xffffffff, htons(67));

    if (bufflen == -1)
    {
        printf("nothing in packet_udp\n");
        return EXIT_FAILURE;
    }

    int sent_b = sendto(sock_send, packet_udp, bufflen, 0, (struct sockaddr *)addrr, sizeof( struct sockaddr_ll ) );
    if (sent_b <= 0)
    {
        perror("Error in send \n");
        return EXIT_FAILURE;
    }
}

/*** end of dhcp.c ***/
