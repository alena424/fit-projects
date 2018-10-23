#include "headers.h"
#include <net/if.h> // if_nametoindex

//-- RIP RESPONSE ---//
#define SIZE 20


typedef struct response_args{
  char interface[SIZE]; //-i
  struct in6_addr addr; //-r
  struct in6_addr next_hop; //-n
  int prefix_int;
  int route_tag; //-t
  int number_of_hops;

} response_args;


void send_response(response_args *arguments);
