#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>

#define CUSTOM_SERVER_PORT 9292

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
	socket_addr.sin_port = htons(CUSTOM_SERVER_PORT); // 9292 picked at random
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	cout << "Now we are to bind ... " << endl;

	//now we try to bind it
	if(bind(socket_fd, (struct sockaddr *) &socket_addr, sizeof(socket_addr)) < 0){
		error("Cannot bind");
	}

	//time to process incoming msg


	return 0;
}

int startUdpClient(){

	// define necessary vars to get host
	struct hostent * myhost;
	char * host = "127.0.0.1";
	int n;

	cout << "Getting host..." << endl;
	// get the host
	myhost = gethostbyname(host);
	if(!myhost){
		error("Cannot obtain the address;");
	}

	// define sockaddr
	struct sockaddr_in servaddr;

	// reset the whole servaddr obj
	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(CUSTOM_SERVER_PORT);

	// now copy the addr to it
	memcpy((void*)&servaddr.sin_addr, myhost->h_addr_list[0], myhost->h_length);

	cout << "Now type in the message:";
	char * msg;
	cin >> msg;

	if(msg[0] == 'e'){
		cout << "Exiting on demand..." << endl;
		exit(0);
	}

	//defined socket fd
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//now we try to bind it
	if(bind(socket_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
		error("Cannot bind");
	}

	// now send the message through UDP
	if(sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		error("Send failed");
	}

	return 0;
}

int main(int argc, char * argv[])
{
    cout << "Parsing args module" << endl;
    if(argc != 2){
    	error("Wrong arg count, Exiting");
    }
    // cout << "argv1:" << argv[1] << endl;
    if(argv[1][0] == 's'){
    	cout << "Entering Server Mode..." << endl;
    	int statusCode = startUdpServer();
    }else if(argv[1][0] == 'c'){
    	cout << "Entering Client Mode..." << endl;
    	int statusCode = startUdpClient();
    }else{
    	error("Error in argument. Exiting");
    }

    return 0;
}
