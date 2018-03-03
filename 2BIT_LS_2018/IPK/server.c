/**
 * @author Alena Teserova
 * @name Klient-server pro ziskani informace o uzivatelich
 * @date 02.03.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFSIZE 1024
#define LINEMAX 1024
#define FILESIZE 2024

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
 * function search - return one information about login found in coulomb in file f
 * @param login - login that we are interested in
 * @param col - number of coulomb to specified output
 * @param strick - if it is set, we search all login, if not just prefix
 * @return final output string
*/
void search(char login[], int col, FILE *f, bool strick, char *out )
{
    char line[LINEMAX];
    int ret = 0;
    while ( fgets(line, LINEMAX, f) != NULL )
    {
        int i = 0;
        int hit = 0;
        //printf("%s", line);
        while ( login[i] != '\0' || line[i] != ':' )
        {
            //printf("LOGIN[%d]: %c LINE[%d]: %c\n", i, login[i], i, line[i]);
            if ( login[i] == line[i] )
            {
                hit++;
            }
            i++;
        }
        if ( strick ) {
            // pokud budeme chtit striktne cely login, musime porovnavat s delkou retezce

            if ( hit == strlen(login) && line[hit] == ':')
            {
                printf("HIT: %d\n", hit);
                printf("%s", line);
                // bereme tento radek
                out = getRecord( line, col );
            }
        } else {
            // staci at aspon 1 znak sedi
            if ( hit == strlen(login))
            {
                printf("HIT: %d\n", hit);
                printf("%s", line);
                // bereme tento radek
                strcat(out, line );
            }

        }
        if (ret == 20)
        {
            printf("OUT: %s", out);
            return ;
        }
        ret++;
    }

}
/**
 * function proceedOutput
 * @param option - [N|F|L]; N: prints names, F: prints information about home directory, L: print list of users specified by login
 * @param login - patter to search
 * @return final output string
*/
void proceedOutput( char option, char login[], FILE *f, char *output )
{
    printf("LOGIN: %s DELKA: %d\n", login, strlen(login));
    int ret = 0;
    if ( option == 'N' )
    {
        search(login, 4, f, true, output);

    }
    else if ( option == 'F' )
    {
        search(login, 5, f, true, output);
    }
    else
    {
       search(login, 999, f, false, output);

    }
}

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
        if (csocket > 0)
        {
            if(inet_ntop(AF_INET, &client_socket.sin_addr, str, sizeof(str)))
            {
                printf("INFO: New connection:\n");
                printf("INFO: Client address is %s\n", str);
                printf("INFO: Client port is %d\n", ntohs(client_socket.sin_port));
            }
            char buff[BUFSIZE] = "";
            char login[100] = "";
            char out[BUFSIZE] = "";
           // bzero(out, BUFSIZE);

            int res = recv(csocket, buff, BUFSIZE,0);
            if ( buff[0] != 'N' &&   buff[0] != 'F' &&  buff[0] != 'L')
            {
                printf("Socket must start either with N, F or L\n",str);
                close(csocket);
                break;
            }
            if ( buff[1] == '1')
            {
                // pokud je zde 1 - znamena to, ze budeme cist login
                strncpy(login, buff + 2, strlen(buff)-2);
                buff[ strlen(buff) ] = '\0';
                printf("LOGIN: %s\n", login);
            }
            else
            {
                login[0] = '\0';
            }
            printf("Option %c\n", buff[0]);
            FILE *f;
            f = fopen( "/etc/passwd", "r" );
            if ( f == NULL )
            {
                fprintf(stderr, "Problems with opening /etc/passwd/\n");
                exit(EXIT_FAILURE);
            }
            proceedOutput( buff[0], login, f, out );
            printf("SENDING %s\n\n", out);
            send(csocket, out, strlen(out), 0);

        }
        else
        {
            printf(".");
        }
        printf("Connection to %s closed\n",str);
        close(csocket);
    }
}
