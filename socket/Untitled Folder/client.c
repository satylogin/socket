#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<string.h>

#define caddr "clientfile"
#define saddr "serverfile"

int main()
{
	int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("socket creation error\n");
		exit(1);
	}

	struct sockaddr_un seraddr,claddr;
	bzero((void *)&seraddr, sizeof(seraddr));
	bzero((void *)&claddr, sizeof(claddr));

	seraddr.sun_family = AF_UNIX;
	strcpy(seraddr.sun_path, saddr);

	claddr.sun_family = AF_UNIX;
	strcpy(claddr.sun_path, caddr);
	unlink(caddr);

	//in unix datagram the client also binds to its local file
	int temp = bind(sock, (struct sockaddr *)&claddr, sizeof(claddr));
	if  (temp < 0) {
		printf("binding error\n");
		exit(3);
	}

	int ssize = sizeof(seraddr);
	char msg[100000];

	bzero((void *)msg, sizeof(msg));
	gets(msg);
	sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&seraddr, ssize);
	
	temp = recvfrom(sock, msg, 100000, 0, (struct sockaddr *)&seraddr, &ssize);
	msg[temp] = '\0';
	printf("%s", msg);

	close(sock);
	
	return 0;
}
