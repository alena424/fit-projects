/**
* myripresponse.c
* @brief
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/


#include "response.h"

#define MAX_ROUTE_TAG 65535

void errorMessage()
{
    fprintf( stderr, "./myripresponse -i <interface> -r <IPv6>/[16-128] {-n <IPv6>} {-m [0-16]} {-t [0-65535]}\n"
    "\t-i: <rozhraní> udává rozhraní, ze kterého má být útočný paket odeslán\n"
"\t-r: v <IPv6> je IP adresa podvrhávané sítě a za lomítkem číselná délka masky sítě\n"
"\t-m: následující číslo udává RIP Metriku, tedy počet hopů, implicitně 1\n"
"\t-n: <IPv6> za tímto parametrem je adresa next-hopu pro podvrhávanou routu, implicitně ::\n"
"\t-t: číslo udává hodnotu Router Tagu, implicitně 0\n" );
};

int main(int argc, char *argv[])
{
    char *interface = NULL; //-i
  //  char *addr_pom = NULL;

    struct in6_addr addr; //-r
    int prefix_int = 0;

    struct in6_addr next_hop; //-n
    //next_hop.s6_addr = { 0 };

    int route_tag = 0; //-t
    int number_of_hops = 1; //-m

    // end of string in conversion to integer
    char *pEnd = NULL;
    response_args arguments;

    // inicializace next hop
    char ip_addr_next_hop[INET6_ADDRSTRLEN] ;
    char hops_default[] = "::";
    inet_ntop(AF_INET6, &next_hop, hops_default, sizeof(hops_default));

    int option = 0;
    while( (option = getopt(argc, argv, "i:r:n:m:t:") ) != -1 ){
        switch (option){
          case 'i':
          {
            interface = optarg;
            printf("Interface is: %s\n",interface );
            break;
          }
          case 'r':
          {
            // we need to separate address from mask
            char *addr_pom = strtok(optarg, "/");

            if (addr_pom == NULL ){
              fprintf(stderr, "Invalid IP address");
              return (EXIT_FAILURE);
            }
            if (! inet_pton( AF_INET6, addr_pom, &addr) ){
              fprintf(stderr, "Invalid IP address");
              return (EXIT_FAILURE);
            }



            char * prefix = strtok(NULL, "/");


            if (prefix == NULL  ){
              fprintf(stderr, "Invalid mask");
              return (EXIT_FAILURE);
            }

            prefix_int = strtol(prefix, NULL, 0);
            if ( prefix_int == 0 || prefix_int < 16 || prefix_int > 128 ){
              fprintf(stderr, "Invalid mask");
              return (EXIT_FAILURE);
            }
            printf("Prefix is: %d\n", prefix_int);

            // need to convert prefix to address
            printf("r: %s\n",optarg );
            break;
          }
          case 'n':
          {

              if (! inet_pton( AF_INET6, optarg, &next_hop ) ){
                fprintf(stderr, "Invalid next hop address");
                return (EXIT_FAILURE);
              }
              // just for printing

              inet_ntop(AF_INET6, &next_hop, ip_addr_next_hop, sizeof(ip_addr_next_hop));
              printf("IP address next hop(n): %s\n",ip_addr_next_hop );

            break;
            }
          case 'm':
            {

              number_of_hops = strtol(optarg, &pEnd, 0);

              if ( *pEnd != '\0' || number_of_hops < 0 || number_of_hops > 16 ){
                fprintf(stderr, "Invalid parametr -m, must be a number from 0-16\n");
                return (EXIT_FAILURE);
              }
              printf("m: %d\n",number_of_hops );
              break;
          }
          case 't':
          {
            route_tag = strtol(optarg, &pEnd, 0);

            if ( *pEnd != '\0' || route_tag < 0 || route_tag > MAX_ROUTE_TAG ){
              fprintf(stderr, "Invalid parametr -t, must be a number from 0-65535\n");
              return (EXIT_FAILURE);
            }
              printf("t: %d\n",route_tag );
              break;
          }
        }
    }

    // just for printing, will be deleted
    char ip_addr[INET6_ADDRSTRLEN];
    if ( interface == NULL || prefix_int == 0 || ( inet_ntop(AF_INET6, &addr, ip_addr, sizeof(ip_addr) ) == NULL ) )
    {
        errorMessage();
        exit(EXIT_FAILURE);
    }
    strcpy( arguments.interface, interface );
    arguments.next_hop = next_hop;
    arguments.addr = addr;

    arguments.prefix_int = prefix_int;
    arguments.route_tag = route_tag;
    arguments.number_of_hops = number_of_hops;
    printf(" Interface %s\n Next hop %s\n addres: %s\n prefix int %d\n route tag: %d\n hops: %d\n",
    arguments.interface,
    hops_default,
    ip_addr,
    arguments.prefix_int,
    arguments.route_tag,
    arguments.number_of_hops );

    printf("IP address: %s\n",ip_addr );
    send_response(&arguments);

}



/********************** end of myripresponse.c ***************************/
