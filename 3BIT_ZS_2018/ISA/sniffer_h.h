/**
* sniffer_h.h
* @brief contains headers of functions belonging to RIP sniffer
* @author Alena Tesarova, xtesar36@stud.fit.vutbr.cz
* @date 20.11.2018
* project ISA 2018
*/

/**
* @brief Function prints message header - packet number and length
* @param header packet header
*/
void print_header(const struct pcap_pkthdr *header);

/**
* @brief Function prints RIP command and version
* @param command RIP command e.g.Request, Response
* @param version_string version of RIP packet
*/
void print_command(int command, char *version_string);

/**
* @brief Function prints IPV6 header
* @param header packet header
*/
void print_header_ripng6(rip_h *rip_header);

/**
* @brief Function prints TPv4 header
* @param header packet header
*/
void print_header_rip(rip_h *rip_header);

/**
* @brief Function parses IPv4 packet
* @param packet captured packet
*/
int ipv4_process( const u_char *packet);

/**
* @brief Function parses IPv6 packet
* @param packet captured packet
*/
int ipv6_process(const u_char *packet);

/**
* @brief Function parses packet
* @param packet captured packet
*/
void parse_packet( const u_char *packet);

/**
* @brief Function prints destination and source MAC address
* @param packet captured packet
*/
void print_ether(const u_char *packet);

/**
* @brief Callback function prints destination and source MAC address
* @param args arguments
* @param header packet given from pcap lib
* @param packet captured packet
*/
void print_packet( u_char *args, const struct pcap_pkthdr *header,const u_char *packet );

/********************** end of sniffer_h.h ***************************/
