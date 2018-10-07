#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pcap.h>

#include <netinet/ip.h>

/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14

using namespace std;
int counter = 0; // packet counter

struct sniff_ip {
		u_char ip_vhl;		/* version << 4 | header length >> 2 */
		u_char ip_tos;		/* type of service */
		u_short ip_len;		/* total length */
		u_short ip_id;		/* identification */
		u_short ip_off;		/* fragment offset field */
	#define IP_RF 0x8000		/* reserved fragment flag */
	#define IP_DF 0x4000		/* dont fragment flag */
	#define IP_MF 0x2000		/* more fragments flag */
	#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
		u_char ip_ttl;		/* time to live */
		u_char ip_p;		/* protocol */
		u_short ip_sum;		/* checksum */
		struct in_addr ip_src,ip_dst; /* source and dest address */
};

/**
* @brief Gets version of protocol
* @param IPv4 header
* @return version of protocol
*/
u_char getVersion(sniff_ip *header){
	return header->ip_vhl >> 4;
}

/**
* @brief Gets version of protocol
* @param IPv4 header
* @return version of protocol
*/
u_char getLengthHeader(sniff_ip *header){
	return header->ip_vhl >> 2;
}

/**
* @brief Gets length header from ip_vhl
* @param header IPv4 header
* @return lengthof header
*/
int getLength(sniff_ip *header){
	return (header->ip_vhl & 0x0f) * 4;
}

// callback function
void print_packet(
	u_char *args, 
	const struct pcap_pkthdr *header,
	const u_char *packet
){
	counter++;
	printf( "%d. Header length is: %d and cap len is %d\n", counter,header->len, header->caplen);
	sniff_ip* ip_hdr;
	
	ip_hdr = (sniff_ip *)(packet + SIZE_ETHERNET);
 	printf("Version is: %d, length is: %d\n", getVersion(ip_hdr), getLengthHeader(ip_hdr));	
	//const struct sniff_ip* ip = (struct sniff_ip*)(packet + SIZE_ETHERNET)	
}

void errorMessage(){
    cerr << "./myripsniffer -i <interface>";
}

int main(int argc, char *argv[])
{
    char *interface = NULL;

	bpf_u_int32 net; // network
	bpf_u_int32 mask; // computer mask
	//struct pcap_pkthdr header; // header of a packet
	//const u_char *packet; // the actual packet
	
	struct bpf_program fp; // compiled pcap with filter
	pcap_t *handle; // handler
	char  errbuf[PCAP_ERRBUF_SIZE]; //errors

	char filter_exp[] = ""; // filter
    int option = getopt(argc, argv, "i:");
    
if ( option == 'i' ){
	//	printf("%s", optarg);
        interface = optarg;
    } else {
        errorMessage();
        exit(EXIT_FAILURE);
    }

    if ( interface == NULL){
        errorMessage();
        exit(EXIT_FAILURE);
    }
	

	if (pcap_lookupnet(interface, &net, &mask, errbuf) == -1) {
		 fprintf(stderr, "Can't get netmask for interfaceice %s\n", interface);
		 net = 0;
		 mask = 0;
	}

	handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
	 if (handle == NULL) {
		 fprintf(stderr, "Couldn't open interfaceice %s: %s\n", interface, errbuf);
		 return(2);
	 }

	if ( pcap_compile(handle, &fp,filter_exp, 0, net) == -1 ){
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		 return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		 fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		 return(2);
	 }

	/* Grab a packet */
	// 0 - immense loop
	pcap_loop(handle, 0, print_packet, NULL);
	/* Print its length */
	
	//const struct sniff_ethernet *ethernet; /* The ethernet header */
	//const struct sniff_ip *ip; /* The IP header */
	//const struct sniff_tcp *tcp; /* The TCP header */
	//const char *payload; /* Packet payload */

	//u_int size_ip;
	//u_int size_tcp;

	// poradne rozebereme paket
	//ethernet = (struct sniff_ethernet*)(packet);
	/*printf("ether: %s", ethernet->ether_dhost);
	const struct sniff_ip* ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)

	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return 0;
	}
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	
	printf( "time \t length \t protocol \t ttl \t ");
	printf("-------------------------------------------\n");

	printf("%d \t%d \t %c \t %c \t \n", header.ts, header.len, ippp_p, ip->ip_ttl);
	*/
	/* And close the session */
	pcap_close(handle);
    return 0;
}
