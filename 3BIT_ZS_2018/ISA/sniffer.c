#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "headers.h"
#include "sniffer_h.h"

#include <string.h> //strcpy
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> 
#include <err.h>

// for Linux
#include <netinet/ether.h> 
#include <time.h>
#include <pcap/pcap.h>

int counter = 0; // packet counter

void print_header(const struct pcap_pkthdr *header){
	counter++;
	printf( "Packet no. %d:\n", counter);
	printf("\tLength %d, received at %s",header->len,ctime((const time_t*)&header->ts.tv_sec));  
}

void print_header_rip(rip_h *rip_header){
	char version_string[20]; // will contain name version string
	char command_string[20]; 

	switch((int) rip_header->version){
		case 1:
			strcpy(version_string, "RIPv1");
			break;
		case 2:
			strcpy(version_string, "RIPv2");
			break;
		default:
			fprintf( stderr, "RIP version is %d.", rip_header->version);
			return;
	}
	switch((int) rip_header->command){
		case 1:
			strcpy(command_string, "REQUEST");
			break;
		case 2:
			strcpy(command_string, "RESPONSE");
			break;
		default:
			fprintf( stderr, "Comamnd is %d.", rip_header->command);
			return;
	
	}
	printf("\t--- RIP info ---\n");
	printf("\tVersion: %s\n",version_string );
	printf("\tCommand: %s\n", command_string);
}

void print_ether_type(u_short ether_type, const u_char *packet){
	struct ip *my_ip;
  	//const struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
  	const struct udphdr *my_udp;    // pointer to the beginning of UDP header
  	u_int size_ip; 
	u_int size_udp; 
	rip_h *rip_header;
	rip_entry *rip_entr;

	switch (ether_type){              
		case ETHERTYPE_IP: // IPv4 packet
			printf("\tEthernet type is  0x%x, i.e. IP packet \n", ether_type );
			my_ip = (struct ip*) (packet+SIZE_ETHERNET);        // skip Ethernet header
			size_ip = my_ip->ip_hl*4;                           // length of IP header

			printf("\tIP: id 0x%x, hlen %d bytes, version %d, total length %d bytes, TTL %d\n",ntohs(my_ip->ip_id),size_ip,my_ip->ip_v,ntohs(my_ip->ip_len),my_ip->ip_ttl);
			printf("\tIP src = %s, ",inet_ntoa(my_ip->ip_src));
			printf("IP dst = %s",inet_ntoa(my_ip->ip_dst));

			printf(", protocol UDP (%d)\n",my_ip->ip_p);
			my_udp = (struct udphdr *) (packet+SIZE_ETHERNET+size_ip); // pointer to the UDP header
			size_udp = my_udp->uh_ulen;
			printf("\tSrc port = %d, dst port = %d, length %d\n",ntohs(my_udp->uh_sport), ntohs(my_udp->uh_dport), ntohs(my_udp->uh_ulen));
			
			rip_header = (rip_h *) (packet + SIZE_ETHERNET + size_ip + UDP_LENGTH_HEADER);
			//printf("\tCommand: %d, RIP version: %d\n", rip_header->command, rip_header->version); 

			print_header_rip(rip_header);

			//rip_entr = (rip_entry *) (packet + SIZE_ETHERNET + size_ip + UDP_LENGTH_HEADER + LENGTH_RIP_HEADER);
			//length of rip packet is length of udp packet minus udp s header
			//int rip_length = ntohs(my_udp->uh_ulen) - UDP_LENGTH_HEADER;
			
			break;

		case ETHERTYPE_IPV6:  // IPv6
			printf("\tEthernet type is 0x%x, i.e., IPv6 packet\n",ether_type);
			break;

		default:
			printf("\tEthernet type 0x%x, not IPv4\n", ether_type);
			return;
	} 

}



void print_ether(const u_char *packet){
	struct ether_header *eptr;

	eptr = (struct ether_header *) packet;
    	printf("\tSource MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost)) ;
    	printf("\tDestination MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_dhost)) ;
	print_ether_type( ntohs(eptr->ether_type), packet);
}

// callback function
void print_packet(
	u_char *args, 
	const struct pcap_pkthdr *header,
	const u_char *packet
){
	print_header(header);
	print_ether(packet);

}

