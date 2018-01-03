/* hello1.c
 * purpose     show the minimal calls needed to use curses
 * outline     initialize, draw stuff, wait for input, quit
 */

#include <stdio.h>
#include <curses.h>

main()
{
	initscr();					/* turn on curses */

	clear();
	move(10, 20);
	addstr("Hello, world");
	move(LINES-1,0);
	refresh();
	getch();
	endwin();
}
