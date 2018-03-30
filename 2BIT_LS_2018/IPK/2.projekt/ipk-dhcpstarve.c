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

#define MAC_ADDRESS_L 6 // length of mac address
#define VARIABLE_L 312 // length of variable in options in dhcp packet

/*  message type */
#define BOOTREQUEST 1
#define BOOTREAPLY 2

/* Hardware address type */
#define HTYPE_ETHER 1

/* Hardware address length */
// 6 for 10mb ethernet
#define HLEN_ETHER 6
 
/* Broadcast flag */
// first broadcast bit is set to one
#define BROADCAST 0x8000

/* DHCP message type, page 24 RFC 1533 
  Code   Len  Type
  +-----+-----+-----+
  |  53 |  1  | 1-7 |
  +-----+-----+-----+
*/
#define DHCP_MESSAGE_TYPE 53
#define DHCP_MESSAGE_LEN 1
#define MSG_DISCOVER 	1
#define MSG_OFFER			2
#define MSG_REQUEST 	3
#define MSG_DECLINE 	4
#define MSG_ACK 			5
#define MSG_NACK			6
#define MSG_RELEASE 	7

/* DHCP options codes, from page 2 in RFC 1533 */
#define SUBNET_MASK 1 // length 4 octets
#define TIME_OFFSET 2 //length 4 octets

/* Options */
// option cookies - to differ beetween BOOTP and a DHCP message
int magic_cookies[] = { 99, 130, 83, 99 };
// option codes 128 - 256 are reserved for site-specific options
// end option is 255 and its length is 1 octet
 
// need to store leases and information
typedef struct dhcp_lease{
	uint32_t hmac; // matches to chaddr
	uint32_t client_addr; // matches to ciadd
	uint32_t server_id; // need to identify server, siaddr
	uint32_t xid;
	struct dhcp_lease *next;

} dhcp_lease;

// see RFC 2131, page 8

typedef struct dhcp_packet{
	uint8_t op; // message type, 1 - BOOTREQUEST, 2 - BOOTREAPLY
	uint8_t htype; // hardware address type
	uint8_t hlen; // hardware address length
	uint8_t htops; // client sets to zero, optionally used by relay agents
	uint32_t xid; // transaction id, a random number chosen by the client, used by the client and server to associate message and responses beteer a client and a server
	uint16_t sec; // filled in by a client, seconds since client began address getting or renewal
	uint16_t flags;			// must be set to zero, leftmost bit is defined as broadcast
	uint32_t ciaddr; // client IP address, only if client is in BOUND, RENEW, REBINDING state
	uint32_t yiaddr; // "your" client IP address
	uint32_t siaddr; // address of the server to use in the next step of the client's bootstrap process
	uint32_t giaddr; //realy agend IP address
	uint32_t chaddr; // client hardware address
	uint32_t sname[64]; //optional server host name
	uint32_t file[128]; // boot file name
	uint32_t options[VARIABLE_L];	
} dhcp_packet;




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
/*
* function request_lease
* @param socket_send socket we use to send DHCPDISCOVER packet
* @param socket_recv socket to get DHCPOFFER packet
* @param mac out mac address
* @param dstmac mac address of interface
*/
int request_lease( int socket_send, int socket_recv, unsigned char* mac, unsigned char* dstmac ){
			// we first creates dhcp packet

		}


int main ( int argc, char *argv[] ){
	/// inicialized random generator
	time_t t;
	srand( (unsigned) time(&t) );

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
	printf( "Interface is: %s\n", interfacename );

	// crete sockets to send and receive data
	int socket_recv = -1;
	int socket_send = -1;

	/* RECEIVE SOCKET */

	// soket on UDP
	if (( socket_recv = socket( PF_PACKET, SOCK_DGRAM, htons( ETH_P_IP ) ) ) <= 0 ){
		perror("Can not create recv socket\n");
		//fprintf( stderr, "Can not create recv socket" );
		return ( EXIT_FAILURE );

	}

	// set options to receive socket 
	int sockopt = 1;
	if ( setsockopt( socket_recv, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt ) ) != 0 ){
		perror("Can not set any options to receive socket\n");
		//fprintf( stderr, "Can not set any options to receive socket" );
		//close(socket_recv);
		return (EXIT_FAILURE);
	}
	/* SENDING SOCKET */

	if ( ( socket_send  = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ) ) == -1 ){
fprintf( stderr, "Can not create send socket\n" );
		return( EXIT_FAILURE );
	}
	// options for send socket
	sockopt = 1;

	if ( ( setsockopt( socket_send, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt )  ) ) != 0 ){
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
	if ( ioctl( socket_send, SIOCGIFINDEX, &ifr ) == -1 ) {
		fprintf( stderr, "ioctl error\n" );
		return( EXIT_FAILURE );
	}
	//index je v ifr.ifr_ifindex
	printf("Index of interface: %d\n", ifr.ifr_ifindex);

	// we need destination mac address
	if ( ioctl( socket_send, SIOCGIFHWADDR, &ifr ) == -1 ){
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
	destmac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
	printf("Mac : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , destmac[0], destmac[1], destmac[2], destmac[3], destmac[4], destmac[5] );
	//strncpy( destmac, ifr.ifr_hwaddr.sa_data, MAC_ADDRESS_L );
	//printf( "Destination mac address: %s\n", ifr.ifr_hwaddr.sa_data );
	/*
	struct dhcp_packet{
		!!!
	}
	   */
	int poc = 10;
	struct dhcp_lease* list = NULL;
	while(poc){
		poc--;
		/* list of leases
		need to send a DHCPDISCOVER packet with reqiest of IP addr
		reply to out request i s a DHCOFFER with offer if IP addr, client can choose one IP addr
		and request for the addr with DHCPREQUEST packet
		server returns DHCPACK as a reply
		*/
		//renew all leases 

		// generate mac address

		// request for lease

		// we not successful, brak while
	}
	// end




	
	return 0;
	
}
