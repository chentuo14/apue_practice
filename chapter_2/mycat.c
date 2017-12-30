#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 4096

int main(int argc, char *argv[])
{
	int fd, ret;
	char buf[BUFSIZE];

	if(argc != 2)
	{
		printf("usage: cat filename");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		perror("open failed");
		exit(1);
	}
	
	while((ret = read(fd, buf, BUFSIZE)) > 0)
	{
		fprintf(stdout, "%s", buf);
	}

	close(fd);
}
