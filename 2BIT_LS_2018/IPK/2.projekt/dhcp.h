/**
* dhcp.h headers to work with DHCP server
* @date 03.04.2018
* @author Alena Tesarova
*/

#define VARIABLE_L      312 // length of variable in options in dhcp packet
#define MAC_ADDRESS_L   6 // length of mac address

/*  message type */
#define BOOTREQUEST     1
#define BOOTREAPLY      2

/* Hardware address type */
#define HTYPE_ETHER     1

/* Hardware address length */
// 6 for 10mb ethernet
#define HLEN_ETHER      6

/* Broadcast flag */
// first broadcast bit is set to one
#define BROADCAST       0x8000

/* DHCP message type, page 24 RFC 1533
  Code   Len  Type
  +-----+-----+-----+
  |  53 |  1  | 1-7 |
  +-----+-----+-----+
*/
#define DHCP_MESSAGE_TYPE   53
#define DHCP_MESSAGE_LEN    1
#define MSG_DISCOVER 	    1
#define MSG_OFFER			2
#define MSG_REQUEST 	    3
#define MSG_DECLINE 	    4
#define MSG_ACK 			5
#define MSG_NACK			6
#define MSG_RELEASE 	    7

/* DHCP options codes, from page 2 in RFC 1533 might be needed */
/*#define SUBNET_MASK 1 // length 4 bytes
#define TIME_OFFSET 2 //length 4 bytes*/

extern unsigned char ifmac[6];

// see RFC 2131, page 8
typedef struct dhcp_packet
{
    uint8_t op;
    uint8_t htype;
    uint8_t hlen;
    uint8_t hops;
    uint32_t xid;
    uint16_t secs;
    uint16_t flags;
    uint32_t ciaddr;
    uint32_t yiaddr;
    uint32_t siaddr;
    uint32_t giaddr;
    uint8_t chaddr[16];
    uint8_t sname[64];
    uint8_t file[128];
    uint8_t options[VARIABLE_L];
} dhcp_packet;

int request_lease(int sock_send, const unsigned char* mac, int interface_index );



/*** end of dhcp.h ***/
