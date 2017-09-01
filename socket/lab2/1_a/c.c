#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 256

#define S_PORT 5500
#define M_PORT 5502

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main() 
{
	int c_sock, s_sock, m_sock;
	struct sockaddr_in s_addr, c_addr, m_addr;
	char buffer[SIZE+1];

	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		error("creating socket.");
	}
	printf("socket created.\n");

	bzero(&s_addr, sizeof(s_addr));
	bzero(&c_addr, sizeof(s_addr));
	bzero(&m_addr, sizeof(s_addr));

	s_addr.sin_family	= AF_INET;
	s_addr.sin_port		= htons(S_PORT);
	s_addr.sin_addr.s_addr	= INADDR_ANY;

	if (bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
		error("binding.");
	}
	printf("binded\n");

	if (listen(s_sock, 10) == -1) {
		error("listining.");
	}
	printf("listining.\n");

	unsigned int len = 0;
	if ((c_sock = accept(s_sock, (struct sockaddr*)&c_addr, &len)) == -1) {
		error("accepting client1 socket.");
	}
	printf("accepted client1 socket.\n");


	if ((m_sock = accept(s_sock, (struct sockaddr*)&m_addr, &len)) == -1) {
		error("accepting client2 socket.");
	}
	printf("accepted client2 socket.");

	int count;
	if ((count = recv(c_sock, buffer, SIZE, 0)) == -1) {
		error("recieving.");
	}
	printf("recieved: %c\n", buffer[0]);
	buffer[0]--;
	
	if ((count = send(m_sock, buffer, 1, 0)) == -1) {
		error("sending.");
	}
	printf("sent: %c\n", buffer[0]);

	close(c_sock);
	close(m_sock);
	close(s_sock);
	
	return 0;
}
