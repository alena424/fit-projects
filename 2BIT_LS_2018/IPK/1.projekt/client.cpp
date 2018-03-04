/**
 * @author Alena Tesarova
 * @name Klient-server pro ziskani informace o uzivatelich
 * @date 02.03.2018
*/


#include <iostream>
#include <string.h>

#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define BUFSIZE 1024 //nejvetsi soubor
#define MAX_LOGIN 10
#define TIME_STRING 100 // makro pro ukladani casu
#define KEY 3 //klic k kodovani

class Message {
private:
    string option;
    string login;
    string message;
    /**
    * cryptLogin - zakodovani loginu to naseho kodovani US3LET
    */
    void cryptLogin( ){
        string log_crypt = this->login;
        for ( auto it = log_crypt.begin(); it < log_crypt.end(); it++ ) {
            // prochazim string
            *it = *it + KEY;
        }
        cout << "LOGIN: " << log_crypt;
        this->login = log_crypt;
    }
public:

    /**
    * Konstruktor
    */
    Message(string option, string login){
        this->login = login;
        this->option = option;
        this->message = "";

    }

    /**
    * createMessage - zapozdreni informaci do zpravy
    */
    void createMessage(){
        this->message = "IPKAPP/1.1.\r\n";

        // jake je dnesni datum
        time_t time_now = time(0);
        struct tm tm = *gmtime(&time_now);
        char string_time[TIME_STRING];
        strftime(string_time, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
        this->message += string_time;
        this->message += "Encrypted: US3LET\r\n";
        this->message += "Content-Length: " + to_string ( login.length() ) + "\r\n";
        this->message += "ExpectedOutput: " +  option + "\r\n";
        this->message += "Data: " + this->login + "\r\n";

    }
    string getMessage(){
        return this->message;
    }

};


int main (int argc, const char * argv[]) {

    bool login; // zda byl zadan login nebo ne
    string regex_login ; // zadany login v argumentecg
    string option ; // jaky vystup nas bude zajimat

    int port_number = 0; //cislo portu

    const char *server_hostname;

    // priklad ./ipk-client -h host -p port [-n|-f|-l] login
    if ( argc <= 5 || argc >= 8 || ( strcmp( argv[1], "-h" ) != 0 ) || ( strcmp( argv[3], "-p" ) != 0 ) ){
        // argumentu je minimalne 6 a maximalne 7
        cerr << "ERROR: Bad format of arguments\n, example: " << argv[0] << " -h host -p port [-n|-f|-l] login\n" << endl ;
        exit(EXIT_FAILURE);
    }
    // kontrola [-n|-f|-l]

    if ( strcmp( argv[5], "-n" ) == 0 )  {
        option = "N";
    } else if ( strcmp( argv[5], "-f" ) == 0 )  {
         option = "F";
    } else if ( strcmp( argv[5], "-l" ) == 0 )  {
        option = "L";
    } else {
        cerr << "ERROR: Bad format of arguments\n, example: " << argv[0] << " -h host -p port [-n|-f|-l] login\n" << endl ;
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
        cerr <<"ERROR: no host named " << server_hostname << "\n" << endl;
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

    //strcat(buf, option);
    //strcat(buf, login);
    //strcat(buf, regex_login);
    //buf[strlen(buf)] = '\0';
    Message mess ( option, regex_login );
    mess.createMessage();
    string output = mess.getMessage();
    cout << output;
    strcpy(buf, output.c_str() );

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
