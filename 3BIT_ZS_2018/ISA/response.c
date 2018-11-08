/**
* response.c
* @brief sned rip packets
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/
#include "response.h"

#define PORT_RIP6 521

void send_response(response_args *arguments){
  // we need to create packet
  u_char *packet;
  // allocate space for packet
  size_t length = LENGTH_RIP_HEADER + LENGTH_RIP_ENTRY + LENGTH_RIP_ENTRY ;
  packet = (u_char *) malloc( length);

  // rip header
  rip_h header;
  header.command = RESPONSE;
  header.version = 1;
  header.zeros = 0;

  // we need to copy info to packet
  memcpy( packet, &header, LENGTH_RIP_HEADER );

  // rip_entry
  ripng_entry rip_entr;
  // ripn entry with next hop
  ripng_entry_next rip_entr_next;

  rip_entr.route_tag = arguments->route_tag ;
  rip_entr.prefixLength = arguments->prefix_int;
  rip_entr.metric = arguments->number_of_hops;

  rip_entr.prefix = arguments->addr;

  rip_entr_next.route_tag = 0;
  rip_entr_next.prefixLength =0;
  rip_entr_next.metric = 0xFF;
  rip_entr_next.next_hop = arguments->next_hop;

  memcpy( (u_char * ) packet + LENGTH_RIP_HEADER, &rip_entr, LENGTH_RIP_ENTRY);
  memcpy( (u_char * ) packet + LENGTH_RIP_HEADER + LENGTH_RIP_ENTRY, &rip_entr_next, LENGTH_RIP_ENTRY);

  // lets create socket, bind and send
  struct sockaddr_in6 my_addr, dest_addr;

  bzero(&my_addr, sizeof(my_addr));
  bzero(&dest_addr, sizeof(dest_addr));

  int my_socket = 0;
  // bind socket
  if ( ( my_socket = socket(AF_INET6, SOCK_DGRAM, 0) ) < 0 ){
  	perror("socket failed\n");
  }

  int ifindex;
  ifindex =if_nametoindex( arguments->interface);
  my_addr.sin6_scope_id=ifindex;
  my_addr.sin6_family = AF_INET6;
  my_addr.sin6_port = htons(PORT_RIP6);
  my_addr.sin6_addr = in6addr_any;

  // socket options
  if ( setsockopt(my_socket, IPPROTO_IPV6, IPV6_MULTICAST_IF,&ifindex,sizeof(ifindex) ) < 0){
      perror("Setsockopt interface failed\n");
      return;
  }
  int hop = 255;
  // allows the hop limit for subsequent multicast datagrams to be set to any value from 0 to 255 -> we want maximum
  if ( setsockopt(my_socket, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,&hop,sizeof(hop) ) < 0){
      perror("Setsockopthop  failed\n");
      return;
  }

  if ( (bind(my_socket, (struct sockaddr*) &my_addr, sizeof( my_addr )) ) < 0){
    perror("ERROR: socket failed");
    return;
  }

  // destination address
  dest_addr.sin6_family = AF_INET6;
  dest_addr.sin6_port = htons(PORT_RIP6);
  dest_addr.sin6_scope_id = ifindex;

    // link local scope f02::9
  if ( ( inet_pton(AF_INET6, "ff02::9",&dest_addr.sin6_addr ) ) == 0 ){
    fprintf(stderr,  "inet top failed\n" );
  }

  sendto(my_socket, packet, length, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));

  close(my_socket);

}

/********************** end of response.c ***************************/
