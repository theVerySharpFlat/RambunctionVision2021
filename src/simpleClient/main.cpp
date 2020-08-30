// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread;
	struct sockaddr_in serv_addr; 
	char buffer[1024] = {0}; 

    if (argc < 3) {
		printf("Too few argumets");
		return -1;
	}

	const char *address = argv[1];
	int port = atoi(argv[2]);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, address, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    while (true) {

        memset(&buffer, 0, sizeof(buffer));

		std::cout << "Request: ";
        std::cin >> buffer;

	    send(sock , &buffer , sizeof(buffer) , 0 ); 

	    valread = read( sock , buffer, 1024); 
		std::cout << "Response: ";
	    printf("%s\n",buffer ); 
    }
	return 0; 
} 