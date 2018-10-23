/**
* headers.h
* @brief contains RIP packets headers
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/
#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <string.h>

#include <pcap.h>
#include <netinet/ip.h>


using namespace std;

// RIP constant
/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14
#define ETHER_ADDR_LEN 6
#define UDP_LENGTH_HEADER 8
#define LENGTH_IPV6_HEADER 40

#define LENGTH_RIP_ENTRY 20
#define LENGTH_RIP_HEADER 4

#define REQUEST 1
#define RESPONSE 2

#define SIMPLE_PASS 2



typedef struct rip_h
{
    // command - https://tools.ietf.org/html/rfc2453
    // 1 - request (request for the responding system to send routing table)
    // 2 - response (contains routing table)
    uint8_t command;
    // Version of RIP protocol
    uint8_t version;
    // Zeros
    uint16_t zeros;
} rip_h;

struct routing_table{
            // IPv4 address
            struct in_addr rip_ip4;
            // Subnet mask
            struct in_addr rip_mask;
            // Next hop -- address of next hop or 0.0.0.0 (means originator of the RIP advertisement)
            struct in_addr rip_next_hop;
            // metric 1 - 15 (inclusive) - metric for the destination
            // 16 indicates infinity -- dest not reachable
            uint32_t rip_metric;

};

typedef struct rip_entry
{
    //address family identifier
    uint16_t family_identif;
    // Route Tag - method for semaration internal RIP routes from external RIP routes -- EGP/IGP
    uint16_t route_tag; // or authentication_type if family_addr: 0xFFFF
    union data
    {
        struct
        {
            // used if family_identif is 0xFFFF
            u_char rip_auth[16];
        };
        routing_table table;
    } data;
} rip_entry;


typedef struct ripv2
{
    rip_h header;
    rip_entry entry;
} ripv2;

typedef struct ripv2_auth
{
    rip_h header;
    rip_entry auth;
} ripv2_auth;

typedef struct ripng_entry{

    struct in6_addr prefix;
    uint16_t route_tag; // if set number of the autonomous system from whucg the router were learned
    uint8_t prefixLength;
    uint8_t metric;
}ripng_entry;

typedef struct ripng_entry_next{

    struct in6_addr next_hop;
    uint16_t route_tag; // if set number of the autonomous system from whucg the router were learned
    uint8_t prefixLength;
    uint8_t metric;
}ripng_entry_next;



/********************** end of headers.h ***************************/
