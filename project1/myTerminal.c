#include <curses.h>
#include <term.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @author Lawrence L. O'Boyle II
 * @version 01/15/2015
 */

void shoot(int old_x, int old_y, int to_x, int to_y);
void explode(int x, int y, int size);
double slope_formula(int old_x, int old_y, int to_x, int to_y);
double distance_formula(int old_x, int old_y, int to_x, int to_y);
int calculate_x(double distance, double slope, int old_x);
int calculate_y(double distance, double slope, int old_y);

int main(){
	char * clear,
		 * home;
	
	int col,
		row;

	//Start curses
	initscr();

	//Allow color to be used
	start_color();

	//Clears the screen
	clear = tigetstr("clear"),
	putp(clear);

	//Refesshs the current screen
	refresh();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	//Gets the home sequence, column and row numbers
	home = tigetstr("home");
	col = tigetnum("cols"),
	row = tigetnum("lines");

	//Turns off user input on current window, removes the cursor
	noecho();
	curs_set(FALSE);

	//Homes the curser
	putp(home);

	//Displays the rows and columns
	printw("Rows: %d\n", row);
	printw("col: %d\n", col);
	refresh();

	//Waits
	usleep(30000);
	fflush(stdout);

	//Shoots the firework
	shoot(lines, 150, 17, 10);
	shoot(lines, 10, 17, 100);
	shoot(lines, 75, 17, 45);

	//Allows the program to wait for enter to be pressed.
	getchar();
	putp(clear);
	//Ends the current window
	endwin();
    return(0);
}

//Calculates the distance between two points
double distance_formula(int old_x, int old_y, int to_x, int to_y){
	return sqrt(pow(to_x - old_x, 2) + pow(to_y - old_y, 2));
}

//Calculates the slope between two points
double slope_formula(int old_x, int old_y, int to_x, int to_y){
	return (double)(to_y - old_y) / (double)(to_x - old_x);
}

//Calculates the new x from distance, slope, and the old x with vectors
int calculate_x(double distance, double slope, int old_x){
	int x;
	double t;
	
	//Calculates the t in the vector formula
	t = (distance) / sqrt(1 + pow(slope, 2));
	//Rounds the new varaible to it's nearest point
	x = round(old_x - t);

	return x;
}

//Calculates the new y from distance, slope, and the old y with vectors
int calculate_y(double distance, double slope, int old_y){
	int y;
	double t;
	
			
	//Calculates the t in the vector formula
	t = (distance * slope) / sqrt(1 + pow(slope, 2));
	//Rounds the new varaible to it's nearest point
	y = round(old_y - t);

	return y;
}

void shoot(int old_x, int old_y, int to_x, int to_y){
	//Gets the initial data for later calculation of path
	double d = distance_formula(old_x, old_y, to_x, to_y);
	double m = slope_formula(old_x, old_y, to_x, to_y);

	int x,
		y,
		e_y,
		e_x;

	double z, z1;

	//Loops across the screen drwaing the shot of the firework
	for(z = 0; z < d; z++){
		refresh();
		//For removing the large tail
		if(z >= 10){
			z1 = z - 10;

			x = calculate_x(z1, m, old_x);
			y = calculate_y(z1, m, old_y);

			mvaddch(x, y, ' ');
		}
		
		//Draws the actual firework
		x = calculate_x(z, m, old_x);
		y = calculate_y(z, m, old_y);

		fflush(stdout);
		usleep(30000);
		mvaddch(x, y, '0');

		e_y = y;
		e_x = x;
		
		//Draws the tail
		if(z >= 10){
			x = calculate_x(z-1, m, old_x);
			y = calculate_y(z-1, m, old_y);

			mvaddch(x, y, '|');
		}
	}
	//Removes the rest of the tail before exploding
	for(; z1 < d - 1; z1++){
		x = calculate_x(z1, m, old_x);
		y = calculate_y(z1, m, old_y);

		mvaddch(x, y, ' ');
	}
	//Explodes the firework
	explode(e_x, e_y, 6);
}

//Explodes the firework
void explode(int x, int y, int size){
	int z;

	//Writes the exploding animation
	for(z = 1; z < size; z++){
		attron(COLOR_PAIR(1));
		mvaddch(x+z, y+z, '\\');

		mvaddch(x-z, y+z, '/');

		mvaddch(x+z, y-z, '/');

		mvaddch(x-z, y-z, '\\');

		mvaddch(x, y-z, '-');

		mvaddch(x, y+z, '-');

		mvaddch(x-z, y, '|');

		mvaddch(x+z, y, '|');
		attroff(COLOR_PAIR(1));

		mvaddch(x, y, ' ');

		refresh();
		attroff(COLOR_PAIR(1));
		
		fflush(stdout);
		usleep(90000);
	}
}
