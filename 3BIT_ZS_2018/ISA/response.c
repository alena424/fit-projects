#include "response.h"

#define PORT_RIP6 521

void send_response(response_args *arguments){
  printf("kukuku\n" );
  // we need to create packet
  u_char *packet;
  // allocate space for packet
  packet = (u_char *) malloc( LENGTH_RIP_HEADER + LENGTH_RIP_ENTRY );

  // header
  rip_h header;
  header.command = RESPONSE;
  header.version = 1;
  header.zeros = 0;

  // we need to copy info to packet
  memcpy( packet, &header, LENGTH_RIP_HEADER );

  // rip_entry
  ripng_entry rip_entr;

  rip_entr.route_tag = arguments->route_tag;
  rip_entr.prefixLength = arguments->prefix_int;
  rip_entr.metric = arguments->number_of_hops;
  rip_entr.prefix = arguments->addr;

  memcpy(packet, &rip_entr, LENGTH_RIP_ENTRY);

  // lets create socket, bind and send
  struct sockaddr_in6 my_addr, dest_addr;


  bzero(&my_addr, sizeof(my_addr));
  bzero(&dest_addr, sizeof(dest_addr));

  int my_socket = socket(AF_INET6, SOCK_DGRAM, 0);

  if ( (setsockopt(my_socket, SOL_SOCKET, SO_BINDTODEVICE, arguments->interface, strlen(arguments->interface) ))< 0 ){
    perror("Setsockopt failed\n");
  }
  int flag = 1;
 setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
 setsockopt(my_socket, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(int));

  my_addr.sin6_family = AF_INET6;
  //socket_struct.sin6_addr=in6addr_any;
//  struct in6_addr my_addr_pom = in6addr_any;

  char ip_addr[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &in6addr_any, ip_addr, sizeof(ip_addr) );;
  printf("My address is: %s\n", ip_addr);

  my_addr.sin6_addr = in6addr_any;
  my_addr.sin6_port =htons(PORT_RIP6);
  //my_addr.sin6_scope_id = 0;

  if ( (bind(my_socket, (struct sockaddr*) &my_addr, sizeof( my_addr )) ) < 0){

    perror("ERROR: socket failed");
  }

  dest_addr.sin6_family = AF_INET;
  dest_addr.sin6_port = htons(PORT_RIP6);

  char multicast_addr[] = "ff02::9";

  if ( ( inet_ntop(AF_INET6, &dest_addr.sin6_addr, multicast_addr, sizeof(multicast_addr) )) == 0 ){
    fprintf(stderr,  "inet top failed\n" );
  }

  //size_t *packet_size = LENGTH_RIP_ENTRY + LENGTH_RIP_HEADER;
  ssize_t size = sendto(my_socket, packet, LENGTH_RIP_ENTRY + LENGTH_RIP_HEADER, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
  fprintf( stderr, "size %d was send \n",size );
  close(my_socket);

}

/********************** end of response.c ***************************/
