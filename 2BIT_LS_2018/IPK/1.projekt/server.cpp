/**
 * @author Alena Teserova
 * @name Klient-server pro ziskani informace o uzivatelich
 * @date 02.03.2018
*/

//#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#include <fcntl.h>
#include <signal.h>

#define BUFSIZE 1025
#define LINEMAX 1024
#define FILESIZE 2024
#define PARSE_LENGTH 947 //maximlani delka dat, protoze hlavicka nejdelsi muze mit delku 75 - 2 pro jistotu

using namespace std;


class MessageParse
{
private:
    string login = "";
    string option = "";
    stringstream message ;
    FILE *file;
    string output_data = "";

    /**
     * method getRecord
     * @param line - one line of /etc/passwd that we want
     * @param col - number of coulomb to specified output
     * @return final output string
    */
    char *getRecord( char line[], int col )
    {
        // parsujeme string
        char *token;
        token = strtok(line, ":");
        int count = 0;
        while( token != NULL && count != col)
        {
            count++;
            token = strtok(NULL, ":");
        }
        printf("%s", token);
        return token;
    }

    /**
     * method search_login - return one information about login found in coulomb in file f
     * @param col - number of coulomb to specified output in /etc/passwd
     * @param strick - if it is set, we search login till ':' in etc/passwd, if not just prefix
    */
    void search_login( int col, bool strick )
    {
        char line[LINEMAX];
        int ret = 0;
        while ( fgets(line, LINEMAX, this->file) != NULL )
        {
            int i = 0;
            int hit = 0;
            while ( this->login[i] != '\0' || line[i] != ':' )
            {
                // musime osetrit pripad, kdy nejsou striktni podminky a nezadame zadny prefix, delka je 0
                if ( strick == false && login.length() == 0)
                {
                    this->output_data += line ;
                    break;
                }

                if ( this->login[i] == line[i] )
                {

                    hit++;
                    // strikne dodrzujeme login, u option N a F
                    if ( strick == true)
                    {
                        // pokud budeme chtit striktne cely login, musime porovnavat s delkou retezce
                        if ( hit == this->login.length()  && line[hit] == ':')
                        {
                            // bereme tento radek
                            this->output_data = getRecord( line, col );
                            this->error = false;
                            return;
                        }
                    }
                    else
                    {
                        // staci at aspon 1 znak sedi
                        if ( hit == login.length() )
                        {
                            // bereme tento radek
                            this->output_data += line ;
                        }
                    }
                }
                else
                {
                    break; // nerovna, muzu preskocit
                }
                i++;
            }

        }
    }
     /**
     * method extractFromMess - extract one information specified by extract
     * @param toExtract - string to get information from, for example Data: - gets all data in out message
    */
    string extractFromMess( string toExtract )
    {
        string line;
        while ( getline( this->message, line))
        {
            int index = line.find( toExtract + ": " ); // hledame informaci, kteru chceme zobracit
            if ( index != string::npos )
            {
                // nasli jsme
                line.erase( 0, toExtract.length() +2 ); // +1 jedna dvojtecka, +1 mezera
                return line;
            }
        }
        return "";
    }
    /**
     * method addHeader - adds header to data (length, status, data)
     * @param data - to this data we are adding header
    */
    string addHeader(string data)
    {
        string status = "OK";
        cout << "stat: " << this->error << endl;
        // pokud se jedna o list, nevypisujeme nic o neexistujicim loginu
        if ( this->error )
        {
            //neexistuje loogin a neni varianta L
            status = this->err_message;
        }
        string header_plus_data =  "IPKAPP/1.1. RESP: " + status + "\n";
        //header_plus_data += "MESSAGE: " + to_string( number_message ) + "/" + to_string( this->number_of_messages ) + "\n";
        header_plus_data += "Content-Length: " + to_string ( data.length() ) + "\n";
        header_plus_data += "Data: " +  data ;
        return header_plus_data;
    }

public:
    bool error = true;
    string err_message = "Unexisting login"; // defaultni chybova hlaska

    /**
    * Konstruktor
    * saves string to stringstream this->message nad opens file
    */
    MessageParse( stringstream& message )
    {
        this->message.str( message.str());
        this->file = fopen( "/etc/passwd", "r" );
    }

