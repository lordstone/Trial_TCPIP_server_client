#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    // define socket file descriptor, port number, and n
    int sockfd, portno, n;

    // define server addr.
    struct sockaddr_in serv_addr;

    // define host ent
    struct hostent *server;

    cout << "Client start preparation ..." << endl;

    // set up the buffer to read TCP payload
    char buffer[256];

    // error detection for wrong number of arg count
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // set up port number
    portno = atoi(argv[2]);

    // set up socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // error detecion for setting up socket fd
    if (sockfd < 0)
        error("ERROR opening socket");

    // set up the server obj with the inputted addr
    server = gethostbyname(argv[1]);

    // if server is not found, then throw exception
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // bzero the
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // set the server addr to be AF_INET (IPV4)
    serv_addr.sin_family = AF_INET;

    // bcopy from
    bcopy((char *)server->h_addr,  // source
         (char *)&serv_addr.sin_addr.s_addr, // dest
         server->h_length);

    // set up the server-address port
    serv_addr.sin_port = htons(portno);

    // try to connect through sockfd for server-addr
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // completed connect function, ready to get msg from user
    printf("Please enter the message: ");

    // set all buffer to zero
    bzero(buffer,256);

    // put the stdin stuff into buffer
    fgets(buffer,255,stdin);

    // write the buffer content to sockfd
    n = write(sockfd,buffer,strlen(buffer));

    // error check, if the write failed, throw exception
    if (n < 0)
         error("ERROR writing to socket");

    // set all buffer to zeros
    bzero(buffer,256);

    // try read the sockfd to buffer
    n = read(sockfd,buffer,255);

    // error check, if the read failed, throw exception
    if (n < 0)
         error("ERROR reading from socket");

    // write the buffer content to screen
    printf("%s\n",buffer);


    // close the socket fd
    close(sockfd);

    // terminate
    cout << "ACK got, closing client ..." << endl;
    return 0;
}
