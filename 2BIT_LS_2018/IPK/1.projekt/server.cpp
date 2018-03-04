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

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#include <fcntl.h>

#define BUFSIZE 2024
#define LINEMAX 1024
#define FILESIZE 2024
#define TIME_STRING 100 // makro pro ukladani casu

using namespace std;


class MessageParse
{
private:
    string login = "";
    string option = "";
    stringstream message ;
    FILE *file;
    string output_data = "";
    string output = "";
    int key = 0;

    /**
    * decryptLogin - dekodovani loginu to naseho kodovani
    */
    void decryptLogin()
    {
        //cout <<"dekodovano, klic je" << this->key << endl;
        string log_crypt = this->login;
        for ( auto it = log_crypt.begin(); it < log_crypt.end(); it++ )
        {
            // prochazim string

            *it = *it - this->key;
        }
        //cout << "LOGIN DEC: " << log_crypt << endl;
        this->login = log_crypt;
    }

    /**
    * cryptLogin - zakodovani loginu to naseho kodovani US3LET
    */
    string cryptOutputData()
    {
        string log_crypt = this->output_data;
        for ( auto it = log_crypt.begin(); it < log_crypt.end(); it++ )
        {
            // prochazim string
            *it = *it + this->key;
        }
        //cout << "DATA: " << log_crypt;
        this->output_data = log_crypt;
    }

