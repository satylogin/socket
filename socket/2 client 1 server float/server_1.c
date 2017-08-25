#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define L_SIZE 256
#define SIZE 512

#define S_PORT 5500
#define R_PORT 5502
#define M_PORT 5503

void error(char msg[])
{
	printf("Error in %s\n", msg);
	exit(0);
}

int main()
{
	int s_sock, tmp, m_sock;
	char buffer[SIZE+1];

	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (s_sock == -1 || m_sock == -1) {
		error("creating socket.");
	}
	printf("created socket.\n");

	struct sockaddr_in s_addr, m_addr, c2_addr;
	bzero(&s_addr, sizeof(s_addr));
	bzero(&m_addr, sizeof(m_addr));
	bzero(&c2_addr, sizeof(c2_addr));

	s_addr.sin_family 	= AF_INET;
	s_addr.sin_port 	= htons(S_PORT);
	s_addr.sin_addr.s_addr 	= INADDR_ANY;

	m_addr.sin_family 	= AF_INET;
	m_addr.sin_port 	= htons(M_PORT);
	m_addr.sin_addr.s_addr 	= INADDR_ANY;

	c2_addr.sin_family	= AF_INET;
	c2_addr.sin_port	= htons(R_PORT);
	c2_addr.sin_addr.s_addr	= INADDR_ANY;

	tmp = bind(s_sock, (const struct sockaddr*)&s_addr, sizeof(s_addr));
	if (tmp == -1) {
		error("binding server.");
	}

	tmp = bind(m_sock, (const struct sockaddr*)&m_addr, sizeof(m_addr));
	if (tmp == -1) {
		error("binding message.");
	}

	printf("biding done.\n");

	tmp = listen(s_sock, 10);

	if (tmp == -1) {
		error("listining.");	
	}
	printf("now llistining.\n");

	struct sockaddr_in c_addr;
	bzero(&c_addr, sizeof(c_addr));
	unsigned int len = 0;

	int n_sock = accept(s_sock, (void *)&c_addr, &len);

	if (n_sock == -1) {
		error("accepting connection.");
	}
	printf("client 1 accepted.\n");

	tmp = connect(m_sock, (const struct sockaddr*)&c2_addr, sizeof(c2_addr));
	if (tmp == -1) {
		error("connecting to server 2.");
	}
	printf("connected to server 2.\n");

	int count;
	float num;

	count = recv(n_sock, (void*)buffer, SIZE, 0);
	sscanf(buffer, "%f", &num);
	
	if (count == 0 || count == -1) {
		printf("error inn recieveing.\n");

	}
	printf("recieved: %.5f\n", num);
	num = num * sqrt(num);
	printf("sent: %.5f\n", num);
	
	sprintf(buffer, "%.5f", num);
	printf("%s %d\n", buffer, strlen(buffer));
	count = send(m_sock, buffer, strlen(buffer), 0);

	return 0;
}

