/**
 * @author Alena Tesarova
 * @name Klient-server pro ziskani informace o uzivatelich
 * @date 02.03.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


#define BUFSIZE 1024
#define MAX_LOGIN 10

int main (int argc, const char * argv[]) {

    // zpracovani argumentu
    int full_name_N = 0;
    int home_dir_F = 0;
    int list_users_L = 0;
    const char *login;

    const char *regex_login ;

    const char *option ;
    int port_number = 0; //cislo portu
    const char *server_hostname;

    // priklad ./ipk-client -h host -p port [-n|-f|-l] login
    if ( argc <= 5 || argc >= 8 || ( strcmp( argv[1], "-h" ) != 0 ) || ( strcmp( argv[3], "-p" ) != 0 ) ){
        // argumentu je minimalne 6 a maximalne 7
        fprintf(stderr, "ERROR: Bad format of arguments\n, example: ./%s -h host -p port [-n|-f|-l] login\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // kontrola [-n|-f|-l]

    if ( strcmp( argv[5], "-n" ) == 0 )  {
        full_name_N = 1;
        option = "N";
    } else if ( strcmp( argv[5], "-f" ) == 0 )  {
         list_users_L = 1;
         option = "F";
    } else if ( strcmp( argv[5], "-l" ) == 0 )  {
        home_dir_F = 1;
        option = "L";
    } else {
        fprintf(stderr, "ERROR: Bad format of arguments\n, example: ./%s -h host -p port [-n|-f|-l] login\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_hostname = argv[2];
    port_number = atoi(argv[4]);

    if ( argc == 7 && strlen(argv[6]) < 10) {
        regex_login = argv[6];
        login = "1";
    } else {
         regex_login = "";
          login = "0";
    }

    // pripojeni
	int client_socket;
    socklen_t serverlen;

    struct hostent *server;
    struct sockaddr_in server_address;


    // ziskani adresy serveru pomoci DNS
    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no host named %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }

    // nalezeni IP adresy serveru
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET; // pro protokoly ipv4

    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number); // prevedeni do spravneho tvaru

    // tiskne informace o vzdalenem soketu pak smazat
    printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    // Vytvoreni soketu, SOCK_STREAM je pro TCP
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		perror("ERROR: socket failed");
		exit(EXIT_FAILURE);
	}


    // posilani zpravy na server, specifikovat, co po nem chceme
    char buf[BUFSIZE] = "";
    bzero(buf, BUFSIZE);
    strcat(buf, option);
    strcat(buf, login);
    strcat(buf, regex_login);
    buf[strlen(buf)] = '\0';
    option = NULL;
    regex_login = NULL;
    printf("delka je: %d", strlen(buf));

    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
		perror("ERROR: connect");
		exit(EXIT_FAILURE);
    }

	// odeslani zpravy na server
	int send_resp = send(client_socket, buf, strlen(buf), 0);
    if (send_resp < 0)
      perror("ERROR in sendto");

    // prijeti odpovedi
    char message[BUFSIZE] = "";
    int receive_mess = recv(client_socket, message, BUFSIZE, 0);
    if (receive_mess < 0)
      perror("ERROR in recvfrom");

    printf("Echo from server: %s\n", message);

    close(client_socket);
    return 0;
}
