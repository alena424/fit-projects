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

#define BUFSIZE 2024  //nejvetsi soubor
#define MAX_LOGIN 10
#define TIME_STRING 100 // makro pro ukladani casu
#define KEY 3 //klic k kodovani

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
    * createMessage - zapozdreni informaci do zpravy
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

    int total_messages = 0; // aspon jedna pokud se pouziva tato trida
    int number_message = 0;
    string data = "";
    string error_message = "";

    Response( )
    {
        this->data = "";
    }
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
                //cout << "LINE erased:"<< line << endl;
                //cout << line;
                return (line);
            }
        }
        return "";
    }
    void parse( stringstream& message )
    {
        // potrebuju ji rozparsovat na map
        string line;
        map <string, string> received_loc;

        while ( getline( message, line) )
        {
            int index = line.find( ": " ); // hledame informaci, kteru chceme zobracit
            if ( index != string::npos )
            {
                // nasli jsme
                received_loc[ line.substr(0, index) ] = line.substr(index+2 ); // parsuju zpravu
            }
            else
            {
                //pokud cteme z dat, bereme cely radek
                received_loc[ "Data" ] += line;
            }
        }
        //cout << "PARSE: " << received_loc[ "Data" ] << endl;
    }

    //pridame si dalsi data to koncove zpravy
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

    bool login; // zda byl zadan login nebo ne
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
        login = "1";
    }
    else
    {
        regex_login = "";
        login = "0";
    }

    // pripojeni
    int client_socket;
    socklen_t serverlen;
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

    // tiskne informace o vzdalenem soketu pak smazat
    //printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    // Vytvoreni soketu, SOCK_STREAM je pro TCP
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("ERROR: socket failed");
        exit(EXIT_FAILURE);
    }

    // posilani zpravy na server, specifikovat, co po nem chceme
    char buf[BUFSIZE] = "";
    bzero(buf, BUFSIZE);

    Message mess ( option, regex_login );
    mess.createMessage();
    string output = mess.getMessage();
    //cout << output;
    strcpy(buf, output.c_str() );

    //printf("delka je: %d", strlen(buf));

    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
        cerr << "ERROR: connect" << endl;
        exit(EXIT_FAILURE);
    }

    // odeslani zpravy na server
    int send_resp = send(client_socket, buf, strlen(buf), 0);
    //cout << send_resp << endl;
    if (send_resp < 0)
    {
        cerr << "ERROR in sendto";
        close(client_socket);
        exit(EXIT_FAILURE);

    }

    // prijeti odpovedi
    int receive_mess = 0;
    Response resp;
    bool skip = false; // flag na podminku odpovedi
    while(true)
    {
        //cout << i << endl;
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
        //printf("\n\n___________________________\n");
        //printf("REC: %d \n", receive_mess);
        //cout << "MESS CHAR:" << message << endl;
        resp.data += message;

        // nejaka chyba napr. neexistujici login
        /*if ( resp.addData( message ) == false && (! skip ) )
        {
            cerr << "Unexisting login" << endl;
            return EXIT_FAILURE;
        }*/
        skip = true; // login je spravny
        //printf("___________________________\n");

    }
    string return_data = resp.returnData();
    if ( resp.error_message.length() > 0 ){
            cerr << resp.error_message << endl;
            return EXIT_FAILURE;
    } else {
        cout << return_data;
    }

    close(client_socket);
    return 0;
}