    /**
     * method proceedOutput - Saves correct data to this->output_data according to option
    */
    void proceedOutput( )
    {
        this->option = extractFromMess( "ExpectedOutput" );
        this->login = extractFromMess( "Data" );
        // odmazeme mezery
        this->option.erase(this->option.find_last_not_of(" \n\r\t") +1);
        this->login.erase(this->login.find_last_not_of(" \n\r\t") +1);

        int ret = 0;
        this->error = true; // defaultne chyba
        cout << "option: " << this->option << endl;
        if ( this->option.compare( "N" ) == 0)
        {
            // 4. sloupec
            search_login(4, true);
            cout << "stat1: " << this->error << endl;
        }
        if ( this->option.compare( "F" ) == 0)
        {
            // 5. sloupec
            search_login(5,  true );
        }
        else if ( this->option.compare( "L" ) == 0)
        {
            // chceme vse => 999
            search_login( 999, false);
            this->error = false; // nikdy neni chyba pokud se jedna o L

        }
    }

    /**
     * method sendData - sending all data (means output_data + header)
    */
    bool sendData(int csocket)
    {
        string all_data = this->addHeader(this->output_data);
        int all_data_length = all_data.length();
        int once = 1;
        while (once)
        {
            cout << all_data << endl;
            int send_resp = send(csocket, all_data.c_str(), all_data_length, 0);
            if (send_resp < 0)
            {
                cerr << "ERROR in sendto";
                return false;

            }
            else if ( send_resp == 0 )
            {
                return true;
            }
            all_data.erase(0, send_resp);
            once = 0;
        }
    }

    ~MessageParse()
    {
    }

};

int csocket;
void quit(int signal)
{
    close(csocket);
    exit(signal);
}

int main (int argc, const char * argv[])
{
    signal(SIGINT, quit);
    struct sockaddr_in server_socket;
    struct sockaddr_in client_socket;
    char str[INET6_ADDRSTRLEN];
    int port_number;

    /* argumenty kontrola */
    if (argc != 3 || ( strcmp( argv[1], "-p" ) != 0 ) )
    {
        cerr <<"bad number of arguments: "<< argv[0] << "-p <port>" << endl;
        exit(EXIT_FAILURE);
    }
    port_number = atoi(argv[2]);

    // vytvarime soket klienta
    int welcome_socket;
    if ((welcome_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "ERROR: socket" << endl;
        exit(EXIT_FAILURE);
    }

    memset(&server_socket,0,sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = INADDR_ANY;
    server_socket.sin_port = htons(port_number);

    if (( bind(welcome_socket, (struct sockaddr*)&server_socket, sizeof(server_socket))) < 0)
    {
        cerr << "ERROR: Unable to bind" << endl;
        exit(EXIT_FAILURE);
    }

    if ((listen(welcome_socket, 1)) < 0)
    {
        cerr << "ERROR: Unable to listen" << endl;
        exit(EXIT_FAILURE);
    }

    while( true )
    {
        int error_resv = 0;
        // delka soketu klienta
        socklen_t client_socket_len=sizeof(client_socket);
        csocket = accept(welcome_socket, (struct sockaddr*)&client_socket, &client_socket_len);

        // nastaveni neblokujiciho soketu
        int flags = fcntl(csocket, F_GETFL, 0);
        int rc = fcntl(csocket, F_SETFL, flags | O_NONBLOCK);
        if (rc < 0)
        {
            cerr << "fcntl() failed" << endl;
            exit(EXIT_FAILURE);
        }

        if (csocket > 0)
        {
            char buff[BUFSIZE] = "";
            memset(buff, 0, BUFSIZE);

            while (true)
            {
                int res = recv(csocket, buff, BUFSIZE,0);
                if ( res <= 0 )
                {

                    break;
                }

                else if ( res == EAGAIN )
                {
                    cerr << "." << endl;
                    continue;
                }
            }
            cout << buff << endl;
            if ( strlen(buff) == 0)
            {
                error_resv = 1; // dame klientovi vedet, ze je neco spatne
            }

            FILE *f;
            f = fopen( "/etc/passwd", "r" );
            if ( f == NULL )
            {
                cerr << "Problems with opening /etc/passwd/" << endl;
                exit(EXIT_FAILURE);
            }

            stringstream os;
            os.str( buff);
            MessageParse mess(os);
            if ( error_resv )
            {
                mess.err_message = "Get no data from client, probably problem in send on client side or recv on server side, please, try again";
                mess.error = 1;
            } else {
                mess.proceedOutput();
            }
            mess.sendData( csocket);
            fclose(f);
        }
        else
        {
            printf(".");
        }
        close(csocket);
    }
}
