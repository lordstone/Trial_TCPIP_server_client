/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    cout << "Server has started preparation..." << endl;

    // define all the objs
     int sockfd, newsockfd, portno;

     // define the sockelength
     socklen_t clilen;

     // define buffer string
     char buffer[256];

     // define socket addr, both server and client
     struct sockaddr_in serv_addr, cli_addr;

     // define the status code int
     int n;

     // parse the arguments
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // setup the socket file desriptor for listening
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     // if the definition fails, throw
     if (sockfd < 0)
        error("ERROR opening socket");

     // set all server addr to be zeros
     bzero((char *) &serv_addr, sizeof(serv_addr));

     // put arg to port number
     portno = atoi(argv[1]);

     // set the server_addr incoming addr to be AF_INET (IPV4)
     serv_addr.sin_family = AF_INET;

     // set the incoming as wildcard ip addr
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     // set the incoming port to be port number
     serv_addr.sin_port = htons(portno); // htons is to convert unsigned
                                         // num to network byte order

     // try bind the socket fd to the server addr.
     // on failure, do error
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     cout <<"Server start listening ..." << endl;

relisten:

     // start listening
     listen(sockfd,5);

     // get the client addr len from cli-addr on request
     clilen = sizeof(cli_addr);

     // set up new socket to deal with client addr
     // to accept the client
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);

     // check error in new socket fd
     if (newsockfd < 0)
          error("ERROR on accept");

     // set all buffer to all zeros
     bzero(buffer,256);

     // try read from the newsock fd to buffer
     n = read(newsockfd,buffer,255);

     // if read is wrong, throw exception
     if (n < 0) error("ERROR reading from socket");

     // print msg from client side
     printf("Here is the message: %s\n",buffer);

     // try write to new sock fd
     n = write(newsockfd,"I got your message",18);

     // if write fails, throw
     if (n < 0) error("ERROR writing to socket");

     // try multiple listen
     if(buffer[0] != '0') goto relisten;

     // close the new created socket for request
     close(newsockfd);

     // close the listening socket
     close(sockfd);

     //terminate
     cout <<"server closed ..." << endl;
     return 0;
}
