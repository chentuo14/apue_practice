#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>

#define PORTNUM 43210
#define oops(msg) \
	do { perror(msg); exit(1); } while(0)

int main()
{
	int sock_fd, accept_fd;
	FILE *sock_fp;
	time_t sertime;
	struct sockaddr_in my_addr;
	char buf[BUFSIZ];
	/* Step 1: ask kernel for a soket */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1)
		oops("socket");

	/* Step 2: bind address to socket. Address is host,port */
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORTNUM);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in)) == -1)
		oops("bind");

	/* Step 3: allow incoming calls with Qsize=1 on socket */
	if(listen(sock_fd, 1) == -1)
		oops("listen");

	/* main loop: accept(), write(), close() */
	while(1)
	{
		accept_fd = accept(sock_fd, NULL, NULL);
		if(accept_fd == -1)
			oops("accept");
		printf("Wow!Got a call!\n");

		//sock_fp = fdopen(accept_fd, "w");
		//if(sock_fp == NULL);
		//	oops("fdopen");

		sertime = time(NULL);

		sprintf(buf, "The time here is ...%s", ctime(&sertime));
//		fprintf(sock_fp, "The time here is ...");
//		fprintf(sock_fp, "%s", ctime(&sertime));
//		fclose(sock_fp);

		write(accept_fd, buf, strlen(buf));
	}

	return 0;
}
