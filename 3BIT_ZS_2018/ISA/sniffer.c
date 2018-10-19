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

#define PASSWORD 16

int counter = 0; // packet counter

void print_header(const struct pcap_pkthdr *header)
{
    counter++;
    printf( "Packet no. %d:\n", counter);
    printf("\tLength %d, received at %s",header->len,ctime((const time_t*)&header->ts.tv_sec));
}
void print_command(int command, char *version_string)
{
    char command_string[20];
    switch( command)
    {
    case 1:
        strcpy(command_string, "REQUEST");
        break;
    case 2:
        strcpy(command_string, "RESPONSE");
        break;
    default:
        fprintf( stderr, "Comamnd is %d.", command);
        return;

    }
    printf("\t--- RIP info ---\n");
    printf("\tVersion: %s\n",version_string );
    printf("\tCommand: %s\n", command_string);
}

void print_header_ripng6(rip_h *rip_header)
{
    char version_string[20];
    strcpy(version_string, "RIPng");
    print_command((int) rip_header->command, version_string);

}
void print_header_rip(rip_h *rip_header)
{
    char version_string[20]; // will contain name version string


    switch((int) rip_header->version)
    {
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
    print_command((int) rip_header->command, version_string);

}


void print_ether_type(u_short ether_type, const u_char *packet)
{
    struct ip *my_ip;
    //const struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
    const struct udphdr *my_udp;    // pointer to the beginning of UDP header
    u_int size_ip;
    //u_int size_udp;

    rip_h *rip_header;
    rip_entry *rip_entr;

    switch (ether_type)
    {
    case ETHERTYPE_IP: // IPv4 packet
    {
        printf("\tEthernet type is  0x%x, i.e. IP packet \n", ether_type );
        my_ip = (struct ip*) (packet+SIZE_ETHERNET);        // skip Ethernet header
        size_ip = my_ip->ip_hl*4;                           // length of IP header

        printf("\tIP: id 0x%x, hlen %d bytes, version %d, total length %d bytes, TTL %d\n",ntohs(my_ip->ip_id),size_ip,my_ip->ip_v,ntohs(my_ip->ip_len),my_ip->ip_ttl);
        printf("\tIP src = %s, ",inet_ntoa(my_ip->ip_src));
        printf("IP dst = %s",inet_ntoa(my_ip->ip_dst));

        printf(", protocol UDP (%d)\n",my_ip->ip_p);
        my_udp = (struct udphdr *) (packet+SIZE_ETHERNET+size_ip); // pointer to the UDP header
        //size_udp = my_udp->uh_ulen;
        printf("\tSrc port = %d, dst port = %d, length %d\n",ntohs(my_udp->uh_sport), ntohs(my_udp->uh_dport), ntohs(my_udp->uh_ulen));

        rip_header = (rip_h *) (packet + SIZE_ETHERNET + size_ip + UDP_LENGTH_HEADER);
        //printf("\tCommand: %d, RIP version: %d\n", rip_header->command, rip_header->version);


        print_header_rip(rip_header);
        rip_entr = (rip_entry *) ((u_char *) rip_header + LENGTH_RIP_HEADER);

        // udp contains rip, udp minus header = rip lenght
        int rip_length = (int)( ntohs(my_udp->uh_ulen) - UDP_LENGTH_HEADER);
        printf("\tRip length: %d\n", rip_length);

        //length of rip packet is length of udp packet minus udp s header
        //int rip_length = ntohs(my_udp->uh_ulen) - UDP_LENGTH_HEADER;
        // get entries

        for(int rip_bytes = LENGTH_RIP_ENTRY; rip_bytes < rip_length; rip_bytes = rip_bytes+LENGTH_RIP_ENTRY )
        {
            if ( rip_entr->family_identif == 0XFFFF )
            {
                char password_type[] = "Simple password";
                // rfc 4.1
                //he remaining 16 octets contain the plain text password.  If
                //the password is under 16 octets, it must be left-justified and padded
                //to the right with nulls
                char password[] = {0};
                //remainder of the entry contains the autentication RFC 4.1
                // - max 24 rip entries
                printf("\tAuthentication\n");


                if ( ntohs( rip_entr->route_tag) != SIMPLE_PASS ){
                    fprintf(stderr, "Invalid password\n");
                    return;
                }
                memcpy(password,rip_entr->data.rip_auth, PASSWORD );
                printf("\tType: %s\n", password_type);
                printf("\tPassword: %s\n", password);

            }
            else
            {
                char rip_ip[INET_ADDRSTRLEN]; // ip address
                char rip_mask[INET_ADDRSTRLEN]; // mask of address
                char rip_next_hop[INET_ADDRSTRLEN]; // next hop address
                // he inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4

                strcpy(rip_ip,inet_ntoa(rip_entr->data.table.rip_ip4));
                strcpy(rip_mask,inet_ntoa(rip_entr->data.table.rip_mask));
                strcpy(rip_next_hop,inet_ntoa(rip_entr->data.table.rip_next_hop));
                char family[20];

                if (  ntohs(rip_entr->family_identif) == AF_INET)
                {
                    strcpy(family, "IP");
                }

                printf("\tRouting Tag: %d\n \tAddress family identif: %s(%d)\n", rip_entr->route_tag, family,ntohs(rip_entr->family_identif) );

                if ( strcmp(family, "IP") == 0 )
                {
                    printf("\tID address is: %s\n", rip_ip );
                }

                if ( rip_header->version == 2 )
                {
                    printf("\tNetmask: %s\n\tNext hop: %s\n", rip_mask, rip_next_hop);
                }

                printf("\tMetric: %d\n\n", ntohl(rip_entr->data.table.rip_metric) );


            }
            rip_entr = (rip_entry *)((u_char *)rip_entr + LENGTH_RIP_ENTRY);
        }
        break;
    }


    case ETHERTYPE_IPV6:  // IPv6
        printf("\tEthernet type is 0x%x, i.e., IPv6 packet\n",ether_type);
        break;

    default:
        printf("\tEthernet type 0x%x, not IPv4\n", ether_type);
        return;
    }

}



void print_ether(const u_char *packet)
{
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
)
{
    print_header(header);
    print_ether(packet);

}

