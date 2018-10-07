#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pcap.h>

using namespace std;

void errorMessage(){
    cerr << "./myripsniffer -i <interface>";
}

int main(int argc, char *argv[])
{
    char *interface = NULL;

	bpf_u_int32 net;
	bpf_u_int32 mask;
	struct pcap_pkthdr header; // header of a packet
	const u_char *packet; // the actual packet
	

    int option = getopt(argc, argv, "i:");
//	printf("opt arg: %c", option);
    if ( option == 'i' ){
	//	printf("%s", optarg);
        interface = optarg;
    } else {
        errorMessage();
        exit(EXIT_FAILURE);
    }

    if ( interface == NULL){
        errorMessage();
        exit(EXIT_FAILURE);
    }
    //cout << "Hello world!" << endl;
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	dev = pcap_lookupdev(errbuf);
		if (dev == NULL) {
			fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
			return(2);
		}

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		 fprintf(stderr, "Can't get netmask for device %s\n", dev);
		 net = 0;
		 mask = 0;
	}
	printf("Device: %s, netmask: %d net: %d", dev, net, mask);

	pcap_t *handle;

//	printf("Device: %s\n", dev);
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	 if (handle == NULL) {
		 fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		 return(2);
	 }
	struct bpf_program fp;
	char filter_exp[] = "port 23";

	if ( pcap_compile(handle, &fp,filter_exp, 0, net) == -1 ){
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		 return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		 fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		 return(2);
	 }
	/* Grab a packet */
	packet = pcap_next(handle, &header);
	/* Print its length */
	printf("Jacked a packet with length of [%d]\n", header.len);
	/* And close the session */
	pcap_close(handle);
    return 0;
}
