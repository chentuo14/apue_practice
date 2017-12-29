#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd1, fd2, fd3;
	char buf[20];
	int ret;

	fd1 = open("./aaa", O_RDONLY);
	if(fd1 == -1)
	{
		perror("open fd1 error ");
		exit(1);
	}

	fd2 = open("./aaa", O_WRONLY);
	if(fd2 == -1)
	{
		perror("open fd2 error");
		exit(1);
	}

	fd3 = open("./aaa", O_RDONLY);
	if(fd3 == -1)
	{
		perror("open fd3 error");
		exit(1);
	}

	ret = read(fd1, buf, 20);
	if(ret != 20)
	{
		perror("read fd1 error");
		exit(1);
	}
	printf("buf: %s\n", buf);

	strcpy(buf, "test 123...");
	ret = write(fd2, buf, 11);
	if(ret != 11)
	{
		perror("read fd1 error");
		exit(1);
	}

	ret = read(fd3, buf, 20);
	if(ret != 20)
	{
		perror("read fd1 error");
		exit(1);
	}
	printf("buf: %s\n", buf);

	
}