    /**
     * function getRecord
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
     * function search_login - return one information about login found in coulomb in file f
     * @param col - number of coulomb to specified output
     * @param strick - if it is set, we search all login, if not just prefix
    */
    void search_login( int col, bool strick )
    {
        char line[LINEMAX];
        int ret = 0;
        while ( fgets(line, LINEMAX, this->file) != NULL )
        {
            int i = 0;
            int hit = 0;
            //printf("%s", line);
            //cout << line;
            //cout << this->output_data  << endl;
            while ( this->login[i] != '\0' || line[i] != ':' )
            {
                // musime osetrit pripad, kdy nejsou striktni podminky a nezadame zadny prefix, delka je 1 - pouze '\0'
                if ( strick == false && login.length() == 1)
                {
                    //cout << "nenene" << endl;
                    this->output_data += line ;
                    break;
                }

                //cout << this->login[i] << endl;
                if ( this->login[i] == line[i] )
                {

                    hit++;
                    // strikne dodrzujeme login, u option N a F
                    if ( strick == true)
                    {
                        // pokud budeme chtit striktne cely login, musime porovnavat s delkou retezce
                        //cout << "HIT:" << hit << endl;
                        if ( hit == this->login.length() -1 && line[hit] == ':')
                        {
                            // bereme tento radek
                            //cout << "HIT:" << hit << endl;
                            //cout << this->login << endl;
                            this->output_data = getRecord( line, col );
                            return;
                        }
                    }
                    else
                    {
                        // staci at aspon 1 znak sedi
                        // length bere i \0, konec retezce

                        if ( hit == login.length() -1)
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
                //cout << "LINE erased:"<< line << endl;
                return line;
            }
        }
        return "";
    }
    void createFinalOutput()
    {
        cout << "adding headers.." << endl;
        this->output = "IPKAPP/1.1. RESPONSE OK \r\n";
        // jake je dnesni datum
        time_t time_now = time(0);
        struct tm tm = *gmtime(&time_now);
        char string_time[TIME_STRING];
        strftime(string_time, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
        this->output += string_time;
        if ( this->key == 3)
        {
            this->output += "Encrypted: US3LET\r\n";
        }
        this->output += "Content-Length: " + to_string ( this->output_data.length() ) + "\r\n";
        this->output += "Output: " +  this->option + "\r\n";

        //this->cryptOutputData();
        this->output += "Data: " + this->output_data + "\r\n";
        //cout << this->output << endl;
        // zbavime se vsech mezer nakonci
        this->output.erase(this->output.find_last_not_of(" \n\r\t") +1);
    }

public:

    /**
    * Konstruktor
    */
    MessageParse( stringstream& message )
    {
        this->message.str( message.str());
        this->file = fopen( "/etc/passwd", "r" );
        this->login += "xtesar";
        this->option += "N";
    }

    /**
     * function proceedOutput
     * @param option - [N|F|L]; N: prints names, F: prints information about home directory, L: print list of users specified by login
     * @param login - patter to search
     * @return final output string
    */
    void proceedOutput( )
    {
        string key = extractFromMess( "Encrypted" );
        this->option = extractFromMess( "ExpectedOutput" );
        this->login = extractFromMess( "Data" );

        // odmazeme mezery
        this->option.erase(this->option.find_last_not_of(" \n\r\t") +1);
        //this->login.erase(this->login.find_last_not_of(" \n\r\t") +1);

        //cout << "klic je :" << key << endl;
        // zjistujeme kodovani
        if ( ( key.find( "US3LET") ) != string::npos )
        {
            this->key = 3; // pro toto kodovani je klic 3
        }
        //this->decryptLogin();

        int ret = 0;
        if ( option.compare( "N" ) == 0)
        {
            search_login(4, true);

        }
        if ( option.compare( "F" ) == 0)
        {
            search_login(5,  true );
        }
        else
        {
            search_login( 999, false);

        }
        this->createFinalOutput();
    }

    string getOutputData()
    {
        return this->output_data;
    }
    string getOutput()
    {
        return this->output;
    }
    string getLogin()
    {
        return this->login;
    }
    string getOption()
    {
        return this->option;
    }

    ~MessageParse()
    {
    }

};

int main (int argc, const char * argv[])
{
    struct sockaddr_in server_socket;
    struct sockaddr_in client_socket;
    char str[INET6_ADDRSTRLEN];
    int port_number;

    if (argc != 3 || ( strcmp( argv[1], "-p" ) != 0 ) )
    {
        fprintf(stderr,"bad number of arguments: %s -p <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    port_number = atoi(argv[2]);

    // vytvarime soket klienta
    int welcome_socket;
    if ((welcome_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    printf("Iniclaizing\n");
    memset(&server_socket,0,sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = INADDR_ANY;
    server_socket.sin_port = htons(port_number);

    printf("Binding\n");
    if (( bind(welcome_socket, (struct sockaddr*)&server_socket, sizeof(server_socket))) < 0)
    {
        perror("ERROR: Unable to bind");
        exit(EXIT_FAILURE);
    }
    printf("Listening\n");
    if ((listen(welcome_socket, 1)) < 0)
    {
        perror("ERROR: Unable to listen");
        exit(EXIT_FAILURE);
    }
    while( true )
    {
        // delka soketu klienta
        socklen_t client_socket_len=sizeof(client_socket);
        int csocket = accept(welcome_socket, (struct sockaddr*)&client_socket, &client_socket_len);

        // nastaveni neblokujiciho soketu
        int flags = fcntl(csocket, F_GETFL, 0);
        int rc = fcntl(csocket, F_SETFL, flags | O_NONBLOCK);
        if (rc < 0)
        {
            perror("fcntl() failed");
            exit(EXIT_FAILURE);
        }

        if (csocket > 0)
        {
            /*if(inet_ntop(AF_INET, &client_socket.sin_addr, str, sizeof(str)))
            {
                printf("INFO: New connection:\n");
                printf("INFO: Client address is %s\n", str);
                printf("INFO: Client port is %d\n", ntohs(client_socket.sin_port));
            }*/
            // buffer pro prijem
            char buff[BUFSIZE] = "";

            int res = recv(csocket, buff, BUFSIZE,0);
            if ( res == 0 )
            {
                // pokud v res nic neni
                break;
            }

            FILE *f;
            f = fopen( "/etc/passwd", "r" );
            if ( f == NULL )
            {
                fprintf(stderr, "Problems with opening /etc/passwd/\n");
                exit(EXIT_FAILURE);
            }

            stringstream os;
            os.str( buff);

            MessageParse mess(os);
            mess.proceedOutput();
            string output = mess.getOutput();
            cout << "OUTPUT: " << mess.getOutput() << endl;
            cout << "Sleeping 10sec" << endl;
            cout << output.length() << endl;
            //sleep(10);
            // cout << "OUTPUT_DATA: " << mess.getOutputData() << endl;
            //cout << "OPTION: " << mess.getOption() << endl;
            //cout << "LOGIN: " << mess.getLogin() << endl;

            // vystupni out pro posilani dat klientovi
            char out[ output.length() ];
            strcpy(out, output.c_str() );
            send(csocket, out, strlen(out), 0);
            fclose(f);


        }
        else
        {
            printf(".");
        }
        printf("Connection to %s closed\n",str);
        close(csocket);
    }
}
