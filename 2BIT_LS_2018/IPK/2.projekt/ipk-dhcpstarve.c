/*
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>

#define MAC_ADDRESS_L 6

int main ( int argc, char *argv[] ){
	// enter: ./ipk-dhcpstarve -i interface
	// interface: name of interface to generate attack
	if ( argc != 3 ){
		fprintf(stderr, "Bad number of arguments");
		return(EXIT_FAILURE);
	}
	if ( strcmp( argv[ 1 ], "-i" ) != 0 ){
		fprintf(stderr, "Bad number of arguments");
		return(EXIT_FAILURE);
	}
	char interfacename[50];
	strncpy( interfacename, argv[2], sizeof( interfacename ) );
	printf( "Interface is: %s", interfacename );

	// crete sockets to send and receive data
	int socket_recv = -1;
	int socket_send = -1;

	/* RECEIVE SOCKET */

	// soket on UDP
	if ( socket_recv = socket( PF_PACKET, SOCK_DGRAM, htons( ETH_P_IP ) ) == -1 ){
		fprintf( stderr, "Can not create recv socket" );
		return ( EXIT_FAILURE );

	}

	// set options to receive socket 
	int sockopt = 1;
	if ( setsockopt( socket_recv, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt ) ) != 0 ){
		fprintf( stderr, "Can not set any options to receive socket" );
		//close(socket_recv);
		return (EXIT_FAILURE);
	}
	/* SENDING SOCKET */

	if ( ( socket_recv = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ) ) == -1 ){
fprintf( stderr, "Can not create send socket\n" );
		return( EXIT_FAILURE );
	}
	// options for send socket
	sockopt = 1;
	if ( setsockopt( socket_send, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt ) ) != 0 ){
		fprintf( stderr, "Ca not not set any options to socket\n");
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
	if ( ioctl( socket_send, SIOCGIFINDEX, &ifr ) == -1 ) {
		fprintf( stderr, "ioctl error\n" );
		return( EXIT_FAILURE );
	}
	//index je v ifr.ifr_ifindex
	printf("Index of interface: %d\n", ifr.ifr_ifindex);

	// we need destination mac address
	if ( socket_send, SIOCGIFHWADDR, &ifr ){
		fprintf(stderr, "ioctl error\n");
		return( EXIT_FAILURE );
	}
	// mac adresa v ifr.ifr_hwaddr.sa_data (unsigned char *)
	unsigned char * destmac;
	destmac = malloc( MAC_ADDRESS_L);
	if ( destmac == NULL ){
		fprintf( stderr, "Error in malloc\n" );
		return(EXIT_FAILURE);
	}
	strncpy( destmac, ifr.ifr_hwaddr.sa_data, MAC_ADDRESS_L );
	printf( "Destination mac address: %s\n", destmac );
	/*
	struct dhcp_packet{
		!!!
	}
	   */
	while(1){
		//renew all leases 

		// generate mac address

		// request for lease

		// we not successful, brak while
	}
	// end




	
	return 0;
	
}
