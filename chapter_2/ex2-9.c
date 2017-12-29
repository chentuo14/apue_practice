#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd;
	int ret;
	char *buf[100];

	fd = open("./aaa", O_RDWR);
	if(fd == -1)
	{
		perror("open failed");
		exit(1);
	}

	ret = lseek(fd, 1000, SEEK_SET);
	if(ret != 1000)
	{
		perror("lseek failed");
		exit(1);
	}

	strcpy(buf, "hello");
	ret = write(fd, buf, 5);
	if(ret != 5)
	{
		perror("write failed");
		exit(1);
	}

	close(fd);
}

