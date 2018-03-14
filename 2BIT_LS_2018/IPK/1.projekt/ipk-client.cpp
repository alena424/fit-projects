/**
 * @author Alena Tesarova
 * @name Klient-server pro ziskani informace o uzivatelich
 * @date 02.03.2018
*/


#include <iostream>
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>


#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define BUFSIZE 1024  //nejvetsi soubor
#define TIME_STRING 100 // makro pro ukladani casu
#define OK 3 // velikost prijmane zpravy od server, bud OK nebo OK

class Message
{
private:
    string option;
    string login;
    string message;

public:

    /**
    * Konstruktor
    */
    Message(string option, string login)
    {
        this->login = login;
        this->option = option;
        this->message = "";

    }

    /**
    * createMessage - adds header and create final message to send to server
    */
    void createMessage()
    {
        this->message = "IPKAPP/1.1.\r\n";

        // jake je dnesni datum
        time_t time_now = time(0);
        struct tm tm = *gmtime(&time_now);
        char string_time[TIME_STRING];
        strftime(string_time, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
        this->message += string_time;
        this->message += "Content-Length: " + to_string ( login.length() ) + "\r\n";
        this->message += "ExpectedOutput: " +  option + "\r\n";
        this->message += "Data: " + this->login + "\n";
    }
    string getMessage()
    {
        return this->message;
    }

};
class Response
{
public:

    string data = "";
    string error_message = "";

    Response( ) {}

    /**
    * getData - extracts information sent from server and extracts data that we need
    * @param message - string to extract from
    * @param toExtract - what to extract
    * @return extracted string
    * @example message = "Data: xlogin36" ; toExtract = "Data: " ; return => xlogin36
    */
    string getData(string message, string toExtract)
    {
        // jedem po radku dokud nenajdeme contentlength

        stringstream os;
        os.str(message);
        string line;
        while ( getline( os, line))
        {
            int index = line.find( toExtract ); // hledame informaci, kteru chceme zobracit
            if ( index != string::npos )
            {
                // nasli jsme
                line.erase( 0, index +toExtract.length() );
                line.erase(line.find_last_not_of(" \n\r\t") +1); // zbavime se koncovych mezer
                return (line);
            }
        }
        return "";
    }

    //pridame si dalsi data to koncove zpravy
    /**
    * returnData - extracts information sent from server and extracts data that we need
    * @return string to put on stdout
    */
    string returnData( )
    {
        string newdata = this->data;
        string data_status = getData( newdata, "RESP: " );
        int index = newdata.find( "Data: ");

        if ( data_status.compare("OK") != 0 )
        {
            // neni to ok
            this->error_message = data_status;
            return "";
        }
        int data_length = stoi(getData( newdata, "Content-Length: " ) );
        if ( index != string::npos )
        {
            return newdata.substr(index+6, data_length);
        }
        return "";
    }
};


int main (int argc, const char * argv[])
{
    string regex_login ; // zadany login v argumentecg
    string option ; // jaky vystup nas bude zajimat

    int port_number = 0; //cislo portu

    const char *server_hostname;

    // priklad ./ipk-client -h host -p port [-n|-f|-l] login
    if ( argc <= 5 || argc >= 8 || ( strcmp( argv[1], "-h" ) != 0 ) || ( strcmp( argv[3], "-p" ) != 0 ) )
    {
        // argumentu je minimalne 6 a maximalne 7
        cerr << "ERROR: Bad format of arguments\n, example: " << argv[0] << " -h host -p port [-n|-f|-l] login\n" << endl ;
        exit(EXIT_FAILURE);
    }
    // kontrola [-n|-f|-l]
    if ( strcmp( argv[5], "-n" ) == 0 )
    {
        option = "N";
    }
    else if ( strcmp( argv[5], "-f" ) == 0 )
    {
        option = "F";
    }
    else if ( strcmp( argv[5], "-l" ) == 0 )
    {
        option = "L";
    }
    else
    {
        cerr << "ERROR: Bad format of arguments\n, example: " << argv[0] << " -h host -p port [-n|-f|-l] login\n" << endl ;
        exit(EXIT_FAILURE);
    }

    server_hostname = argv[2];
    port_number = atoi(argv[4]);

    if ( argc == 7 && strlen(argv[6]) < 10)
    {
        regex_login = argv[6];
    }
    else
    {
        regex_login = "";
    }

    // pripojeni
    int client_socket;
    //socklen_t serverlen;
    struct hostent *server;
    struct sockaddr_in server_address;


    // ziskani adresy serveru pomoci DNS
    if ((server = gethostbyname(server_hostname)) == NULL)
    {
        cerr <<"ERROR: no host named " << server_hostname << "\n" << endl;
        exit(EXIT_FAILURE);
    }

    // nalezeni IP adresy serveru
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET; // pro protokoly ipv4

    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number); // prevedeni do spravneho tvaru

    // Vytvoreni soketu, SOCK_STREAM je pro TCP
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("ERROR: socket failed");
        exit(EXIT_FAILURE);
    }

    // posilani zpravy na server, specifikovat, co po nem chceme
    Message mess ( option, regex_login );
    mess.createMessage(); // vytvorime zpravu
    string output = mess.getMessage();

    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
        cerr << "ERROR: connect" << endl;
        exit(EXIT_FAILURE);
    }

    // odeslani zpravy na server dokud je server neprijme tzn. dokud nedostaneme odpoved od serveru OK
    char ok[OK];
    bool correct = false; // priznak, ze server prijmul data
    while(true && (!correct) )
    {
        // posilam dokud server neposle OK
        int send_resp = send(client_socket, output.c_str(), output.length(), 0);
        if (send_resp < 0)
        {
            cerr << "ERROR in send";
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // cekame na prijmuti OK nebo KO
        while(true)
        {
            int receive_mess_ok = recv(client_socket, ok, OK, 0);
            if ( receive_mess_ok > 0 )
            {
                if ( strcmp(ok, "OK") == 0)
                {
                    // muzeme ukoncit
                    correct = true;
                }
                else
                {
                    //cout << "Posilam znova" << endl;
                }
                break;
            }
        }
    }

    /* prijeti odpovedi */
    int receive_mess = 0;
    Response resp;
    while(true)
    {
        char message[BUFSIZE] = "";
        memset(message, 0, BUFSIZE);
        receive_mess = recv(client_socket, message, BUFSIZE, 0);

        if (receive_mess < 0)
        {
            cerr <<"ERROR in recv" << endl;
            exit(EXIT_FAILURE);
        }
        else if ( receive_mess == 0 )
        {
            break;
        }
        resp.data += message;
    }

    // zpracujeme data na vystup
    string return_data = resp.returnData();
    cout << return_data ;

    close(client_socket);
    return 0;
}

/*** konec ipk-client.cpp ***/
