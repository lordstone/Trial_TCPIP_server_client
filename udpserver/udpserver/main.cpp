#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

void error(const char * msg){
	perror(msg);
	exit(1);
}

int startUdpServer(){
	// this is the main func for udp server

	// start creating socket obj
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0){
		error("Cannot create socket");
	}

	// define sockaddr_in struct
	struct sockaddr_in socket_addr;

	// set the mem to be all 0
	memset((char *)&socket_addr, 0, sizeof(socket_addr));

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(9292); // 9292 picked at random
	socket_addr.sin_addr = htonl(INADDR_ANY);

	//now we try to bind it
	if(bind(socket_fd, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) < 0){
		error("Cannot bind");
	}

	//time to process incoming msg


	return 0;
}

int main()
{
    cout << "Calling UDP module" << endl;
    return 0;
}
