#include <curses.h>
 
int main () {
	initscr ();
	start_color ();
	init_pair (1, COLOR_RED, COLOR_GREEN);
	attrset (COLOR_PAIR(1));
	mvaddch(2, 5,'*');
	refresh ();
	getch();
	endwin ();
	return 0;
}
