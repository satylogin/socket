#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512

int main()
{
	struct sockaddr_in 	myaddr;
	struct sockaddr_in 	clientaddr;
	int 			addrlen;

	// create a socket and get the file descripter
	// AF_INET 	- defines family.
	// SOCK_STREAM	- since we are creating stream socket
	// 0		- since the protocol is defined by default
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	// check for error
	if (sock == -1) {
		printf("Unable to create socket.\n");
		exit(1);
	}
	printf("Socket created successfully\n");

	/**
	 * struct sockaddr_in {
	 * 	sa_family_t    sin_family; // address family: AF_INET 
	 * 	in_port_t      sin_port;   // port in network byte order
	 * 	struct in_addr sin_addr;   // internet address 
	 * };
	 */
	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family	= AF_INET;
	myaddr.sin_port 	= htons(5500);
	myaddr.sin_addr.s_addr	= INADDR_ANY;

	// bind the created socket
	int tmp = bind(sock, (const struct sockaddr*) &myaddr, sizeof(myaddr));

	// check for error
	if (tmp == -1) {
		printf("bind error\n");
		exit(2);
	}

	tmp = listen(sock, 10);
	
	// check for error
	if (tmp == -1) {
		printf("Listen error\n");
		exit(3);
	}

	bzero(&clientaddr, sizeof(clientaddr));
	
	unsigned int len = 0;

	int new_id = accept(sock, (void*)&clientaddr, &len);

	// check for error
	if (new_id == -1) {
		printf("Error in new id\n");
		exit(1);
	}
	
	char buffer[BUFFER_SIZE+1];

	while (1) {
		int count = recv(new_id, (void*)buffer, BUFFER_SIZE, 0);
		
		if (count <= 0) {
			printf("Error in recieve.\n");
			break;
		}
		if (strcmp(buffer, "bye") == 0) {
			printf("client exiting.\n");
			break;
		}
		printf("msg recv - %s\n", buffer);
		bzero(buffer, sizeof(buffer));
	}

	// close the file pointed by sock
	close(sock);
	close(new_id);

	printf("Closed main socket\n");

	return 0;
}
