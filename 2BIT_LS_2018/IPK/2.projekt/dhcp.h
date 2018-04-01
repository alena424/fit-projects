/*
* headers to work with DHCP server
* @date
* @author
*/

#define VARIABLE_L 312 // length of variable in options in dhcp packet
#define MAC_ADDRESS_L 6 // length of mac address

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
#define SUBNET_MASK 1 // length 4 bytes
#define TIME_OFFSET 2 //length 4 bytes


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

//int dhcp_discover( dhcp_packet* dhcp, struct dhcp_lease *lease  );
//int dhcp_request( dhcp_packet* dhcp, struct dhcp_lease *lease );
int request_lease(int socket_send, int socket_recv, unsigned char * mac, unsigned char *dstmac, int max_tries, unsigned char *interface_mac, int iface_index);


 /*** end of dhcp.h ***/
