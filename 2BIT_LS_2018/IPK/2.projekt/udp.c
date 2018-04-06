/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

#include "headers.h"
#include "udp.h"

/*
 * Initialize UDP packet and store data for sending in `buffer'. Return size of
 * data in `buffer' of -1 on error.
 *
 * Before call `data' contains actual data for sending (UDP payload) and
 * `datalen' its length. `srcmac' and `dstmac' are source and destination
 * hardware addresses (at least ETH_ALEN bytes long). `srchost' and `dsthost'
 * are source and destination IP addresses (network byte order). `srcport' and
 * `dstport' are source and destination ports (network byte order).
 */
 
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

size_t init_udp_packet(unsigned char* buffer,
		size_t bufflen,
		const void* data,
		size_t datalen,
		const unsigned char* srcmac,
		unsigned int srchost,
		unsigned short srcport,
		unsigned char* dstmac,
		unsigned int dsthost,
		unsigned short dstport)
{
	size_t packet_len;
	unsigned short udplen;
	int udppadded = 0;

	/* header and data pointers */
	struct ethhdr* eth = (struct ethhdr*) buffer;
	struct iphdr* ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
	struct udphdr* udp = (struct udphdr*)(buffer + sizeof(struct ethhdr) +
			sizeof(struct iphdr));
	unsigned char* udpdata = (unsigned char*) udp + sizeof(struct udphdr);


	/* whole packet length */
	packet_len = sizeof(struct ethhdr) + sizeof(struct iphdr) +
		sizeof(struct udphdr) + datalen;
	/* UDP packet and data length */
	udplen = sizeof(struct udphdr) + datalen;

	memset(buffer, 0, packet_len);
	memcpy(udpdata, data, datalen);

	udp->source = srcport;
	udp->dest = dstport;
	udp->len = htons(udplen);
	udp->check = 0;

    /*
    * Internet Header protocol
    * page 10, RFC 791
    */
	ip->ihl = 5; // header length, minimum value for a correct header is 5: means 5 rows in message (each 4 bytes)
	ip->version = 4;
	ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + datalen); // total length,  16 bits
	ip->frag_off |= htons(IP_DF); //0x4000
	ip->ttl = 64; //maximum time the datagram is allowed to remain in the internet system
	ip->protocol = IPPROTO_UDP; // udp
	ip->saddr = srchost;
	ip->daddr = dsthost;
	ip->check = 0; // set to 0 
    memcpy(&eth->h_source, srcmac, sizeof(eth->h_source));
	memcpy(&eth->h_dest, dstmac, sizeof(eth->h_dest));
	eth->h_proto = htons(ETH_P_IP);

	return packet_len;
}

