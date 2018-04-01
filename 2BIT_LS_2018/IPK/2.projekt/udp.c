/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

#include "headers.h"
#include "udp.h"

int init_ether_header(struct ethhdr* eth,
		const unsigned char* srcmac,
		const unsigned char* dstmac)
{
	assert(eth);
	assert(srcmac);
	assert(dstmac);

	memcpy(&eth->h_source, srcmac, sizeof(eth->h_source));
	memcpy(&eth->h_dest, dstmac, sizeof(eth->h_dest));
	eth->h_proto = htons(ETH_P_IP);

	return sizeof(struct ethhdr);
}

/*
 * Return checksum of IP header. `words' is number of 2-byte words stored in
 * `data'.
 */
unsigned short ip_csum(unsigned short* data, int words)
{
	unsigned int sum, i;

	assert(data);
	assert(words > 0);

	sum = 0;
	for (i = 0; i < words; i++)
		sum += data[i];
	sum += sum >> 16;

	return ~sum;
}

/*
 * Initialize IP header. `len' is data size (UDP header and payload for UDP
 * packet). `srchost' and `dsthost' are in network byte order.
 *
 * Always return IP header size.
 */
int init_ip_header(struct iphdr* ip,
		size_t len,
		unsigned short proto,
		uint32_t srchost,
		uint32_t dsthost)
{
	assert(ip);

	ip->ihl = sizeof(struct iphdr) / 4;
	ip->version = 4;
	ip->tot_len = htons(sizeof(struct iphdr) + len);
	ip->frag_off |= htons(IP_DF);	/* do not fragment */
	ip->ttl = 64;
	ip->protocol = proto;
	ip->saddr = srchost;
	ip->daddr = dsthost;
	ip->check = 0; /* set to 0 before checksum computation */
	ip->check = ip_csum((unsigned short*) ip, sizeof(struct iphdr) >> 1);

	return sizeof(struct iphdr);
}



/*
 * Return UDP checksum (header and payload). `words' contains number of 2-byte
 * words stored in `data'. `srchost' and `dsthost' are source and destination IP
 * address respectively (network byte order). `udplen' is UDP header and data
 * length in bytes without padding byte (host byte order).
 */
unsigned short udp_csum(unsigned short* data,
		int words,
		unsigned int srchost,
		unsigned int dsthost,
		unsigned short udplen)
{
	unsigned int sum, i;

	/* UDP pseudo-header for checksum computation */
	struct udp_pseudo_hdr {
		uint32_t srchost;
		uint32_t dsthost;
		uint8_t zero;
		uint8_t proto;
		uint16_t length;
	} pseudohdr;
	unsigned short* pseudoptr = (unsigned short*) &pseudohdr;

	assert(data);
	assert(words > 0);

	memset(&pseudohdr, 0, sizeof(pseudohdr));
	pseudohdr.srchost = srchost;
	pseudohdr.dsthost = dsthost;
	pseudohdr.proto = IPPROTO_UDP;
	pseudohdr.length = htons(udplen);

	/*
	 * This code is the same as in ip_csum except that UDP pseudoheader is
	 * calculated also.
	 */
	sum = 0;
	for (i = 0; i < sizeof(pseudohdr) / 2; i++)
		sum += pseudoptr[i];
	for (i = 0; i < words; i++)
		sum += data[i];
	sum += sum >> 16;

	return ~sum;
}

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

	/* if data length is odd add padding byte to compute UDP checksum */
	if (datalen % 2 != 0) {
		packet_len ++;
		udplen ++;
		udppadded = 1;
	}

	if (packet_len > bufflen)
		return -1;

	memset(buffer, 0, packet_len);
	memcpy(udpdata, data, datalen);

	udp->source = srcport;
	udp->dest = dstport;
	udp->len = htons(udplen);
	udp->check = udp_csum((unsigned short*) udp,
			udplen / 2,
			srchost,
			dsthost,
			udppadded ? udplen - 1 : udplen);

	init_ip_header(ip, sizeof(struct udphdr) + datalen, IPPROTO_UDP,
			srchost, dsthost);
	init_ether_header(eth, srcmac, dstmac);

	return packet_len;
}

