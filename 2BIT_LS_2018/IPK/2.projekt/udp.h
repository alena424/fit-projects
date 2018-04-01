/**
 * IPK 2. project
 * @date 28.03.2018
 * @author Alena Tesarova
 *
 */

size_t init_udp_packet(unsigned char* buffer,
		size_t bufflen,
		const void* data,
		size_t datalen,
		const unsigned char* srcmac,
		unsigned int srchost,
		unsigned short srcport,
		unsigned char* dstmac,
		unsigned int dsthost,
		unsigned short dstport);


unsigned short udp_csum(unsigned short* data,
		int words,
		unsigned int srchost,
		unsigned int dsthost,
		unsigned short udplen);

int init_ip_header(struct iphdr* ip,
		size_t len,
		unsigned short proto,
		uint32_t srchost,
		uint32_t dsthost);


