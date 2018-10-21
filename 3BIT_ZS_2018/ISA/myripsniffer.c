/**
* myripsniffer.c
* @brief functions to parse and print RIP packets (RIPv1, RIPv2, RIPng)
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/

#include "headers.h"
#include "sniffer_h.h"


void errorMessage()
{
    fprintf( stderr, "./myripsniffer -i <interface>" );
}

int main(int argc, char *argv[])
{
    char *interface = NULL;

    bpf_u_int32 net; // network
    bpf_u_int32 mask; // computer mask

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
        return(EXIT_FAILURE);
    }

    if ( pcap_compile(handle, &fp,filter_exp, 0, net) == -1 )
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(EXIT_FAILURE);
    }
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(EXIT_FAILURE);
    }

    /* Grab a packet */
    // 0 - immense loop
    pcap_loop(handle, 0, print_packet, NULL);

    /* And close the session */
    pcap_close(handle);
    return 0;
}
/********************** end of myripsniffer.c ***************************/
