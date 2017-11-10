#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<string.h>

#define addr "serverfile"

int main()
{
	//creating socket
	int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("socket creation error\n");
		exit(1);
	}

	struct sockaddr_un seraddr,claddr;
	bzero((void *)&seraddr, sizeof(seraddr));
	bzero((void *)&claddr, sizeof(claddr));

	seraddr.sun_family = AF_UNIX;
	strcpy(seraddr.sun_path,addr);
	unlink(addr);

	//binding socket
	int temp = bind(sock, (struct sockaddr *)&seraddr, sizeof(struct sockaddr));
	if(temp < 0) {
		printf("Binding error\n");
		exit(2);
	}

	//creating pipe
	int fd[2];
	if (pipe(fd)) {
		printf("Pipe creation error\n");
		exit(3);
	}


	//close stdout and replace it by fd[1]
	close(1);
	int d = dup(fd[1]);
	close(fd[1]);

	//sending and receiving
	int csize = sizeof(claddr);
	char msg[100000];

	bzero((void *)msg,sizeof(msg));
	temp = recvfrom(sock, (void *)msg, 100000, 0, (struct sockaddr *)&claddr, &csize);
	system(msg);
	
	read(fd[0], msg, 100000);
	sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&claddr, csize);
	
	close(sock);
	close(d);
	close(fd[0]);
	
	return 0;
}
