#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define oops(msg) do{ perror(msg); exit(1); } while(0)

#define PORTNUM 43210

int main(int argc, char *argv[])
{
	int sock_id;
	int n_read;
	char buffer[BUFSIZ];
	struct sockaddr_in my_addr;
	
	if(argc !=3)
	{
		printf("usage: myrls hostname dirpath\n");
		exit(1);
	}
	/* Step 1: Get a socket */
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_id == -1)
		oops("socket");

	/* Step 2: connect to server */
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORTNUM);
	if(inet_pton(AF_INET, argv[1], &my_addr.sin_addr) != 1)
		oops("inet_pton");

	if(connect(sock_id, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
		oops("connect");

	/* Step 3: send directory name,then read back results */
	if(write(sock_id, argv[2], strlen(argv[2])) == -1)
		oops("write");

	while((n_read = read(sock_id, buffer, BUFSIZ)) > 0)
		if(write(1, buffer, n_read) == -1)
			oops("write");
	close(sock_id);
}
