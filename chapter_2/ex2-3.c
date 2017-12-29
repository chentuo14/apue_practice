#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 512

int main(int argc, char *argv[])
{
	int out_fd;
	int in_fd;
	int readNum = 0;
	int writeNum = 0;
	char buf[BUFFERSIZE];

	if(argc !=2)
	{
		fprintf(stdout, "usage: %s filename\n", *argv);
		exit(1);
	}
	if((in_fd = open("/dev/tty", O_RDONLY)) == -1)
	{
		perror("open error ");
		exit(1);
	}

	if((out_fd = open(argv[1], O_RDWR|O_CREAT, 0644)) == -1)
	{
		perror("can't open ");
		exit(1);
	}
	
	while((readNum = read(in_fd, buf, BUFFERSIZE)) != 0)
	{
		if((readNum == 1) && (buf[0] == '\n'))
			break;

		if((writeNum = write(out_fd, buf, readNum)) != readNum)
		{
			perror("write error");
			exit(1);
		}
	}

	close(in_fd);
	close(out_fd);

	return(0);
}

