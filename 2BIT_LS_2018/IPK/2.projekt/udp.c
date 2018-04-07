/**
 * IPK 2. project
 * udp.c - fills udp packet
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

#include "udp.h"

unsigned char dstmac[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/**
* ip_csum - gets check sum for IP protocol
* @param    buf the IP header content
* @param    hdr_len the IP header length
* @Source   http://minirighi.sourceforge.net/html/ip_8c.html#a2
*/
uint16_t ip_csum( void* buf, size_t hdr_len)
{
    unsigned long sum = 0;
    const uint16_t *ip1;

    ip1 = buf;
    while( hdr_len > 1 ){
        sum += *ip1++;
        if ( sum & 0x80000000 )
            sum = (sum & 0xFFFF) + (sum >> 16);
        hdr_len -= 2;
    }
    while( sum >> 16 ){
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return ( ~sum );
}

/**
 * udp_csum calculate check sum of UDP
 * @param buff 	        the UDP packet
 * @param len 	The     UDP packet length
 * @param src_addr      the IP source address (in network format)
 * @param dest_addr     the IP destination address (in network format)
 * @Source              http://minirighi.sourceforge.net/html/udp_8c.html#a0
 */
unsigned short udp_csum(const void* buff,
		size_t len,
		in_addr_t src_addr,
		in_addr_t dest_addr)
{
	const uint16_t *buf=buff;
         uint16_t *ip_src=(void *)&src_addr, *ip_dst=(void *)&dest_addr;
         uint32_t sum;
         size_t length=len;

        // Calculate the sum
         sum = 0;
         while (len > 1)
         {
                 sum += *buf++;
                 if (sum & 0x80000000)
                         sum = (sum & 0xFFFF) + (sum >> 16);
                 len -= 2;
         }

         if ( len & 1 )
                 // Add the padding if the packet lenght is odd
                 sum += *((uint8_t *)buf);

         // Add the pseudo-header
         sum += *(ip_src++);
         sum += *ip_src;

         sum += *(ip_dst++);
         sum += *ip_dst;

         sum += htons(IPPROTO_UDP);
         sum += htons(length);

         // Add the carries
         while (sum >> 16)
                 sum = (sum & 0xFFFF) + (sum >> 16);

         // Return the one's complement of sum
         return ( (uint16_t)(~sum)  );
}

 /**
* init_udp_packet - funkction fills udp packet
* @param output contains of UDP packet
* @param dhcp data in dhcp structure
* @param srcmac source mac address (interface mac)
*/

size_t init_packet(unsigned char* output,
		const void* data,
		size_t datalen,
		const unsigned char* srcmac )
{
	size_t packet_len;
	unsigned short udplen;

	/* header and data pointers */
	struct ethhdr* eth = (struct ethhdr*) output;

	struct iphdr* ip = (struct iphdr*)(output + sizeof(struct ethhdr));
	struct udphdr* udp = (struct udphdr*)(output + sizeof(struct ethhdr) + sizeof(struct iphdr));
	unsigned char* udpdata = (unsigned char*) udp + sizeof(struct udphdr);

	packet_len = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + datalen;
	// UDP packet and data length
	udplen = sizeof(struct udphdr) + datalen;

	memset(output, 0, packet_len);
	memcpy(udpdata, data, datalen);

	udplen = sizeof(struct udphdr) + datalen;

    unsigned int srchost = 0;
    unsigned int dsthost =  0xffffffff;
    unsigned short dstport = htons(67);
    unsigned short srcport = htons(68);
	udp->source = srcport;
	udp->dest = dstport;
	udp->len = htons(udplen);
	udp->check = udp_csum((unsigned short*) udp,udplen, srchost, dsthost );

    /*
    * Internet Header protocol
    * page 10, RFC 791
    */
	ip->ihl = 5; // header length, minimum value for a correct header is 5: means 5 rows in message (each 4 bytes)
	ip->version = 4;
	ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + datalen); // total length,  16 bits
	ip->frag_off |= htons(IP_DF); //0x4000
	ip->ttl = 128; //maximum time the datagram is allowed to remain in the internet system
	ip->protocol = IPPROTO_UDP; // udp
	ip->saddr = srchost;
	ip->daddr = dsthost;
	ip->check = ip_csum((unsigned short*) ip, sizeof(struct iphdr) >> 1); // set to 0

    memcpy(&eth->h_source, srcmac, sizeof(eth->h_source));
	memcpy(&eth->h_dest, dstmac, sizeof(eth->h_dest));

	eth->h_proto = htons(ETH_P_IP);
	return packet_len;
}
/** end of udp.c */

