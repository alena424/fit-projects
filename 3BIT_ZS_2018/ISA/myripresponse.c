/**
* myripresponse.c
* @brief main file for rip response, process all arguments
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/

#include "response.h"

// maximum route tag
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

    struct in6_addr addr; //-r
    int prefix_int = 0;

    struct in6_addr next_hop; //-n

    int route_tag = 0; //-t
    int number_of_hops = 1; //-m

    // end of string in conversion to integer
    char *pEnd = NULL;

    // structure to save all arguments
    response_args arguments;

    // inicializace next hop
    inet_pton(AF_INET6, "::", &next_hop );
    int address_flag = 0; // was parametr -r chosen
    int option = 0;
    while( (option = getopt(argc, argv, "i:r:n:m:t:") ) != -1 )
    {
        switch (option)
        {
        case 'i':
        {
            interface = optarg;
            break;
        }
        case 'r':
        {
            // we need to separate address from mask
            char *addr_pom = strtok(optarg, "/");
            address_flag = 1;
            if (addr_pom == NULL )
            {
                fprintf(stderr, "Invalid IP address");
                return (EXIT_FAILURE);
            }

            if (! inet_pton( AF_INET6, addr_pom, &addr) )
            {
                fprintf(stderr, "Invalid IP address");
                return (EXIT_FAILURE);
            }

            // address prefix
            char * prefix = strtok(NULL, "/");

            if (prefix == NULL  )
            {
                fprintf(stderr, "Invalid mask");
                return (EXIT_FAILURE);
            }
            // get int from char
            prefix_int = strtol(prefix, NULL, 0);
            if ( prefix_int == 0 || prefix_int < 16 || prefix_int > 128 )
            {
                fprintf(stderr, "Invalid mask");
                return (EXIT_FAILURE);
            }
            break;
        }
        case 'n':
        {

            if (! inet_pton( AF_INET6, optarg, &next_hop ) )
            {
                fprintf(stderr, "Invalid next hop address");
                return (EXIT_FAILURE);
            }

            break;
        }
        case 'm':
        {
            // get int from char
            number_of_hops = strtol(optarg, &pEnd, 0);
            //controls
            if ( *pEnd != '\0' || number_of_hops < 0 || number_of_hops > 16 )
            {
                fprintf(stderr, "Invalid parametr -m, must be a number from 0-16\n");
                return (EXIT_FAILURE);
            }

            break;
        }
        case 't':
        {
            route_tag = strtol(optarg, &pEnd, 0);

            if ( *pEnd != '\0' || route_tag < 0 || route_tag > MAX_ROUTE_TAG )
            {
                fprintf(stderr, "Invalid parametr -t, must be a number from 0-65535\n");
                return (EXIT_FAILURE);
            }
            //printf("t: %d\n",route_tag );
            break;
        }
        }
    }

    // did we get all the compulsory arguments?
    if ( interface == NULL )
    {
        fprintf(stderr, "Parameter -i is compulsory!\n");
        errorMessage();
        return(EXIT_FAILURE);
    }
    // no -r parametr
    if (! address_flag)
    {
        fprintf(stderr, "Parameter -r is compulsory!\n");
        errorMessage();
        return(EXIT_FAILURE);
    }


    // save to structure and proceed
    strcpy( arguments.interface, interface );
    arguments.next_hop = next_hop;
    arguments.addr = addr;
    arguments.prefix_int = prefix_int;
    arguments.route_tag = route_tag;
    arguments.number_of_hops = number_of_hops;

    // send packet
    send_response(&arguments);

}



/********************** end of myripresponse.c ***************************/
