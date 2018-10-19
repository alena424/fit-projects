/**
* @author Alena Tesarova (xtesar36@stud.fit.vutbr.cz)
* @date 21.10.2018
* projekt ISA 2018
*
*/

#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "headers.h"
#include "sniffer_h.h"


void errorMessage()
{
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

    char filter_exp[] = " (udp port 520 || udp port 521)"; // filter udp port 520 || udp port 521 RIPng
    int option = getopt(argc, argv, "i:");

    if ( option == 'i' )
    {
        interface = optarg;
    }
    else
    {
        errorMessage();
        exit(EXIT_FAILURE);
    }

    if ( interface == NULL)
    {
        errorMessage();
        exit(EXIT_FAILURE);
    }


    if (pcap_lookupnet(interface, &net, &mask, errbuf) == -1)
    {
        fprintf(stderr, "Can't get netmask for interfaceice %s\n", interface);
        net = 0;
        mask = 0;
    }

    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open interfaceice %s: %s\n", interface, errbuf);
        return(2);
    }

    if ( pcap_compile(handle, &fp,filter_exp, 0, net) == -1 )
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }
    if (pcap_setfilter(handle, &fp) == -1)
    {
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
