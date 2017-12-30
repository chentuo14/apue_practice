#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 4096
#define MAXLINE 10

int main(int argc, char *argv[])
{
	FILE *fp;
	char *ret;
	char buf[BUFSIZE];
	int i = 0;

	if(argc != 2)
	{
		printf("usage: cat filename\n");
		exit(1);
	}

	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("open failed");
		exit(1);
	}
	
	while((ret = fgets(buf, BUFSIZE, fp)) != NULL)
	{
		fputs(buf, stdout);
		i++;
		if(i == MAXLINE)
			break;
	}

	fclose(fp);
}
