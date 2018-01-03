#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int main()
{
	int response;
	tty_mode(0);
	set_crmode();
	response = get_response(QUESTION);
	tty_mode(1);
	return response;
}

int get_response(char *question)
{
	int input;
	printf("%s(y/n)?", question);
	while(1)
	{
		switch(input = getchar())
		{
		case 'Y':
		case 'y': return 0;
		case 'n':
		case 'N':
		case EOF: return 1;
		default:
			printf("\ncannot understand %c, ", input);
			printf("Please type y or no \n");
		}
	}
}

set_crmode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;		/* don't need return enter */
	ttystate.c_cc[VMIN] = 1;			/* one char byte */
	tcsetattr(0, TCSANOW, &ttystate);
}

/* how == 0 ->save current mode, how == 1->restore mode */
tty_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode);
}
