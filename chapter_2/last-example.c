#include <stdio.h>
#include <utmp.h>
#include <string.h>
#include <sys/utsname.h>
#include "utmplib.c"

struct acc{
    short ut_type;
    char ut_name[UT_NAMESIZE];
    char ut_line[UT_LINESIZE];
    char ut_host[UT_HOSTSIZE];
    int32_t login_sec;
    int32_t logout_sec; /* 0 still in, -1 down*/
    int diff; /* logout_sec - login_sec */
} record[1024];

static int num_rd;

#define STILL_IN 0
#define DOWN -1

void output_ans(void);
char *format_time(int t);

int main(int ac, char *av[])
{
	struct utmp *utbufp, *utbufp_next(void);
    int utmp_open(char *filename);
    void utmp_close(void);
    int i;

	if(utmp_open(WTMP_FILE) == -1){
		perror(WTMP_FILE);
		exit(1);
	 }

	num_rd = 0;

	
	while(utbufp = utmp_next()){
		switch(utbufp->ut_type){			
			case USER_PROCESS:
	        case BOOT_TIME:
	        	record[num_rd].login_sec = utbufp->ut_time;
				record[num_rd].logout_sec = STILL_IN;
				record[num_rd].ut_type = utbufp->ut_type;
				strcpy(record[num_rd].ut_name, utbufp->ut_name);
				strcpy(record[num_rd].ut_line, utbufp->ut_type == USER_PROCESS ? utbufp->ut_line : "system");
				strcpy(record[num_rd].ut_host, utbufp->ut_host);
				++num_rd;
				break;
			case DEAD_PROCESS:
				for(i = 0; i < num_rd; ++i){
					if(record[i].ut_type == USER_PROCESS && !strcmp(record[i].ut_line, utbufp->ut_line)){
		                record[i].logout_sec = utbufp->ut_time;
						record[i].ut_type = DEAD_PROCESS;
						record[i].diff = record[i].logout_sec - record[i].login_sec;
						break;
					}
				}
				break;
			case RUN_LVL:
				/* utbufp->ut_time > record[num_rd - 1].login_sec guarantee that can record boot time normally */
				if(!strcmp("runlevel", utbufp->ut_name) && utbufp->ut_time > record[num_rd - 1].login_sec)
					for(i = 0; i < num_rd; ++i)
						if(record[i].ut_type == USER_PROCESS || 
								record[i].ut_type == BOOT_TIME){
							record[i].logout_sec = utbufp->ut_time;
							record[i].diff = record[i].logout_sec - record[i].login_sec;
							if(record[i].ut_type == USER_PROCESS)
								record[i].logout_sec = DOWN; /* quit because shutdown */
							record[i].ut_type = DEAD_PROCESS;
						}
				break;
		}
	}
	output_ans();
	utmp_close();

	return 0;
}
void output_ans(void)
{
	    int i;
		time_t now_t = time(NULL);
		struct utsname buf;

		uname(&buf);
		for(i = num_rd - 1; i >= 0; --i){
			if(record[i].ut_type == BOOT_TIME){
				record[i].logout_sec = (int32_t)now_t;
				record[i].diff = record[i].logout_sec - record[i].login_sec;
			}
			printf("%-8.8s ", record[i].ut_name);
			printf("%-6.6s ", record[i].ut_line);
			if(!strcmp(record[i].ut_line, "system")){
				printf("%-5.5s ", "boot");
				printf("%-16.16s ", buf.release); /* 4.2.0-27-generic */
			}else{
				printf("%-5.5s ", "");
				printf("%-16.16s ", record[i].ut_host);
			}
			printf("%-16.16s ", asctime(localtime(&record[i].login_sec)));
			switch(record[i].logout_sec){
				case STILL_IN:
					printf("  %-5.5s ", "still");   
					break;
				case DOWN:
					printf("- %-5.5s ", "down");
					break;
				default:
					printf("- %-5.5s ", asctime(localtime(&record[i].logout_sec)) + 11);
					break;
			}
			
			if(record[i].ut_type == USER_PROCESS)
				printf("logged in\n");
			else
				printf(" (%s)\n", format_time(record[i].diff));
		}
}

char *format_time(int t)
{
	    static char buf[10];
		int h;
		int m;
		t /= 60;
		m = t % 60;
		h = t / 60;
		sprintf(buf, "%.2d:%.2d", h, m);
		return buf;
}
